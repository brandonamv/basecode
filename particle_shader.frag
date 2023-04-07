
#version 330 core
out vec4 color;

uniform vec4 Color;
uniform float size;
void main()
{
     if(dot(gl_PointCoord-0.5,gl_PointCoord-0.5)>0.25) 
        discard;
    else
        color = (Color);
}  