#ifndef __NewScene_H__
#define __NewScene_H__

#include "cocos2d.h"
#include "Role.h"
#include "Grid.h"
#include "HandCardHud.h"
#include "Element.h"
#include <string>
//#include <vector>
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
	void initGrid();

	virtual void update(float fDelta);
	
	static Point tileCoordForPosition(Point position);
	static Point PositionFortileCoord(Point tileCoord);
	static HandCardHud* getHud();
	map<Point, Grid>& gettile();
	void reStart();
	//role
	void damage(Role* from, Role* to, int num = 1);
	void death(Role* role);
	void ResetArea();
	void GetMoveArea(CCNode *pSender);
	void GetSkillArea(Element* e);
	void GetPrioPoint();

//player
	void moveRole(Role* r, Point position); // move to tile pos
	bool onTouchBegan(Touch* touch, Event* unused_event);
	void onTouchMoved(Touch* touch, Event* unused_event);
	void onTouchEnded(Touch *touch, Event *unused_event);
	static Player* getPlayer();
	void drawCards(int num); // draw n cards
	void add_to_hand(Card* c);
	void clearEle();
	void gameOver();
	//phase
	Phase getPhase();
	void toStandbyPhase();
	void toMovePhase();
	void toPlayPhase();
	void toEnemyPhase();
//enemy
	Vector<Enemy*>* getEnemy();
	void attackPlayer(Enemy* from);

private:
	static CCTMXTiledMap *_tileMap;
	CCTMXLayer *_background;
	TMXLayer *_meta;
	TMXLayer *_foreground;
	TMXLayer *_mvarea;
	
	Phase _phase;
	static Player* _player;
	set<Point> _move_area;
	Vector<Enemy*> _enemys;
	map<Point, Grid> _tile; // save the dict with player

	Point sel_tile;
	static HandCardHud *_hud;

};
#endif