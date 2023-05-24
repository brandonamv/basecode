#version 330 core
layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

out vec2 TexCoord;
uniform float size;
void main() {    
    gl_Position = gl_in[0].gl_Position + vec4(-size, -size, 0.0, 0.0); 
    TexCoord=vec2(0.0,0.0);
    EmitVertex();

    gl_Position = gl_in[0].gl_Position + vec4( size, -size, 0.0, 0.0); 
    TexCoord=vec2(1.0,0.0);
    EmitVertex();

    gl_Position = gl_in[0].gl_Position + vec4( -size, size, 0.0, 0.0); 
    TexCoord=vec2(0.0,1.0);
    EmitVertex();

    gl_Position = gl_in[0].gl_Position + vec4( size, size, 0.0, 0.0);
    TexCoord=vec2(1.0,1.0);
    EmitVertex();
        

    EndPrimitive();
    
}  