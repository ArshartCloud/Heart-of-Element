#ifndef __NewScene_H__
#define __NewScene_H__

#include "Role.h"
#include "cocos2d.h"
#include "HandCardHud.h"
#include "Element.h"
#include <string>
using namespace std;
USING_NS_CC;

class PlayScene: public cocos2d::Layer
{
public:
	enum Phase{standby, move, play, enemy};
	static cocos2d::Scene* createScene();
	virtual bool init();
	CREATE_FUNC(PlayScene);
	void initEnemy();

	virtual void update(float fDelta);
	
	static Point tileCoordForPosition(Point position);
	static Point PositionFortileCoord(Point tileCoord);

	//role
	void damage(Role* from, Role* to, int num = 1);
	void death(Role* role);
	void dfs(int x, int y, int m);
	void ResetArea();
	void GetMoveArea(CCNode *pSender);
	void GetSkillArea(Element* e);

//player
	void setPlayerPosition(Point position);
	void onTouchEnded(Touch *touch, Event *unused_event);
	static Player* getPlayer();
	void drawCards(int num); // draw n cards
	void add_to_hand(Card* c);
	void clearEle();
	//phase
	Phase getPhase();
	void toStandbyPhase();
	void toMovePhase();
	void toPlayPhase();
	void toEnemyPhase();
//enemy
	Vector<Role*>* getEnemy();

private:
	static CCTMXTiledMap *_tileMap;
	CCTMXLayer *_background;
	TMXLayer *_meta;
	TMXLayer *_foreground;
	TMXLayer *_mvarea;
	
	Phase _phase;
	static Player* _player;
	set<Point> _move_area;
//	set<Point> _skill_area;
	Vector<Role*> _enemys;

	static HandCardHud *_hud;

};
#endif