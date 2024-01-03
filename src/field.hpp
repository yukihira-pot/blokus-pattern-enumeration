#include <array>
#include <bitset>
#include <cassert>
#include <fstream>
#include <iostream>

#pragma once
#include "blocks.hpp"
#include "directions.hpp"
#include "players.hpp"

// 上下左右の 4 近傍
constexpr short adjacent_neighbouring_dx[] = {-1, 1, 0, 0};
constexpr short adjacent_neighbouring_dy[] = {0, 0, -1, 1};
// 斜めの 4 近傍
constexpr short diagonal_neighbouring_dx[] = {-1, -1, 1, 1};
constexpr short diagonal_neighbouring_dy[] = {-1, 1, -1, 1};

// フィールドの縦横の長さ
constexpr std::size_t FIELD_WIDTH = 20;

/// @brief 盤面を表すクラス. 符号なし 8 bit 整数で, 上位 6 bit はターン数
/// (1-indexed), 下位 2 bit はプレイヤー (00 ~ 11) を表す
/// 例：
/// マスの値が 0000_0000
/// であれば, そのマスにはどのブロックも置かれていないことを表す. マスの値が
/// 0000_1001 であれば, そのマスはプレイヤー 1 が 2
/// 手目に置いたブロックによって使われていることを表す.
class Field {
    std::array<std::array<short, FIELD_WIDTH>, FIELD_WIDTH> _field{};

    bool is_in_field(const unsigned short x, const unsigned short y) {
        return 0 <= x and x < FIELD_WIDTH and 0 <= y and y < FIELD_WIDTH;
    }

    /// @brief 上下左右の 4 近傍に自分のブロックがあるかを返す
    bool has_my_block_in_adjacent_neighbouring_position(const short x,
                                                        const short y,
                                                        const Player &player) {
        for(unsigned short i = 0; i < 4; i++) {
            short nx = x + adjacent_neighbouring_dx[i];
            short ny = y + adjacent_neighbouring_dy[i];
            if(!is_in_field(nx, ny)) {
                continue;
            }
            // 上下左右の隣接マスに自分が置いていたら true を返す
            if(_field[nx][ny] != 0 and (_field[nx][ny] & 0b11) == player) {
                return true;
            }
        }
        return false;
    }

    /// @brief 斜めの 4 近傍に自分のブロックがあるかを返す
    bool has_my_block_in_diagonal_neighbouring_position(const short x,
                                                        const short y,
                                                        const Player &player) {
        for(unsigned short i = 0; i < 4; i++) {
            short nx = x + diagonal_neighbouring_dx[i];
            short ny = y + diagonal_neighbouring_dy[i];
            if(!is_in_field(nx, ny)) {
                continue;
            }
            // 斜めの隣接マスの少なくとも 1 つに自分が置いていればよい
            if(_field[nx][ny] != 0 and (_field[nx][ny] & 0b11) == player) {
                return true;
            }
        }
        return false;
    }

  public:
    unsigned short current_turn = 0;
    Field() {}

    /// @brief 座標 (x, y) にブロックを配置可能か判断する.
    /// ブロックを置きたいマスがすべて 0 かどうかをチェックする
    /// @param x x 座標
    /// @param y y 座標
    /// @param block 使用するブロック
    bool is_able_to_place(short x, short y, const Block &block,
                          const Player &player) {
        assert(is_in_field(x, y));

        // 1. 上下左右の 4 近傍に自分が置いたブロックが 1 つでもあれば NG
        // 2.
        // ブロックを構成する各マスについて、そのいずれもが斜めに自分のブロックと接していなければ
        // NG
        // 3. 置きたいマスが他のブロックにすでに占有されていれば NG

        // 各プレイヤー 1 ターン目であれば、四隅以外は false を返す
        // 3...0
        // .....
        // .....
        // .....
        // 2...1
        if(current_turn == 0) {
            if(!(x == 0 and y == FIELD_WIDTH - 1)) {
                return false;
            }
        } else if(current_turn == 1) {
            if(!(x == FIELD_WIDTH - 1 and y == FIELD_WIDTH - 1)) {
                return false;
            }
        } else if(current_turn == 2) {
            if(!(x == FIELD_WIDTH - 1 and y == 0)) {
                return false;
            }
        } else if(current_turn == 3) {
            if(!(x == 0 and y == 0)) {
                return false;
            }
        }

        // ブロックを配置できるか (ブロックを配置したいマスがすべて 0000_0000
        // か) をチェック
        unsigned short field_value = _field[x][y];

        // 上下左右の 4 近傍に自分が置いたブロックがないかチェック
        bool has_my_block_in_adjacent_position =
            has_my_block_in_adjacent_neighbouring_position(x, y, player);

        // ブロックを構成する各マスについて、そのいずれかが斜めに自分のブロックと接しているかをチェック
        // current_turn が 0, 1, 2, 3 のときは、各プレイヤー初手なので特別に true とする
        bool has_my_block_in_diagonal_position =
            has_my_block_in_diagonal_neighbouring_position(x, y, player) ||
            current_turn <= 3;

        for(const Direction &direction : block) {
            x += direction.dx;
            y += direction.dy;
            // フィールド外からはみ出ていたら false を返す
            if(!is_in_field(x, y)) {
                return false;
            }
            // 「今まで通ってきたマスの値」と OR を取る
            field_value |= _field[x][y];
            // 「今まで通ってきたマスが、斜めに自分のブロックと接していたかどうか」と
            // OR を取る
            has_my_block_in_diagonal_position |=
                has_my_block_in_diagonal_neighbouring_position(x, y, player);
            // 「今まで通ってきたマスの中に、自分のマスと接しているものがあるかどうか」と
            // OR を取る
            has_my_block_in_adjacent_position |=
                has_my_block_in_adjacent_neighbouring_position(x, y, player);
        }

        return (field_value == 0) && has_my_block_in_diagonal_position && !has_my_block_in_adjacent_position;
    }

    /// @brief 座標 (x, y) にブロックを置く
    /// @param x x 座標
    /// @param y y 座標
    /// @param block 置くべきブロック
    /// @param player ブロックを置くプレイヤー
    void place(unsigned short x, unsigned short y, const Block &block,
               const Player &player) {
        assert(is_able_to_place(x, y, block, player));
        // ターンを 1 増やす
        current_turn++;
        // 埋めるべきフィールドの値は, 上位 6 bit をターン, 下位 2 bit
        // をプレイヤーの番号としたもの
        unsigned short field_value = (current_turn << 2) | player;
        _field[x][y] = field_value;
        for(const Direction &direction : block) {
            x += direction.dx;
            y += direction.dy;
            _field[x][y] = field_value;
        }
    }

    /// @brief 座標 (x, y) からブロック block を除去可能か判断する.
    /// @param x x 座標
    /// @param y y 座標
    /// @param block 除去するブロック
    bool is_able_to_remove(unsigned short x, unsigned short y, Block &block) {
        assert(is_in_field(x, y));
        unsigned short field_value = _field[x][y];
        for(const Direction &direction : block) {
            x += direction.dx;
            y += direction.dy;
            // フィールド外からはみ出ていたら false を返す
            if(!is_in_field(x, y)) {
                return false;
            }
            // 指定していた形の通りにブロックが置かれていなければ false を返す
            if(_field[x][y] != field_value) {
                return false;
            }
        }
        return true;
    }

    /// @brief 座標 (x, y) からブロック block を除去する.
    /// @param x x 座標
    /// @param y y 座標
    /// @param block 除去するブロック
    void remove(unsigned short x, unsigned short y, Block &block) {
        assert(is_able_to_remove(x, y, block));
        // ターンを 1 減らす
        current_turn--;
        // ブロックが置いてあるマスをすべて 0 にする
        _field[x][y] = 0;
        for(const Direction &direction : block) {
            x += direction.dx;
            y += direction.dy;
            _field[x][y] = 0;
        }
    }

    /// @brief _field を指定したファイルパスに出力する
    /// @param file_path 出力先のファイルパス
    void save_to_file(const std::string &file_path) {
        std::ofstream file(file_path);
        if(!file) {
            throw std::runtime_error("Failed to open file: " + file_path);
        }

        for(const auto &row : _field) {
            for(const auto &cell : row) {
                file << cell << ' ';
            }
            file << '\n';
        }
    }

    void show() {
        for(const auto &row : _field) {
            for(const auto &cell : row) {
                std::cerr << std::setw(2) << std::setfill('0') << (cell & 0b11)
                          << "," << std::setw(3) << std::setfill('0')
                          << (cell >> 2) << " ";
            }
            std::cerr << std::endl;
        }
        std::cerr << std::endl;
    }
};
