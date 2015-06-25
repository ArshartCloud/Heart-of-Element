#include "HandCardHud.h"
#include "playscene.h"
#include "Element.h"
#include <iostream>


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
	HandCardArea->setPosition(Vec2(visibleSize.width / 2 , HandCardArea->getContentSize().height / 8));
	this->addChild(HandCardArea);

	ElementArea = Sprite::create("element/bar.png");
	ElementArea->setPosition(100, 120);
	addChild(ElementArea);

	auto item3 = MenuItemLabel::create(Label::createWithTTF("Cancel", "fonts/Marker Felt.ttf", 40), CC_CALLBACK_0(HandCardHud::returnEle, this));
	auto item2 = MenuItemLabel::create(Label::createWithTTF("Move", "fonts/Marker Felt.ttf", 40), CC_CALLBACK_0(PlayScene::toMovePhase, _plays));
	auto item1 = MenuItemLabel::create(Label::createWithTTF("Draw", "fonts/Marker Felt.ttf", 40), CC_CALLBACK_0(PlayScene::drawCards, _plays, 1));
	item1->setColor(Color3B::YELLOW);
	auto menu = Menu::create(item1, item2, item3, NULL);
	menu->setPosition(Director::getInstance()->getVisibleSize().width - 50, 80);
	menu->alignItemsVerticallyWithPadding(10);
	this->addChild(menu, 1);

	return true;
}

void HandCardHud::TouchSystem() {
	dispatcher = Director::getInstance()->getEventDispatcher();

	auto listener = EventListenerTouchOneByOne::create();

	listener->onTouchBegan = [&](Touch* touch, Event* event){
		
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
		if (_sel_card) {
			_sel_card->setPosition(touch_pos);
		}
	};

	listener->onTouchEnded = [&](Touch* touch, Event* event){
		if (_sel_card && _player->getEle()->getVec()->size() < 3) {
 			add_to_ele();
			showEle();
		}
		_sel_card = NULL;
		log("onTouchEnded");
	};

	dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void HandCardHud::setPlayScene(PlayScene *_ps) {
	_plays = _ps;
}

void HandCardHud::_update(Vector<Card*>* newhands) {
	showCards();
}

void HandCardHud::showCards(){
	int size = _player->getHands()->size();
	for (int i = 0; i < size; i++) {
		Card* sp = _player->getHands()->at(i);
		removeChild(sp, false);
		sp->setPosition(100 * (i+1), 50);
		addChild(sp);
	}
}

void HandCardHud::showEle() {
	int size = _player->getEle()->getVec()->size();
	for (int i = 0; i < size; i++) {
		Card* sp = _player->getEle()->getVec()->at(i);
		removeChild(sp, false);
		sp->setPosition(50 * (i + 1), 150);
		addChild(sp);
	}
}

void HandCardHud::add_to_ele() {
	auto ele = _sel_card->toEle();
	_player->getEle()->pushBack(ele);
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
	_player->getEle()->getVec()->clear();
	showEle();
}

void HandCardHud::returnEle() {
	int size = _player->getEle()->getVec()->size();
	for (int i = 0; i < size; i++) {
		Card* ele = _player->getEle()->getVec()->at(i);
		Card* card = ele->toCard();
		_plays->add_to_hand(card);
	}
	clearEle();
	showCards();
}
