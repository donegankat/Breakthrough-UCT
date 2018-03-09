/*
 *  PlayRandom.cpp
 *  Breakthrough UCT
 *
 *  Created by K Donegan on 2/24/13.
 *  Copyright 2013 University of Denver. All rights reserved.
 *
 */

#include "PlayRandom.h"

#include <cstdlib>
#include <iostream>
using std::cerr;
using std::cout;
using std::endl;
#include <string>
using std::string;
#include <stdlib.h>
#include <time.h>

#include "Game.h"
#include "Math.h"
#include "Eval.h"

BoardState* board;
int current_player = 2;
int my_player = 1;
int opp_player = 2;
string name = "Random Player";
string opp_name = "Opponent";

const int board_size = 8; // We play on an 8x8 board


PieceAction next_move(BoardState s);
int random_play_game(BoardState s,  int playerNum);

PieceAction next_move(BoardState s)
{
	
	srand ((unsigned)clock());
	
	vector<PieceAction> moves;
	moves.clear();
	
	if(s.playerTurn == 1)
	{
		moves = GetLegalActions1(s);
	}
	else {
		moves = GetLegalActions2(s);
	}

    if (moves.size() > 0)
    {
        //unsigned int choice = Math::uniform_rand(static_cast<unsigned int>(moves.size()));
        int choice = rand() % moves.size();
		//cout << "Random choice is: " << choice << endl;
		return moves[choice];
    }

    return PieceAction(0, 0); //null move
}


int random_play_game(BoardState s, int playerNum)
{	
	current_player = playerNum;
	
    // Main game loop
    for (;;)
    {
        if (current_player == 1)
        {
            // My turn
            if (GetGameOver(s) == 2)
            {
                //cerr << "By looking at the board, I know that I, " << current_player << ", have lost." << endl;
				//PrintBoard(s);
                current_player = 2;
                return Evaluation(s);//2
            }
            // Determine next move
            PieceAction a = next_move(s);
		
			// Apply it
            s = ApplyAction(s, a);
			
            if (a.from == 0 && a.to == 0)
            {
                // Concede to the server so we know what is going on
                cout << "# I, " << current_player << ", have no moves to play." << endl;
				
                current_player = 2;
                // End game locally, server should detect and send #quit
                return Evaluation(s);//2
            }
			
            // Tell the world
//            PrintBoard(s);
			
            // It is the opponents turn
            current_player = 2;
			s.playerTurn = current_player;
        }
        else
        {
			// Opponent's turn
			if (GetGameOver(s) == 1)
			{
				current_player = 1;//(current_player == 2) ? 1 : 2;
				return Evaluation(s);//1
			}
			// Determine next move
			PieceAction a = next_move(s);
				
			// Apply it
			s = ApplyAction(s, a);
				
			if (a.from == 0 && a.to == 0)
			{
				// Concede to the server so we know what is going on
				cout << "# I, " << current_player << ", have no moves to play." << endl;
				
				current_player = 1;
				// End game locally, server should detect and send #quit
				return Evaluation(s);//1
			}
				
			// Tell the world
//			PrintBoard(s);
			
			// It is the opponents turn
			current_player = 1;//(current_player == 2) ? 1 : 2;
			s.playerTurn = current_player;
		}
		
	}
	return 0;
}

