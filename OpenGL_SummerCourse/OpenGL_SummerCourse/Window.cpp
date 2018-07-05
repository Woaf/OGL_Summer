#include "Window.h"

Window::Window()
{
	width = 800;
	height = 600;
}

Window::Window(GLint windowsWidth, GLint windowHeight) : height(windowHeight), width(windowsWidth)
{
}

int Window::Initialize()
{
	// init glfw
	if (!glfwInit())
	{
		printf("Error while initializing glfw!");
		glfwTerminate();
		return 1;
	}

	//set up glfw window properties
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//core profile = no backw compat
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//allow forward compat
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	mainWindow = glfwCreateWindow(width, height, "Test window", NULL, NULL);
	if (!mainWindow)
	{
		printf("Window creation failed");
		glfwTerminate();
		return 1;
	}

	// get buffer size information
	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

	// set context for glew to use
	glfwMakeContextCurrent(mainWindow);

	//allow modern external features
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		printf("GLew init failed!");
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 2;
	}

	glEnable(GL_DEPTH_TEST);

	// set up viewport size
	glViewport(0, 0, bufferWidth, bufferHeight);
}


Window::~Window()
{
	glfwDestroyWindow(mainWindow);
	glfwTerminate();
}
