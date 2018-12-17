#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vert;
uniform mat4 MVP;

void main(){
    gl_Position = MVP * vec4(vert.x / 200.0 - 1.0, (200 - vert.y) / 200.0 , 0.0, 1.0);
    gl_Position.w = 1.0;
}
