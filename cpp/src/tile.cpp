#include "tile.h"
#include "mouse_handler.h"
#include "player_manager.h"

glm::vec4 Tile::hexToColor(std::string color) {
	if (color[0] != '#') {
		printf("ERROR::GUI_ITEM: Color is invalid\n");
		return glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	}
	float r = std::stoi(color.substr(1, 2), nullptr, 16) / 255.0f;
	float g = std::stoi(color.substr(3, 2), nullptr, 16) / 255.0f;
	float b = std::stoi(color.substr(5, 2), nullptr, 16) / 255.0f;
	float a = 1.0f;
	if (color.size() == 9) {
		a = std::stoi(color.substr(7, 2), nullptr, 16) / 255.0f;
	}

	return glm::vec4(r, g, b, a);
}

void Tile::setUpBackground() {
	float fWidth = (width / (window_dims.x / 2.0f));
	float fHeight = (height / (window_dims.y / 2.0f));
	float fStart_pos_x = (start_pos_x / (window_dims.x / 2.0f)) - 1;
	float fStart_pos_y = (start_pos_y / (window_dims.y / 2.0f)) - 1;

	glm::vec3 bottom_left = {fStart_pos_x, fStart_pos_y, 0.0f};
	glm::vec3 bottom_right = {fStart_pos_x + fWidth, fStart_pos_y, 0.0f};
	glm::vec3 top_left = {fStart_pos_x, fStart_pos_y + fHeight, 0.0f};
	glm::vec3 top_right = {fStart_pos_x + fWidth, fStart_pos_y + fHeight, 0.0f};

	background.Init(bottom_left, bottom_right, top_left, top_right);
	highlight.Init(bottom_left, bottom_right, top_left, top_right);
}

void Tile::Render() {
	background.Draw(color);
	highlight.Draw(high_color);

	if (isMouseOn()) {
		if (!MouseHandler::GetFocusTile() || MouseHandler::GetFocusTile() != this) {
			MouseHandler::SetFocusTile(this);
		}
	} else if (MouseHandler::GetFocusTile() == this) {
		MouseHandler::SetFocusTile(nullptr);
	}

	if (piece == nullptr) {
		return;
	}
	piece->Draw(glm::vec2(start_pos_x, start_pos_y), glm::vec2(width, height), Renderer);
}

bool Tile::Handle() {
	if (MouseHandler::GetPiece() != nullptr && piece == nullptr) {
		return true;
	} else if (MouseHandler::GetPiece() == nullptr && piece != nullptr) {
		if (PlayerManager::Curr_player == Player::WHITE && piece->Color == Piece::WHITE ||
			PlayerManager::Curr_player == Player::BLACK && piece->Color == Piece::BLACK) {
			MouseHandler::SetPiece(std::move(piece));
		}
		return false;
	}
	return false;
}

bool Tile::isMouseOn() {
	double mouseX, mouseY;
	GLFWwindow *window = glfwGetCurrentContext();
	glfwGetCursorPos(window, &mouseX, &mouseY);
	int y = (window_dims.y) - mouseY;
	int x = mouseX;

	if (start_pos_x > x || x > start_pos_x + width) {
		return false;
	}
	if (start_pos_y > y || y > start_pos_y + height) {
		return false;
	}

	return true;
}

int Tile::GetId() { return this->id; }

Piece *Tile::GetPiece() { return this->piece.get(); }

void Tile::SetId(int id) { this->id = id; }

void Tile::SetColor(std::string color_hex) { this->color = hexToColor(color_hex); }
void Tile::SetHighlight(std::string color_hex) { high_color = hexToColor(color_hex); }

void Tile::SetPiece(char c) {
	if (c != ' ') {
		piece = std::make_unique<Piece>(c);
	} else {
		piece.reset();
	}
}
