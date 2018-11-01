#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 2, std140) uniform Light {

	  vec4 direction;
	  vec4 ambient;
	  vec4 diffuse;
	  vec4 specular;

} uLight;

layout(push_constant, std140) uniform Material {

	  vec4 color;
	  float specularPower;

} uMaterial;


layout(location = 0) in vec3 vNormal;
layout(location = 1) in vec3 vViewPath;

layout(location = 0) out vec4 outColor;

void main() {
	//ambient
	vec3 ambient = uLight.ambient.xyz * uMaterial.color.xyz;
	//--------------

    //diffuse
    vec3 L = normalize(uLight.direction.xyz);
	vec3 N = normalize(vNormal);
    float diffuseIntensity = max(dot(N, -L), 0.0);
    vec3 diffuse = diffuseIntensity * uLight.diffuse.xyz * uMaterial.color.xyz;
	//---------------

    //specular
    float specularIntensity = 0.0;
    if (uMaterial.specularPower > 0.0) {
        vec3 V = normalize(vViewPath);
        vec3 R = reflect(L, N);
        specularIntensity = pow(max(dot(R, V), 0.0), uMaterial.specularPower);
    }
    vec3 specular = specularIntensity * uLight.specular.xyz * uMaterial.color.xyz;
	//---------------------------------

    
    vec3 color = clamp(ambient + diffuse + specular, 0.0, 1.0);
    outColor = vec4(color, 1.0);
}
