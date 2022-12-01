#version 330 core

//interpolated normal
in vec3 Normal;  
in vec3 Color;  
in vec2 TexCoord;

uniform sampler2D ourTexture;

// constant color
uniform vec3 objectColor;

// output color
out vec4 color;

void main()
{
	if(TexCoord.x>0){
		color = texture(ourTexture, TexCoord)*  vec4(objectColor * Color, 1.0);
	}else{
		color =vec4(objectColor * Color, 1.0);
	}
	
	//color = texture(ourTexture, TexCoord);
} 
