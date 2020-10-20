#ifndef __AUDIO_H__
#define __AUDIO_H__

#include <stdbool.h>

#define SND_DEVICE_SAMPLE_RATE (44100)
#define SND_DEVICE_NUM_CHANNELS 2
#define SND_DEVICE_BUFFER_FRAMES 2048

void audio_init();
void audio_shutdown();
void sound_play(const char *filename);
void music_play(const char *filename, bool loop);
void music_stop();
bool music_isplaying();

#endif
