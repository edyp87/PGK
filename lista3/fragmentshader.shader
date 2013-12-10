#version 330 core

// Ouput data
in vec3 fragmentColor;
out vec3 color;
uniform vec3 ucolor;

void main()
{

    color = fragmentColor + ucolor;

}