#ifndef LIBRAC5_SOUND_H
#define LIBRAC5_SOUND_H
#include <stdbool.h>
#include "game.h"
#ifdef __cplusplus
extern "C" {
#endif
typedef struct GDE_Sound SoundDef;
typedef struct SND_Positional SoundInstance;
typedef struct SoundPosition { float x, y, z; } SoundPosition;
typedef unsigned int SoundHandle;

SoundDef *soundGet(unsigned int signature);
SoundDef *soundGetGlobal(unsigned int signature);
SoundHandle soundPlay(SoundDef *, float volume, float pitch, SoundInstance **, unsigned int ticks);
SoundHandle soundPlayAt(SoundDef *, const SoundPosition *, float parameter, SoundInstance **, unsigned int ticks);
void soundStop(SoundInstance *, bool immediate);
void soundPause(SoundInstance *, bool paused);
bool soundIsPlaying(const SoundInstance *);
bool soundIsLooping(const SoundDef *);
void soundPauseAll(bool paused);
void soundSetVolume(float volume);
void soundSetMusicVolume(float volume);
void soundSetVoiceVolume(float volume);
unsigned int soundGetSlotCount(void);
SoundInstance *soundGetSlot(unsigned int index);
#ifdef __cplusplus
}
#endif
#endif
