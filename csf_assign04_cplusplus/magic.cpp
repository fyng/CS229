#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cstdarg>
#include <cerrno>
#include <cstdint>
#include <iostream>
#include <fstream>
#include <string>
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

  char* filename = argv[1];
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
  
  // TODO: check if valid ELF file
  if (elf_header->e_ident[EI_MAG0] != 0x7F
      && elf_header->e_ident[EI_MAG1] != 0x45
      && elf_header->e_ident[EI_MAG2] != 0x4c
      && elf_header->e_ident[EI_MAG3] != 0x46) {
    cout << "Not an ELF file" << endl;
    return 0;
  }

  // printf(".shstrtab section index is %u\n", elf_header->e_shstrndx);
  string endian;
  if (elf_header->e_ident[EI_DATA] == 1) {
    endian = "Little endian";
  } else if (elf_header->e_ident[EI_DATA] == 2) {
    endian = "Big ndian";
  }

  // print the header information
  cout << "Object file type: " << get_type_name(elf_header->e_type) << endl;
  cout << "Instruction set: " << get_machine_name(elf_header->e_machine) << endl;
  cout << "Endianness: " << endian << endl;

  // section header
  Elf64_Shdr *section_header = (Elf64_Shdr *) ((uint8_t*)data + elf_header->e_shoff);
  uint8_t num_entries = elf_header->e_shnum;

  // uint8_t stringTable_idx = elf_header->e_shstrndx;
  // char* stringTable = (char*) (section_header + stringTable_idx);
  Elf64_Shdr *shstrtab = &section_header[elf_header->e_shstrndx];
  // uint8_t data_offset = shstrtab->sh_offset;
  unsigned char* name = (uint8_t*)data + shstrtab->sh_offset;
  
  for (unsigned i = 0; i < num_entries; i++){
    // uint8_t name_index = (uint8_t) section_header[i].sh_name;

    printf("Section header %u: name=%s, ", i, name +section_header[i].sh_name);
    printf("type=%lx, ", (long unsigned) section_header[i].sh_type);
    printf("offset=%lx, ", section_header[i].sh_offset);
    printf("size=%lx\n", section_header[i].sh_size);
    // section_header++;
  }


  return 0;
}
