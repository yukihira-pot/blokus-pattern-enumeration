#include <algorithm>
#include <array>

#include "blocks.hpp"
#include "field.hpp"
#include "players.hpp"
#include "position.hpp"

class Solver {
    Field _field;
    // 各プレイヤーのブロック集合を表す配列 (回転, 裏返し含む) .
    // _blocks[i] := プレイヤー i のブロック集合.
    // 8 つごとに回転と裏返しで同じブロックを表す.
    // (i.e. j を 8 の倍数として, _blocks[i][j], ..., _blocks[i][j + 8]
    // は回転と裏返しにより一致する)
    std::array<std::array<Block, TOTAL_BLOCK_SIZE>, PLAYER_SIZE>
        _blocks{};
    // 各プレイヤーについて, どのブロックが使用済みかどうかを借りする配列.
    // _used[i][j] := プレイヤー i のブロック j が使用済みかどうか
    std::array<std::array<bool, TOTAL_BLOCK_SIZE>, PLAYER_SIZE>
        _used{};

  public:
    Solver() { setup_blocks(); }

  private:
    /// @brief 各プレイヤーの保持するブロック集合に対し、各 mode
    /// の各ブロックを割り当てる
    void setup_blocks() {
        // TOTAL_BLOCK_SIZE == BLOCK_SIZE * BLOCK_MODE_SIZE
        for(unsigned short player = 0; player < PLAYER_SIZE; player++) {
            for(unsigned short mode = 0; mode < BLOCK_MODE_SIZE; mode++) {
                Blocks blocks_impl = Blocks(mode);
                for(unsigned short i = 0; i < BLOCK_SIZE; i++) {
                    _blocks[player][BLOCK_MODE_SIZE * i + mode] =
                        blocks_impl[i];
                }
            }
        }
    }

    bool
    is_block_used(const std::array<bool, TOTAL_BLOCK_SIZE> &block) {
        return std::all_of(block.begin(), block.end(),
                           [](bool y) { return y; });
    }

    bool is_all_blocks_used() {
        return std::all_of(
            _used.begin(), _used.end(),
            [this](
                const std::array<bool, TOTAL_BLOCK_SIZE> &block) {
                return is_block_used(block);
            });
    }
};
