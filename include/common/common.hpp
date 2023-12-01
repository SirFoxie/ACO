#pragma once

#include "raylib.h"

#define MAP_XPOS    20
#define MAP_YPOS    20
#define MAP_WIDTH   200
#define MAP_HEIGHT  200

#define START_XPOS    MAP_XPOS
#define START_YPOS    MAP_YPOS
#define START_WIDTH   60
#define START_HEIGHT  200

#define SEARCH_XPOS    START_XPOS+(START_WIDTH*2)
#define SEARCH_YPOS    START_YPOS
#define SEARCH_WIDTH   140
#define SEARCH_HEIGHT  200

#define CELL_SIZE     50


// Return Mouse Rel pos based on grid orgin
Vector2 calcMouseRel(const Vector2& mousePos, const float& cam_zoom, const Vector2& cam_offset) {
	Vector2 MouseRel = {
		((mousePos.x / cam_zoom) + cam_offset.x),
		((mousePos.y / cam_zoom) + cam_offset.y)
	};
	return MouseRel;
}

// Return Cell Coords based on Mouse Rel position
Vector2 calcCellCoords(const Vector2& position) {

	Vector2 result{ (int)(position.x / CELL_SIZE), (int)(position.y / CELL_SIZE) };
	if (position.x < 0) result.x--;
	if (position.y < 0) result.y--;
	return result;
}
Rectangle DefineRectangle(const Vector2& rect_pt1, const Vector2& rect_pt2) {
	Vector2 rectEnd = Vector2Subtract(rect_pt1, rect_pt2);
	Vector2 rectStart = {
		(rect_pt1.x <= rect_pt2.x) ? rect_pt1.x : rect_pt2.x,
		(rect_pt1.y <= rect_pt2.y) ? rect_pt1.y : rect_pt2.y
	};
	return { rectStart.x, rectStart.y, abs(rectEnd.x), abs(rectEnd.y) };
}

