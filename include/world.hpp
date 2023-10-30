#pragma once

#include <algorithm>
#include <vector>

struct _TileCell {
	enum TileType {
		OOB,
		START, 
		SEARCH_HIDDEN,
		SEARCH_EXPLORED,
		SEARCH_WALL,
		SEARCH_RESCUE_1,
		SEARCH_RESCUE_2
	};
	TileType Type;
};

class World {
private:
	unsigned int StartXPos;
	unsigned int StartYPos;
	unsigned int Height2DGrid;
	unsigned int Width2DGrid;
	std::vector<std::vector<_TileCell>> Tile2DGrid;	// 2d Grid array
	//unsigned char *tileFog;
public:
	enum TileType { Start, Search };

	World(int _startX, int _startY, int _height, int _weight)
		:StartXPos(_startX), StartYPos(_startY),Height2DGrid(_height), Width2DGrid(_weight){
		Tile2DGrid.resize(Height2DGrid, std::vector<_TileCell>(Width2DGrid, _TileCell()));
	}

	void setTileType(int xPos, int yPos, _TileCell::TileType type) {
		Tile2DGrid[xPos][yPos].Type = type;
	}

	void setAreaType(int xPos, int yPos, int width, int height, _TileCell::TileType type) {
		for (int x = xPos; x < xPos + width; x++) {
			for (int y = yPos; y < yPos + height; y++) {
				Tile2DGrid[x][y].Type = type;
			}
		}
	}

	_TileCell::TileType checkTileType(int xPos, int yPos) {
		if (xPos > Height2DGrid || yPos > Width2DGrid) {
			return _TileCell::OOB;
		}
		return Tile2DGrid[xPos][yPos].Type;
	}
};