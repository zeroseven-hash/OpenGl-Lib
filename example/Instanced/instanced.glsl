#vertex shader
#version 330 core
layout (location = 0)in vec4 aPos;
layout (location = 1) in vec4 aColor;
layout(location = 2) in mat4 offset;


out vec4 oColor;
void main()
{
	gl_Position=offset*aPos;
	oColor=aColor;
}


#fragment shader
#version 330 core
in vec4 oColor;
out vec4 FragColor;

void main()
{
	FragColor=vec4(oColor.xyz,1.0f);
}