#include <GLFW/glfw3.h>
#include <iostream>
#include "Graphics.h"
#include "Player.h"
#include "GameScene.h"



void GlfwErrorCallback(int _errorCode, const char* _description)
{
	printf(_description);
}

void GlfwKeyCallback(GLFWwindow* _window, int _key, int _scancode, int _action, int _mods)
{
	(void)_scancode; (void)_mods;

	if (_key == GLFW_KEY_ESCAPE && _action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(_window, GLFW_TRUE);
	}

	if (_key == GLFW_KEY_R && _action == GLFW_PRESS)
	{
		//restart?
	}

	Player& player = Player::GetInstance();
	player.Move(_key, _action);

}

int main(void)
{
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

	glfwSetKeyCallback(p_window, GlfwKeyCallback);


	Graphics& graphics = Graphics::GetInstance();
	graphics.Init(p_window);

	GameScene& game = GameScene::GetInstance();
	game.Init();


	float startTime = glfwGetTime();
	float endTime = glfwGetTime();
	float deltaTime = 0.0f;

	while (!glfwWindowShouldClose(p_window))
	{
		//do stuff
		glfwPollEvents();

		startTime = glfwGetTime();
		graphics.StartDraw();

		game.Update(deltaTime);

		graphics.EndDraw();
		endTime = glfwGetTime();
		deltaTime = endTime - startTime;
		
	}

	game.Shutdown();

	graphics.Shutdown();
	glfwDestroyWindow(p_window);
	glfwTerminate();

	return 0;
}