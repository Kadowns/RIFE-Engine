#version 450
#extension GL_ARB_separate_shader_objects : enable
#extension GL_KHR_vulkan_glsl : enable

layout( binding = 0 ) uniform UniformBufferObject {
	mat4x4 mvp;
	
}Transform;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec4 inColor;

layout(location = 0) out vec4 fragColor;

out gl_PerVertex {
    vec4 gl_Position;
};

void main() {

    gl_Position = Transform.mvp * vec4(inPosition, 1.0);
    fragColor = inColor;
}