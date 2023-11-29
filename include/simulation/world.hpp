#pragma once

#include <algorithm>
#include <vector>

#include "./common/common.hpp"
#include "./common/globaldata.hpp"

class World {
public:
	enum TileType { OOB = -1, Fog, Explored, Wall};

	struct Area {
		int x;
		int y;
		int width;
		int height;
	};

	static World& getInstance() {
		static World instance;
		return instance;
	}

	void setWorldDims(int _startX, int _startY, int _height, int _width);
	void setTileType(int xPos, int yPos, TileType type);
	void setAreaType(int xPos, int yPos, int width, int height, TileType type);

	bool& checkPlacingWalls() { return isPlacingWalls; }
	Area& getMap() { return Map; }
	Area& getStartArea() { return StartArea; }
	Area& getSearchArea() { return SearchArea; }
	int& getStartSearchBorder() { return StartSearchBorder; }
	TileType checkTileType(int xPos, int yPos);

	void handleInputs();
	void update();
	void render();
private:
	World() {};
	~World() = default;

	Area Map;
	Area StartArea;
	Area SearchArea;
	Area TileMap;

	int StartSearchBorder = 6;

	std::vector<std::vector<TileType>> Tile2DGrid;	// 2d Grid array
	//unsigned char *tileFog;
	bool isValidPos(int xPos, int yPos);

	bool isPlacingWalls = 0;

	void drawArea();
	void drawMaze();
};


void World::setWorldDims(int _startX, int _startY, int _width, int _height) {
	Map = { _startX, _startY, _width , _height };
	StartArea = { Map.x, Map.y, StartSearchBorder , Map.height };
	SearchArea = { (Map.x + StartSearchBorder), Map.y, (Map.width - StartSearchBorder), Map.height, };
	TileMap = { Map.x, Map.y, Map.width, Map.height };
	this->Tile2DGrid.resize(TileMap.width, std::vector<TileType>(TileMap.height, TileType::Fog));

	return;
}



bool World::isValidPos(int xPos, int yPos) {
	if (xPos < Map.x || xPos >= Map.width) return false;
	if (yPos < Map.y || yPos >= Map.height) return false;
	return true;
}

void World::setTileType(int xPos, int yPos, TileType type) {
	if (!isValidPos(xPos, yPos)) return;
	 this->Tile2DGrid[xPos][yPos] = type;
	return;
}

void World::setAreaType(int xPos, int yPos, int width, int height, TileType type) {
	for (int x = xPos; x < xPos + width; x++) {
		for (int y = yPos; y < yPos + height; y++) {
			Tile2DGrid[x][y] = type;
		}
	}
}

World::TileType World::checkTileType(int xPos, int yPos) {
	if (!isValidPos(xPos, yPos)) return TileType::OOB;
	return Tile2DGrid[xPos][yPos];
}

void World::drawArea() {
	Rectangle StartRec = { StartArea.x, StartArea.y, StartArea.width * CELL_SIZE, StartArea.height * CELL_SIZE };
	DrawRectangleRec(StartRec, Fade(GREEN, 0.2f));
	Rectangle SearchRec = { SearchArea.x * CELL_SIZE, SearchArea.y, SearchArea.width * CELL_SIZE, SearchArea.height * CELL_SIZE };
	DrawRectangleRec(SearchRec, Fade(BLUE, 0.2f));
	Rectangle MapRec = { Map.x, Map.y, Map.width * CELL_SIZE , Map.height * CELL_SIZE };
	DrawRectangleLinesEx(MapRec, 5.0, BLACK);
	return;
}

void World::drawMaze() {
	for (int i = SearchArea.x; i < SearchArea.x+SearchArea.width; ++i) {
		for (int j = SearchArea.y; j < SearchArea.y + SearchArea.height; ++j) {
			TileType currentTile = Tile2DGrid[i][j];
			switch (currentTile) {
			case Fog:
				DrawRectangle(i * CELL_SIZE, j * CELL_SIZE, CELL_SIZE, CELL_SIZE, Fade(BLACK, 0.5f));
				break;
			case Wall:
				DrawRectangle(i * CELL_SIZE, j * CELL_SIZE, CELL_SIZE, CELL_SIZE, Fade(GRAY, 0.5f));
				break;
			case Explored:
				DrawRectangle(i * CELL_SIZE, j * CELL_SIZE, CELL_SIZE, CELL_SIZE, Fade(BLUE, 0.2f));
				break;
			default:
				break;
			}
		}

	}
	return;
}

void World::handleInputs() {
	if (isPlacingWalls) {
		if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
			Vector2 SelectedGrid = calcCellCoords(GlobalData::getInstance().getMouseRel());
			setTileType(SelectedGrid.x, SelectedGrid.y, Wall);
		}
	}
	
	DrawText(TextFormat("TileType: %d", checkTileType(GetMouseX(), GetMouseY())), GetMouseX(), GetMouseY(), 10, LIME);
	return;
}

void World::update() {
	if (TileMap.x != Map.x || TileMap.y != Map.y ||
		TileMap.width != Map.width || TileMap.height != Map.height) {
		this->setWorldDims(Map.x, Map.y, Map.width, Map.height);
	}
	if (StartSearchBorder != StartArea.width) {
		StartArea = { Map.x, Map.y, StartSearchBorder , Map.height };
		SearchArea = { (Map.x + StartSearchBorder), Map.y, (Map.width - StartSearchBorder), Map.height, };
	}
	return;
}

void World::render() {
	this->update();
	this->drawArea();
	this->drawMaze();
	if (isPlacingWalls) {
		Vector2 SelectedGrid = calcCellCoords(GlobalData::getInstance().getMouseRel());
		DrawRectangle((int)SelectedGrid.x * CELL_SIZE, (int)SelectedGrid.y * CELL_SIZE, CELL_SIZE, CELL_SIZE, BLACK);
	}
	return;
}

// Outside Helper Functions
World::Area PixelPosToGridPos(const World::Area& area) {
	World::Area newArea = {
		(int)area.x / CELL_SIZE,
		(int)area.y / CELL_SIZE,
		(int)area.width / CELL_SIZE,
		(int)area.height / CELL_SIZE,
	};
	return newArea;
}

World::Area GridPosToPixelPos(const World::Area& area) {
	World::Area newArea = {
		area.x * CELL_SIZE,
		area.y * CELL_SIZE,
		area.width * CELL_SIZE,
		area.height * CELL_SIZE,
	};
	return newArea;
}
