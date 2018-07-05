#version 330

layout (location = 0) in vec3 pos;

out vec4 vcol;

uniform mat4 model;
uniform mat4 proj;

void main()
{
	gl_Position = proj * model * vec4(pos, 1.0);
	vcol = vec4(clamp(pos, 0.0f, 1.0f), 1.0f);
}