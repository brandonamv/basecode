#version 330 core

//interpolated normal
in vec3 Normal;  
in vec2 TexCoord;
in vec3 FragPos;
in vec3 N, L, V;

uniform sampler2D ourTexture;
uniform bool texturized;

//luces
uniform vec3 ambient_color, difuse_color, specular_color;
uniform vec3 viewPos;

//materiales
uniform vec3 Ka, Kd, Ks;
uniform float alfa, shininess;

//verificando bounding
uniform bool bounding;

// output color
out vec4 color;

vec4 phong(vec3 N, vec3 L, vec3 V){
	float NdotL = dot(N, L);
	vec3 tColor = Ka * ambient_color;
	if(NdotL > 0.0){
		vec3 R = normalize(2 * N * NdotL - L);
		float RdotV_n = pow(max(0.0, dot(R, V)), shininess);
		tColor = tColor + (NdotL * (difuse_color * Kd)) + (RdotV_n * (specular_color * Ks));
	}
	return vec4(tColor, 1.0);
}

void main()
{

	if(bounding){
		color=vec4(Kd,1);
	}else{
		vec4 temp_color= phong(N, L, V);
		if(texturized){
			color = texture(ourTexture, TexCoord)*temp_color;
		}else{
			color = temp_color;
		}
	}
	
} 
