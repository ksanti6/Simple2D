#include "GameScene.h"
#include "Player.h"
#include "LevelGeneration.h"
#include "Collision.h"
#include "Enemy.h"
#include "PathingAlgorithm.h"
#include "Graphics.h"

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

	PathingAlgorithm& algorithm = PathingAlgorithm::GetInstance();
	algorithm.Init();

	m_timer = 50;
	m_waitTimer = 0;
}

void GameScene::Update(float _deltaTime)
{
	Player& player = Player::GetInstance();

	if (player.GetLives() <= 0)
	{
		return;
	}

	m_timer -= _deltaTime;
	++m_waitTimer;

	if (m_waitTimer % 65 == 0)
	{
		printf_s("TIMER: %4.2f\n", m_timer);
	}

	if (m_timer < 0)
	{
		//win
		//printf_s("YOU WIN\n");
		//player.AdjustScore(0);

		return;

	}




	
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
		player.AdjustLives(1);

		if (player.GetLives() <= 0)
		{
			//game over
		}
		else
		{
			//reset positions
			level.ResetPlayerEnemyPositions();

		}
	}

	

}

void GameScene::Draw(void)
{
	Player& player = Player::GetInstance();
	Enemy& enemy = Enemy::GetInstance();
	LevelGeneration& level = LevelGeneration::GetInstance();

	if (player.GetLives() <= 0)
	{
		Graphics& graphics = Graphics::GetInstance();
		graphics.Draw(Graphics::Textures::lose, { 800, 450 }, { 500, 500 });
		return;
	}

	if (m_timer < 0)
	{
		Graphics& graphics = Graphics::GetInstance();
		graphics.Draw(Graphics::Textures::win, { 800, 450 }, { 500, 500 });
		return;
	}

	level.Draw();
	player.Draw();
	enemy.Draw();
}

void GameScene::Shutdown(void)
{
	Player& player = Player::GetInstance();
	Enemy& enemy = Enemy::GetInstance();
	LevelGeneration& level = LevelGeneration::GetInstance();
	PathingAlgorithm& algorithm = PathingAlgorithm::GetInstance();
	Grid& grid = Grid::GetInstance();

	player.Shutdown();
	enemy.Shutdown();
	level.Shutdown();
	algorithm.Shutdown();
	grid.Shutdown();
}

void GameScene::Reset(void)
{

	Shutdown();
	Init();
}
