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

    std::vector<Direction>::const_iterator begin() const {
        return block_directions.begin();
    }
    std::vector<Direction>::const_iterator end() const {
        return block_directions.end();
    }

    const Direction& operator[](std::size_t index) const {
        return block_directions[index];
    }
    std::size_t size() const {
        return block_directions.size();
    }
};
