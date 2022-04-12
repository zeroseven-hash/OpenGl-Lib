#vertex shader
#version 330 core
layout (location = 0)in vec4 aPos;
layout (location = 1) in vec4 aColor;
layout (location = 2) in vec2 aCoords;

uniform mat4 u_ModelMatrix;
uniform mat4 pers;
uniform mat4 view;
out vec2 oCoords;
void main()
{
	gl_Position=pers*view*u_ModelMatrix*aPos;
	oCoords=aCoords;
}


#fragment shader
#version 330 core


in vec2 oCoords;
out vec4 FragColor;
uniform sampler2D tex;
void main()
{
	
	FragColor=texture(tex,oCoords);
}