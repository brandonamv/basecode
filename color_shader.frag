#version 330 core

//interpolated normal
in vec3 Normal;  
in vec2 TexCoord;
in vec3 FragPos;
uniform sampler2D ourTexture;
uniform bool texturized;

//luces
uniform int option;
uniform bool ambient,lambert,phong;
uniform vec3 ambient_color,lambert_color,phong_color;
uniform vec3 viewPos;

struct light {    
    vec3 position;

	int type;
    float intensity;

	vec3 diffuse;
    vec3 specular;

	bool isActive;
};  



uniform light light1,light2,light3;
//materiales
uniform vec3 Ka, Kd, Ks;
uniform float alfa, shininess;

//verificando bounding
uniform bool bounding;

// output color
out vec4 color;

void main()
{
	
	vec3 temp_color=Kd*ambient_color;
	//modelo phong
	if (phong){
		if(light1.isActive){
			vec3 norm = normalize(Normal);
			vec3 lightDir = normalize(light1.position - FragPos); 
			vec3 viewDir = normalize(viewPos - FragPos);
			vec3 reflectDir = reflect(-light1.position, norm);  
			float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
			float attenuation=1.0;
			if(light1.type==1){
				float distance    = length(light1.position - FragPos);
				attenuation = 1.0 / distance;
				
			}
			//mantener propiedades del objeto
			if(option==0){
				vec3 specular=spec * Ks;
				temp_color+=(Ka*attenuation + Kd*attenuation + specular*attenuation)*light1.intensity;
			}//mix entre propiedades de objeto y escena
			else if(option==1){
				vec3 specular = spec * (light1.specular*Ks);  
				temp_color+=((Ka*ambient_color)*attenuation + (Kd*light1.diffuse)*attenuation + specular*attenuation)*light1.intensity;
			}//solo propiedades de la escena
			else if(option==2){
				vec3 specular = spec * light1.specular;  
				temp_color+=(ambient_color*attenuation + light1.diffuse*attenuation + specular*attenuation)*light1.intensity;
			}
		}
		if(light2.isActive){
			vec3 norm = normalize(Normal);
			vec3 lightDir = normalize(light2.position - FragPos); 
			vec3 viewDir = normalize(viewPos - FragPos);
			vec3 reflectDir = reflect(-light2.position, norm);  
			float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
			float attenuation=1.0;
			if(light2.type==1){
				float distance    = length(light2.position - FragPos);
				attenuation = 1.0 / distance;
				
			}
			//mantener propiedades del objeto
			if(option==0){
				vec3 specular=spec * Ks;
				temp_color+=(Ka*attenuation + Kd*attenuation + specular*attenuation)*light2.intensity;
			}//mix entre propiedades de objeto y escena
			else if(option==1){
				vec3 specular = spec * (light2.specular*Ks);  
				temp_color+=((Ka*ambient_color)*attenuation + (Kd*light2.diffuse)*attenuation + specular*attenuation)*light2.intensity;
			}//solo propiedades de la escena
			else if(option==2){
				vec3 specular = spec * light2.specular;  
				temp_color+=(ambient_color*attenuation + light2.diffuse*attenuation + specular*attenuation)*light2.intensity;
			}
		}
		if(light3.isActive){
			vec3 norm = normalize(Normal);
			vec3 lightDir = normalize(light3.position - FragPos); 
			vec3 viewDir = normalize(viewPos - FragPos);
			vec3 reflectDir = reflect(-light3.position, norm);  
			float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
			float attenuation=1.0;
			if(light3.type==1){
				float distance    = length(light3.position - FragPos);
				attenuation = 1.0 / distance;
				
			}
			//mantener propiedades del objeto
			if(option==0){
				vec3 specular=spec * Ks;
				temp_color+=(Ka*attenuation + Kd*attenuation + specular*attenuation)*light3.intensity;
			}//mix entre propiedades de objeto y escena
			else if(option==1){
				vec3 specular = spec * (light3.specular*Ks);  
				temp_color+=((Ka*ambient_color)*attenuation + (Kd*light3.diffuse)*attenuation + specular*attenuation)*light3.intensity;
			}//solo propiedades de la escena
			else if(option==2){
				vec3 specular = spec * light3.specular;  
				temp_color+=(ambient_color*attenuation + light3.diffuse	*attenuation + specular*attenuation)*light3.intensity;
			}
		}
		
	}

	//modelo lambert
	if (lambert){
		if(light3.isActive){
			vec3 norm = normalize(Normal);
			vec3 lightDir = normalize(light3.position - FragPos); 
			float diff = max(dot(norm, lightDir), 0.0);
			float attenuation=1.0;
			//mantener propiedades del objeto
			if(light3.type==1){
				float distance = length(light3.position - FragPos);
				attenuation = 1.0 / distance;
			}
			if(option==0){
				vec3 diffuse = (diff*Kd);
				temp_color+=(diffuse*attenuation+Ka*attenuation)*light3.intensity;
			}//mix entre propiedades de objeto y escena
			else if(option==1){
				vec3 diffuse = ambient_color*(diff*Kd);
				temp_color+=(diffuse*attenuation+(Ka*light3.diffuse*attenuation))*light3.intensity;
			}//solo propiedades de la escena
			else if(option==2){
				vec3 diffuse = ambient_color*(diff);
				temp_color+=(diffuse*attenuation+(light3.diffuse*attenuation))*light3.intensity;
			}
		}
		if(light2.isActive){
			vec3 norm = normalize(Normal);
			vec3 lightDir = normalize(light2.position - FragPos); 
			float diff = max(dot(norm, lightDir), 0.0);
			float attenuation=1.0;
			//mantener propiedades del objeto
			if(light3.type==1){
				float distance = length(light2.position - FragPos);
				attenuation = 1.0 / distance;
			}
			if(option==0){
				vec3 diffuse = (diff*Kd);
				temp_color+=(diffuse*attenuation+Ka*attenuation)*light2.intensity;
			}//mix entre propiedades de objeto y escena
			else if(option==1){
				vec3 diffuse = ambient_color*(diff*Kd);
				temp_color+=(diffuse*attenuation+(Ka*light2.diffuse*attenuation))*light2.intensity;
			}//solo propiedades de la escena
			else if(option==2){
				vec3 diffuse = ambient_color*(diff);
				temp_color+=(diffuse*attenuation+(light2.diffuse*attenuation))*light2.intensity;
			}
		}
		if(light1.isActive){
			vec3 norm = normalize(Normal);
			vec3 lightDir = normalize(light1.position - FragPos); 
			float diff = max(dot(norm, lightDir), 0.0);
			float attenuation=1.0;
			//mantener propiedades del objeto
			if(light1.type==1){
				float distance = length(light1.position - FragPos);
				attenuation = 1.0 / distance;
			}
			if(option==0){
				vec3 diffuse = (diff*Kd);
				temp_color+=(diffuse*attenuation+Ka*attenuation)*light1.intensity;
			}//mix entre propiedades de objeto y escena
			else if(option==1){
				vec3 diffuse = ambient_color*(diff*Kd);
				temp_color+=(diffuse*attenuation+(Ka*light1.diffuse*attenuation))*light1.intensity;
			}//solo propiedades de la escena
			else if(option==2){
				vec3 diffuse = ambient_color*(diff);
				temp_color+=(diffuse*attenuation+(light1.diffuse*attenuation))*light1.intensity;
			}
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
		if(texturized){
			color = texture(ourTexture, TexCoord)*vec4(temp_color,1);
		}else{
			color = vec4(temp_color,1);
		}
	}
	
} 
