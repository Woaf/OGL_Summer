#version 330

out vec4 col;
in vec4 vcol;

void main()
{
	col = vcol;
}