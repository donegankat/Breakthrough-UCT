/*
 *  PlayRandom.h
 *  Breakthrough UCT
 *
 *  Created by K Donegan on 2/24/13.
 *  Copyright 2013 University of Denver. All rights reserved.
 *
 */

#ifndef PLAYRANDOM_H
#define PLAYRANDOM_H

#include "Game.h"

PieceAction next_move(BoardState s);
int random_play_game(BoardState s,  int playerNum);

#endif