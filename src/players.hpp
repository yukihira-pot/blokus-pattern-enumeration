#pragma once
constexpr unsigned short PLAYER_SIZE = 4;

enum Player {
    PLAYER_A = 0, 
    PLAYER_B = 1, 
    PLAYER_C = 2, 
    PLAYER_D = 3
};

Player next_player(Player current) {
    return static_cast<Player>((static_cast<int>(current) + 1) % PLAYER_SIZE);
}