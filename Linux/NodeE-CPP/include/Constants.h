#pragma once

namespace Xceed {
    namespace Constants {
        const int n = 2;                                    // Must be a factor of 8
        const int M = 5;
        const int mat_size = 1 << n;

        const uint8_t mask = ((uint8_t)(-1)) << (8 - n);
        const uint16_t block_size = 1024;                    // In byte
    }
}
