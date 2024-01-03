#include <array>
#include <cassert>

#include "blocks.hpp"
#include "directions.hpp"
#include "players.hpp"

constexpr std::size_t FIELD_WIDTH = 20;

/// @brief 盤面を表すクラス. 符号なし 8 bit 整数で, 上位 6 bit はターン数
/// (1-indexed), 下位 2 bit はプレイヤー (00 ~ 11) を表す
/// 例：
/// マスの値が 0000_0000
/// であれば、そのマスにはどのブロックも置かれていないことを表す. マスの値が
/// 0000_1001 であれば、そのマスはプレイヤー 1 が 2
/// 手目に置いたブロックによって使われていることを表す.
class Field {
    std::array<std::array<short, FIELD_WIDTH>, FIELD_WIDTH> _field;
    unsigned short current_turn = 0;

   public:
    Field() {}

    /// @brief 座標 (x, y) にブロックを配置可能か判断する.
    /// ブロックを置きたいマスがすべて 0 かどうかをチェックする
    /// @param x x 座標
    /// @param y y 座標
    /// @param block 使用するブロック
    bool is_able_to_place(unsigned short x, unsigned short y, Block& block) {
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
        assert(is_able_to_place(x, y, block));
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
};