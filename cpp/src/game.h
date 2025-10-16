#pragma once

#include "board.h"
#include <string>

class Game {
	json config;

	int top_menu_height;
	short lastTile = -1;
	short winner = -1;

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
	void Print();
	bool ChosenTile(double x, double y);
	bool ChosenTile(int tileId);
	bool WinCondition(std::vector<char> state);
	bool IsDraw(std::vector<char> state);
	short GetLastTile();
	void Restart();

	char GetWinner();
};
