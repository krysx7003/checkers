#include "board.h"

#include "core/resources/shader.h"
#include "piece.h"

#include <GLFW/glfw3.h>
#include <cstdio>

void Board::Init() {
	config = ResourceManager::Config;

	float window_height = config["window"]["height"].get<int>();
	float window_width = config["window"]["width"].get<int>();
	tiles_num = config["board"]["tiles_num"];
	width = config["board"]["width"];
	tile_width = config["board"]["tile_width"];
	std::string fen = config["board"]["state"];
	initial_state = parseState(fen);

	light_color = config["board"]["light_color"];
	dark_color = config["board"]["dark_color"];

	Tiles = new Tile[tiles_num];

	for (int row = 0; row < width; row++) {
		for (int col = 0; col < width; col++) {
			int id = col + row * width;
			int x = col * tile_width;
			int y = (width - row - 1) * tile_width;
			Tiles[id] = Tile(x, y, tile_width, tile_width);
			if (col % 2 == row % 2) {
				Tiles[id].SetColor(light_color);
			} else {
				Tiles[id].SetColor(dark_color);
			}

			Tiles[id].SetId(id);
			Tiles[id].SetPiece(initial_state[id]);
		}
	}

	glm::mat4 projection = glm::ortho(0.0f, window_width, 0.0f, window_height, -1.0f, 1.0f);
	ResourceManager::GetShader("piece").Use().SetInteger("image", 0);
	ResourceManager::GetShader("piece").SetMatrix4("projection", projection);
	Renderer = new SpriteRenderer(ResourceManager::GetShader("piece"));
}

std::vector<char> Board::parseState(std::string initial_state) {
	std::vector<char> state(tiles_num);
	std::vector<std::string> ranks;

	std::stringstream ss(initial_state);
	std::string rank;
	while (std::getline(ss, rank, '/')) {
		ranks.push_back(rank);
	}

	if (ranks.size() != width) {
		printf("ERROR::BOARD: Invalid fen invalid ranks number %lu", ranks.size());
		exit(-1);
	}

	int id = 0;
	for (std::string rank : ranks) {
		int file = 0;
		for (char piece : rank) {
			if (piece >= '1' && piece <= '8') {
				int num_spaces = piece - '0';
				for (int i = 0; i < num_spaces; i++) {
					state.at(id) = ' ';
					id++;
				}

				file += num_spaces;

			} else if (Piece::IsValid(piece)) {
				state.at(id) = piece;
				id++;
				file++;

			} else {
				printf("ERROR::BOARD: Invalid fen, reason invalid char %c", piece);
				exit(-1);
			}
		}
		if (file != width) {
			printf("ERROR::BOARD: Invalid fen, reason invalid lenght of file %d", file);
			exit(-1);
		}
	}
	return state;
}

void Board::Render() {
	for (int i = 0; i < tiles_num; i++) {
		Tiles[i].Render();
	}
}

void Board::RemovePiece(int id) { Tiles[id].piece.reset(); }

std::vector<int> Board::ValidMoves(int id, Piece *p) {
	int rank = id / width;

	std::vector<int> forward_moves = genrateMoves(id, width, p);

	std::vector<int> backward_moves = genrateMoves(id, width * -1, p);

	std::vector<int> moves;
	if (p->Type == Piece::PAWN) {
		if (p->Color == Piece::BLACK) {
			moves.insert(moves.begin(), forward_moves.begin(), forward_moves.end());
		} else {
			moves.insert(moves.begin(), backward_moves.begin(), backward_moves.end());
		}

	} else if (p->Type == Piece::DAME) {
		moves.insert(moves.begin(), backward_moves.begin(), backward_moves.end());
		moves.insert(moves.begin(), forward_moves.begin(), forward_moves.end());
	}

	return moves;
}

std::vector<int> Board::genrateMoves(int id, int direction, Piece *p) {
	int file = id % width;
	std::vector<int> moves;
	if (file - 1 >= 0) {
		int tile_id = id + direction - 1;
		if (tile_id >= 0 && tile_id < tiles_num) {
			if (Tiles[tile_id].piece == nullptr) {
				moves.push_back(tile_id);

			} else if (Tiles[tile_id].piece->Color != p->Color) {
				tile_id += direction - 1;
				if (Tiles[tile_id].piece == nullptr && tile_id >= 0 && file - 2 >= 0)
					moves.push_back(tile_id);
			}
		}
	}

	if (file + 1 < width) {
		int tile_id = id + direction + 1;
		if (tile_id >= 0 && tile_id < tiles_num) {
			if (Tiles[tile_id].piece == nullptr) {
				moves.push_back(tile_id);

			} else if (Tiles[tile_id].piece->Color != p->Color) {
				tile_id += direction + 1;
				if (Tiles[tile_id].piece == nullptr && tile_id < tiles_num && file + 2 < width)
					moves.push_back(tile_id);
			}
		}
	}
	return moves;
}

void Board::ResetHighlight(std::vector<int> validTiles) {
	for (int tile : validTiles) {
		Tiles[tile].SetHighlight("#FFFFFF00");
	}
}

void Board::RestetTiles() {
	for (int i = 0; i < tiles_num; i++) {
		Tiles[i].SetPiece(initial_state[i]);
	}
}

char Board::getPieceChar(int i) {
	Piece *piece = Tiles[i].GetPiece();
	if (piece != nullptr) {
		return piece->ToChar();
	} else {
		return ' ';
	}
}

std::vector<char> Board::GetTilesState() {
	std::vector<char> tab(tiles_num);
	for (int i = 0; i < tiles_num; i++) {
		tab[i] = getPieceChar(i);
	}
	return tab;
}

std::string Board::GetState() {
	stringstream ss;
	for (int i = 0; i < tiles_num; i++) {
		ss << "\\" << getPieceChar(i);
	}

	ss << "\\";
	return ss.str();
}
