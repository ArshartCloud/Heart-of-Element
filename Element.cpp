#include "Element.h"
#include "playscene.h"
using namespace std;
USING_NS_CC;

Element::Element() {
	range = 3;
	is_line = false;
}

bool Element::onEffort(PlayScene* _plays, Point _target) {
	vector<Point> v;
	v.push_back(_target);
	if (area == "3X3") {
		int dx[9] = { -1, 0, 1, -1, 1, -1, 0, 1 };
		int dy[9] = { -1, -1, -1, 0, 0, 1, 1, 1 };
		for (int i = 0; i < 8; i++) {
			v.push_back(Point(_target.x + dx[i], _target.y + dy[i]));
		}
	}
	else if (area == "5X5") {
		int dx[25] = { -2, -1, 0, 1, 2, -2, -1, 0, 1, 2, -2, -1, 1, 2, -2, -1, 0, 1, 2, -2, -1, 0, 1, 2};
		int dy[25] = { -2, -2, -2, -2, -2, -1, -1, -1, -1, -1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2};
		for (int i = 0; i < 24; i++) {
			v.push_back(Point(_target.x + dx[i], _target.y + dy[i]));
		}
	}

	if (spell == "lifting wind") {
		if (_plays->gettile()[_target].is_mov()) {
			PlayScene::getHud()->clearEle();
			_plays->moveRole(_plays->getPlayer(), _target);
		}
		return false;
	}
	else for (auto &p : v) {   // damage/heal spell
		if (_plays->getPlayer()->getPos() == p) {
			_plays->damage(_plays->getPlayer(), _plays->getPlayer(), damage);
		}
		else for (auto& ene : *(_plays->getEnemy())) {
			if (ene->getPos() == p) {
				_plays->damage(_plays->getPlayer(), ene, damage);
				break;
			}
		}
	}

	//anim
	if (anim == "fire") {
		ParticleFire* fire = ParticleFire::create();
		fire->setPosition(PlayScene::PositionFortileCoord(_target));
		auto ac = MoveBy::create(1, Point(0, 0));
		_plays->addChild(fire);
		fire->runAction(Sequence::create(ac, CCCallFunc::create(fire, callfunc_selector(Node::removeFromParent)), NULL));
	}
	else if (anim == "meteor") {
		ParticleMeteor* meteor = ParticleMeteor::create();
		meteor->setPosition(PlayScene::PositionFortileCoord(_target));
		_plays->addChild(meteor);
		auto ac = MoveBy::create(1, Point(0, 0));
		meteor->runAction(Sequence::create(ac, CCCallFunc::create(meteor, callfunc_selector(Node::removeFromParent)), NULL));
	}

//	PlayScene::getHud()->clearDes();
	PlayScene::getHud()->clearEle();
	return true;
}

void Element::fireball(PlayScene* _plays, Point _target) {
	ParticleFire* firework = ParticleFire::create();
	Point pos = PlayScene::PositionFortileCoord(_target);
	firework->setPosition(pos);
	auto ac = MoveBy::create(1, Point(0, 0));
	_plays->addChild(firework);
	firework->runAction(Sequence::create(ac, CCCallFunc::create(firework, callfunc_selector(Node::removeFromParent)), NULL));

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

void Element::clear() {
	cleSpell();
	_elements.clear();
}

void Element::cleSpell() {
	spell = "";
	des = "";
	damage = 0;
	range = -1;
	is_line = false;
	area = "";
	anim = "";
}

Vector<Card*>* Element::getVec() {
	return &_elements;
}

int Element::getRange() const {
	return range;
}

bool Element::getLine() const { return this->is_line; }
string Element::getSpell() const { return spell; }
string Element::getDes() const { return des; }

void Element::pushBack(PlayScene* _plays, Card* c) {
	_elements.pushBack(c);
	vector<Card::CardType> v;
	for (auto &i : _elements) {
		v.push_back(i->getType());
	}
	sort(v.begin(), v.end());
	v.size();
	PlayScene::getHud()->showEle();

	cleSpell();
	range = 2;
	int size = _elements.size();
	if (v[0] == Card::CardType::fireE) {
		anim = "fire";
		damage++;
		if (size == 1) {
			spell = "fireball";
			des = "Fireball\nRange: 2\nEffort: Deal 1 damage to target.";
		}
		else {
			if (v[1] == Card::CardType::fireE) {
				damage++;
				if (size == 2) {
					spell = "firebolt";
					des = "Firebolt\nRange: 2\nEffort: Deal 2 damage to target.";
				}
				else {
					if (v[2] == Card::CardType::fireE) {
						area = "3X3";
						spell = "inferno flame";
						des = "Inferno Flame\nRange: 2\nEffort: Deal 2 damage to 3X3\n area around target.";
					}
					else if (v[2] == Card::CardType::windE) {
						range++;
						area = "3X3";
						spell = "firestorm";
						des = "Firestorm\nRange: 3\nEffort: Deal 2 damage to 3X3\n area around target.";
					}
					else if (v[2] == Card::CardType::earthE) {
						anim = "meteor";
						damage++;
						spell = "meteor falling";
						des = "Meteor Falling\nRange: 2\nEffort: Deal 3 damage to target.";
					}
				}
			}
			else if (v[1] == Card::CardType::windE) {
//				is_line = true;
				range++;
				if (size == 2) {
					spell = "blast ball";
					des = "Firestorm\nRange: 3\nEffort: Deal 1 damage to target.";
				}
				else if (v[2] == Card::CardType::windE) {
					range++;
					spell = "blast ball";
					des = "Firestorm\nRange: 4\nEffort: Deal 1 damage to target.";
				}
			}
			else if (v[1] == Card::CardType::earthE) {
				spell = "";
			}
		}
	}
	else if (v[0] == Card::CardType::waterE) {
		if (size == 1) {
			damage = -1;
			spell = "waterheal";
			des = "Waterheal\nRange: 2\nEffort: Heal 1 damage from target.";
		}
		else {
			if (v[1] == Card::CardType::waterE) {
				damage++;
				if (size == 2) {
					spell = "frostbolt";
					des = "Frostbolt\nRange: 2\nEffort: Deal 1 damage to target.";
				}
				else {
					if (v[2] == Card::CardType::waterE) {
						spell = "";
					}
					else if (v[2] == Card::CardType::windE) {
						range++;
						is_line = true;
						damage++;
						area = "3X3";
						spell = "freezing sky";
						des = "Freezing Sky\nRange: 3 in line\nEffort: Deal 2 damage to 3X3\n area around target.";
					}
					else if (v[2] == Card::CardType::earthE) {
						spell = "";
					}
				}
			}
			else if (v[1] == Card::CardType::windE) {
				if (size == 2) {
					spell = "";
				}
				else if (v[2] == Card::CardType::earthE) {
					damage = -3;
					range = 0;
					spell = "regeneration";
					des = "Regeneration\nRange: 0\nEffort: Heal 3 damage from target.";
				}
			}
			else if (v[1] == Card::CardType::earthE) {
				if (size == 2) {
					spell = "";
				}
				else {
					spell = "";
				}
			}
		}
	}
	else if (v[0] == Card::CardType::windE) {
		is_line = true;
		range++;
		if (size == 1) {
			damage = 1;
			spell = "aircut";
			des = "Aircut\nRange: 3 in line\nEffort: Deal 1 damage to target.";
		}
		else {
			if (v[1] == Card::CardType::windE) {
				range++;
				if (size == 2) {
					spell = "lifting wind";
					des = "Lifting Wind\nRange: 4 in line\nEffort: Move to valid target.";
				}
				else {
					if (v[2] == Card::CardType::windE) {
						range++;
						damage = 1;
						spell = "cloud blaster";
						des = "Cloud Blaster\nRange: 5 in line\nEffort: Deal 1 damage to target.";
					}
					else if (v[2] == Card::CardType::earthE) {
						anim = "meteor";
						damage = 1;
						area = "3X3";
						spell = "flying rock";
						des = "Flying Rock\nRange: 4 in line\nEffort: Deal 1 damage to 3X3\n area around target.";
					}
				}
			}
			else if (v[1] == Card::CardType::earthE) {
				anim = "meteor";
				damage++;
				if (size == 2) {
					area = "3X3";
					spell = "flying rock";
					des = "Flying Rock\nRange: 3 in line\nEffort: Deal 1 damage to 3X3\n area around target.";
				}
				else if (v[2] == Card::CardType::earthE){
					damage++;
					area = "3X3";
					spell = "flying rock";
					des = "Flying Rock\nRange: 3 in line\nEffort: Deal 2 damage to 3X3\n area around target.";
				}
			}
		}
	}
	else if (v[0] == Card::CardType::earthE) {
		range = 0;
		damage++;
		if (size == 1) {
			area = "3X3";
			spell = "earth bound";
			des = "Earth Bound\nRange: 0\nEffort: Deal 1 damage to 3X3\n area around target.";
		}
		else {
			if (v[1] == Card::CardType::earthE) {
				damage++;
				if (size == 2) {
					damage = -2;
					spell = "nature heal";
					des = "Nature Heal\nRange: 0\nEffort: Heal 2 damage from target.";
				}
				else {
					if (v[1] == Card::CardType::earthE) {
						damage++;
						area = "5X5";
						spell = "earthquake";
						des = "Earthquake\nRange: 0\nEffort: Deal 3 damage to 5X5\n area around target.";
					}
				}
			}
		}
	}

	if (spell == "") {
		cleSpell();
	}
	PlayScene::getHud()->setSpellDes(des);
}