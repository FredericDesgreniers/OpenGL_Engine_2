// shadertype=glsl
#version 330 core


layout (location = 0) in vec3 position;



out vec3 ourColor;


uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 addColor;

void main()
{
	gl_Position = projection * view * model * vec4(position, 1.0f);

	ourColor = vec3((position.y+1)/2,(position.y+1)/2,0) + addColor;


}