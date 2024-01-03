#include <cassert>
#include <vector>

#pragma once
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

struct Blocks {
    Direction up, down, right, left;
    Blocks(int mode_) {
        assert(0 <= mode_ and mode_ <= 7);
        Directions directions(mode_);
        up = directions.directions[0];
        down = directions.directions[1];
        right = directions.directions[2];
        left = directions.directions[3];
    }

    // ...
    // .s.
    // ...
    Block monomino = Block({});

    // ...
    // .#.
    // .s.
    // ...
    Block domino = Block({up});

    // ...
    // .#.
    // .#.
    // .s.
    // ...
    Block tromino_i = Block({up, up});

    // ....
    // .#..
    // .#s.
    // ....
    Block tromino_l = Block({left, up});

    // .....
    // ###s.
    // .....
    Block tetromino_straight = Block({left, left, left});

    // ....
    // .##.
    // .#s.
    // ....
    Block tetromino_square = Block({left, down, right});

    // .....
    // .##s.
    // ..#..
    // .....
    Block tetromino_t = Block({left, down, up, left});

    // ....
    // .#..
    // .#..
    // .#s.
    // ....
    Block tetromino_l = Block({left, up, up});

    // .....
    // ..#s.
    // .##..
    // .....
    Block tetromino_skew = Block({left, down, left});

    // .....
    // ..#s.
    // .##..
    // ..#..
    // .....
    Block pentomino_f = Block({left, down, left, right, down});

    // ...
    // .#.
    // .#.
    // .#.
    // .#.
    // .s.
    // ...
    Block pentomino_i = Block ({up, up, up, up});

    // ....
    // .#..
    // .#..
    // .#..
    // .#s.
    // ....
    Block pentomino_l = Block({left, up, up, up});

    // ....
    // ..#.
    // ..#.
    // .##.
    // .s..
    // ....
    Block pentomino_n = Block({up, right, up, up});

    // ....
    // .##.
    // .##.
    // .s..
    // ....
    Block pentomino_p = Block({up, up, right, down});

    // .....
    // .###.
    // ..#..
    // ..s..
    // .....
    Block pentomino_t = Block({up, up, left, right, right});

    // .....
    // .#.s.
    // .###.
    // .....
    Block pentomino_u = Block({down, left, left, up});

    // .....
    // .#...
    // .#...
    // .##s.
    // .....
    Block pentomino_v = Block({left, left, up, up});

    // .....
    // .#...
    // .##..
    // ..#s.
    // .....
    Block pentomino_w = Block({left, up, left, up});

    // .....
    // ..#..
    // .###.
    // ..s..
    // .....
    Block pentomino_x = Block({up, left, right, right, left, up});

    // ......
    // ...#..
    // .###s.
    // ......
    Block pentomino_y = Block({left, up, down, left, left});

    // .....
    // .##..
    // ..#..
    // ..#s.
    // .....
    Block pentomino_z = Block({left, up, up, left});
};