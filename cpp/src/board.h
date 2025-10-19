#pragma once
#include "core/resources/sprite.h"
#include "tile.h"

#include "glad/glad.h"
#include "json.hpp"

#include <GLFW/glfw3.h>
#include <cstdio>
#include <glm/glm.hpp>
#include <vector>

using json = nlohmann::json;
using namespace std;

class Board {
	json config;

	float tile_width;
	std::string light_color;
	std::string dark_color;
	std::vector<char> initial_state;

	SpriteRenderer *Renderer;

	char getPieceChar(int i);
	std::vector<char> parseState(std::string initial_state);

  public:
	Tile *Tiles;
	int tiles_num;
	int width;

	Board() { Tiles = new Tile[tiles_num]; };
	void Init();

	void Render();
	bool TakeTile(int pos);
	void RestetTiles();

	std::vector<char> GetTilesState();

	std::string GetState();
};
