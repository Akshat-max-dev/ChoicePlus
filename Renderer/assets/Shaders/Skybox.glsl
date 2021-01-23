#source vertex
#version 460 core

layout(location = 0)in vec3 aPosition;

out vec3 vTexCoords;

uniform mat4 uViewProjection;

void main()
{
	vTexCoords = aPosition;
	gl_Position = uViewProjection * vec4(aPosition, 1.0);
}

#source fragment
#version 460 core

out vec4 fSkybox;

in vec3 vTexCoords;

uniform samplerCube Skybox;

void main()
{
	fSkybox = texture(Skybox, vTexCoords);
}