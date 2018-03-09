/*
 *  UCT.h
 *  Breakthrough UCT
 *
 *  Created by K Donegan on 2/23/13.
 *  Copyright 2013 University of Denver. All rights reserved.
 *
 */

#include "Game.h"
#include "Eval.h"


struct StateArray
{
	BoardState current;
	PieceAction move_taken;
	int payoff;
	int times_sampled;
	int turn;
	
	StateArray()
	{
		current = NullBoard();
		move_taken = PieceAction();
		payoff = 0;
		times_sampled = 0;
		turn = 0;
	}
	
	StateArray(BoardState s)
	{
		current = s;
		move_taken = PieceAction();
		payoff = 0;
		times_sampled = 0;
		turn = s.playerTurn;
	}
	
	StateArray(BoardState s, PieceAction a, int p, int t)
	{
		current = s;
		move_taken = a;
		payoff = p;
		times_sampled = t;
		turn = s.playerTurn;
	}
};

void RunUCT(BoardState root);