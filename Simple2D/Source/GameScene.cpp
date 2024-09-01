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

	
	player.Update(_deltaTime);
	enemy.Update(_deltaTime);

	level.Update();

	bool isColliding = false;

	isColliding = Collision::CheckCollision(player.GetPosition(),
		player.GetSize(), enemy.GetPosition(), enemy.GetSize());

	if (isColliding)
	{
		player.ResolveEnemyCollision();
	}

	

}

void GameScene::Draw(void)
{
	Player& player = Player::GetInstance();
	Enemy& enemy = Enemy::GetInstance();
	LevelGeneration& level = LevelGeneration::GetInstance();

	level.Draw();
	player.Draw();
	enemy.Draw();
}

void GameScene::Shutdown(void)
{
	Player& player = Player::GetInstance();
	Enemy& enemy = Enemy::GetInstance();
	LevelGeneration& level = LevelGeneration::GetInstance();

	player.Shutdown();
	enemy.Shutdown();
	level.Shutdown();
}
