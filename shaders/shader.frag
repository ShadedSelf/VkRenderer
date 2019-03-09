#version 460
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) out vec4 outColor;
layout(location = 0) in vec2 texCoord;

layout (set = 0, binding = 3) uniform sampler2D image;

layout(set = 0, binding = 0) uniform UniformBufferObject
{
	float time;
} ubo;

void main()
{
	vec2 uv = texCoord; uv.y = 1.0 - uv.y;

	vec4 res = texture(image, uv, 0);
	outColor = res;

	// outColor = vec4(vec3(uv.x), 1.0);
	// outColor = vec4(vec3(asd.time), 1.0);
	
}
