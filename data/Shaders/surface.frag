#version 450
#extension GL_ARB_separate_shader_objects : enable

struct DirectionalLight{
	vec4 direction;
	vec4 color;
	vec4 intensitys;
};

struct PointLight{
	vec4 position;
	vec4 color;
	vec4 intensitys;
};

struct SpotLight{
	vec4 position;
	vec4 direction;
	vec4 color;
	float cutoff;
	float intensity;
};

struct MaterialProperties {

	vec4 color;
	float specularPower;
	float tiling;
};

layout(binding = 2, std140) uniform Lights {

	  DirectionalLight directional;
	  PointLight point[16];
	  int pointLightCount;

} uLights;

layout(push_constant, std140) uniform Material {
	  MaterialProperties properties;
} uMaterial;

layout(binding = 3) uniform sampler2D uMainTex;
layout(binding = 4) uniform sampler2D uSpecularTex;
layout(binding = 5) uniform samplerCube uCubemap; 


layout(location = 0) in vec3 vPosition;
layout(location = 1) in vec3 vNormal;
layout(location = 2) in vec3 vViewPath;
layout(location = 3) in vec2 vTexCoord;

layout(location = 0) out vec4 outColor;

vec3 calculateDirectionalLight(in DirectionalLight light, in vec2 texCoord) {

	//ambient
	vec3 ambient = (light.color.xyz * texture(uMainTex, texCoord).xyz) * light.intensitys.x;
	//------

	//diffuse
	vec3 L = normalize(light.direction.xyz);
	vec3 N = normalize(vNormal);
    float diffuseIntensity = max(dot(N, -L), 0.0);
    vec3 diffuse = diffuseIntensity * light.color.xyz * texture(uMainTex, texCoord).xyz;
	//------

	//specular
	float specularIntensity = 0.0;
    if (uMaterial.properties.specularPower > 0.0) {
        vec3 V = normalize(vViewPath);
        vec3 R = reflect(L, N);
        specularIntensity = pow(max(dot(R, V), 0.0), uMaterial.properties.specularPower);
    }
    vec3 specular = specularIntensity * light.color.xyz * texture(uSpecularTex, texCoord).x;
	//--------
	vec3 color = (clamp(ambient + diffuse + specular, 0.0, 1.0) * uMaterial.properties.color.xyz) * light.intensitys.y;
	return color;
}

vec3 calculatePointLight(in PointLight light, in vec2 texCoord){

	//diffuse
	vec3 L = normalize(light.position.xyz - vPosition); 
	vec3 N = normalize(vNormal);
    float diffuseIntensity = max(dot(N, L), 0.0);
    vec3 diffuse = diffuseIntensity * light.color.xyz * texture(uMainTex, texCoord).xyz;

	//specular
	float specularIntensity = 0.0;
	if (uMaterial.properties.specularPower > 0.0) {
		vec3 V = normalize(vViewPath);
		vec3 R = reflect(-L, N);
		specularIntensity = pow(max(dot(R, V), 0.0), uMaterial.properties.specularPower);
	}
	vec3 specular = specularIntensity * light.color.xyz *  texture(uSpecularTex, texCoord).x;
	//---------------------
	float dist = length(light.position.xyz - vPosition);
	float attenuation = 1.0 / (light.intensitys.x + light.intensitys.y * dist + light.intensitys.z * (dist * dist));    

	diffuse  *= attenuation;
	specular *= attenuation;

	vec3 color = (clamp(diffuse + specular, 0.0, 1.0) * uMaterial.properties.color.xyz) * light.intensitys.w;
	return color;
}

void main() {

	vec2 texCoord = vTexCoord * uMaterial.properties.tiling;

	vec3 color = calculateDirectionalLight(uLights.directional, texCoord);

	for (int i = 0; i < uLights.pointLightCount; i++){
		color += calculatePointLight(uLights.point[i], texCoord);
	}

    outColor = vec4(color, 1.0);
}
//é, preciso usar vec4 pra essas merdas pq glsl É UMA MERDA