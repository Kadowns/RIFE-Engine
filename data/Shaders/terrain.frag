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
	float reflectionPower;
};

layout(binding = 2, std140) uniform Lights {

	  DirectionalLight directional;
	  PointLight point[16];
	  int pointLightCount;

} uLights;

layout(push_constant, std140) uniform Material {
	  MaterialProperties properties;
} uMaterial;

layout(binding = 3) uniform sampler2D uTex0;
layout(binding = 4) uniform sampler2D uTex1;
layout(binding = 5) uniform sampler2D uTex2;
layout(binding = 6) uniform sampler2D uTex3;


layout(location = 0) in vec3 vPosition;
layout(location = 1) in vec3 vNormal;
layout(location = 2) in vec3 vViewPath;
layout(location = 3) in vec2 vTexCoord;
layout(location = 4) in float vDepth;
layout(location = 5) in vec4 vTexWeight;

layout(location = 0) out vec4 outColor;

vec3 calculateDirectionalLight(in DirectionalLight light, in float texSpecular) {

	//ambient
	vec3 ambient = light.intensitys.x * light.color.rgb;

	//diffuse
	vec3 L = normalize(light.direction.xyz);
	vec3 N = normalize(vNormal);
    float diffuseIntensity = max(dot(N, -L), 0.0);
    vec3 diffuse = diffuseIntensity * light.color.xyz;
	//------

	//specular
	float specularIntensity = 0.0;
    if (uMaterial.properties.specularPower > 0.0 || texSpecular == 0) {
        vec3 V = normalize(vViewPath);
        vec3 R = reflect(L, N);
        specularIntensity = pow(max(dot(R, V), 0.0), uMaterial.properties.specularPower);
    }
    vec3 specular = specularIntensity * light.color.xyz * texSpecular;
	//--------
	return clamp(ambient + diffuse + specular, 0.0, 1.0) * light.intensitys.y;
}

vec3 calculatePointLight(in PointLight light, in float texSpecular){

	//diffuse
	vec3 L = normalize(light.position.xyz - vPosition); 
	vec3 N = normalize(vNormal);
    float diffuseIntensity = max(dot(N, L), 0.0);
    vec3 diffuse = diffuseIntensity * light.color.xyz;

	//specular
	float specularIntensity = 0.0;
	if (uMaterial.properties.specularPower > 0.0 || texSpecular == 0) {
		vec3 V = normalize(vViewPath);
		vec3 R = reflect(-L, N);
		specularIntensity = pow(max(dot(R, V), 0.0), uMaterial.properties.specularPower);
	}
	vec3 specular = specularIntensity * light.color.xyz *  texSpecular;
	//---------------------
	float dist = length(light.position.xyz - vPosition);
	float attenuation = 1.0 / (light.intensitys.x + light.intensitys.y * dist + light.intensitys.z * (dist * dist));    

	diffuse  *= attenuation;
	specular *= attenuation;

	return clamp(diffuse + specular, 0.0, 1.0) * light.intensitys.w;
}

void main() {

	float texSpecular = 0;

	vec3 illumination = calculateDirectionalLight(uLights.directional, texSpecular);

	for (int i = 0; i < uLights.pointLightCount; i++){

		illumination += calculatePointLight(uLights.point[i], texSpecular);
	}

	float blendFactor = clamp((vDepth - 0.99) * 100.0, 0.0, 1.0);

    vec2 farCoord = vTexCoord * uMaterial.properties.tiling * 0.2;
    vec4 texelFar = texture(uTex0, farCoord) * vTexWeight.x +
                    texture(uTex1, farCoord) * vTexWeight.y +
                    texture(uTex2, farCoord) * vTexWeight.z +
                    texture(uTex3, farCoord) * vTexWeight.w;

    vec2 nearCoord = vTexCoord * uMaterial.properties.tiling;
    vec4 texelNear = texture(uTex0, nearCoord) * vTexWeight.x +
                     texture(uTex1, nearCoord) * vTexWeight.y +
                     texture(uTex2, nearCoord) * vTexWeight.z +
                     texture(uTex3, nearCoord) * vTexWeight.w;

    vec3 texel = mix(texelNear.rgb, texelFar.rgb, blendFactor) * uMaterial.properties.color.rgb;

    outColor = vec4(clamp(texel * illumination, 0.0, 1.0), 1.0);
}
//é, preciso usar vec4 pra essas merdas pq glsl É UMA MERDA