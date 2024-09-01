#include "GameScene.h"
#include "Player.h"
#include "LevelGeneration.h"
#include "Collision.h"
#include "Enemy.h"

GameScene& GameScene::GetInstance(void)
{
	static GameScene game;
	return game;
}

void GameScene::Init(void)
{
	Player& player = Player::GetInstance();
	player.Init();

	Enemy& enemy = Enemy::GetInstance();
	enemy.Init();

	LevelGeneration& level = LevelGeneration::GetInstance();
	level.Init();
}

void GameScene::Update(float _deltaTime)
{
	Player& player = Player::GetInstance();
	Enemy& enemy = Enemy::GetInstance();
	LevelGeneration& level = LevelGeneration::GetInstance();

	level.DrawLevel();
	player.Update(_deltaTime);
	enemy.Update(_deltaTime);

	bool temp = Collision::AABBCollision(player.GetPosition(), { 100, 100 }, { 400, 300 }, { 100, 100 });

	if (temp)
	{
		player.ResolveWallCollision({ 400, 300 }, { 100, 100 });
	}

	player.Draw();
	enemy.Draw();

}

void GameScene::Shutdown(void)
{
}
