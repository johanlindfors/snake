#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 position;
layout(location = 1) in vec4 color;
uniform mat4 MVP;

//Out values
out vec4 frag_color;

void main(){
    gl_Position = MVP * vec4(position, 1.0);
    frag_color = color;
}
