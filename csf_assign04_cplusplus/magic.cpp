#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cstdarg>
#include <cerrno>
#include <cstdint>
#include <elf.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/unistd.h>
#include <fcntl.h>

#include "elf_names.h"

int main(int argc, char **argv) {
  if (argc != 1) {
    return 1;
  }

  char* filename = argv[0];
  // open file
  int fd = open(filename, O_RDONLY);
  struct stat statbuf;
  int rc = fstat(fd, &statbuf);
  if (rc != 0) {
    // std::cerr << "cannot open input file." << std::endl;
    return 1;
  } 
  size_t file_size = statbuf.st_size;
  void *data = mmap(NULL, file_size, PROT_READ, MAP_PRIVATE, fd, 0);
  if (data == ((void *) - 1)) {
    // std::cerr << "Program cannot access memory region." << std::endl;
    return 1;
  }

  
  return 0;
}
