#pragma once

#include "core/primitives/rect.h"
#include "core/resource_manager.h"
#include "core/resources/sprite.h"
#include "piece.h"

class Tile {
	int start_pos_x, start_pos_y;
	int width, height;
	int id;
	Rect background;
	glm::vec4 color;
	Rect highlight;
	glm::vec4 high_color = glm::vec4(1.0f, 1.0f, 1.0f, 0.0f);
	glm::vec2 window_dims;

	SpriteRenderer *Renderer;

	glm::vec4 hexToColor(std::string);
	void setUpBackground();
	bool isMouseOn();

  public:
	std::unique_ptr<Piece> piece;

	Tile(int start_pos_x, int start_pos_y, int width, int height)
		: start_pos_x(start_pos_x), start_pos_y(start_pos_y), width(width), height(height) {

		json config = ResourceManager::Config;
		window_dims.x = config["window"]["width"].get<int>();
		window_dims.y = config["window"]["height"].get<int>();

		setUpBackground();

		glm::mat4 projection = glm::ortho(0.0f, window_dims.x, 0.0f, window_dims.y, -1.0f, 1.0f);
		ResourceManager::GetShader("piece").Use().SetInteger("image", 0);
		ResourceManager::GetShader("piece").SetMatrix4("projection", projection);
		Renderer = new SpriteRenderer(ResourceManager::GetShader("piece"));
	};
	Tile() {};

	void Render();
	bool Handle();

	int GetId();
	Piece *GetPiece();

	void SetId(int id);
	void SetColor(std::string);
	void SetHighlight(std::string color_hex);
	void SetPiece(char state);
};
