#include "Card.h"


USING_NS_CC;

Card* Card::create(CardType ty){
	std::string name;
	if (ty == fire) name = "card/fire.png";
	else if (ty == water) name = "card/water.png";
	else if (ty == wind) name = "card/wind.png";
	else if (ty == earth) name = "card/earth.png";
	else if (ty == fireE) name = "element/fire.png";
	else if (ty == waterE) name = "element/water.png";
	else if (ty == windE) name = "element/wind.png";
	else if (ty == earthE) name = "element/earth.png";
	Card* ret = new Card();
	if (ret && ret->init(name)){
		ret->autorelease();
		ret->card_type = ty;
		return ret;
	}
	CC_SAFE_DELETE(ret);
	return nullptr;
}

bool Card::init(const std::string& name){
	return initWithFile(name);
}

Card::CardType Card::getType() {
	return card_type;
}

Card* Card::toEle() {
	CardType ty = this->getType();
	if (ty == fire) {
		Card* card = Card::create(fireE);
		return card;
	}
	else if (ty == water) {
		Card* card = Card::create(waterE);
		return card;
	}
	else if (ty == wind) {
		Card* card = Card::create(windE);
		return card;
	}
	else if (ty == earth) {
		Card* card = Card::create(earthE);
		return card;
	}
	return NULL;
}

Card* Card::toCard() {
	CardType ty = this->getType();
	if (ty == fireE) {
		Card* card = Card::create(fire);
		return card;
	}
	else if (ty == waterE) {
		Card* card = Card::create(water);
		return card;
	}
	else if (ty == windE) {
		Card* card = Card::create(wind);
		return card;
	}
	else if (ty == earthE) {
		Card* card = Card::create(earth);
		return card;
	}
	return NULL;
}
