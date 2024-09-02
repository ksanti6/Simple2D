/**********************************************************************************************************************
*
* Author : Kiara Santiago
* File   : GameScene.cpp
* Purpose: where all the game code lives! so it doesnt clog up main
*
**********************************************************************************************************************/
#include "GameScene.h"
#include "Player.h"               //for all things player
#include "LevelGeneration.h"      //for generating the level and handling all non player objects
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
* takes a level so we know what level to spawn
*
************************************************/
void GameScene::Init(int _level)
{
	Player& player = Player::GetInstance();
	player.Init();

	LevelGeneration& level = LevelGeneration::GetInstance();
	level.Init(_level);

	PathingAlgorithm& algorithm = PathingAlgorithm::GetInstance();
	algorithm.Init();
}

/************************************************
*
* updates all the objects in game and for 
* win/lose conditions
*
************************************************/
void GameScene::Update(float _deltaTime)
{
	Player& player = Player::GetInstance();
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

	//update the player and then the rest of the objects
	//collision checks happen in the level update
	player.Update(_deltaTime);
	level.Update(_deltaTime);
}

/************************************************
*
* draws all the objects in the game
*
************************************************/
void GameScene::Draw(void)
{
	Player& player = Player::GetInstance();
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
	LevelGeneration& level = LevelGeneration::GetInstance();
	PathingAlgorithm& algorithm = PathingAlgorithm::GetInstance();
	Grid& grid = Grid::GetInstance();

	player.Shutdown();
	level.Shutdown();
	algorithm.Shutdown();
	grid.Shutdown();
}

/************************************************
*
* for hitting R reset
*
************************************************/
void GameScene::Reset(int _level)
{
	Shutdown();
	Init(_level);
}