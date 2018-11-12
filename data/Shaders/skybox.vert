#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 0, std140) uniform Skybox {

	mat4 projection;
	mat4 view;

} uSkyBox;

layout(location = 0) in vec3 aPosition;

layout(location = 0) out vec3 vTexCoord;


out gl_PerVertex {
    vec4 gl_Position;
};

void main() {

	vTexCoord = aPosition;
	gl_Position = uSkyBox.projection * uSkyBox.view * vec4(aPosition, 1.0);

}