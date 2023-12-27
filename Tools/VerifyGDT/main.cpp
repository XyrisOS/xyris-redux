#include <Arch/GDT.hpp>
#include <cstdlib>
#include <iostream>
#include "../HexDump.hpp"

int main(int argc, char* argv[]) {
    (void) argc;
    (void) argv;

    GDT::Initialize();
    return EXIT_SUCCESS;
}

namespace GDT {
    extern "C" void FlushGDT(GDTR* pGDTR) {
        std::cout << "Flushing GDTR! [" << pGDTR << "]" << std::endl
            << "  Size: " << pGDTR->size << std::endl
            << "  Addr: " << pGDTR->addr << std::endl;

        GDT* pGDT = reinterpret_cast<GDT*>(pGDTR->addr);
        std::cout << "Validating GDT [" << pGDT << "]" << std::endl;
        for (size_t i = 0; i != 6; i++) {
            Entry& entry = pGDT->entries[i];
            std::cout << "  [" << i << "] " << HexDump(&entry, sizeof(Entry));
        }
    }
}
