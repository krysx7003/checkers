#pragma once

#include "core/primitives/rect.h"
#include "core/resource_manager.h"
#include "core/resources/sprite.h"

class Tile {
	int start_pos_x, start_pos_y;
	int width, height;
	int id;
	Rect background;
	glm::vec4 color;
	glm::vec2 window_dims;

	SpriteRenderer *Renderer;

	glm::vec4 hexToColor(std::string);
	void setUpBackground();
	bool isMouseOn();

  public:
	class State {
	  public:
		static const char Empty = ' ';
		static const char TakenWhite = 'P';
		static const char TakenBlack = 'p';
		static const char TakenWhiteDame = 'D';
		static const char TakenBlackDame = 'd';
	};
	char State;

	Tile(int start_pos_x, int start_pos_y, int width, int height)
		: start_pos_x(start_pos_x), start_pos_y(start_pos_y), width(width), height(height) {

		json config = ResourceManager::Config;
		window_dims.x = config["window"]["width"].get<int>();
		window_dims.y = config["window"]["height"].get<int>();

		setUpBackground();

		glm::mat4 projection = glm::ortho(0.0f, 600.0f, 640.0f, 0.0f, -1.0f, 1.0f);
		ResourceManager::GetShader("piece").Use().SetInteger("image", 0);
		ResourceManager::GetShader("piece").SetMatrix4("projection", projection);
		Renderer = new SpriteRenderer(ResourceManager::GetShader("piece"));
	};
	Tile() {};

	static bool IsValidPiece(char piece);

	void Render();
	void Handle();

	int GetId();

	void SetId(int id);
	void SetColor(std::string);
	void SetState(char state);
};
