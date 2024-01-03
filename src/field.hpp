#include <array>
#include <cassert>
#include <fstream>

#pragma once
#include "blocks.hpp"
#include "directions.hpp"
#include "players.hpp"

// 上下左右の 4 近傍
constexpr unsigned short adjacent_neighbouring_dx[] = {-1, 1, 0, 0};
constexpr unsigned short adjacent_neighbouring_dy[] = {0, 0, -1, 1};
// 斜めの 4 近傍
constexpr unsigned short diagonal_neighbouring_dx[] = {-1, -1, 1, 1};
constexpr unsigned short diagonal_neighbouring_dy[] = {-1, 1, -1, 1};

// フィールドの縦横の長さ
constexpr std::size_t FIELD_WIDTH = 20;

/// @brief 盤面を表すクラス. 符号なし 8 bit 整数で, 上位 6 bit はターン数
/// (1-indexed), 下位 2 bit はプレイヤー (00 ~ 11) を表す
/// 例：
/// マスの値が 0000_0000
/// であれば、そのマスにはどのブロックも置かれていないことを表す. マスの値が
/// 0000_1001 であれば、そのマスはプレイヤー 1 が 2
/// 手目に置いたブロックによって使われていることを表す.
class Field {
    std::array<std::array<short, FIELD_WIDTH>, FIELD_WIDTH> _field{};
    unsigned short current_turn = 0;

   public:
    Field() {}

    /// @brief 座標 (x, y) にブロックを配置可能か判断する.
    /// ブロックを置きたいマスがすべて 0 かどうかをチェックする
    /// @param x x 座標
    /// @param y y 座標
    /// @param block 使用するブロック
    bool is_able_to_place(unsigned short x, unsigned short y, Block& block, Player& player) {
        assert(0 <= x and x < FIELD_WIDTH and 0 <= y and y < FIELD_WIDTH);

        // 上下左右の 4 近傍に自分が置いたブロックがないかチェック
        for (unsigned short i = 0; i < 4; i++) {
            unsigned short nx = x + adjacent_neighbouring_dx[i];
            unsigned short ny = y + adjacent_neighbouring_dy[i];
            if (!(0 <= nx and x < FIELD_WIDTH and 0 <= ny and ny < FIELD_WIDTH)) {
                continue;
            }
            // 上下左右の隣接マスに自分が置いていたら false を返す
            if (_field[nx][ny] != 0 and (_field[nx][ny] & 0b11) == player) {
                return false;
            }
        }

        // 斜めの 4 近傍に自分が置いたブロックが少なくとも 1 つあるかチェック
        bool has_my_block_in_diagonal_position = false;
        for (unsigned short i = 0; i < 4; i++) {
            unsigned short nx = x + diagonal_neighbouring_dx[i];
            unsigned short ny = y + diagonal_neighbouring_dy[i];
            if (!(0 <= nx and x < FIELD_WIDTH and 0 <= ny and ny < FIELD_WIDTH)) {
                continue;
            }
            // 斜めの隣接マスの少なくとも 1 つに自分が置いていればよい
            if (_field[nx][ny] != 0 and (_field[nx][ny] & 0b11) == player) {
                has_my_block_in_diagonal_position = true;
                break;
            }
        }
        if (!has_my_block_in_diagonal_position) {
            return false;
        }

        // ブロックを配置できるか (ブロックを配置したいマスがすべて 0000_0000 か) をチェック
        unsigned short result = _field[x][y];
        for (const Direction& direction : block) {
            x += direction.dx;
            y += direction.dy;
            // フィールド外からはみ出ていたら false を返す
            if (!(0 <= x and x < FIELD_WIDTH and 0 <= y and y < FIELD_WIDTH)) {
                return false;
            }
            result |= _field[x][y];
        }
        return result != 0;
    }

    /// @brief 座標 (x, y) にブロックを置く
    /// @param x x 座標
    /// @param y y 座標
    /// @param block 置くべきブロック
    /// @param player ブロックを置くプレイヤー
    void place(unsigned short x, unsigned short y, Block& block,
               Player& player) {
        assert(is_able_to_place(x, y, block, player));
        // ターンを 1 増やす
        current_turn++;
        // 埋めるべきフィールドの値は、上位 6 bit をターン、下位 2 bit
        // をプレイヤーの番号としたもの
        unsigned short field_value = (current_turn << 2) | player;
        _field[x][y] = field_value;
        for (const Direction& direction : block) {
            x += direction.dx;
            y += direction.dy;
            _field[x][y] = field_value;
        }
    }

    /// @brief 座標 (x, y) からブロック block を除去可能か判断する.
    /// @param x x 座標
    /// @param y y 座標
    /// @param block 除去するブロック
    bool is_able_to_remove(unsigned short x, unsigned short y, Block& block) {
        assert(0 <= x and x < FIELD_WIDTH and 0 <= y and y < FIELD_WIDTH);
        unsigned short field_value = _field[x][y];
        for (const Direction& direction : block) {
            x += direction.dx;
            y += direction.dy;
            // フィールド外からはみ出ていたら false を返す
            if (!(0 <= x and x < FIELD_WIDTH and 0 <= y and y < FIELD_WIDTH)) {
                return false;
            }
            // 指定していた形の通りにブロックが置かれていなければ false を返す
            if (_field[x][y] != field_value) {
                return false;
            }
        }
        return true;
    }

    /// @brief 座標 (x, y) からブロック block を除去する.
    /// @param x x 座標
    /// @param y y 座標
    /// @param block 除去するブロック
    void remove(unsigned short x, unsigned short y, Block& block) {
        assert(is_able_to_remove(x, y, block));
        // ブロックが置いてあるマスをすべて 0 にする
        _field[x][y] = 0;
        for (const Direction& direction : block) {
            x += direction.dx;
            y += direction.dy;
            _field[x][y] = 0;
        }
    }

    /// @brief _field を指定したファイルパスに出力する
    /// @param file_path 出力先のファイルパス
    void save_to_file(const std::string& file_path) {
        std::ofstream file(file_path);
        if (!file) {
            throw std::runtime_error("Failed to open file: " + file_path);
        }

        for (const auto& row : _field) {
            for (const auto& cell : row) {
                file << cell << ' ';
            }
            file << '\n';
        }
    }
};
