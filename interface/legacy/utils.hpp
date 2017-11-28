#ifndef __UTILS_H_INCLUDED__
#define  __UTILS_H_INCLUDED__

int open_file(char const *path);
int pwrite_all(int fildes, const void *buf, size_t nbyte, off_t offset);
int pread_all(int fildes, void *buf, size_t nbyte, off_t offset);
int close_file(int fd);

#endif
