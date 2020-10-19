#include "game.h"
#include "examples/ex1_stars.h"
#include "examples/ex2_stars2.h"
#include "examples/ex3_font.h"
#include <stdlib.h>
#include <string.h>

#define SND_DEVICE_SAMPLE_RATE (44100)
#define SND_DEVICE_NUM_CHANNELS 2
#define SND_DEVICE_BUFFER_FRAMES 2048

int example;
drwav wav;
bool isPlaying = false;

void stream_cb(float *buffer, int num_frames, int num_channels) {
  drwav_uint64 r = 0;
  if (isPlaying) {
    r = drwav_read_pcm_frames_f32(&wav, num_frames, buffer);
  }
  if (r < num_frames) {
    memset(buffer + r * num_channels, 0x0,
           (num_frames - r) * num_channels * sizeof(float));
    // drwav_seek_to_pcm_frame(&wav, 0);
  }
}

void start(void) {
  printf("Starting Game\n");
  example = 1;
  ex1_start();
  ex2_start();
  ex3_start();

  saudio_setup(&(saudio_desc){
      .stream_cb = stream_cb,
      .num_channels = SND_DEVICE_NUM_CHANNELS,
      .sample_rate = SND_DEVICE_SAMPLE_RATE,
      .buffer_frames = SND_DEVICE_BUFFER_FRAMES,
  });
  printf("audio init: %d\nSample rate: %d\nChannels: %d\n", saudio_isvalid(),
         saudio_sample_rate(), saudio_channels());

  isPlaying = true;
  if (!drwav_init_file(&wav, "assets/PinkPanther60_44.wav", NULL)) {
    printf("Error opening WAV file.\n");
  }
  if (wav.channels != SND_DEVICE_NUM_CHANNELS &&
      wav.sampleRate != SND_DEVICE_SAMPLE_RATE) {
    printf("Only audio file with 44100Hz and 2 Channels allowed.\n");
    printf("Channels: %d\nSample Rate: %d\n", wav.channels, wav.sampleRate);
    isPlaying = false;
  }
}

void frame(const sapp_event *e) {
  if (e != NULL && e->type == SAPP_EVENTTYPE_KEY_UP) {
    if (e->key_code == SAPP_KEYCODE_1) {
      example = 1;
    } else if (e->key_code == SAPP_KEYCODE_2) {
      example = 2;
    } else if (e->key_code == SAPP_KEYCODE_3) {
      example = 3;
    } else if (e->key_code == SAPP_KEYCODE_0) {
      example = 0;
    }
  }

  if (example == 1) {
    ex1_frame(e);
  } else if (example == 2) {
    ex2_frame(e);
  } else if (example == 3) {
    ex3_frame(e, 0);
  } else {
    int r1 = ex1_frame(e);
    int r2 = ex2_frame(e);
    ex3_frame(e, r1 + r2);
  }
}

void end(void) {
  isPlaying = false;
  saudio_shutdown();
  drwav_uninit(&wav);
  printf("Ending Game\n");
  ex1_end();
  ex2_end();
  ex3_end();
}
