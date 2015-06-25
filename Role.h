#ifndef __ROLE__
#define __ROLE__

#include "cocos2d.h"
#include "Card.h"
#include "Element.h"
using namespace std;
USING_NS_CC;

class PlayScene;

class Role :public cocos2d::Sprite
{
public:
//	CREATE_FUNC(Role);
	static Role* create(const std::string& name, Point pos);
	static Role* create(ValueMap& dict);
	virtual bool init(const std::string& name);
	void MoveTo(Point position);
	Point getPos();
	int getHp();
	bool takeDamage(int dam); // take dam damage, return if alive
	int getMP(); // get move point
protected:
	int _move_point;
	Point _tileCoor; // tile position
	int hp;
};

class Player :public Role
{
public:
	//	CREATE_FUNC(Role);
	static Player* create(const std::string& name);
	virtual bool init(const std::string& name);

	void drawCard();
	void drawCard(Card* c);
	bool useSkill(PlayScene* _plays, Point target);

	Vector<Card*>* getHands();
	Element* getEle();
protected:
	Vector<Card*> _hands;
	Element* _element;
};

class Enemy : public Role
{
public:
	enum Strategy{ Aggressive, Protected, Stable };

	static Enemy* create(const std::string& name);
	static Enemy* create(ValueMap& dict);
	virtual bool init(const std::string& name);
protected:
	Strategy _stra;
};

#endif
