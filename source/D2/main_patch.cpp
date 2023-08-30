#include "skyline/inlinehook/And64InlineHook.hpp"
#include "skyline/utils/cpputils.hpp"
#include "skyline/inlinehook/memcpy_controlled.hpp"

//[13 C8 80 D2] [93 70 C0 F2] - [movz x19, #0x640] [movk x19, #0x384, lsl #32]
constinit uint8_t res[8] = {0x13, 0xC8, 0x80, 0xD2, 0x93, 0x70, 0xC0, 0xF2};
							 
//[13 F0 80 D2] [13 87 C0 F2] - [movz x19, #0x780] [movk x19, #0x438, lsl #32]
constinit uint8_t res_p[8] = {0x13, 0xF0, 0x80, 0xD2, 0x13, 0x87, 0xC0, 0xF2};


bool memcmp_f (const unsigned char *s1, const unsigned char *s2, size_t count) {
    while (count-- > 0)
        if (*s1++ != *s2++) 
            return false;
    return true;
}

size_t getCodeSize() {
        return (size_t)skyline::utils::getRegionAddress(skyline::utils::region::Rodata) - (size_t)skyline::utils::getRegionAddress(skyline::utils::region::Text);
}

/* You must provide full instructions in arrays of uint8_t.
It returns pointer to first found code in main 
or 0 if nothing have been found. */
uintptr_t findTextCode(const uint8_t* code, size_t size) {
    if (size % 4 != 0) return 0;
    uintptr_t addr = (uintptr_t)skyline::utils::getRegionAddress(skyline::utils::region::Text);
    size_t addr_size = getCodeSize();	
    uintptr_t addr_start = addr;
    while (addr != addr_start + addr_size) {
        bool result = memcmp_f((const unsigned char*)addr, code, size);
        if (result) return addr;
        addr += 4;
    }
    return 0;
}

void D2_graphics_main() {	
    uintptr_t pointer = findTextCode(&res[0], sizeof(res));
    if (pointer) {
        sky_memcpy((void*)pointer, &res_p[0], sizeof(res_p));
    }
}