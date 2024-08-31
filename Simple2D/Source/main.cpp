#include <GLFW/glfw3.h>
#include <iostream>
#include "Graphics.h"


void GlfwErrorCallback(int _errorCode, const char* _description)
{
	printf(_description);
}

void GlfwKeyCallback(GLFWwindow* _window, int _key, int _scancode, int _action, int mods)
{

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
	p_window = glfwCreateWindow(800, 600, "Simple2DGame", nullptr, nullptr);

	if (!p_window)
	{
		printf("ERROR: glfw window does not exist.\n");
		return -1;
	}

	glfwSetKeyCallback(p_window, GlfwKeyCallback);


	Graphics& graphics = Graphics::GetInstance();
	graphics.Init(p_window);


	while (!glfwWindowShouldClose(p_window))
	{
		//do stuff
		glfwPollEvents();
		graphics.StartDraw();

		graphics.Draw();

		graphics.EndDraw();
	}

	graphics.Shutdown();
	glfwDestroyWindow(p_window);
	glfwTerminate();

	return 0;
}