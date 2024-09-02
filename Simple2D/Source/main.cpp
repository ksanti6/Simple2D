#include <GLFW/glfw3.h>
#include <iostream>
#include "Graphics.h"
#include "Player.h"
#include "GameScene.h"

#include "Grid.h"
#include"Enemy.h"


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
		GameScene& game = GameScene::GetInstance();
		game.Reset();
		
	}

	Player& player = Player::GetInstance();
	if (_key == GLFW_KEY_T && _action == GLFW_PRESS)
	{
		printf_s("requesting new enemy path\n");
		Enemy& enemy = Enemy::GetInstance();
		enemy.FollowPlayer();
		
		//Grid& grid = Grid::GetInstance();
		//
		//printf_s("Player Position is: %f %f \n", player.GetPosition().x, player.GetPosition().y);
		//
		//DirectX::XMUINT2 g = grid.WorldtoGrid(player.GetPosition());
		//
		//printf_s("World to Grid: %i %i \n", g.x, g.y);
		//DirectX::SimpleMath::Vector2 b = grid.GridtoWorld(g);
		//
		//printf_s("Grid to World: %f %f\n", b.x, b.y);

	}

	
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


	float startTime = static_cast<float>(glfwGetTime());
	float endTime = static_cast<float>(glfwGetTime());
	float deltaTime = 0.0f;

	while (!glfwWindowShouldClose(p_window))
	{
		//do stuff
		glfwPollEvents();

		startTime = static_cast<float>(glfwGetTime());
		graphics.StartDraw();

		game.Update(deltaTime);
		game.Draw();

		graphics.EndDraw();
		endTime = static_cast<float>(glfwGetTime());
		deltaTime = endTime - startTime;
		
	}

	game.Shutdown();

	graphics.Shutdown();
	glfwDestroyWindow(p_window);
	glfwTerminate();

	return 0;
}