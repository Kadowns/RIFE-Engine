#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 1) uniform samplerCube uCubemap;

layout(location = 0) in vec3 vTexCoord;

layout(location = 0) out vec4 outColor;

void main() {

	outColor = texture(uCubemap, vTexCoord);

}