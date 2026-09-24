#ifndef LIBRAC5_SIF_H
#define LIBRAC5_SIF_H

#ifdef __cplusplus
extern "C" {
#endif

void sceSifInitRpc(unsigned int mode);
void sceSifExitRpc(void);
int sceSifLoadModule(const char *path, int argLength, const char *args);
int sceSifLoadStartModule(const char *path, int argLength, const char *args, int *moduleResult);
int sceSifLoadModuleBuffer(void *iopBuffer, int argLength, const char *args);
int sceSifSearchModuleByName(const char *name);
int sceSifStopModule(int moduleId, int argLength, const char *args, int *moduleResult);
int sceSifUnloadModule(int moduleId);
int sceSifInitIopHeap(void);
void *sceSifAllocIopHeap(int size);
int sceSifResetIop(const char *args, int mode);
int sceSifRebootIop(const char *imagePath);
int sceSifSyncIop(void);

#ifdef __cplusplus
}
#endif

#endif
