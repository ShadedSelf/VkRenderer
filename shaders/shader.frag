#version 460
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) out vec4 outColor;
layout(location = 0) in vec2 texCoord;

layout (set = 1, binding = 0) uniform sampler2D image;

layout(set = 0, binding = 0) uniform UniformBufferObject
{
	float time;
} ubo;

void main()
{
	vec2 uv = texCoord; uv.y = 1.0 - uv.y;
	// // outColor = mix(
	// // 			texture(test, uv, 0),
	// // 			texture(image, uv, 0),
	// // 			pow(clamp(float(ubo.uboframe) / 100.0, 0.0, 1.0), 2.0));

	// vec4 res = texture(image, uv, 0);
	// outColor = res;

	// outColor = vec4(vec3(uv.x), 1.0);
	outColor = vec4(vec3(ubo.time), 1.0);
	
}
