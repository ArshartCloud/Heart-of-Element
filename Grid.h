#ifndef __GRID_H__
#define __GRID_H__

#include "cocos2d.h"
#include "Role.h"
#include "Element.h"
#include <string>
//#include <vector>
using namespace std;
USING_NS_CC;

class Grid {
public:
	Grid();
	Grid(Point t, Role* r = nullptr);
	~Grid();
	void setPrio(int p);
	void setRole(Role* r);
	void setMov(bool b);
	bool is_people();
	bool is_mov();
	bool is_justmov();
	Point getTile() const;
	Role* getRole() const;
	int getPrio() const;
private:
	Point tile;
	int _prio;
	Role* role;
	bool movable;
};

#endif