#pragma once

#include "game_state.h"

void save_game(const GameState& g);
bool load_game(GameState& g);
void delete_save(void);