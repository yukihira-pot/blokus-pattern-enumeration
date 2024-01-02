#include <cassert>
#include <vector>

#include "directions.hpp"

struct Block {
    std::vector<Direction> block_directions;
    Block(std::initializer_list<Direction> block_list) {
        for (Direction block_direction : block_list) {
            block_directions.push_back(block_direction);
        }
    }
};
