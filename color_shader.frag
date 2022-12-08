#version 330 core

//interpolated normal
in vec3 Normal;  
in vec2 TexCoord;
in vec3 FragPos;
uniform sampler2D ourTexture;


//luces
uniform int option;
uniform bool ambient,lambert,phong;
uniform vec3 ambient_color,lambert_color,phong_color;
uniform vec3 viewPos;

//materiales
uniform vec3 Ka, Kd, Ks;
uniform float alfa, shininess;

//verificando bounding
uniform bool bounding;

// output color
out vec4 color;

void main()
{
	
	vec3 temp_color=Kd;
	vec3 lightPos=vec3(10,30,10);
	//modelo phong
	if (phong){
		vec3 norm = normalize(Normal);
		vec3 lightDir = normalize(lightPos - FragPos); 
		vec3 viewDir = normalize(viewPos - FragPos);
		vec3 reflectDir = reflect(-lightDir, norm);  
		float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
		//mantener propiedades del objeto
		if(option==0){
			vec3 specular=spec * Ks;
			temp_color=(Ka + Kd + specular);
		}//mix entre propiedades de objeto y escena
		else if(option==1){
			vec3 specular = spec * (phong_color*Ks);  
			temp_color=((Ka*ambient_color) + (Kd*lambert_color) + specular);
		}//solo propiedades de la escena
		else if(option==2){
			vec3 specular = spec * phong_color;  
			temp_color=(ambient_color + lambert_color + specular);
		}
	}

	//modelo lambert
	if (lambert){
		vec3 norm = normalize(Normal);
		vec3 lightDir = normalize(lightPos - FragPos); 
		float diff = max(dot(norm, lightDir), 0.0);
		//mantener propiedades del objeto
		if(option==0){
			vec3 diffuse = (diff*Kd);
			temp_color=diffuse+Ka;
		}//mix entre propiedades de objeto y escena
		else if(option==1){
			vec3 diffuse = ambient_color*(diff*Kd);
			temp_color=diffuse+(Ka*lambert_color);
		}//solo propiedades de la escena
		else if(option==2){
			vec3 diffuse = ambient_color*(diff);
			temp_color=diffuse+(lambert_color);
		}
	}

	if (ambient){
		//mantener propiedades del objeto
		if(option==0){
			temp_color=Kd;
		}//mix entre propiedades de objeto y escena
		else if(option==1){
			temp_color=ambient_color*Kd;
		}//solo propiedades de la escena
		else if(option==2){
			temp_color=ambient_color;
		}
	}
	if(bounding){
		color=vec4(Kd,1);
	}else{
		if(TexCoord.x>=0){
			color = texture(ourTexture, TexCoord)*vec4(temp_color,1);
		}else{
			color = vec4(temp_color,1);
		}
	}
	
} 
