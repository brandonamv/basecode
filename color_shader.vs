
#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 aTexCoord;


//modelo
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec2 maxTexture;
//luces
uniform vec3 ligth_position;
out vec3 N, L, V;

out vec3 Normal;
out vec2 TexCoord;
out vec3 FragPos;
void main()
{
	
	// ojo: investigar el problema de que el "model" contiene scaling factors
	Normal = (model * vec4(normal, 0.0)).xyz;

	FragPos = vec3(model * vec4(position, 1.0));

	vec4 eye_position = view * model * vec4(position, 1.0);
	gl_Position = projection * eye_position;
	N = normalize (Normal * normal);

	L = vec3 (normalize(vec4(ligth_position, 0.0) - eye_position));
	
	V = normalize( vec3(-eye_position));

	TexCoord = aTexCoord;
} 

