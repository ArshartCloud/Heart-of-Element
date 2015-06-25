#include "Role.h"
#include "playscene.h"
using namespace std;
USING_NS_CC;

Role* Role::create(const std::string& name, Point pos){
	Role* ret = new Role();
	if (ret && ret->init(name)){
		ret->autorelease();
		return ret;
	}
	CC_SAFE_DELETE(ret);
	return nullptr;
}

Role* Role::create(ValueMap& dict) {
	Role* ret = new Role();
	string name = "Player.png";
	int x = dict["x"].asInt();
	int y = dict["y"].asInt();
	ret->_tileCoor = PlayScene::tileCoordForPosition(Point(x, y));
	ret->hp = dict["hp"].asInt();
	if (ret && ret->init(name)){
		ret->autorelease();
		return ret;
	}
	CC_SAFE_DELETE(ret);
	return nullptr;
}

bool Role::init(const std::string& name){
	_move_point = 3;
	
	return initWithFile(name);
}

void Role::MoveTo(Point position){ ; }

Point Role::getPos() {
	return _tileCoor;
}
int Role::getHp() {
	return hp;
}

bool Role::takeDamage(int dam){
	hp -= dam;
	if (hp) return true;
	else return false;
}

int Role::getMP(){
	return _move_point;
}


Player* Player::create(const std::string& name){
	Player* ret = new Player();
	ret->_element = Element::create("element/bar.png");
	if (ret && ret->init(name)){
		ret->autorelease();
		return ret;
	}
	CC_SAFE_DELETE(ret);
	return nullptr;
}

bool Player::init(const std::string& name){
	_move_point = 3;
	return initWithFile(name);
}

void Player::drawCard(){
	int ran = CCRANDOM_0_1() * 4;
	Card::CardType f = (Card::CardType)ran;
	Card* card = Card::create(f);
	_hands.pushBack(card);
}

void Player::drawCard(Card* c) {
	_hands.pushBack(c);
}

bool Player:: useSkill(PlayScene* _plays, Point target) {
	return _element->onEffort(_plays, target);
}

Vector<Card*>* Player::getHands() {
	return &_hands;
}

Element* Player::getEle() {
	return _element;
}

Enemy* Enemy::create(const std::string& name){
	Enemy* ret = new Enemy();
	if (ret && ret->init(name)){
		ret->autorelease();
		return ret;
	}
	CC_SAFE_DELETE(ret);
	return nullptr;
}
Enemy* Enemy::create(ValueMap& dict) {
	Enemy* ret = new Enemy();
	string name = "Player.png";
	int x = dict["x"].asInt();
	int y = dict["y"].asInt();
	ret->_tileCoor = PlayScene::tileCoordForPosition(Point(x, y));
	ret->hp = dict["hp"].asInt();
	if (ret && ret->init(name)){
		ret->autorelease();
		return ret;
	}
	CC_SAFE_DELETE(ret);
	return nullptr;
}
bool Enemy::init(const std::string& name){
	_move_point = 3;

	return initWithFile(name);
}