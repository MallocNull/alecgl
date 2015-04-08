#version 330

in vec2 textex;
out vec4 frag;
uniform sampler2D depthMap;

void main() {
	//frag = vec4(0.0,0.0,0.0,1.0);
	frag = texture(depthMap, textex);
}