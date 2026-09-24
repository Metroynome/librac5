#ifndef LIBRAC5_FILE_H
#define LIBRAC5_FILE_H

int fileOpen(const char *path, int flags, unsigned short mode);
int fileRead(int fd, void *buffer, int size);
int fileClose(int fd);

#endif
