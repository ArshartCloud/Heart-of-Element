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

	void showPInfo();
	void setSpellDes(string s);
	void clearDes();
	void showInfo(Role* r);
	void clearInfo();

	void _update(Vector<Card*>* newHands);// update hands.
	void showCards();
	void showEle();
	void add_to_ele();
	void clearEle();
	void returnEle(); // get all ele back to hands

private:
	EventDispatcher* dispatcher;

	Sprite* HandCardArea;
	Sprite* _eleArea_up;
	Sprite* _eleArea_low;
	Label* mesg;

	static PlayScene *_plays;
	Player* _player;
	Card* _sel_card;

	//info
	Sprite* _role;
	Label* _emsg;
	//Label* _mp; // move point
	//player info
	Sprite* _head;
	Label* _pmsg;
	Label* _spdes;  // spell description
};


#endif
