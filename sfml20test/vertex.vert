#version 330

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 textex;

out vec2 ftextex;
out vec3 vnorm;
out vec3 vpos;

uniform float time;

uniform vec2 texScaleFactor;
uniform vec2 texOffsetFactor;

uniform mat4 modelToCam;
uniform mat4 camToWorld;
uniform mat4 worldToClip;

void main(void) {
	gl_Position = (worldToClip * camToWorld * modelToCam) * vec4(position, 1.0);
	
	ftextex = texOffsetFactor + textex*texScaleFactor;
	vnorm = normalize(vec3(modelToCam * vec4(normal, 0.0)));
	vpos = position;
}