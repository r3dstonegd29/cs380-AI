#version 440
in vec3 attr_pos;
in vec2 attr_uvs;
in vec3 attr_norm;

uniform mat4 Model;
uniform mat4 VP;
uniform bool doClip = false;
uniform vec4 clip_normal;

out vec2 uv;
out vec3 normal;
out float c_intensity;
void main()
{
	vec4 wPos = Model * vec4(attr_pos, 1.0);
	if(doClip)
		gl_ClipDistance[0] = dot(clip_normal,wPos);

	uv = attr_uvs;
	normal = attr_norm;
	gl_Position = VP * wPos;
	c_intensity = attr_pos.y;
}