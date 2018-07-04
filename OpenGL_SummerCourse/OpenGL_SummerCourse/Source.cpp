#include <stdio.h>
#include <string>
#include <cmath>

#include <gl\glew.h>
#include <GLFW\glfw3.h>

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

const GLint WIDTH = 800, HEIGHT = 600;

const float toRadians = 3.14159265f / 180.0f;

GLuint VAO, VBO, shader, uniformModel;

bool direction = true;
float triOffset = 0.0f;
float triMaxOffset = 0.7f;
float triIncrement = 0.0005f;

bool sizeDirection = true;
float curSIze = 0.4f;
float maxSize = 0.8f;
float minSize = 0.1f;

// vertex shader
static const char* vSHader = "		\n\
#version 330						\n\
									\n\
layout (location = 0) in vec3 pos;  \n\
									\n\
out vec4 vcol;						\n\
									\n\
uniform mat4 model;				\n\
									\n\
void main()							\n\
{									\n\
	gl_Position = model * vec4(pos, 1.0); \n\
	vcol = vec4(clamp(pos, 0.0f, 1.0f), 1.0f); \n\
}";

// fragment shader
static const char* fSHader = "		\n\
#version 330						\n\
									\n\
out vec4 col;						\n\
in vec4 vcol;						\n\
									\n\
void main()							\n\
{									\n\
	col = vcol; \n\
}";

void createTriangle()
{
	GLfloat vertices[] = 
	{
		-1.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		0.0f, 1.0f, 0.0f
	};

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

}

void addShader(GLuint theProgram, const char* shaderCode, GLenum shaderType)
{
	GLuint theShader = glCreateShader(shaderType);

	const GLchar* theCode[1];
	theCode[0] = shaderCode;

	GLint codeLength[1];
	codeLength[0] = strlen(shaderCode);

	glShaderSource(theShader, 1, theCode, codeLength);
	glCompileShader(theShader);

	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(theShader, sizeof(eLog), NULL, eLog);
		printf("Error compiling the %d shader: %s \n", shaderType, eLog);
		return;
	}

	glAttachShader(theProgram, theShader);
}

void compileShaders()
{
	shader = glCreateProgram();

	if (!shader)
	{
		printf("Error creating shader program!");
		return;
	}

	addShader(shader, vSHader, GL_VERTEX_SHADER);
	addShader(shader, fSHader, GL_FRAGMENT_SHADER);

	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	glLinkProgram(shader);
	glGetProgramiv(shader, GL_LINK_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
		printf("Error linking program: %s \n", eLog);
		return;
	}

	glValidateProgram(shader);
	glGetProgramiv(shader, GL_VALIDATE_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
		printf("Error validating program: %s \n", eLog);
		return;
	}

	uniformModel = glGetUniformLocation(shader, "model");

}

int main()
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

	GLFWwindow *win = glfwCreateWindow(WIDTH, HEIGHT, "Test window", NULL, NULL);	
	if (!win)
	{
		printf("Window creation failed");
		glfwTerminate();
		return 1;
	}

	// get buffer size information
	int bufferWidth, bufferHeight;
	glfwGetFramebufferSize(win, &bufferWidth, &bufferHeight);

	// set context for glew to use
	glfwMakeContextCurrent(win);

	//allow modern external features
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		printf("GLew init failed!");
		glfwDestroyWindow(win);
		glfwTerminate();
		return 2;
	}

	// set up viewport size
	glViewport(0, 0, bufferWidth, bufferHeight);

	createTriangle();
	compileShaders();

	// loop until win closed
	while (!glfwWindowShouldClose(win))
	{
		// get and handle user input events
		glfwPollEvents();

		if (direction)
		{
			triOffset += triIncrement;
		}
		else
		{
			triOffset -= triIncrement;
		}

		if (abs(triOffset) >= triMaxOffset)
		{
			direction = !direction;
		}

		if (sizeDirection)
		{
			curSIze += 0.001f;
		}
		else
		{
			curSIze -= 0.001f;
		}

		if (curSIze >= maxSize || curSIze <= minSize)
		{
			sizeDirection = !sizeDirection;
		}

		// clear window
		glClearColor(0.3f, 0.3f, 0.4f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shader);

		glm::mat4 model = {1, 0, 0, 0,
							0, 1, 0, 0,
							0, 0, 1, 0,
							0, 0, 0, 1};
		//model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		//model = glm::translate(model, glm::vec3(triOffset, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		glBindVertexArray(VAO);

		glDrawArrays(GL_TRIANGLES, 0, 3);

		glBindVertexArray(0);

		glUseProgram(0);

		glfwSwapBuffers(win);
	}

	return 0;
}