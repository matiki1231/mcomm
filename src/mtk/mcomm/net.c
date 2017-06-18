#include "net.h"

#include <unistd.h>
#include <string.h>

int read_exact(int fd, char* buf, size_t n) {
  int acc = 0, curr;

  while (acc < n) {
    if ((curr = read(fd, buf + acc, n - acc)) <= 0)
      return curr;

    acc += curr;
  }

  return acc;
}
