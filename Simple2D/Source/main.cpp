/**********************************************************************************************************************
*
* Author : Kiara Santiago
* File   : main.cpp
* Purpose: the main entry point for the game
*
**********************************************************************************************************************/
#include <GLFW/glfw3.h>     //window and input
#include "Graphics.h"       //drawing with dx12
#include "Player.h"         //player input
#include "GameScene.h"      //games code

/************************************************
*
* callback for errors with glfw
*
************************************************/
void GlfwErrorCallback(int _errorCode, const char* _description)
{
	printf(_description);
}

/************************************************
*
* callback for interpretting input with glfw
*
************************************************/
void GlfwKeyCallback(GLFWwindow* _window, int _key, int _scancode, int _action, int _mods)
{
	//we dont need these
	(void)_scancode; (void)_mods;

	//close the window
	if (_key == GLFW_KEY_ESCAPE && _action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(_window, GLFW_TRUE);
	}

	GameScene& game = GameScene::GetInstance();
	//reset the game
	if (_key == GLFW_KEY_R && _action == GLFW_PRESS)
	{
		//restart?
		game.Reset(0);
	}

	//play a specific level
	//level 1
	if (_key == GLFW_KEY_1 && _action == GLFW_PRESS)
	{
		game.Reset(1);
	}
	//level 2
	if (_key == GLFW_KEY_2 && _action == GLFW_PRESS)
	{
		game.Reset(2);
	}
	//level 3
	if (_key == GLFW_KEY_3 && _action == GLFW_PRESS)
	{
		game.Reset(3);
	}
	//level 4
	if (_key == GLFW_KEY_4 && _action == GLFW_PRESS)
	{
		game.Reset(4);
	}

	//player movement
	Player& player = Player::GetInstance();
	player.Move(_key, _action);

}

/************************************************
*
* main! where all the fun happens
*
************************************************/
int main(void)
{
	//set up the window
	if (!glfwInit())
	{
		printf("ERROR: glfw init did not work.\n");
		return -1;
	}

	glfwSetErrorCallback(GlfwErrorCallback);
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

	GLFWwindow* p_window;
	p_window = glfwCreateWindow(1600, 900, "Simple2DGame", nullptr, nullptr);

	if (!p_window)
	{
		printf("ERROR: glfw window does not exist.\n");
		return -1;
	}

	//set up input
	glfwSetKeyCallback(p_window, GlfwKeyCallback);

	//initialize dx12 and game
	Graphics& graphics = Graphics::GetInstance();
	graphics.Init(p_window);

	GameScene& game = GameScene::GetInstance();
	game.Init(0);

	//for calculating delta time between frames
	float startTime = static_cast<float>(glfwGetTime());
	float endTime = static_cast<float>(glfwGetTime());
	float deltaTime = 0.0f;

	//our main update loop
	//update objects, draw them, etc
	while (!glfwWindowShouldClose(p_window))
	{
		glfwPollEvents();

		startTime = static_cast<float>(glfwGetTime());
		graphics.StartDraw();

		game.Update(deltaTime);
		game.Draw();

		graphics.EndDraw();
		endTime = static_cast<float>(glfwGetTime());
		deltaTime = endTime - startTime;
	}
	//time to shutdown everything, in reverse order of initialization
	game.Shutdown();
	graphics.Shutdown();
	glfwDestroyWindow(p_window);
	glfwTerminate();

	return 0;
}