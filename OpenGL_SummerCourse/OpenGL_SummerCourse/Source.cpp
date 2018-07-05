#include <stdio.h>
#include <string>
#include <cmath>
#include <vector>

#include <gl\glew.h>
#include <GLFW\glfw3.h>

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

#include "Mesh.h"
#include "Shader.h"
#include "Window.h"

const GLint WIDTH = 800, HEIGHT = 600;

const float toRadians = 3.14159265f / 180.0f;

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderlList;

// vertex shader
static const char* vShader = "shaders/shader.vert";

// fragment shader
static const char* fSHader = "shaders/shader.frag";

void createObjects()
{
	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2, 
		2, 3, 0,
		0, 1, 2
	};

	GLfloat vertices[] =
	{
		-1.0f, -1.0f, 0.0f,
		0.0f, -1.0f, 1.0f,
		1.0f, -1.0f, 0.0f,
		0.0f, 1.0f, 0.0f
	};

	Mesh *obj1 = new Mesh();
	obj1->createMesh(vertices, indices, 12, 12);
	meshList.push_back(obj1);

	Mesh *obj2 = new Mesh();
	obj2->createMesh(vertices, indices, 12, 12);
	meshList.push_back(obj2);

}

void createShaders()
{
	Shader *shader1 = new Shader();
	shader1->createFromFiles(vShader, fSHader);
	shaderlList.push_back(*shader1);
}

int main()
{
	mainWindow = Window(WIDTH, HEIGHT);
	mainWindow.Initialize();

	createObjects();
	createShaders();

	GLuint uniformProj = 0, uniformModel = 0;
	glm::mat4 proj = glm::perspective(45.0f, mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);

	// loop until win closed
	while (!mainWindow.getShouldClose())
	{
		// get and handle user input events
		glfwPollEvents();

		// clear window
		glClearColor(0.3f, 0.3f, 0.4f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shaderlList[0].useShader();
		uniformModel = shaderlList[0].getModelLocation();
		uniformProj = shaderlList[0].getProjLocation();

		glm::mat4 model = {1, 0, 0, 0,
							0, 1, 0, 0,
							0, 0, 1, 0,
							0, 0, 0, 1};

		model = glm::translate(model, glm::vec3(-1.5f, 0.0f, -5.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProj, 1, GL_FALSE, glm::value_ptr(proj));
		meshList[0]->renderMesh();

		model = { 1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1 };
		model = glm::translate(model, glm::vec3(1.5f, 0.0f, -5.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[1]->renderMesh();

		glUseProgram(0);
		mainWindow.swapBuffers();
	}

	return 0;
}