#pragma once

#include "board.h"
#include <string>

class Game {
	json config;

	int top_menu_height;
	short lastTile = -1;
	std::string winner = " ";

	void setWinner(std::string state);
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
	bool ChosenTile(int tileId);
	bool WinCondition(std::vector<std::string> state);
	bool IsDraw(std::vector<std::string> state);
	short GetLastTile();
	void Restart();

	std::string GetWinner();
};
