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
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;
layout(location = 3) in vec4 aTexWeight;

layout(location = 0) out vec3 vPosition;
layout(location = 1) out vec3 vNormal;
layout(location = 2) out vec3 vViewPath;
layout(location = 3) out vec2 vTexCoord;

layout(location = 4) out float vDepth;
layout(location = 5) out vec4 vTexWeight;


out gl_PerVertex {
    vec4 gl_Position;
};

void main() {
	
	vec4 worldPosition = uModel.m * vec4(aPosition, 1.0);

	vPosition = worldPosition.xyz;
	mat4 m = inverse(transpose(uModel.m));
	vNormal = mat3(m) * aNormal; 
	vViewPath = uCamera.position.xyz - worldPosition.xyz;
	vTexCoord = aTexCoord;

	gl_Position = uCamera.vp * worldPosition;

	vDepth = gl_Position.z / gl_Position.w;
    float t = aTexWeight.x + aTexWeight.y + aTexWeight.z + aTexWeight.w;

    vTexWeight = aTexWeight / t;

}