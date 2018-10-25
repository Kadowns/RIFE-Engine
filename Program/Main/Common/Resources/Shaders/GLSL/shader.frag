#version 450
#extension GL_ARB_separate_shader_objects : enable
#extension GL_KHR_vulkan_glsl : enable

layout(binding = 1) uniform Light {
	 vec3 direction;
} uLight;


layout(location = 0) in vec4 vColor;

layout(location = 1) in  vec3 vNormal;

layout(location = 0) out vec4 outColor;

void main() {
	//vec3 ambient = uLight.ambient;
	//vec3 L = normalize(uLight.direction);
	//vec3 N = normalize(vNormal);
	//float diffuseIntensity = max(dot(N, -L), 0.0);
	//vec3 diffuse = diffuseIntensity * uLight.diffuse;

	//vec3 color = clamp(ambient + diffuse, 0.0, 1.0);

    outColor = clamp(vColor + vec4(uLight.direction, 1.0), 0.0, 1.0);
}