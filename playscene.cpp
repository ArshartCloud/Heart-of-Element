#include "playscene.h"
#define _GID_MOVE 50
#define _GID_SKILL 51

USING_NS_CC;
HandCardHud *PlayScene::_hud = NULL;
Player* PlayScene::_player = NULL;
CCTMXTiledMap *PlayScene::_tileMap = NULL;

Scene* PlayScene::createScene()
{
	Scene* scene = Scene::create();
	PlayScene* layer = PlayScene::create();
	HandCardHud::setPlayScene(layer);
	HandCardHud* hud = HandCardHud::create();
	_hud = hud;
	scene->addChild(layer);
	scene->addChild(hud);
	layer->toStandbyPhase(); // begin the game
	return scene;
}

bool PlayScene::init()
{
	if (!Layer::init())
	{
		return false;
	}
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto mapLayer = LayerColor::create(Color4B(78, 127, 41, 255));
	this->addChild(mapLayer, -1);

	_tileMap = TMXTiledMap::create("des1.tmx");
	mapLayer->setContentSize(_tileMap->getContentSize());
	_meta = _tileMap->getLayer("meta");
	_mvarea = _tileMap->getLayer("MoveArea");
	_meta->setVisible(false);
	_foreground = _tileMap->getLayer("foreground");
	mapLayer->addChild(_tileMap, 10);

	TMXObjectGroup *objects = _tileMap->getObjectGroup("Objects");
	CCASSERT(NULL != objects, "'Objects' object group not found");
	auto spawnPoint = objects->getObject("swp");
	CCASSERT(!spawnPoint.empty(), "SpawnPoint object not found");
	int x = spawnPoint["x"].asInt();
	int y = spawnPoint["y"].asInt();

	_player = Player::create("Player.png");
	_player->setPosition(x, y);
	addChild(_player);
	addChild(_player->getEle());

	initEnemy();

	this->scheduleUpdate();

	//listener
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [&](Touch *touch, Event *unused_event)->bool { return true; };
	listener->onTouchEnded = CC_CALLBACK_2(PlayScene::onTouchEnded, this);
	this->_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

//	toStandbyPhase();
	return true;
}

void PlayScene::initEnemy() {
	int x, y;
	TMXObjectGroup *object2 = _tileMap->getObjectGroup("EnemySpawn1");
	for (auto& eSpawnPoint : object2->getObjects()) {
		ValueMap& dict = eSpawnPoint.asValueMap();
		if (dict["enemy"].asInt() == 1) {
			x = dict["x"].asInt();  // pos
			y = dict["y"].asInt();
//			this->addEnemyAtPos(tileCoordForPosition(Point(x, y)));
			auto pos = tileCoordForPosition(Point(x, y));
			pos = PositionFortileCoord(pos);
			Role* r = Role::create(dict);
			r->setPosition(pos);
			this->addChild(r);
			_enemys.pushBack(r);
		}
	}
}

void PlayScene::update(float fDelta) {
	auto position = _player->getPosition();
	auto winSize = Director::getInstance()->getWinSize();

	int x = MAX(position.x, winSize.width / 2);
	int y = MAX(position.y, winSize.height / 2);
	x = MIN(x, (_tileMap->getMapSize().width * this->_tileMap->getTileSize().width) - winSize.width / 2);
	y = MIN(y, (_tileMap->getMapSize().height * _tileMap->getTileSize().height) - winSize.height / 2);
	auto actualPosition = Point(x, y);

	Point centerOfView = Point(winSize.width / 2, winSize.height / 2);
	auto viewPoint = centerOfView - actualPosition + Point(0, 50);
	this->setPosition(viewPoint);
}

void PlayScene::setPlayerPosition(Point position)
{
	position = PositionFortileCoord(position);

	auto ac1 = MoveTo::create(0.5, position);
	auto ac2 = Sequence::create(CallFunc::create(this, callfunc_selector(PlayScene::ResetArea)), ac1, CallFunc::create(this, callfunc_selector(PlayScene::toPlayPhase)), NULL);

	_player->runAction(ac2);

}

void PlayScene::onTouchEnded(Touch *touch, Event *unused_event)
{
	auto touchLocation = touch->getLocation();
	touchLocation = this->convertToNodeSpace(touchLocation);
//	touchLocation = tileCoordForPosition(touchLocation); // tile position

	Point tilePos = tileCoordForPosition(touchLocation);
	//	int tileGid = _mvarea->getTileGIDAt(tilePos);
	if (_phase == Phase::move) {
		if (_move_area.find(tilePos) != _move_area.end()) {
			if ((touchLocation.x < _tileMap->getMapSize().width * _tileMap->getTileSize().width) &&
				(touchLocation.y < _tileMap->getMapSize().height * _tileMap->getTileSize().height) &&
				touchLocation.y >= 0 &&
				touchLocation.x >= 0)
				setPlayerPosition(tilePos); // and set to play phase
	//			toPlayPhase();
		}
	}
	else if (_phase == Phase::play) {
		if (_move_area.find(tilePos) != _move_area.end()) {
			if ((touchLocation.x < _tileMap->getMapSize().width * _tileMap->getTileSize().width) &&
				(touchLocation.y < _tileMap->getMapSize().height * _tileMap->getTileSize().height) &&
				touchLocation.y >= 0 &&
				touchLocation.x >= 0)
				if (_player->useSkill(this, tilePos)) {
					clearEle();
					toEnemyPhase();
				}
		}
	}
}

Point PlayScene::tileCoordForPosition(Point position)
{
	int x = position.x / _tileMap->getTileSize().width;
	int y = ((_tileMap->getMapSize().height * _tileMap->getTileSize().height) - position.y - 0.1) / _tileMap->getTileSize().height;
	return Point(x, y);
}

Point PlayScene::PositionFortileCoord(Point tileCoord)
{
	int x = tileCoord.x * _tileMap->getTileSize().width + _tileMap->getTileSize().width * 0.5;
	int y = (_tileMap->getMapSize().height * _tileMap->getTileSize().height) - (tileCoord.y + 0.5) *  _tileMap->getTileSize().height;
	return Point(x, y);
}

void PlayScene::ResetArea() {
	for (set<Point>::iterator it = _move_area.begin(); it != _move_area.end(); it++) {
		int tileGid = _mvarea->getTileGIDAt(*it);
		if (tileGid) {
			_mvarea->removeTileAt(*it);
		}
	}
	_move_area.clear();
}
void PlayScene::dfs(int x, int y, int m){

}

void PlayScene::GetMoveArea(CCNode *pSender){
	ResetArea();
	Role *r = (Role*)pSender;
	auto playerPos = _player->getPosition();
	Point tileCoord = this->tileCoordForPosition(playerPos);
	//	int x = tileCoord.x, y = tileCoord.y;
	int mp = r->getMP();

	int dx[4] = { 1, -1, 0, 0 };
	int dy[4] = { 0, 0, 1, -1 };
	queue<Point> q;
	map<Point, int> m;
	m.insert(make_pair(tileCoord, mp));
	q.push(tileCoord);
	while (!q.empty()) {
		Point fr = q.front();
		int mv = m[fr];
		q.pop();
		if (!mv) continue;

		Point t;
		for (int i = 0; i < 4; i++) {
			t.x = fr.x + dx[i];
			t.y = fr.y + dy[i];
			if (t.x >= 0 && t.y >= 0 && t.x < _tileMap->getMapSize().width && t.y < _tileMap->getMapSize().height)
				if (_move_area.find(t) == _move_area.end()) {
					int tileGid = _meta->getTileGIDAt(t);
					if (tileGid) {
						auto properties = _tileMap->getPropertiesForGID(tileGid).asValueMap();
						if (!properties.empty()) {
							auto collision = properties["Collidable"].asString();
							if ("true" != collision) {
								_move_area.insert(t);
								q.push(t);
								m.insert(make_pair(t, mv - 1));
							}
						}
					}
				}
		}
	}
	//	dfs(x, y, mp);
	for (set<Point>::iterator it = _move_area.begin(); it != _move_area.end(); it++) {
		_mvarea->setTileGID(_GID_MOVE, *it);
	}

}

void PlayScene::GetSkillArea(Element* e) {
	ResetArea();

	auto playerPos = _player->getPosition();
	Point tileCoord = this->tileCoordForPosition(playerPos);
	//	int x = tileCoord.x, y = tileCoord.y;
	int mp = e->getRange();

	int dx[4] = { 1, -1, 0, 0 };
	int dy[4] = { 0, 0, 1, -1 };
	queue<Point> q;
	map<Point, int> m;
	m.insert(make_pair(tileCoord, mp));
	q.push(tileCoord);
	while (!q.empty()) {
		Point fr = q.front();
		int mv = m[fr];
		q.pop();
		if (!mv) continue;

		Point t;
		for (int i = 0; i < 4; i++) {
			t.x = fr.x + dx[i];
			t.y = fr.y + dy[i];
			if (t.x >= 0 && t.y >= 0 && t.x < _tileMap->getMapSize().width && t.y < _tileMap->getMapSize().height)
				if (_move_area.find(t) == _move_area.end()) {
					int tileGid = _meta->getTileGIDAt(t);
					if (tileGid) {
						_move_area.insert(t);
						q.push(t);
						m.insert(make_pair(t, mv - 1));
					}
				}
		}
	}
	//	dfs(x, y, mp);
	for (set<Point>::iterator it = _move_area.begin(); it != _move_area.end(); it++) {
		_mvarea->setTileGID(_GID_SKILL, *it);
	}

}

Player*  PlayScene::getPlayer() {
	return _player;
}

void PlayScene::drawCards(int num) {
	for (int i = 0; i < num; i++) {
		if (_player->getHands()->size() >= 6) return;
		_player->drawCard();
		_hud->_update(_player->getHands());
	}
}

void PlayScene::add_to_hand(Card* c) {
	_player->drawCard(c);
	_hud->_update(_player->getHands());
}

void PlayScene::clearEle() {
	_hud->clearEle();
}

Vector<Role*>* PlayScene::getEnemy() {
	return &_enemys;
}

PlayScene::Phase PlayScene::getPhase() {
	return _phase;
}

void PlayScene::toStandbyPhase() {
	drawCards(2);
	toMovePhase();
}

void PlayScene::toMovePhase() {
	_phase = Phase::move;
//	ResetArea();
	GetMoveArea(_player);
}
void PlayScene::toPlayPhase() {
	_phase = Phase::play;
//	GetSkillArea();
}
void PlayScene::toEnemyPhase(){
	_phase = Phase::enemy; 
	for (auto& ene : _enemys) {
		GetMoveArea(ene);
	}
	toStandbyPhase();
}

void PlayScene::damage(Role* from, Role* to, int num) {
	if ( !(to->takeDamage(num))) {
		death(to);
	}
	
}

void PlayScene::death(Role* role) {
	if (role != _player) {
		_enemys.eraseObject(role, false);
		removeChild(role);
	}
}