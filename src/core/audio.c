#include "audio.h"
#include "../../libs/dr_libs/dr_mp3.h"
#include "../../libs/dr_libs/dr_wav.h"
#include "../../libs/sokol/sokol_audio.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct {
  drwav sound;
  drmp3 music;
  bool isSoundLoaded;
  bool isPlayingSound;
  bool isMusicLoaded;
  bool isPlayingMusic;
  bool loopMusic;
  int channels;
  int samplerate;
} state;

void _unload_music() {
  if (state.isMusicLoaded) {
    drmp3_uninit(&state.music);
    state.isMusicLoaded = false;
    state.isPlayingMusic = false;
  }
}

void _unload_sound() {
  if (state.isSoundLoaded) {
    drwav_uninit(&state.sound);
    state.isSoundLoaded = false;
    state.isPlayingSound = false;
  }
}

void stream_cb(float *buffer, int num_frames, int num_channels) {
  drwav_uint64 rs = 0;
  drwav_uint64 rm = 0;
  const uint16_t data_len = num_frames * num_channels * sizeof(float);
  float *data_s = (float *)malloc(data_len);
  float *data_m = (float *)malloc(data_len);

  if (state.isPlayingSound) {
    rs = drwav_read_pcm_frames_f32(&state.sound, num_frames, data_s);
  }
  if (state.isPlayingMusic) {
    rm = drmp3_read_pcm_frames_f32(&state.music, num_frames, data_m);
  }

  if (rs < num_frames) {
    memset(data_s + rs * num_channels, 0x0,
           (num_frames - rs) * num_channels * sizeof(float));
    state.isPlayingSound = false;
  }
  if (rm < num_frames) {
    memset(data_m + rm * num_channels, 0x0,
           (num_frames - rm) * num_channels * sizeof(float));
    if (state.loopMusic) {
      drmp3_seek_to_pcm_frame(&state.music, 0);
    } else {
      state.isPlayingMusic = false;
    }
  }

  for (int i = 0; i < num_frames * num_channels; ++i) {
    buffer[i] = data_s[i] + data_m[i];
  }
  free(data_s);
  free(data_m);
}

void audio_init() {
  saudio_setup(&(saudio_desc){
      .stream_cb = stream_cb,
      .num_channels = SND_DEVICE_NUM_CHANNELS,
      .sample_rate = SND_DEVICE_SAMPLE_RATE,
      .buffer_frames = SND_DEVICE_BUFFER_FRAMES,
  });
  state.channels = saudio_channels();
  state.samplerate = saudio_sample_rate();
  state.isPlayingSound = false;
  state.isSoundLoaded = false;
  state.isPlayingMusic = false;
  state.isMusicLoaded = false;
  state.loopMusic = false;
}

void audio_shutdown() {
  _unload_sound();
  _unload_music();
  saudio_shutdown();
}

void sound_play(const char *filename) {
  _unload_sound();
  if (!drwav_init_file(&state.sound, filename, NULL)) {
    printf("Error opening WAV file: %s\n", filename);
    return;
  }
  if (state.sound.channels != state.channels &&
      state.sound.sampleRate != state.samplerate) {
    printf("Only audio file with %dHz and %d Channels allowed.\n",
           state.samplerate, state.channels);
    printf("But loaded sound has %dHz and %d Channels\n",
           state.sound.sampleRate, state.sound.channels);
    return;
  }
  state.isPlayingSound = true;
  state.isSoundLoaded = true;
}

void music_play(const char *filename, bool loop) {
  _unload_music();
  if (!drmp3_init_file(&state.music, filename, NULL)) {
    printf("Error opening MP3 file: %s\n", filename);
    return;
  }
  if (state.music.channels != state.channels &&
      state.music.sampleRate != state.samplerate) {
    printf("Only audio file with %dHz and %d Channels allowed.\n",
           state.samplerate, state.channels);
    printf("But loaded sound has %dHz and %d Channels\n",
           state.music.sampleRate, state.music.channels);
    return;
  }
  state.isPlayingMusic = true;
  state.isMusicLoaded = true;
  state.loopMusic = loop;
}

void music_stop() {
  if (state.isPlayingMusic) {
    _unload_music();
  }
}

bool music_isplaying() { return state.isPlayingMusic; }
