/*
 *  UCT.cpp
 *  Breakthrough UCT
 *
 *  Created by K Donegan on 2/23/13.
 *  Copyright 2013 University of Denver. All rights reserved.
 *
 */

#include "UCT.h"
#include "Timer.h"
#include "Game.h"
#include "Eval.h"
#include "PlayRandom.h"
#include <iostream>

//Timer *myTimer = new Timer();

//BoardState root = GetStartState()
int best_payoff;
int worst_payoff;
PieceAction best_move_UCT;
PieceAction worst_move_UCT;


void RunUCT(BoardState root)
{
	std::vector<PieceAction> moves;
	std::vector<StateArray> state_array;
	
	if(root.playerTurn == 1)
	{
		std::cout << "Player 1 turn" << std::endl;
		moves = GetLegalActions1(root);
		std::cout << "Moves size: " << moves.size() << std::endl;
	}
	else {
		std::cout << "Player 2 turn" << std::endl;
		moves = GetLegalActions2(root);
		std::cout << "Moves size: " << moves.size() << std::endl;
	}
	
	if(moves.size() == 0)
	{
		std::cout << "No moves available" << std::endl;
		return;
	}

	int *payoffs = new int[moves.size()];

	for(int i = 0; i < moves.size(); i++)
	{
		payoffs[i] = 0;
		BoardState temp = ApplyAction(root, moves[i]);
		
		if(GetGameOver(temp) == 1)
		{	
			std::cout << "Player 1 win from: " << moves[i].from << " to: " << moves[i].to << "\nWith the board: " << std::endl;
			PrintBoard(temp);
			return;
		}
		else if(GetGameOver(temp) == 2)
		{
			std::cout << "Player 2 win from: " << moves[i].from << " to: " << moves[i].to << "\nWith the board: " << std::endl;
			PrintBoard(temp);
			return;
		}
		
		int rand_play_result;
		
		if(root.playerTurn == 1)
		{
			rand_play_result = random_play_game(temp, 2);
			payoffs[i] = rand_play_result;
		}
		else
		{
			rand_play_result = random_play_game(temp, 1);
			payoffs[i] = rand_play_result;
		}

	}

	int highest_payoff = 0;
	int lowest_payoff = 0;
	int highest_index = 0;
	int lowest_index = 0;
	for(int i = 0; i < moves.size(); i++)
	{
		if(root.playerTurn == 1)
		{
			if(payoffs[i] > highest_payoff)
			{
				highest_index = i;
				highest_payoff = payoffs[i];
				best_move_UCT = moves[i];
			}
		}
		else
		{
			if(payoffs[i] < lowest_payoff)
			{
				lowest_index = i;
				lowest_payoff = payoffs[i];
				worst_move_UCT = moves[i];
			}
		}
		
		//std::cout << "Payoff " << i << ": " << payoffs[i] << std::endl;
	}
	
	std::vector<PieceAction> child_moves;
	BoardState try_child_move;
	int child_rand_result;
	if(root.playerTurn == 1)
	{
		std::cout << "Highest payoff: " << highest_payoff << " at index: " << highest_index << std::endl;
		
		BoardState best_child = ApplyAction(root, moves[highest_index]);
		child_moves = GetLegalActions2(best_child);
		try_child_move = ApplyAction(best_child, child_moves[0]);
		
		if(GetGameOver(try_child_move) == 1)
		{	
			std::cout << "Player 1 win from: " << child_moves[0].from << " to: " << child_moves[0].to << "\nWith the board: " << std::endl;
			PrintBoard(try_child_move);
			return;
		}
		else if(GetGameOver(try_child_move) == 2)
		{
			std::cout << "Player 2 win from: " << child_moves[0].from << " to: " << child_moves[0].to << "\nWith the board: " << std::endl;
			PrintBoard(try_child_move);
			return;
		}
		
		child_rand_result = random_play_game(try_child_move, 1);
		payoffs[highest_index] = child_rand_result;
		
		StateArray root_best_array = StateArray(best_child, moves[highest_index], payoffs[highest_index], 1);
		state_array.push_back(root_best_array);
	}
	else
	{
		std::cout << "Lowest payoff: " << lowest_payoff << " at index: " << lowest_index << std::endl;
		
		BoardState worst_child = ApplyAction(root, moves[lowest_index]);
		child_moves = GetLegalActions1(worst_child);
		try_child_move = ApplyAction(worst_child, child_moves[0]);
		
		if(GetGameOver(try_child_move) == 1)
		{	
			std::cout << "Player 1 win from: " << child_moves[0].from << " to: " << child_moves[0].to << "\nWith the board: " << std::endl;
			PrintBoard(try_child_move);
			return;
		}
		else if(GetGameOver(try_child_move) == 2)
		{
			std::cout << "Player 2 win from: " << child_moves[0].from << " to: " << child_moves[0].to << "\nWith the board: " << std::endl;
			PrintBoard(try_child_move);
			return;
		}
		
		child_rand_result = random_play_game(try_child_move, 2);
		payoffs[highest_index] = child_rand_result;
		
		StateArray root_worst_array = StateArray(worst_child, moves[lowest_index], payoffs[lowest_index], 1);
		state_array.push_back(root_worst_array);
	}
	
	std::vector<PieceAction> sequence;
	for(int i = 0; i < moves.size(); i++)
	{
		if((root.playerTurn == 1 && payoffs[i] > (highest_payoff/2)) || (root.playerTurn == 2 && payoffs[i] < (lowest_payoff/2)))
		{
			BoardState root_child = ApplyAction(root, moves[i]);
			
			if(GetGameOver(root_child) == 0)
			{
				StateArray new_array = StateArray(root_child, moves[i], payoffs[i], 1);
				
				state_array.push_back(new_array);
				
				/*
				if(root.playerTurn == 1)
				{
					root_child.playerTurn = 2;
				}
				else
				{
					root_child.playerTurn = 1;
				}
				*/
				
				//RunUCT(root_child);
			}
			else if(GetGameOver(root_child) == 1)
			{	
				std::cout << "Player 1 win from: " << state_array.back().move_taken.from << " to: " << state_array.back().move_taken.to << "\nWith the board: " << std::endl;
				PrintBoard(root_child);
			}
			else
			{
				std::cout << "Player 2 win from: " << state_array.back().move_taken.from << " to: " << state_array.back().move_taken.to << "\nWith the board: " << std::endl;
				PrintBoard(root_child);
			}
		}
	}
	
	
	
}

void PlayToEnd(BoardState root)
{
	std::vector<PieceAction> root_moves;
	
	if(root.playerTurn == 1)
	{
		root_moves = GetLegalActions1(root);
	}
	else {
		root_moves = GetLegalActions2(root);
	}
	
	BoardState new_state;
	for(int i = 0; i < root_moves.size(); i++)
	{
		new_state = ApplyAction(root, root_moves[i]);
		
		if(GetGameOver(new_state) == 1)
		{
			root_moves[i];
		}
	}

}

void BuildRandomTree()
{
	BoardState RandomStart = GetStartState();
	

}