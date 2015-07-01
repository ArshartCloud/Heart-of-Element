#ifndef __ELEMENT_H__
#define __ELEMENT_H__

#include "cocos2d.h"
#include "Card.h"

class PlayScene;
using namespace std;
USING_NS_CC;

class Element 
{
public:

	Element();
	~Element();
	bool onEffort(PlayScene* _plays, Point _target);
	void fireball(PlayScene* _plays, Point _target);
	void waterHeal(PlayScene* _plays, Point _target);
	//
	void firebolt(PlayScene* _plays, Point _target);
	void firestorm(PlayScene* _plays, Point _target);
	void meteorfall(PlayScene* _plays, Point _target);
	//

	void clear();
	void cleSpell();
	Vector<Card*>* getVec();
	int getRange() const;
	bool getLine() const;
	string getSpell() const;
	string getDes() const;
	void pushBack(PlayScene* _plays, Card* c);

protected:
	Vector<Card*> _elements;
	int range;
	bool is_line;
	int damage;
	string spell;
	string des; // description
	string area;
	string anim;
};

#endif
