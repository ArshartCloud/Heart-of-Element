#ifndef _HandCardHud_H__
#define _HandCardHud_H__

#include "cocos2d.h"
#include "Role.h"
#include "Element.h"
#include <string>
//#include "playscene.h"
using namespace std;
USING_NS_CC;

class PlayScene;
enum Phase;

class HandCardHud : public cocos2d::Layer
{
public:
	virtual bool init();
	CREATE_FUNC(HandCardHud);

	void TouchSystem();
	static void setPlayScene(PlayScene *_plays);

	void _update(Vector<Card*>* newHands);
	void showCards();
	void showEle();
	void add_to_ele();
	void clearEle();
	void returnEle(); // get all ele back to hands

private:
	EventDispatcher* dispatcher;

	Sprite* HandCardArea;
	Sprite* ElementArea;
//	Vector<Card*>* _hands;
	Label* mesg;

	static PlayScene *_plays;
//	Element* _element;
	Player* _player;
	Card* _sel_card;
};


#endif
