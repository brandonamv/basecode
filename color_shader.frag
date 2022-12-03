#version 330 core

//interpolated normal
in vec3 Normal;  
in vec2 TexCoord;
in vec3 FragPos;
uniform sampler2D ourTexture;


//luces
uniform bool ambient,lambert,phong;
uniform vec3 light_color;
uniform vec3 viewPos;

//materiales
uniform vec3 Ka, Kd, Ks;
uniform float alfa, shininess;

// output color
out vec4 color;

void main()
{
	vec3 tempColor=Kd;
	vec3 lightPos=vec3(10,30,10);
	
	if (phong){
		vec3 norm = normalize(Normal);
		vec3 lightDir = normalize(lightPos - FragPos); 
		vec3 viewDir = normalize(viewPos - FragPos);
		vec3 reflectDir = reflect(-lightDir, norm);  
		float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
		vec3 specular = spec * light_color;  
		tempColor = (Ka + Kd + specular);

	}

	if (lambert){
		vec3 norm = normalize(Normal);
		vec3 lightDir = normalize(lightPos - FragPos); 
		float diff = max(dot(norm, lightDir), 0.0);
		vec3 diffuse = light_color*(diff*Kd);
		tempColor=diffuse+Ka;
	}
	
	if (ambient){
		tempColor=tempColor*light_color;
	}
	if(TexCoord.x>0){
		color = texture(ourTexture, TexCoord)*vec4(tempColor,1);
	}else{
		color=vec4(tempColor,1);
	}
	
} 
