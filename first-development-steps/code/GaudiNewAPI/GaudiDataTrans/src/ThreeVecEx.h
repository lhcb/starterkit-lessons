#pragma once

#include <iostream>

struct ThreeVecEx {
    int x;
    int y;
    int z;
};

inline std::ostream& operator<< (std::ostream& io, const ThreeVecEx& vec) {
    return io << vec.x << " "
              << vec.y << " "
              << vec.z;
}

