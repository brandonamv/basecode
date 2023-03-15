#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 init_color;
// <vec2 position, vec2 texCoords>

out vec4 base_color;
out vec4 ParticleColor;

uniform mat4 projection;
uniform mat4 view;
uniform vec3 offset;
uniform vec4 color;

void main()
{
    float scale = 10.0f;
    base_color = vec4(init_color,1.0);
    ParticleColor = color;
    gl_Position = projection * view * vec4((position * scale) + offset, 1.0);
}