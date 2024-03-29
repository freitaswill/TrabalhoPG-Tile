#version 410 
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;
layout (location = 2) in vec2 texCoord;

out vec3 ourColor;
out vec2 TexCoord;
  
uniform mat4 model;
uniform mat4 projection;
uniform float offsetX;
uniform float offsetY;
uniform float qtdSpritesX;
uniform float qtdSpritesY;
uniform float clique;

void main()
{	
	if(clique == 1)
	{
	ourColor = vec3(1.0, 1.0, 10.0);
	}
	else if(clique == 2)
	{
	ourColor = vec3(5.0, 1.0, 3.0);
	}
	/*else if(clique == 3)
	{
	ourColor = vec3(1.0, 60.0, 1.0);
	}*/
	else
	{
		ourColor = color;
	}
    gl_Position = projection * model * vec4(position, 1.0f);
	// We swap the y-axis by substracing our coordinates from 1. This is done because most images have the top y-axis inversed with OpenGL's top y-axis.
	// TexCoord = texCoord;
	TexCoord = vec2(texCoord.x/ qtdSpritesX + offsetX , (1.0 - texCoord.y) / qtdSpritesY + offsetY);
}