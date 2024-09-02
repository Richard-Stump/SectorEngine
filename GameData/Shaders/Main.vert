#version 330 core

layout(location = 0) in vec3 vPosition;
layout(location = 1) in vec3 vColor;

out vec3 fColor;

uniform mat4 matTrans;

void main()
{
    fColor =  vColor;
    
    // Here we swap the z and y components because our world has the Z axis set to be up.
    gl_Position = matTrans * vec4(-vPosition.x, vPosition.z, vPosition.y, 1.0);
}