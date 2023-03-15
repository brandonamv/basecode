
#version 330 core
in vec4 base_color;
in vec4 ParticleColor;
out vec4 color;


void main()
{
    color = (base_color * ParticleColor);
}  