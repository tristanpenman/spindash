#pragma once

#include <cstdint>
#include <vector>

#include "../RingGroup.h"

class Sonic2RingLayout
{
public:
    static std::vector<RingGroup> read(const std::vector<uint8_t>& data);
};
