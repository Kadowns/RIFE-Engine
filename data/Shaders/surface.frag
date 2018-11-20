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

layout(binding = 3) uniform sampler2D uMainTex;
layout(binding = 4) uniform sampler2D uSpecularTex;
layout(binding = 5) uniform samplerCube uSkybox;


layout(location = 0) in vec3 vPosition;
layout(location = 1) in vec3 vNormal;
layout(location = 2) in vec3 vViewPath;
layout(location = 3) in vec2 vTexCoord;

layout(location = 0) out vec4 outColor;

void calculateDirectionalLight(in DirectionalLight light, in float texSpecular, out vec3 diff, out vec3 spec) {

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

	diff = clamp(ambient + diffuse, 0.0, 1.0) * light.intensitys.y;
	spec = clamp(specular, 0.0, 1.0) * light.intensitys.y;
}

void calculatePointLight(in PointLight light, in float texSpecular, out vec3 diff, out vec3 spec){

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
	diff += clamp(diffuse, 0.0, 1.0) * light.intensitys.w;
	spec += clamp(specular, 0.0, 1.0) * light.intensitys.w;
}

void main() {

	vec3 texColor = texture(uMainTex, vTexCoord * uMaterial.properties.tiling).xyz;
	vec3 temp = texture(uSpecularTex, vTexCoord * uMaterial.properties.tiling).xyz;
	float texSpecular = (temp.r + temp.g + temp.b) / 3;

	vec3 diffuse = vec3(0.0, 0.0, 0.0);
	vec3 specular = vec3(0.0, 0.0, 0.0);

	calculateDirectionalLight(uLights.directional, texSpecular, diffuse, specular);

	for (int i = 0; i < uLights.pointLightCount; i++){

		calculatePointLight(uLights.point[i], texSpecular, diffuse, specular);
	}
	
	float brightness = (diffuse.r + diffuse.g + diffuse.b) / 3;
	vec3 R = reflect(-vViewPath, normalize(vNormal));	
	vec3 sky = texture(uSkybox, R).rgb * clamp(brightness, 0.0, 1.0);
	diffuse = mix(diffuse, sky, uMaterial.properties.reflectionPower);


	vec3 texel = texColor * uMaterial.properties.color.rgb;

    outColor = vec4(clamp(texel * diffuse + specular, 0.0, 1.0), 1.0);
}