#include "core/resource_manager.h"

#include "game.h"
#include "player_manager.h"
#include <cctype>
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

bool Game::CapturePiece(int start, int end, Piece *p) {
	if (start + board.width * 2 + 2 == end) {
		int captured_piece = start + board.width + 1;
		if (board.Tiles[captured_piece].piece->Color != p->Color) {
			board.RemovePiece(captured_piece);
			return true;
		}
	} else if (start + board.width * 2 - 2 == end) {
		int captured_piece = start + board.width - 1;
		if (board.Tiles[captured_piece].piece->Color != p->Color) {
			board.RemovePiece(captured_piece);
			return true;
		}
	} else if (start - board.width * 2 + 2 == end) {
		int captured_piece = start - board.width + 1;
		if (board.Tiles[captured_piece].piece->Color != p->Color) {
			board.RemovePiece(captured_piece);
			return true;
		}
	} else if (start - board.width * 2 - 2 == end) {
		int captured_piece = start - board.width - 1;
		if (board.Tiles[captured_piece].piece->Color != p->Color) {
			board.RemovePiece(captured_piece);
			return true;
		}
	}

	return false;
}

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

	swapPlayer();
	lastTile = tileId;
	return true;
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
}
void Game::CheckWinCond() {
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
	if (state == Piece::WHITE) {
		winner = Player::WHITE;
	} else if (state == Piece::BLACK) {
		winner = Player::BLACK;
	}
}

bool Game::WinCondition(std::vector<char> state) {
	int white_count = 0;
	int black_count = 0;

	for (char tile : state) {
		if (islower(tile)) {
			black_count++;
		} else if (isupper(tile)) {
			white_count++;
		}
	}

	if (white_count == 0) {
		winner = Player::BLACK;
		return true;

	} else if (black_count == 0) {
		winner = Player::WHITE;
		return true;
	} else if (!playerHasMoves(Player::WHITE)) {
		winner = Player::BLACK;
		return true;
	} else if (!playerHasMoves(Player::BLACK)) {
		winner = Player::WHITE;
		return true;
	}

	return false;
}

void Game::RankUp(int id, char color) {
	int rank = id / board.width;

	if (rank == 0 && color == Piece::WHITE) {
		board.Tiles[id].SetPiece('D');
	} else if (rank == board.width - 1 && color == Piece::BLACK) {
		board.Tiles[id].SetPiece('d');
	}
}

bool Game::playerHasMoves(std::string player) {
	char player_c = Piece::BLACK;
	if (player == Player::WHITE) {
		player_c = Piece::WHITE;
	}

	for (int i = 0; i < board.tiles_num; i++) {
		Piece *p = board.Tiles[i].piece.get();
		if (p != nullptr) {
			if (p->Color == player_c) {
				std::vector<int> vec = board.ValidMoves(i, p);
				if (!vec.empty())
					return true;
			}
		}
	}
	return false;
}

bool Game::IsDraw(std::vector<char> state) { return false; }

std::string Game::GetWinner() { return winner; }

short Game::GetLastTile() { return lastTile; }
