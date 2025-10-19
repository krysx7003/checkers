#include "piece.h"

#include <cctype>

void Piece::parseChar(char c) {
	if (std::isupper(c)) {
		Color = Piece::WHITE;
	} else {
		Color = Piece::BLACK;
	}

	Type = std::tolower(c);
}

bool Piece::IsValid(char piece) {
	if (std::tolower(piece) == Piece::PAWN)
		return true;

	if (std::tolower(piece) == Piece::DAME)
		return true;

	return false;
}

void Piece::Draw(glm::vec2 positon, glm::vec2 size, SpriteRenderer *renderer) {
	renderer->DrawSprite(texture, positon, size, 180.0f, color);
}

char Piece::ToChar() {
	char type = this->Type;
	if (Color == Piece::WHITE)
		type = std::toupper(type);

	return type;
}
