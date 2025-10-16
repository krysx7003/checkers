#include "board.h"

#include "core/resources/shader.h"
#include "player_manager.h"

#include <GLFW/glfw3.h>
#include <cstdio>

void Board::Init() {
	config = ResourceManager::Config;

	boardGui = config["gui"];

	float window_height = config["window"]["height"].get<int>();
	float window_width = config["window"]["width"].get<int>();
	tiles_num = config["board"]["tiles_num"];
	width = config["board"]["width"];
	tile_size = 2.0f / width;
	tile_width = config["board"]["tile_width"];
	tile_size_px = glm::vec2(tile_width, tile_width);
	prettyPrint = config["pretty_print"];
	light_color = config["board"]["light_color"];
	dark_color = config["board"]["dark_color"];

	Tiles = new Tile[tiles_num];

	if (boardGui) {
		for (int row = 0; row < width; row++) {
			for (int col = 0; col < width; col++) {
				int id = col + row * width;
				int x = col * tile_width;
				int y = row * tile_width;
				Tiles[id] = Tile(x, y, tile_width, tile_width);
				if (col % 2 == row % 2) {
					Tiles[id].SetColor(light_color);
				} else {
					Tiles[id].SetColor(dark_color);
				}

				Tiles[id].SetId(id);
				Tiles[id].State = Tile::State::Empty;
			}
		}

		glm::mat4 projection = glm::ortho(0.0f, window_width, 0.0f, window_height, -1.0f, 1.0f);
		ResourceManager::GetShader("piece").Use().SetInteger("image", 0);
		ResourceManager::GetShader("piece").SetMatrix4("projection", projection);
		Renderer = new SpriteRenderer(ResourceManager::GetShader("piece"));
	} else {
		for (int i = 0; i < tiles_num; i++) {
			Tiles[i] = Tile();
			Tiles[i].State = Tile::State::Empty;
		}
	}
}

void Board::Render() {
	for (int i = 0; i < tiles_num; i++) {
		Tiles[i].Render();
	}
}

void Board::Print(bool tooltip) {
	if (!prettyPrint) {
		printf("%s\n", GetState().c_str());
		return;
	}

	printf("\n-------------\n");
	printf("| %s | %s | %s | ", Tiles[0].State.c_str(), Tiles[1].State.c_str(),
		   Tiles[2].State.c_str());
	if (tooltip)
		printf("| 0 | 1 | 2 |");
	printf("\n----------\n");

	printf("| %s | %s | %s | ", Tiles[3].State.c_str(), Tiles[4].State.c_str(),
		   Tiles[5].State.c_str());
	if (tooltip)
		printf("| 3 | 4 | 5 |");
	printf("\n----------\n");

	printf("| %s | %s | %s | ", Tiles[6].State.c_str(), Tiles[7].State.c_str(),
		   Tiles[8].State.c_str());
	if (tooltip)
		printf("| 6 | 7 | 8 |");
	printf("\n----------\n");
}

bool Board::TakeTile(int pos) {
	if (Tiles[pos].State != Tile::State::Empty) {
		return false;
	}

	if (PlayerManager::Curr_player == Player::WHITE) {
		Tiles[pos].State = Tile::State::TakenWhite;
	} else {
		Tiles[pos].State = Tile::State::TakenBlack;
	}
	return true;
}

void Board::RestetTiles() {
	for (int i = 0; i < tiles_num; i++) {
		Tiles[i].State = Tile::State::Empty;
	}
}

std::vector<std::string> Board::GetTilesState() {
	std::vector<std::string> tab(tiles_num);
	for (int i = 0; i < tiles_num; i++) {
		tab[i] = Tiles[i].State;
	}
	return tab;
}

std::string Board::GetState() {
	stringstream ss;
	for (int i = 0; i < tiles_num; i++) {
		ss << "\\" << Tiles[i].State;
	}

	ss << "\\";
	return ss.str();
}
