#include "Grid.h"

Grid::Grid(){
	tile = NULL;
	_prio = 0;
	role = nullptr;
}

Grid::Grid(Point t, Role* r) {
	tile = t;
	_prio = 0;
	role = r;
	movable = true;
}
Grid::~Grid() {}

void Grid::setPrio(int p) {
	_prio = p;
}
void Grid::setRole(Role* r) {
	role = r;
}

void Grid::setMov(bool b) {
	movable = b;
}

bool Grid::is_people() {
	if (role != nullptr) return true;
	else return false;
}

bool Grid::is_mov() {
	if ((!movable) || is_people()) return false;
	return true;
}

bool Grid::is_justmov() {
	return movable;
}

Point Grid::getTile() const {
	return tile;
}

Role* Grid::getRole() const { return role; }
int Grid::getPrio() const { return _prio; }