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

  // TODO: check if valid ELF file


  Elf64_Ehdr *elf_header = (Elf64_Ehdr *) data;
  // printf(".shstrtab section index is %u\n", elf_header->e_shstrndx);
  string endian;
  if (elf_header->e_ident[EI_DATA] == 1) {
    endian = "Little endian";
  } else if (elf_header->e_ident[EI_DATA] == 2) {
    endian = "Big ndian";
  }

  cout << "Object file type: " << get_type_name(elf_header->e_type) << endl;
  cout << "Instruction set: " << get_machine_name(elf_header->e_machine) << endl;
  cout << "Endianness: " << endian << endl;

  // section header
  uint8_t section_offset = elf_header->e_shoff;
  Elf64_Shdr *section_header = (Elf64_Shdr *) ((uint8_t*)data + section_offset);
  uint8_t num_entries = elf_header->e_shnum;

  uint8_t stringTable_idx = elf_header->e_shstrndx;
  // char* stringTable = (char*) (section_header + stringTable_idx);
  Elf64_Shdr *shstrtab = &section_header[stringTable_idx];
  uint8_t data_offset = shstrtab->sh_offset;
  char* name = (char *) (shstrtab + data_offset);
  
  for (unsigned i = 0; i < num_entries; i++){
    uint8_t name_index = (uint8_t) section_header[i].sh_name;

    printf("Section header %u: name=%s, ", i, name + name_index);
    printf("type=%lx, ", section_header[i].sh_type);
    printf("offset=%lx, ", section_header[i].sh_offset);
    printf("size=%lx\n", section_header[i].sh_size);
    // section_header++;
  }


  return 0;
}
