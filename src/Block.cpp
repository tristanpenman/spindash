#include <cstring>
#include <stdexcept>

#include "Pattern.h"

#include "Block.h"

void Block::fromSegaFormat(uint8_t buffer[BLOCK_SIZE_IN_ROM])
{
  for (unsigned int i = 0; i < PATTERNS_PER_BLOCK; i++) {
    uint16_t index = (static_cast<uint16_t>(buffer[0]) << 8) & 0xFF00;
    index |= (buffer[1]) & 0x00FF;

    // Set index
    m_patternDescs[i].set(index);

    buffer += PatternDesc::getIndexSize();
  }
}

const PatternDesc& Block::getPatternDesc(uint8_t x, uint8_t y) const
{
  if (x > 1 || y > 1) {
    throw std::runtime_error("Invalid pattern index");
  }

  return m_patternDescs[y * 2 + x];
}
