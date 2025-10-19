#pragma once

#include "core/resource_manager.h"
#include "core/resources/sprite.h"

class Piece {
	Texture2D texture;
	glm::vec4 color;
	void parseChar(char c);

  public:
	static const char PAWN = 'p';
	static const char DAME = 'd';

	static const char WHITE = 'w';
	static const char BLACK = 'b';

	char Color;
	char Type;

	Piece() {};
	Piece(char piece) {
		parseChar(piece);
		texture = ResourceManager::GetTexture("normal_piece");
		color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

		if (Type == Piece::DAME) {
			if (Color == Piece::BLACK) {
				texture = ResourceManager::GetTexture("black_dame");
			} else {
				texture = ResourceManager::GetTexture("white_dame");
			}

		} else if (Type == Piece::PAWN) {
			if (Color == Piece::BLACK) {
				color = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
			}
		}
	};

	static bool IsValid(char piece);
	void Draw(glm::vec2 positon, glm::vec2 size, SpriteRenderer *renderer);
	char ToChar();
};
