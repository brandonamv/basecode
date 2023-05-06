
#version 330 core
out vec4 color;

uniform vec4 Color;
uniform bool point;
uniform sampler2D ourTexture;

in vec2 TexCoord; 
void main()
{
    
    if(point){
        if(dot(gl_PointCoord-0.5,gl_PointCoord-0.5)>0.25) 
            discard;
        else
            color = (Color);
    }
    else
        color = texture(ourTexture, TexCoord)*Color;
    
      }  