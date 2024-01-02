#include <array>
#include <cassert>
#include <utility>

class Direction {
   public:
    short dx, dy;
    Direction() : dx(0), dy(0) {}
    Direction(short dx_, short dy_) : dx(dx_), dy(dy_) {}
};

constexpr unsigned short DIRECTION_SIZE = 4;
Direction default_up(1, 0);
Direction default_down(-1, 0);
Direction default_right(0, 1);
Direction default_left(0, -1);

class Directions {
    public:
    std::array<Direction, DIRECTION_SIZE> directions{};

   public:
    Directions(int mode_) {
        assert(0 <= mode_ and mode_ <= 7);
        init(mode_);
    }

   private:
    void init(int mode) {
        std::array<Direction, DIRECTION_SIZE> directions_impl{};
        // mode が 0 から 7 で 4 方位と裏表を設定する
        if (mode <= 3) {
            directions_impl = {default_up, default_down, default_right,
                               default_left};
        } else {
            // 左右を逆にして裏返しとする
            directions_impl = {default_up, default_down, default_left,
                               default_right};
        }
        for (unsigned short i = 0; i < DIRECTION_SIZE; i++) {
            directions[i] = directions_impl[(i + mode) % DIRECTION_SIZE];
        }
    }
};
