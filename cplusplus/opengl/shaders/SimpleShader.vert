#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vert;

void main(){
    gl_Position.xyz = vec4(vert.x / 400.0 - 1.0, vert.y / 400.0 - 1.0, 0.0, 1.0);
    gl_Position.w = 1.0;
}
