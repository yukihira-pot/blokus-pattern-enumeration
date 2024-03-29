#include <array>
#include <cassert>
#include <iostream>
#include <vector>

#pragma once
#include "directions.hpp"

constexpr unsigned short BLOCK_SIZE = 21;
constexpr unsigned short BLOCK_MODE_SIZE = 8;
constexpr unsigned short TOTAL_BLOCK_SIZE = BLOCK_SIZE * BLOCK_MODE_SIZE;

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
    std::size_t size() const { return block_directions.size(); }

    friend std::ostream& operator<<(std::ostream&, const Block&);
};

std::ostream& operator<<(std::ostream& stream, const Block& block) {
    constexpr short DISP_WIDTH = 20;
    std::array<std::array<char, DISP_WIDTH>, DISP_WIDTH> disp{};
    short x = DISP_WIDTH / 2;
    short y = DISP_WIDTH / 2;
    disp[x][y] = 's';
    for (const Direction& direction : block) {
        x += direction.dx;
        y += direction.dy;
        disp[x][y] = '#';
    }
    short min_row = DISP_WIDTH, max_row = 0;
    short min_col = DISP_WIDTH, max_col = 0;
    for (short i = 0; i < DISP_WIDTH; i++) {
        for (short j = 0; j < DISP_WIDTH; j++) {
            if (disp[i][j] == '#' or disp[i][j] == 's') {
                min_row = std::min(min_row, i);
                max_row = std::max(max_row, i);
                min_col = std::min(min_col, j);
                max_col = std::max(max_col, j);
            } else {
                disp[i][j] = '.';
            }
        }
    }
    // [min_row-1, max_row+1], [min_col-1, max_col+1] の範囲を出力
    for (short i = min_row - 1; i <= max_row + 1; i++) {
        for (short j = min_col - 1; j <= max_col + 1; j++) {
            stream << disp[i][j];
        }
        stream << "\n";
    }
    return stream;
}

struct Blocks {
    Direction up, down, right, left;
    std::array<Block, BLOCK_SIZE> blocks{};

    Blocks(int mode_) {
        assert(0 <= mode_ and mode_ <= 7);
        Directions directions(mode_);
        up = directions.directions[0];
        left = directions.directions[1];
        down = directions.directions[2];
        right = directions.directions[3];
        setup_blocks();
    }

    const Block& operator[](std::size_t index) const { return blocks[index]; }
    std::size_t size() const { return blocks.size(); }

    void setup_blocks() {
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
        Block pentomino_i = Block({up, up, up, up});

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

        blocks = {
            monomino,           domino,           tromino_i,   tromino_l,
            tetromino_straight, tetromino_square, tetromino_t, tetromino_l,
            tetromino_skew,     pentomino_f,      pentomino_i, pentomino_l,
            pentomino_n,        pentomino_p,      pentomino_t, pentomino_u,
            pentomino_v,        pentomino_w,      pentomino_x, pentomino_y,
            pentomino_z};
    }
};
