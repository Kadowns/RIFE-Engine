#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 0) uniform Model {
	mat4 m;	
} uModel;

layout(binding = 1) uniform Camera { 
	mat4 vp;
	vec4 position;
} uCamera;

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec4 aColor;
layout(location = 2) in vec3 aNormal;

layout(location = 0) out vec4 vColor;
layout(location = 1) out vec3 vNormal;
layout(location = 2) out vec3 vViewPath;

out gl_PerVertex {
    vec4 gl_Position;
};

void main() {
	vec4 worldPos = uModel.m * vec4(aPosition, 1.0);

    gl_Position = uCamera.vp * worldPos;
	
    vColor = aColor;
	vNormal = (uModel.m * vec4(aNormal, 0.1)).xyz;
	vViewPath = uCamera.position.xyz - worldPos.xyz;
}