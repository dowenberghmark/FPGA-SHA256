#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h> // errno

#include "defs.hpp"


int open_file(char const *path) {
  int fd;

  if (MODE == LOCAL) {
    fd = open(path, O_RDWR | O_CREAT | O_TRUNC);
  } else if (MODE == AWS) {
    fd = open(path, O_RDWR);
  }

  if (fd == -1) {
    perror("open failed with errno");
    return -1;
  }
  return fd;
}


int close_file(int fd) {
  int ret;

  ret = close(fd);
  if (ret < 0) {
    perror("close failed with errno");
    return -1;
  }
  return 1;
}


int pread_all(int fildes, void *buf, size_t nbyte, off_t offset) {
  ssize_t bytes_read;

  while (nbyte) {
    bytes_read = pread(fildes, buf, nbyte, offset);

    if (bytes_read < 0) {
      perror("read failed with errno");
      return -1;
    } else {
      nbyte -= bytes_read;
      offset += bytes_read;
    }
  }
  return 1;
}


int pwrite_all(int fildes, const void *buf, size_t nbyte, off_t offset) {
  ssize_t bytes_written;

  while (nbyte) {
    bytes_written = pwrite(fildes, buf, nbyte, offset);
    if (bytes_written < 0) {
      perror("write failed with errno");
      return -1;
    } else {
      nbyte -= bytes_written;
      offset += bytes_written;
    }
  }
  return 1;
}
