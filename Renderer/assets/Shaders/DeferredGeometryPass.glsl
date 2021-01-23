#source vertex
#version 460 core

layout(location = 0)in vec3 aPosition;
layout(location = 1)in vec3 aNormal;
layout(location = 2)in vec2 aTexCoords;
layout(location = 3)in vec3 aTangent;
layout(location = 4)in vec3 aBiTangent;

uniform mat4 uViewProjection;
uniform mat4 uTransform;

out VS_OUT
{
	vec3 vFragPos;
	vec3 vNormal;
	vec2 vTexCoords;
}vs_out;

void main()
{
	vs_out.vFragPos = (uTransform * vec4(aPosition, 1.0)).xyz;
	vs_out.vNormal = aNormal;
	vs_out.vTexCoords = aTexCoords;
	gl_Position = uViewProjection * uTransform * vec4(aPosition, 1.0);
}

#source fragment
#version 460 core

layout(location = 0)out vec3 gPosition;
layout(location = 1)out vec3 gNormal;
layout(location = 2)out vec4 gAlbedoS;

struct Material
{
	sampler2D Diffuse;
	sampler2D Normal;
	sampler2D Specular;
};

uniform Material gMaterial;

in VS_OUT
{
	vec3 vFragPos;
	vec3 vNormal;
	vec2 vTexCoords;
}fs_in;

void main()
{
	gPosition = fs_in.vFragPos;
	gNormal = texture(gMaterial.Normal, fs_in.vTexCoords).rgb;
	gAlbedoS.rgb = texture(gMaterial.Diffuse, fs_in.vTexCoords).rgb;
	gAlbedoS.a = texture(gMaterial.Specular, fs_in.vTexCoords).r;
}