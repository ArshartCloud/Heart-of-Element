#include "Element.h"
#include "playscene.h"
using namespace std;
USING_NS_CC;
Element* Element::create(const std::string& name){
	Element* ret = new Element();
	if (ret && ret->init(name)){
		ret->autorelease();
		return ret;
	}
	CC_SAFE_DELETE(ret);
	return nullptr;
}

bool Element::init(const std::string& name){
	range = 3;
	is_line = false;
	return initWithFile(name);
}

bool Element::onEffort(PlayScene* _plays, Point _target) {
//	if (_target == NULL) { return; }
	int size = _elements.size();
	if (size == 1) {
		Card* first = _elements.at(0);
		if (first->getType() == Card::CardType::fireE) fireball(_plays, _target);
		else if (first->getType() == Card::CardType::waterE) waterHeal(_plays, _target);
		
	}
	return true;
}

void Element::fireball(PlayScene* _plays, Point _target) {
	ParticleFire* firework = ParticleFire::create();
	Point pos = PlayScene::PositionFortileCoord(_target);
	firework->setPosition(pos);
//	auto ac = FadeOut::create(1);
	
	_plays->addChild(firework);
//	_plays->clearEle();
	for (auto& ene : *(_plays->getEnemy())) {
		if (ene->getPos() == _target) {
			_plays->damage(_plays->getPlayer(), ene);
			return;
		}
	}
}

void Element::waterHeal(PlayScene* _plays, Point _target) {
	ParticleMeteor* snow = ParticleMeteor::create();
	Point pos = PlayScene::PositionFortileCoord(_target);
	snow->setPosition(pos);
	_plays->addChild(snow);
}

Vector<Card*>* Element::getVec() {
	return &_elements;
}

int Element::getRange() const {
	return range;
}

void Element::pushBack(Card* c) {
	_elements.pushBack(c);
}