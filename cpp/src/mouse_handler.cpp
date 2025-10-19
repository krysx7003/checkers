#include "mouse_handler.h"
#include "gui/gui_system.h"

Tile *MouseHandler::focusedTile = nullptr;
std::unique_ptr<Piece> MouseHandler::piece;
glm::vec2 MouseHandler::window_dims;
glm::vec2 MouseHandler::size;

SpriteRenderer *MouseHandler::renderer;

void MouseHandler::Init() {
	json config = ResourceManager::Config;
	window_dims.x = config["window"]["width"].get<int>();
	window_dims.y = config["window"]["height"].get<int>();

	int tile_width = config["board"]["tile_width"];
	size = glm::vec2(tile_width, tile_width);

	glm::mat4 projection = glm::ortho(0.0f, window_dims.x, 0.0f, window_dims.y, -1.0f, 1.0f);
	ResourceManager::GetShader("piece").Use().SetInteger("image", 0);
	ResourceManager::GetShader("piece").SetMatrix4("projection", projection);
	renderer = new SpriteRenderer(ResourceManager::GetShader("piece"));
}

bool MouseHandler::Handle(bool gui_visible) {
	if (gui_visible) {
		Gui_System::Handle();

	} else if (Gui_System::GetFocus() != nullptr) {
		Gui_System::Handle();

	} else if (focusedTile != nullptr) {
		focusedTile->Handle();
		return true;
	}

	return false;
}

void MouseHandler::Draw() {
	if (piece == nullptr || focusedTile == nullptr) {
		return;
	}

	double mouseX, mouseY;
	GLFWwindow *window = glfwGetCurrentContext();
	glfwGetCursorPos(window, &mouseX, &mouseY);
	int y = (window_dims.y) - mouseY - size.y / 2;
	int x = mouseX - size.x / 2;

	piece->Draw(glm::vec2(x, y), size, renderer);
}

void MouseHandler::SetPiece(char c) {
	if (c != ' ') {
		piece = std::make_unique<Piece>(c);
	} else {
		piece.reset();
	}
}

void MouseHandler::SetFocusGui(Button *button) { Gui_System::SetFocus(button); }
void MouseHandler::SetFocusTile(Tile *tile) { focusedTile = tile; }

Tile *MouseHandler::GetFocusTile() { return focusedTile; }
Button *MouseHandler::GetFocusButton() { return Gui_System::GetFocus(); }
