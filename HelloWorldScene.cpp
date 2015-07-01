#include "HelloWorldScene.h"
#include "playscene.h"
#include "SimpleAudioEngine.h"
#include <iostream>

#pragma execution_character_set("utf-8")
using namespace std;
using namespace CocosDenshion;

USING_NS_CC;
uint32_t tg = 0;

Scene* HelloWorld::createScene()
{
	auto scene = Scene::create();
	auto layer = HelloWorld::create();
	scene->addChild(layer);
//	auto scene = PlayScene::createScene();
	return scene;
}

bool HelloWorld::init()
{
	if (!Layer::init())
	{
		return false;
	}
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	Sprite* bg = Sprite::create("start.png");
	bg->setPosition(Director::getInstance()->getVisibleSize().width / 2, Director::getInstance()->getVisibleSize().height / 2);
	addChild(bg);

	auto begin = MenuItemLabel::create(Label::createWithTTF("Play", "fonts/Marker Felt.ttf", 40), CC_CALLBACK_0(HelloWorld::playGame, this));
	begin->setColor(Color3B::BLACK);
	auto menu = Menu::create(begin, NULL);
	menu->setPosition(Director::getInstance()->getVisibleSize().width / 2, Director::getInstance()->getVisibleSize().height / 2 - 20);
	this->addChild(menu, 2);

	SimpleAudioEngine::getInstance()->preloadBackgroundMusic("bgm/start.mp3");
	SimpleAudioEngine::getInstance()->preloadBackgroundMusic("bgm/bgm1.mp3");
	SimpleAudioEngine::getInstance()->playBackgroundMusic("bgm/start.mp3", true);

	return true;
}

void HelloWorld::playGame() {
	SimpleAudioEngine::getInstance()->stopBackgroundMusic();
	SimpleAudioEngine::getInstance()->playBackgroundMusic("bgm/bgm1.mp3", true);
	auto scene = PlayScene::createScene();
	Director::getInstance()->replaceScene(scene);
}