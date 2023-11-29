#pragma once

#include "./common.hpp"


class GlobalData {
public:
	static GlobalData& getInstance() {
		static GlobalData instance;
		return instance;
	}

	Vector2 getMouseRel();
	void updateMouseRel(Vector2& newMouseRel);

private:
	GlobalData() {} // Private Ctor
	Vector2 MouseRel = { 0.0, 0.0 };
};


Vector2 GlobalData::getMouseRel() {
	return this->MouseRel;
}

void GlobalData::updateMouseRel(Vector2& newMouseRel) {
	this->MouseRel = newMouseRel;
	return;
}