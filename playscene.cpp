#include "playscene.h"
#define _GID_MOVE 2
#define _GID_SKILL 3
#define _MAPHEIGHT 1000
#define _MAPWIDTH 1000

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
	scene->addChild(hud, 1);
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

	srand(time(0));
	auto mapLayer = LayerColor::create(Color4B(78, 127, 41, 255));
	this->addChild(mapLayer, -1);

	_tileMap = TMXTiledMap::create("maps/mission1.tmx");
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

	initGrid();

	_player = Player::create("role/small/matt.png");
	_player->setAnchorPoint(Point(0.5, 0.2));
	_player->setPosition(PositionFortileCoord(tileCoordForPosition(Point(x, y))));
	addChild(_player);
	_tile[tileCoordForPosition(Point(x, y))].setRole(_player);

	initEnemy();
	this->scheduleUpdate();

	//listener
	auto listener = EventListenerTouchOneByOne::create();
//	listener->onTouchBegan = [&](Touch *touch, Event *unused_event)->bool { return true; };
	listener->onTouchBegan = CC_CALLBACK_2(PlayScene::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(PlayScene::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(PlayScene::onTouchEnded, this);
	this->_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

//	toStandbyPhase();
	return true;
}

void  PlayScene::initGrid() {
	int w = _tileMap->getMapSize().width, h = _tileMap->getMapSize().height;
	for (int i = 0; i < w; i++) {
		for (int j = 0; j < h; j++) {
			Point t(i, j);
			Grid g(t);
			int tileGid = _meta->getTileGIDAt(t);
			if (tileGid) {
				auto properties = _tileMap->getPropertiesForGID(tileGid).asValueMap();
				if (!properties.empty()) {
					auto collision = properties["Collidable"].asString();
					if ("true" == collision) {
						g.setMov(false);
					}
				}
			}
			_tile.insert(make_pair(Point(i, j), g));
		}
	}
}

void PlayScene::initEnemy() {
	int x, y;
	TMXObjectGroup *object2 = _tileMap->getObjectGroup("EnemySpawn1");
	for (auto& eSpawnPoint : object2->getObjects()) {
		ValueMap& dict = eSpawnPoint.asValueMap();
			x = dict["x"].asInt();  // pos
			y = dict["y"].asInt();
			auto tpos = tileCoordForPosition(Point(x, y));
			auto pos = PositionFortileCoord(tpos);
			Enemy* r = Enemy::create(dict);
			r->setPosition(pos);
			r->setAnchorPoint(Point(0.5, 0.2));
			this->addChild(r);
			_enemys.pushBack(r);
			_tile[tpos].setRole(r);
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
	auto viewPoint = centerOfView - actualPosition ;
	this->setPosition(viewPoint);
}

void PlayScene::moveRole(Role* r, Point position)
{
	_tile[r->getPos()].setRole(nullptr);
	_tile[position].setRole(r);
	position = PositionFortileCoord(position);

	auto ac1 = MoveTo::create(0.5, position);
	if (r == _player) {
		if (_phase == Phase::move) {
			auto ac2 = Sequence::create(CallFunc::create(this, callfunc_selector(PlayScene::ResetArea)), ac1, CallFunc::create(this, callfunc_selector(PlayScene::toPlayPhase)), NULL);
			_player->runAction(ac2);
		}
		else {
			auto ac2 = Sequence::create(CallFunc::create(this, callfunc_selector(PlayScene::ResetArea)), ac1, CallFunc::create(this, callfunc_selector(PlayScene::toEnemyPhase)), NULL);
			_player->runAction(ac2);
		}
	}
	else {
		r->runAction(ac1);
	}
}

bool PlayScene::onTouchBegan(Touch* touch, Event* unused_event) {
	auto touch_pos = touch->getLocation();
	touch_pos = this->convertToNodeSpace(touch_pos);
	Point tilePos = tileCoordForPosition(touch_pos);
	if (_move_area.find(tilePos) != _move_area.end()) sel_tile = tilePos;
	for (auto& ene : _enemys) {
		if (ene->getPos() == tilePos) {
			_hud->showInfo(ene);
			return true;
		}
	}
	return true;
}

void PlayScene::onTouchMoved(Touch* touch, Event* unused_event) {
	auto touch_pos = touch->getLocation();
	touch_pos = this->convertToNodeSpace(touch_pos);
	Point tilePos = tileCoordForPosition(touch_pos);
	for (auto& ene : _enemys) {
		if (ene->getPos() == tilePos) {
			_hud->showInfo(ene);
			return;
		}
	}
}

void PlayScene::onTouchEnded(Touch *touch, Event *unused_event)
{
	auto touchLocation = touch->getLocation();
	touchLocation = this->convertToNodeSpace(touchLocation);

	Point tilePos = tileCoordForPosition(touchLocation);
	_hud->clearInfo();
	if (_phase == Phase::move) {
		if (_move_area.find(tilePos) != _move_area.end()) {
			if (sel_tile == tilePos)
				moveRole(_player, tilePos); // and set to play phase
		}
	}
	else if (_phase == Phase::play) {
		if (_move_area.find(tilePos) != _move_area.end()) {
			if (sel_tile == tilePos)
				if (_player->useSkill(this, tilePos)) {
					clearEle();
					toEnemyPhase();
				}
		}
	}
	sel_tile = Point(0, 0);
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

HandCardHud* PlayScene::getHud() {
	return _hud;
}

map<Point, Grid>& PlayScene::gettile() { return _tile; }

void PlayScene::reStart() {
	auto scene = PlayScene::createScene();
	Director::getInstance()->replaceScene(scene);
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

void PlayScene::GetMoveArea(CCNode *pSender){
	ResetArea();
	Role *r = (Role*)pSender;
	auto rolePos = r->getPosition();
	Point tileCoord = this->tileCoordForPosition(rolePos);
	//	int x = tileCoord.x, y = tileCoord.y;
	int mp = r->getMP();
	if (mp == -1) return;

	int dx[4] = { 1, -1, 0, 0 };
	int dy[4] = { 0, 0, 1, -1 };
	queue<Point> q;
	map<Point, int> m;
	m.insert(make_pair(tileCoord, mp));
	q.push(tileCoord);
	_move_area.insert(tileCoord);
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
					if (_tile[t].is_mov()) {
						_move_area.insert(t);
						q.push(t);
						m.insert(make_pair(t, mv - 1));
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
	bool l = e->getLine();
	if (mp == -1) return;

	int dx[4] = { 1, -1, 0, 0 };
	int dy[4] = { 0, 0, 1, -1 };
	queue<Point> q;
	map<Point, int> m;
	m.insert(make_pair(tileCoord, mp));
	q.push(tileCoord);
	_move_area.insert(tileCoord);
	if (l) {
		for (int i = 0; i < 4; i++) {
			Point t(tileCoord);
			for (int j = 0; j < mp; j++) {
				t.x = t.x + dx[i];
				t.y = t.y + dy[i];
				if (t.x >= 0 && t.y >= 0 && t.x < _tileMap->getMapSize().width && t.y < _tileMap->getMapSize().height) {
					_move_area.insert(t);
				}
			}
		}
	} 
	else while (!q.empty()) {
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
					_move_area.insert(t);
					q.push(t);
					m.insert(make_pair(t, mv - 1));
				}
		}
	}
	//	dfs(x, y, mp);
	for (set<Point>::iterator it = _move_area.begin(); it != _move_area.end(); it++) {
		_mvarea->setTileGID(_GID_SKILL, *it);
	}

}

void PlayScene::GetPrioPoint() {
//	ResetArea();
	auto playerPos = _player->getPosition();
	Point tileCoord = this->tileCoordForPosition(playerPos);
	//	int x = tileCoord.x, y = tileCoord.y;

	int dx[4] = { 1, -1, 0, 0 };
	int dy[4] = { 0, 0, 1, -1 };
	queue<Point> q;
	bool vis[_MAPWIDTH][_MAPHEIGHT];
	int wid = _tileMap->getMapSize().width;
	for (int i = 0; i < wid; i++) memset(vis, 0, sizeof(vis));

	_tile[tileCoord].setPrio(0);
	q.push(tileCoord);
	while (!q.empty()) {
		Point fr = q.front();
		int dis = _tile[fr].getPrio();
		q.pop();
		Point t;
		for (int i = 0; i < 4; i++) {
			t.x = fr.x + dx[i];
			t.y = fr.y + dy[i];
			int tx = t.x, ty = t.y;
			if (t.x >= 0 && t.y >= 0 && t.x < _tileMap->getMapSize().width && t.y < _tileMap->getMapSize().height)
				if (!vis[tx][ty]) {
					if (_tile[t].is_justmov()) {
						vis[tx][ty] = true;
						q.push(t);
						_tile[t].setPrio(dis + 1);
					}
				}
		}
	}
}

Player* PlayScene::getPlayer() {
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

Vector<Enemy*>* PlayScene::getEnemy() {
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
	GetMoveArea(_player);
}
void PlayScene::toPlayPhase() {
	_phase = Phase::play;
}
void PlayScene::toEnemyPhase(){
	GetPrioPoint();
	_phase = Phase::enemy; 
	for (auto& ene : _enemys) {
		GetMoveArea(ene);
		int min = _tile[ene->getPos()].getPrio();
		Point des = ene->getPos();
		for (auto &i : _move_area) {
			if (min > _tile[i].getPrio() && _tile[i].getPrio()) {
				min = _tile[i].getPrio();
				des = i;
			}
		}
		if (ene->getStra() == Enemy::Strategy::Stable) {
			;
		}
		else if (ene->getStra() == Enemy::Strategy::Protected) {
			if (min <= ene->getRange()) ene->setStra(Enemy::Strategy::Aggressive);
		}
		if (ene->getStra() == Enemy::Strategy::Aggressive) {
			moveRole(ene, des);
		}
		if (_tile[ene->getPos()].getPrio() <= ene->getRange()) attackPlayer(ene);
	}
	toStandbyPhase();
}

void PlayScene::attackPlayer(Enemy* from) {
	damage(from, _player, from->getAtk());
}

void PlayScene::damage(Role* from, Role* to, int num) {
	if ( !(to->takeDamage(num))) {
		death(to);
	}
	if (to == _player) _hud->showPInfo();
	
}

void PlayScene::death(Role* role) {
	if (role != _player) {
		for (auto &ene : _enemys) {
			if (ene->getPos() == role->getPos()) {
				_tile[ene->getPos()].setRole(nullptr);
				_enemys.eraseObject(ene, false);
				break;
			}
		}
//		_enemys.eraseObject(role, false);
		removeChild(role);
	}
	else {
		gameOver();
	}
}

void PlayScene::gameOver() {
	stopAllActions();
//	removeChild(_player);
	auto item1 = MenuItemLabel::create(Label::createWithTTF("Retry~!", "fonts/Marker Felt.ttf", 40), CC_CALLBACK_0(PlayScene::reStart, this));
	auto menu = Menu::create(item1, NULL);
	menu->setPosition(Director::getInstance()->getVisibleSize().width / 2, Director::getInstance()->getVisibleSize().height / 2);
	addChild(menu, 2);
}