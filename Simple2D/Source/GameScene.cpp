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
	m_tileSize = { 100, 100 };

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

	bool isColliding = false;

	for (int k = 0; k < level.GetWallPositions().size(); ++k)
	{
		//check for player colliding against walls
		isColliding = Collision::CheckCollision(player.GetPosition(),
			m_tileSize, level.GetWallPositions()[k], m_tileSize);

		if (isColliding)
		{
			player.ResolveWallCollision(level.GetWallPositions()[k], m_tileSize);
		}


		//check for enemy colliding against walls
		//isColliding = Collision::CheckCollision(enemy.GetPosition(),
		//	m_tileSize, level.GetWallPositions()[k], m_tileSize);
		//
		//if (isColliding)
		//{
		//	enemy.ResolveWallCollision(level.GetWallPositions()[k], m_tileSize);
		//}
	}

	isColliding = Collision::CheckCollision(player.GetPosition(),
		m_tileSize, enemy.GetPosition(), m_tileSize);

	if (isColliding)
	{
		player.ResolveEnemyCollision();
	}


	player.Draw();
	enemy.Draw();

}

void GameScene::Shutdown(void)
{
}
