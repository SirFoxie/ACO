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

	struct Point {
		int x;
		int y;
	};

	static World& getInstance() {
		static World instance;
		return instance;
	}

	void setWorldDims(int _startX, int _startY, int _height, int _width);
	void setTileType(int xPos, int yPos, TileType type);
	void setAreaType(int xPos, int yPos, int width, int height, TileType type);

	bool& checkPlacingWalls() { return isPlacingWalls; }
	bool& checkPlacingMarkers() { return isPlacingMarkers; }
	Area& getMap() { return Map; }
	Area& getStartArea() { return StartArea; }
	Area& getSearchArea() { return SearchArea; }
	float& getStartSearchBorder() { return StartSearchBorder; }
	TileType checkTileType (int xPos, int yPos) const;

	Vector2 getMarkerPos() { return markerPos; }
	Vector2 getRandomFogPoint();
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

	float StartSearchBorder = 0.3;

	std::vector<std::vector<TileType>> Tile2DGrid;	// 2d Grid array
	std::vector<std::pair<int,int>> fogTiles;
	bool isValidPos(int xPos, int yPos) const;

	bool isPlacingWalls = false;
	bool isPlacingMarkers = false;

	Vector2 markerPos;

	void drawArea();
	void drawMaze();
};

World::Point GridPtToPixelPt(const World::Point& gridPt);
World::Area PixelAreaToGridArea(const World::Area& area);
World::Area GridAreaToPixelArea(const World::Area& area);

void World::setWorldDims(int _startX, int _startY, int _width, int _height) {
	Map = { _startX, _startY, _width , _height };
	TileMap = { Map.x, Map.y, Map.width, Map.height };
	this->Tile2DGrid.resize(TileMap.width, std::vector<TileType>(TileMap.height, TileType::Fog));

	StartArea = { Map.x, Map.y, int(Map.width * StartSearchBorder) , Map.height };
	SearchArea = { (Map.x + StartArea.width), Map.y, (Map.width - StartArea.width), Map.height, };
	setAreaType(StartArea.x, StartArea.y, StartArea.width, StartArea.height, Explored);

	// Create FogTiles
	for (int row = 0; row < Tile2DGrid.size(); ++row) {
		for (int col = 0; col < Tile2DGrid[row].size(); ++col) {
			if (Tile2DGrid[row][col] == TileType::Fog) {
				fogTiles.emplace_back(row, col);
			}
		}
	}

	return;
}

Vector2 World::getRandomFogPoint() {
	if (fogTiles.empty()) {
		return { 0,0 };
	}
	std::pair<int, int> randFogTile = fogTiles[rand() % fogTiles.size()];
	Point randFogPixelPt = GridPtToPixelPt({ randFogTile.first, randFogTile.second });
	return Vector2{ (float)randFogPixelPt.x, (float)randFogPixelPt.y };
}


bool World::isValidPos(int xPos, int yPos) const {
	if (xPos < Map.x || xPos >= Map.width) return false;
	if (yPos < Map.y || yPos >= Map.height) return false;
	return true;
}

void World::setTileType(int xPos, int yPos, TileType type) {
	if (!isValidPos(xPos, yPos)) return;
	
	if (this->Tile2DGrid[xPos][yPos] == Fog) {
		auto it = std::find(fogTiles.begin(), fogTiles.end(), std::make_pair( xPos, yPos ));
		if (it != fogTiles.end()) {
			fogTiles.erase(it);
		}
	}

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

World::TileType World::checkTileType(int xPos, int yPos) const {
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

	if (isPlacingMarkers) {
		if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
			this->markerPos = (GlobalData::getInstance().getMouseRel());
		}
	}
	
	Vector2 mousePos = GlobalData::getInstance().getMouseRel();
	mousePos = calcCellCoords(mousePos);
	DrawText(TextFormat("TileType: %d", checkTileType(mousePos.x, mousePos.y)), GetMouseX(), GetMouseY(), 20, LIME);
	return;
}

void World::update() {
	if (TileMap.x != Map.x || TileMap.y != Map.y ||
		TileMap.width != Map.width || TileMap.height != Map.height) {
		this->setWorldDims(Map.x, Map.y, Map.width, Map.height);
	}
	if (int(Map.width * StartSearchBorder) != StartArea.width) {
		StartArea = { Map.x, Map.y, int(Map.width * StartSearchBorder) , Map.height };
		SearchArea = { (Map.x + StartArea.x), Map.y, (Map.width - StartArea.width), Map.height, };
		setAreaType(StartArea.x, StartArea.y, StartArea.width, StartArea.height, Explored);
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
World::Area PixelAreaToGridArea(const World::Area& area) {
	World::Area newArea = {
		(int)area.x / CELL_SIZE,
		(int)area.y / CELL_SIZE,
		(int)area.width / CELL_SIZE,
		(int)area.height / CELL_SIZE,
	};
	return newArea;
}


World::Area GridAreaToPixelArea(const World::Area& area) {
	World::Area newArea = {
		(int)area.x * CELL_SIZE,
		(int)area.y * CELL_SIZE,
		(int)area.width * CELL_SIZE,
		(int)area.height * CELL_SIZE,
	};
	return newArea;
}

World::Point GridPtToPixelPt(const World::Point& gridPt) {
	World::Point pixelPt = {
		gridPt.x * CELL_SIZE,
		gridPt.y * CELL_SIZE,
	};

	return pixelPt;
}


