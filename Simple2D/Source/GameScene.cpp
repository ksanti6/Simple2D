/**********************************************************************************************************************
*
* Author : Kiara Santiago
* File   : GameScene.cpp
* Purpose: where all the game code lives! so it doesnt clog up main
*
**********************************************************************************************************************/
#include "GameScene.h"
#include "Player.h"               //for all things player
#include "LevelGeneration.h"      //for generating the level
#include "Collision.h"            //for checking collision bt player and enemy
#include "Enemy.h"                //for all things enemy
#include "PathingAlgorithm.h"     //for initializing the A Star algorithm
#include "Graphics.h"             //for drawing win and lose

/************************************************
*
* for getting an instance of the gamescene
*
************************************************/
GameScene& GameScene::GetInstance(void)
{
	static GameScene game;
	return game;
}

/************************************************
*
* the init of all inits, initializing all things
* for the game to work
*
************************************************/
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
}

/************************************************
*
* updates all the objects in game and checks for
* collisions, and for win/lose conditions
*
************************************************/
void GameScene::Update(float _deltaTime)
{
	Player& player = Player::GetInstance();
	Enemy& enemy = Enemy::GetInstance();
	LevelGeneration& level = LevelGeneration::GetInstance();

	//if the player has lost, dont update anymore
	//lose condition == player loses all lives
	if (player.GetLives() <= 0)
	{
		return;
	}

	//if the player has won, dont update anymore
	//win condition == no more cheese on map
	if (level.GetNumberofCheeseLeft() == 0)
	{
		return;
	}

	//update the player and enemy, then check for collisions
	player.Update(_deltaTime);
	enemy.Update(_deltaTime);

	level.Update();

	bool isColliding = false;

	isColliding = Collision::CheckCollision(player.GetPosition(),
		player.GetSize(), enemy.GetPosition(), enemy.GetSize());

	if (isColliding)
	{
		player.AdjustLives(1);

		//not dead yet
		if (player.GetLives() > 0)
		{
			//reset positions
			level.ResetPlayerEnemyPositions();
			enemy.ResetPathing();
		}
	}
}

/************************************************
*
* draws all the objects in the game
*
************************************************/
void GameScene::Draw(void)
{
	Player& player = Player::GetInstance();
	Enemy& enemy = Enemy::GetInstance();
	LevelGeneration& level = LevelGeneration::GetInstance();

	//lose condition - draw lost screen
	if (player.GetLives() <= 0)
	{
		Graphics& graphics = Graphics::GetInstance();
		graphics.Draw(Graphics::Textures::lose, { 800, 450 }, { 500, 500 });
		return;
	}

	//win condition - draw win screen
	if (level.GetNumberofCheeseLeft() == 0)
	{
		Graphics& graphics = Graphics::GetInstance();
		graphics.Draw(Graphics::Textures::win, { 800, 450 }, { 500, 500 });
		return;
	}

	level.Draw();
	player.Draw();
	enemy.Draw();
}

/************************************************
*
* shutdown of all the shutdowns!
* make sure everything gets cleaned up
*
************************************************/
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

/************************************************
*
* for hitting R reset
*
************************************************/
void GameScene::Reset(void)
{
	Shutdown();
	Init();
}