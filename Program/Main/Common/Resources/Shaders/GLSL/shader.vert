#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 0) uniform UniformBufferObject {
    mat4 mvp;
} ubo;

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec4 aColor;
layout(location = 2) in vec3 aNormal;

layout(location = 0) out vec4 vColor;
layout(location = 1) out vec3 vNormal;

out gl_PerVertex {
    vec4 gl_Position;
};

void main() {
    gl_Position = ubo.mvp * vec4(aPosition, 1.0);
    vColor = aColor;
	vNormal = aNormal;
}