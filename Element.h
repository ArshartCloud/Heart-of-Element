#ifndef __ELEMENT_H__
#define __ELEMENT_H__

#include "cocos2d.h"
#include "Card.h"

class PlayScene;
using namespace std;
USING_NS_CC;

class Element : public cocos2d::Sprite
{
public:
//	CREATE_FUNC(Element);
	static Element* create(const std::string& name);
	virtual bool init(const std::string& name);

	bool onEffort(PlayScene* _plays, Point _target);
	void fireball(PlayScene* _plays, Point _target);
	void waterHeal(PlayScene* _plays, Point _target);

	Vector<Card*>* getVec();
	int getRange() const;
	void pushBack(Card* c);

protected:
	Vector<Card*> _elements;
	int range;
	bool is_line;
};

#endif
