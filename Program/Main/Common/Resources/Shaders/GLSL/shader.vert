#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 0) uniform UniformBufferObject {
    mat4 vp;
	mat4 m;
	vec4 cameraPos;
} ubo;

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
	vec4 worldPos = ubo.m * vec4(aPosition, 1.0);

    gl_Position = ubo.vp * worldPos;
	
    vColor = aColor;
	vNormal = (ubo.m * vec4(aNormal, 0.1)).xyz;
	vViewPath = ubo.cameraPos.xyz - worldPos.xyz;
}