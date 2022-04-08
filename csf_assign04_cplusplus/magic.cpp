#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cstdarg>
#include <cerrno>
#include <cstdint>
#include <iostream>
#include <elf.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/unistd.h>
#include <fcntl.h>

#include "elf_names.h"

using namespace std;

int main(int argc, char **argv) {
  if (argc != 2) {
    return 1;
  }

  char* filename = argv[0];
  // open file
  int fd = open(filename, O_RDONLY);
  struct stat statbuf;
  int rc = fstat(fd, &statbuf);
  if (rc != 0) {
    cerr << "cannot open input file." << endl;
    return 1;
  } 
  size_t file_size = statbuf.st_size;
  void *data = mmap(NULL, file_size, PROT_READ, MAP_PRIVATE, fd, 0);
  if (data == ((void *) - 1)) {
    cerr << "Program cannot access memory region." << endl;
    return 1;
  }

  Elf64_Ehdr *elf_header = (Elf64_Ehdr *) data;
  printf(".shstrtab section index is %u\n", elf_header->e_shstrndx);
  // determine ELF file

  string endian;
  if (elf_header->e_ident[EI_DATA] == 1) {
    endian = "Little Endian";
  } else if (elf_header->e_ident[EI_DATA] == 2) {
    endian = "Big Endian";
  }

  cout << "Object file type: " << get_type_name(elf_header->e_type) << endl;
  cout << "Instruction set: " << get_machine_name(elf_header->e_machine) << endl;
  cout << "Endianness: " << endian << endl;


  
  return 0;
}
