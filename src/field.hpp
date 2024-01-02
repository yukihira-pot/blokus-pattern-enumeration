#include <array>
#include "players.hpp"

class Field {
    Field() {
        constexpr std::size_t FIELD_WIDTH = 20;
        std::array<std::array<short, FIELD_WIDTH>, FIELD_WIDTH> _field{};
    }
};