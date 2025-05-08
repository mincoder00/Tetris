#include "tetris.h"
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>

using namespace std;

void init_game(GameState& state) {
    srand(888);
    state.board = std::vector<std::vector<char>>(BOARD_HEIGHT, std::vector<char>(BOARD_WIDTH, '.'));
    state.current_block = std::vector<std::vector<int>>(4, std::vector<int>(4, 0));
    state.tetrominoes = {
        // I-tetromino (2 rotations)
        {
            {
                {0, 0, 0, 0},
                {1, 1, 1, 1},
                {0, 0, 0, 0},
                {0, 0, 0, 0}
            },
            {
                {0, 0, 1, 0},
                {0, 0, 1, 0},
                {0, 0, 1, 0},
                {0, 0, 1, 0}
            }
        },
        // O-tetromino (1 rotation)
        {
            {
                {0, 1, 1, 0},
                {0, 1, 1, 0},
                {0, 0, 0, 0},
                {0, 0, 0, 0}
            }
        },
        // T-tetromino (4 rotations)
        {
            {
                {0, 1, 0, 0},
                {1, 1, 1, 0},
                {0, 0, 0, 0},
                {0, 0, 0, 0}
            },
            {
                {0, 1, 0, 0},
                {0, 1, 1, 0},
                {0, 1, 0, 0},
                {0, 0, 0, 0}
            },
            {
                {0, 0, 0, 0},
                {1, 1, 1, 0},
                {0, 1, 0, 0},
                {0, 0, 0, 0}
            },
            {
                {0, 1, 0, 0},
                {1, 1, 0, 0},
                {0, 1, 0, 0},
                {0, 0, 0, 0}
            }
        },
        // S-tetromino (2 rotations)
        {
            {
                {0, 1, 1, 0},
                {1, 1, 0, 0},
                {0, 0, 0, 0},
                {0, 0, 0, 0}
            },
            {
                {0, 1, 0, 0},
                {0, 1, 1, 0},
                {0, 0, 1, 0},
                {0, 0, 0, 0}
            }
        },
        // Z-tetromino (2 rotations)
        {
            {
                {1, 1, 0, 0},
                {0, 1, 1, 0},
                {0, 0, 0, 0},
                {0, 0, 0, 0}
            },
            {
                {0, 0, 1, 0},
                {0, 1, 1, 0},
                {0, 1, 0, 0},
                {0, 0, 0, 0}
            }
        },
        // J-tetromino (4 rotations)
        {
            {
                {1, 0, 0, 0},
                {1, 1, 1, 0},
                {0, 0, 0, 0},
                {0, 0, 0, 0}
            },
            {
                {0, 1, 1, 0},
                {0, 1, 0, 0},
                {0, 1, 0, 0},
                {0, 0, 0, 0}
            },
            {
                {0, 0, 0, 0},
                {1, 1, 1, 0},
                {0, 0, 1, 0},
                {0, 0, 0, 0}
            },
            {
                {0, 1, 0, 0},
                {0, 1, 0, 0},
                {1, 1, 0, 0},
                {0, 0, 0, 0}
            }
        },
        // L-tetromino (4 rotations)
        {
            {
                {0, 0, 1, 0},
                {1, 1, 1, 0},
                {0, 0, 0, 0},
                {0, 0, 0, 0}
            },
            {
                {0, 1, 0, 0},
                {0, 1, 0, 0},
                {0, 1, 1, 0},
                {0, 0, 0, 0}
            },
            {
                {0, 0, 0, 0},
                {1, 1, 1, 0},
                {1, 0, 0, 0},
                {0, 0, 0, 0}
            },
            {
                {1, 1, 0, 0},
                {0, 1, 0, 0},
                {0, 1, 0, 0},
                {0, 0, 0, 0}
            }
        }
    };

    state.current_tetromino = 0;
    state.current_rotation = 0;
    state.block_x = 0;
    state.block_y = 0;
    state.score = 0;
    state.level = 1;
    state.lines_cleared = 0;
    state.high_score = 2200;
    state.running = true;
    state.next_tetromino = rand() % state.tetrominoes.size();
    state.fall_timer = 0;
    state.fall_interval = 10 - (state.level - 1);
    if (state.fall_interval < 1) state.fall_interval = 1;
    spawn_new_block(state);
}


void handle_game_action(GameState& state, int action) {
    switch (action) {
    case ACTION_QUIT:
        state.running = false;
        break;
    case ACTION_LEFT:
        move_block(state, -1, 0);
        break;
    case ACTION_RIGHT:
        move_block(state, 1, 0);
        break;
    case ACTION_DOWN:
        move_block(state, 0, 1);
        break;
    case ACTION_ROTATE:
        rotate_block(state);
        break;
    case ACTION_DROP:
        drop_block_to_bottom(state);
        break;
    default:
        break;
    }
}

bool is_valid_position(const std::vector<std::vector<char>>& board, int x, int y, const std::vector<std::vector<int>>& block) {
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            if (block[i][j]) {
                int nx = x + j;
                int ny = y + i;
                if (nx < 0 || nx > BOARD_WIDTH - 1 ||
                    ny < 0 || ny > BOARD_HEIGHT - 1 ||
                    board[ny][nx] == '#') return false;
            }
        }
    }
    return true;
}

void rotate_block(GameState& state) {
    int now_block = state.current_tetromino;
    int next_rotate = (state.current_rotation + 1) % state.tetrominoes[now_block].size();
    if (is_valid_position(state.board, state.block_x, state.block_y, state.tetrominoes[now_block][next_rotate]))
    {
        state.current_rotation = next_rotate;
        state.current_block = state.tetrominoes[state.current_tetromino][state.current_rotation];
    }
}

void land_block(GameState& state) {
    for (int i = 0; i < 4; i++)
    {
        int y = state.block_y + i;
        for (int j = 0; j < 4; j++)
        {
            int x = state.block_x + j;
            if (state.current_block[i][j] == 1) state.board[y][x] = '#';
        }
    }
}

bool move_block(GameState& state, int dx, int dy) {
    auto nx = state.block_x + dx;
    auto ny = state.block_y + dy;
    if (dx == 0 && dy == 1)
    {
        if (!is_valid_position(state.board, nx, ny, state.current_block))
        {
            land_block(state);
            spawn_new_block(state);
            return false;
        }
    }
    if (is_valid_position(state.board, nx, ny, state.current_block)) {
        state.block_x = nx;
        state.block_y = ny;
        return true;
    }
    else return false;
}

void drop_block_to_bottom(GameState& state) {
    // Implmenet your code
    while (move_block(state, 0, 1)) {}
}

void spawn_new_block(GameState& state) {
    // Implmenet your code
    state.block_x = BOARD_WIDTH / 2 - 2;
    state.block_y = 0;
    state.current_tetromino = state.next_tetromino;
    state.current_rotation = 0;
    state.next_tetromino = rand() % state.tetrominoes.size();
    state.current_block = state.tetrominoes[state.current_tetromino][0];

    if (!is_valid_position(state.board, state.block_x, state.block_y, state.current_block))
        state.running = false;
}

void check_lines(GameState& state) {
    int score = 0;
    vector<char> clear(BOARD_WIDTH, '#');
    for (int i = 0; i < BOARD_HEIGHT; i++)
    {
        if (state.board[i] == clear)
        {
            score += 100 * state.level;
            for (int j = i; j > 0; j--) state.board[j] = state.board[j - 1];
        }
    }

    state.score += score;
}

void update_game(GameState& state) {
    if (++state.fall_timer >= state.fall_interval) {
        // Implmenet your code
        // move block
        // check landing
        check_lines(state);
        move_block(state, 0, 1);
        state.fall_timer = 0;
        state.fall_interval = 10 - (state.level - 1);
        if (state.fall_interval < 1) state.fall_interval = 1;
    }
}
// Implement your code