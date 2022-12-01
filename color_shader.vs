
#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec3 color;
layout (location = 3) in vec2 aTexCoord;


//modelo
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

//materiales
uniform vec4 Ka, Kd, Ks;
uniform float alfa;

out vec3 Normal;
out vec3 Color;
out vec2 TexCoord;

void main()
{
	gl_Position = projection * view * model * vec4(position, 1.0);
	
	// ojo: investigar el problema de que el "model" contiene scaling factors
	Normal = (model * vec4(normal, 0.0)).xyz;
	
	Color = color;
	TexCoord = aTexCoord;
} 

