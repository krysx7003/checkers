#pragma once

#include "gui/widgets/button.h"
#include "tile.h"

class MouseHandler {
	static Tile *focusedTile;
	static std::unique_ptr<Piece> piece;
	static glm::vec2 window_dims;
	static glm::vec2 size;

	static SpriteRenderer *renderer;

  public:
	static int Return_id;
	static int LastClickedTile;

	static void Init();
	static bool Handle(bool gui_visible);
	static void Draw();
	static void Reset();

	static void SetFocusGui(Button *button);
	static void SetPiece(char c);
	static void SetPiece(std::unique_ptr<Piece> p);
	static void SetFocusTile(Tile *tile);

	static Piece *GetPiece();
	static std::unique_ptr<Piece> TakePiece();
	static Tile *GetFocusTile();
	static Button *GetFocusButton();
};
