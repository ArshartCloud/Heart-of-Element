#include "HandCardHud.h"
#include "playscene.h"
#include "Element.h"
#include <iostream>
#define _ELEGAP 56

#pragma execution_character_set("utf-8")
using namespace std;

USING_NS_CC;

PlayScene* HandCardHud::_plays = NULL;

bool HandCardHud::init()
{
	if (!Layer::init())
	{
		return false;
	}
	_player = _plays->getPlayer();
//	_element = Element::create("element/bar.png");
//	addChild(_element);
//	_hands = _plays->getPlayer()->getHands();
	_sel_card = NULL;

	auto visibleSize = Director::getInstance()->getVisibleSize();

	TouchSystem();

	int margin = 10;
	HandCardArea = Sprite::create("handcard_area.png");
	HandCardArea->setPosition(Vec2(visibleSize.width / 2 , HandCardArea->getContentSize().height / 2));
	this->addChild(HandCardArea, 1);

	_eleArea_low = Sprite::create("element/bar_low.png");
	_eleArea_low->setPosition(110, 170);
	addChild(_eleArea_low, 1);

	_eleArea_up = Sprite::create("element/bar_up.png");
	_eleArea_up->setPosition(110, 170);
	addChild(_eleArea_up, 5);

	auto cancel = MenuItemImage::create("element/button1.png", "element/button2.png", CC_CALLBACK_0(HandCardHud::returnEle, this));
//	auto item3 = MenuItemLabel::create(Label::createWithTTF("Cancel", "fonts/Marker Felt.ttf", 40), CC_CALLBACK_0(HandCardHud::returnEle, this));
//	auto item2 = MenuItemLabel::create(Label::createWithTTF("Move", "fonts/Marker Felt.ttf", 40), CC_CALLBACK_0(PlayScene::toMovePhase, _plays));
//	auto item1 = MenuItemLabel::create(Label::createWithTTF("Draw", "fonts/Marker Felt.ttf", 40), CC_CALLBACK_0(PlayScene::drawCards, _plays, 1));
//	item1->setColor(Color3B::YELLOW);
	auto menu = Menu::create(cancel, NULL);
	menu->setPosition(Director::getInstance()->getVisibleSize().width - cancel->getContentSize().width / 2, 80);
	menu->alignItemsVerticallyWithPadding(10);
	this->addChild(menu, 2);

	_head = Sprite::create("role/big/matt.png");
	_head->setPosition(_head->getContentSize().width / 2, Director::getInstance()->getVisibleSize().height - _head->getContentSize().height / 2);
	addChild(_head, 5);

	auto bg = Sprite::create("bg.png");
	bg->setPosition(Director::getInstance()->getVisibleSize() / 2);
	addChild(bg, 1);

	_pmsg = Label::createWithTTF("Hp:", "fonts/MFSHANGHEI_NONCOMMERCIAL-REGULAR.OTF", 20);
	_pmsg->setPosition(274, 471);
	addChild(_pmsg, 5);
	showPInfo();

	_spdes = Label::createWithTTF("", "fonts/MFSHANGHEI_NONCOMMERCIAL-REGULAR.OTF", 20);
	_spdes->setPosition(Director::getInstance()->getVisibleSize().width / 2, 491);
	addChild(_spdes, 5);
	
	_emsg = Label::createWithTTF("1", "fonts/MFSHANGHEI_NONCOMMERCIAL-REGULAR.OTF", 20);
	_emsg->setPosition(686, 471);
	addChild(_emsg, 5);

	_role = nullptr;
	return true;
}

void HandCardHud::TouchSystem() {
	dispatcher = Director::getInstance()->getEventDispatcher();

	auto listener = EventListenerTouchOneByOne::create();

	listener->onTouchBegan = [&](Touch* touch, Event* event){
		_sel_card = nullptr;
		auto touch_pos = touch->getLocation();
		if (_plays->getPhase() == PlayScene::Phase::play) {
			int size = _player->getHands()->size();
			for (int i = 0; i < size; i++) {
				Card* sp = _player->getHands()->at(i);
				if (sp->getBoundingBox().containsPoint(touch_pos)) {
					_sel_card = sp;
					return true;
				}
			}
		}
		return true;
	};

	listener->onTouchMoved = [&](Touch* touch, Event* event){
		auto touch_pos = touch->getLocation();
		if (_sel_card != nullptr) {
			_sel_card->setPosition(touch_pos);
		}
	};

	listener->onTouchEnded = [&](Touch* touch, Event* event){
		if (_sel_card != nullptr && _player->getEle()->getVec()->size() < 3) {
 			add_to_ele();
			showEle();
			showCards();
		}
		_sel_card = nullptr;
		log("onTouchEnded");
	};

	dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void HandCardHud::setPlayScene(PlayScene *_ps) {
	_plays = _ps;
}

void HandCardHud::showPInfo() {
	char showStr[20];
	sprintf(showStr, "Hp: %d", _player->getHp());
	_pmsg->setString(showStr);
}

void HandCardHud::setSpellDes(string s) {
	_spdes->setString(s);
}

void HandCardHud::clearDes() {
	_spdes->setString("");
}

void HandCardHud::showInfo(Role* r) {
	clearInfo();
	string s = "role/big/";
	s += r->get_name();
	s += ".png";
	_role = Sprite::create(s);
	_role->setPosition(Director::getInstance()->getVisibleSize().width - _role->getContentSize().width / 2, Director::getInstance()->getVisibleSize().height - _role->getContentSize().height / 2);
	addChild(_role, 5);

	string str = r->get_name();
	char showStr[100];
	sprintf(showStr, "\nHP: %d\nMOV: %d\nRAN: %d\nATK: %d\n", r->getHp(), r->getMP(), r->getRange(), r->getAtk());
	str += showStr;
	_emsg->setString(str);
}

void HandCardHud::clearInfo() {
	_emsg->setString("");
	if (_role != nullptr)
	removeChild(_role, false);
	_role = nullptr;
}

void HandCardHud::_update(Vector<Card*>* newhands) {
	showCards();
}

void HandCardHud::showCards(){
	int size = _player->getHands()->size();
	for (int i = 0; i < size; i++) {
		Card* sp = _player->getHands()->at(i);
		removeChild(sp, false);
	}
	for (int i = 0; i < size; i++) {
		Card* sp = _player->getHands()->at(i);
		sp->setPosition(100 * (i + 1), 70);
		addChild(sp, 4);
	}
}

void HandCardHud::showEle() {
	int size = _player->getEle()->getVec()->size();
	for (int i = 0; i < size; i++) {
		Card* sp = _player->getEle()->getVec()->at(i);
		removeChild(sp, false);
	}
//	int size = _player->getEle()->getVec()->size();
	Point fir(_eleArea_low->getPositionX() - _ELEGAP, _eleArea_low->getPositionY());
	for (int i = 0; i < size; i++) {
		Card* sp = _player->getEle()->getVec()->at(i);
		sp->setPosition(_ELEGAP * (i) + fir.x, fir.y);
		addChild(sp, 4);
	}
}

void HandCardHud::add_to_ele() {
	auto ele = _sel_card->toEle();
	_player->getEle()->pushBack(_plays, ele);
	_player->getHands()->eraseObject(_sel_card, false);
	removeChild(_sel_card, true);
	_plays->GetSkillArea(_player->getEle());
}

void HandCardHud::clearEle() {
	int size = _player->getEle()->getVec()->size();
	for (int i = 0; i < size; i++) {
		Card* ele = _player->getEle()->getVec()->at(i);
		removeChild(ele, false);
	}
	_player->getEle()->clear();
	showEle();
	clearDes();
}

void HandCardHud::returnEle() {
	if (_plays->getPhase() != PlayScene::Phase::play) return;
	int size = _player->getEle()->getVec()->size();
	for (int i = 0; i < size; i++) {
		Card* ele = _player->getEle()->getVec()->at(i);
		Card* card = ele->toCard();
		_plays->add_to_hand(card);
	}
	clearEle();
	showCards();
	_plays->ResetArea();
}
