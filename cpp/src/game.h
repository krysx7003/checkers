#pragma once

#include "board.h"
#include <string>

class Game {
	short lastTile = -1;
	std::string winner = " ";

	void setWinner(char state);
	void swapPlayer();

  public:
	Board board;
	bool active = false;
	bool ended = false;

	void Init();
	Game() {};

	void Start(std::string player1, std::string player2);
	void Render();
	bool ChosenTile(int tileId);
	bool WinCondition(std::vector<char> state);
	bool IsDraw(std::vector<char> state);
	short GetLastTile();
	void Restart();

	std::string GetWinner();
};
