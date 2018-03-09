#include "Game.h"
#include "Alpha Beta.h"
#include "PlayRandom.h"
#include "UCT.h"
#include <vector>
#include <iostream>
#include <map>


const int Infinity = INT_MAX;
const int WinValue = INT_MAX;



int main()
{
	BoardState GameBoard = GetStartState();
	GameBoard.playerTurn = 1;
	PrintBoard(GameBoard);
	
//	random_play_game(GameBoard);
	
	while(!GameOver(GameBoard))
	{
		
		if(GameBoard.playerTurn == 1)
		{
			RunUCT(GameBoard);
			GameBoard.playerTurn = 2;
		}
		else {
			HumanTurn(GameBoard, 0);
			GameBoard.playerTurn = 1;
		}

	}

	return 0;
}