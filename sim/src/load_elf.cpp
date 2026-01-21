#include "load_elf.hpp"

#include <elf.h>

#include <fstream>
#include <iostream>
#include <vector>

#include "fmt/core.h"

void load_elf(const char* path, VerilatedContainer& v) {
    std::ifstream f(path, std::ios::binary);
    if (!f) {
        throw std::runtime_error("Failed to open ELF");
    }

    Elf32_Ehdr eh;
    f.read(reinterpret_cast<char*>(&eh), sizeof(eh));
    if (eh.e_ident[EI_MAG0] != ELFMAG0 || eh.e_ident[EI_CLASS] != ELFCLASS32) {
        f.close();
        throw std::runtime_error("File is not ELF32");
    }

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
        std::cout << fmt::format(
                         "Read segment [{:08x}:{:08x}] ({:d} bytes)",
                         ph.p_vaddr,
                         ph.p_vaddr + ph.p_memsz - 1,
                         ph.p_memsz
                     )
                  << std::endl;

        for (size_t j = 0; j < buf.size(); j++) {
            if (!v.write_byte(addr + j, buf[j])) {
                std::cerr << fmt::format("Failed to write at {:08x}", addr + j) << std::endl;
                return;
            }
        }
    }
}
