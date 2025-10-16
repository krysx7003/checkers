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

	SpriteRenderer *Renderer;

  public:
	Tile *Tiles;
	int tiles_num;
	int width;

	Board() { Tiles = new Tile[tiles_num]; };
	void Init();

	void Render();
	bool TakeTile(int pos);
	void RestetTiles();

	std::vector<std::string> GetTilesState();

	std::string GetState();
};
