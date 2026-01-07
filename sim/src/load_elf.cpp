#include "load_elf.hpp"

#include <elf.h>
#include <cstdio>
#include <fstream>
#include <vector>
#include "vtop_adapter.hpp"

void load_elf(const char* path, VtopAdapter &adapter) {
  std::ifstream f(path, std::ios::binary);
  if (!f) {
    throw std::runtime_error("Failed to open ELF");
  }

  Elf32_Ehdr eh;
  f.read(reinterpret_cast<char*>(&eh), sizeof(eh));
  if (eh.e_ident[EI_MAG0] != ELFMAG0 || eh.e_ident[EI_CLASS] != ELFCLASS32) {
    throw std::runtime_error("File is not ELF32");
  }

  std::printf("Loaded ELF at %s\n", path);
  f.seekg(eh.e_phoff);

  for (int i = 0; i < eh.e_phnum; i++) {
    Elf32_Phdr ph;
    f.read(reinterpret_cast<char*>(&ph), sizeof(ph));
    if (ph.p_type != PT_LOAD) continue;

    std::vector<uint8_t> buf(ph.p_filesz);
    auto pos = f.tellg();

    f.seekg(ph.p_offset);
    f.read(reinterpret_cast<char*>(buf.data()), ph.p_filesz);
    f.seekg(pos);

    uint32_t addr = ph.p_vaddr;
    std::printf("Reading segment [%08x:%08x]\n", ph.p_vaddr, ph.p_vaddr + ph.p_memsz - 1);

    for (size_t j = 0; j < buf.size(); j++) {
      adapter.write_byte(addr + j, buf[j]);
    }
  }
}
