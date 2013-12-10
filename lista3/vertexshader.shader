#version 330 core

layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec3 theColor;

uniform vec2 offset;

out vec3 fragmentColor;

void main(){
	
	vec3 totalOffset = vec3(offset.x, offset.y, 0.0);
    gl_Position.xyz = vertexPosition_modelspace + totalOffset;
    gl_Position.w = 1.0;
	fragmentColor = theColor;

}

