#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 0) uniform UniformBufferObject {
    mat4 projection;
    mat4 view;
    mat4 model;
} ubo;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec4 inColor;

layout(location = 0) out vec4 fragColor;

out gl_PerVertex {
    vec4 gl_Position;
};

void main() {

    gl_Position = ubo.projection * ubo.view * ubo.model * vec4(inPosition, 1.0);
    fragColor = inColor;
}