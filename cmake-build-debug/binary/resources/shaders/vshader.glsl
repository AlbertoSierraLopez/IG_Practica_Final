#version 330 core

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec3 inNormal;

uniform mat4     uPVM;
uniform mat4     uN;
uniform mat4     uM;

out vec3 vpos;
out vec3 vnor;

void main() {

    // Pasar a homogéneas
    vec4 inPositionH = vec4(inPosition,1);
    vec4 inNormalH   = vec4(inNormal  ,0);

    // Proyectar los vértices
    gl_Position = uPVM * inPositionH;

    // Pasar un par de cosas de los vértices que necesita el shader de fragmentos al shader
    vpos = (uM*inPositionH).xyz;
    vnor = (uN*inNormalH).xyz;
	
}
