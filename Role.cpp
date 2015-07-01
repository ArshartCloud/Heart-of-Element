#include "Role.h"
#include "playscene.h"
#include "time.h"
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

bool Role::init(const std::string& name){
//	_move_point = 3;
	
	return initWithFile(name);
}

void Role::MoveTo(Point position){ ; }

Point Role::getPos() {
	return PlayScene::tileCoordForPosition(this->getPosition());
}
int Role::getHp() {
	return hp;
}

int Role::getmaxHp() const { return maxHp; }
int Role::getRange() const { return ran; }
int Role::getAtk() const { return atk; }
string Role::get_name() const{ return _name; }
string Role::getCla() const{ return cla; }

bool Role::takeDamage(int dam){
	hp -= dam;
	if (hp > 0) return true;
	else return false;
}

int Role::getMP(){
	return mov;
}


Player* Player::create(const std::string& name){
	Player* ret = new Player();
	ret->_element = new Element();
	if (ret && ret->init(name)){
		ret->autorelease();
		return ret;
	}
	CC_SAFE_DELETE(ret);
	return nullptr;
}

bool Player::init(const std::string& name){
	mov = 3;
	hp = 20;
	maxHp = 20;
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
//	string name = "enemy.png";
	string cla = "role/small/";
	cla += dict["class"].asString();
	cla += ".png";
	ret->_name = dict["_name"].asString();
	int x = dict["x"].asInt();
	int y = dict["y"].asInt();
	ret->hp = dict["hp"].asInt();
	ret->mov = dict["mov"].asInt();
	ret->atk = dict["atk"].asInt();
	ret->ran = dict["ran"].asInt();
	string str =  dict["strategy"].asString();
	if (str == "protected") ret->_stra = Strategy::Protected;
	else if (str == "aggressive") ret->_stra = Strategy::Aggressive;
	else if (str == "stable") ret->_stra = Strategy::Stable;
	if (ret && ret->init(cla)){
		ret->autorelease();
		return ret;
	}
	CC_SAFE_DELETE(ret);
	return nullptr;
}
bool Enemy::init(const std::string& name){
//	_move_point = 3;

	return initWithFile(name);
}

void Enemy::setStra(Enemy::Strategy s) {
	_stra = s;
}

Enemy::Strategy Enemy::getStra() {
	return _stra;
}