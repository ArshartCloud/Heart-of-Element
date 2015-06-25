#include "HelloWorldScene.h"
#include "playscene.h"
#include <iostream>


#pragma execution_character_set("utf-8")
using namespace std;

USING_NS_CC;
uint32_t tg = 0;

Scene* HelloWorld::createScene()
{
	auto scene = PlayScene::createScene();
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

	return true;
}
