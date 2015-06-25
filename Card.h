#ifndef __CARD_H__
#define __CARD_H__

#include "cocos2d.h"

USING_NS_CC;

class Card : public cocos2d::Sprite
{
public:
	//	CREATE_FUNC(Role);
	enum CardType { fire, water, wind, earth, fireE, waterE, windE, earthE };
	static Card* create(CardType ty);
	virtual bool init(const std::string& name);

	CardType getType();
	Card* toEle();
	Card* toCard();
//	virtual void doEffect(const Point target) = 0;
protected:
	CardType card_type;
};

#endif
