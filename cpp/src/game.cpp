#include "core/resource_manager.h"

#include "game.h"
#include "player_manager.h"
#include "tile.h"
#include <glm/ext/vector_float2.hpp>

void Game::Init() {
	ResourceManager::LoadShader("piece.vs", "piece.frag", "piece");
	ResourceManager::LoadShader("flat_argb.vs", "flat_argb.frag", "argb");

	ResourceManager::LoadTexture("black_dame.png", true, "black_dame");
	ResourceManager::LoadTexture("white_dame.png", true, "white_dame");
	ResourceManager::LoadTexture("checkers_normal.png", true, "normal_piece");

	board.Init();
}

void Game::Start(std::string player1, std::string player2) {
	PlayerManager::Curr_player = Player::WHITE;
	PlayerManager::SetPlayers(player1, player2);

	PlayerManager::StartServer();

	active = true;
}

void Game::Render() { board.Render(); }

void Game::Restart() {
	PlayerManager::Curr_player = Player::WHITE;
	lastTile = -1;
	winner = -1;
	ended = false;
	active = false;

	board.RestetTiles();
}

bool Game::ChosenTile(int tileId) {
	if (!active) {
		return false;
	}

	if (board.TakeTile(tileId)) {
		swapPlayer();
		lastTile = tileId;
		return true;
	}
	return false;
}

void Game::swapPlayer() {
	if (!active) {
		return;
	}
	if (PlayerManager::Curr_player == Player::WHITE) {
		PlayerManager::Curr_player = Player::BLACK;
	} else {
		PlayerManager::Curr_player = Player::WHITE;
	}

	std::vector<char> state = board.GetTilesState();
	if (WinCondition(state)) {
		ended = true;
		active = false;
	} else if (IsDraw(state)) {
		ended = true;
		active = false;
	}
}

void Game::setWinner(char state) {
	if (state == Tile::State::TakenWhite) {
		winner = Player::WHITE;
	} else if (state == Tile::State::TakenBlack) {
		winner = Player::BLACK;
	}
}

bool Game::WinCondition(std::vector<char> state) { return false; }

bool Game::IsDraw(std::vector<char> state) {
	for (int i = 0; i < board.tiles_num; i++) {
		if (state[i] == Tile::State::Empty) {
			return false;
		}
	}
	winner = " ";
	return true;
}

std::string Game::GetWinner() { return winner; }

short Game::GetLastTile() { return lastTile; }
