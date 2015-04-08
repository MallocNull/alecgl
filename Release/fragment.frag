#version 330
#define M_PI 3.1415926535897932384626433832795

in vec2 ftextex;
in vec3 vnorm;
in vec3 vpos;

out vec4 cout;

uniform sampler2D texcol;

uniform bool enableFlashlight;

uniform mat4 modelToCam;
uniform vec3 lightPos;
uniform vec4 lightIntensity;
uniform vec4 ambIntensity;

uniform vec3 lookPos;
uniform vec3 lookVector;

uniform vec3 flashLightPos;
uniform vec3 flashLightDir;
uniform vec4 flashLightIntensity;

vec4 getLightIntensity(in float dist) {
	return lightIntensity * (1/(1 + .1 * dist));
}

float deg2Rad(float deg) {
	return (deg*M_PI)/180;
}

void main(void) {
	vec3 vcpos = vec3(modelToCam * vec4(vpos, 1.0));
	
	vec3 fragToLight = lightPos - vcpos;
	float uccostheta = dot(vnorm, normalize(fragToLight));
	float costheta = clamp(uccostheta, 0, 1);

	vec3 fragToFlashlight = vcpos - flashLightPos;
	float component = 1-clamp(abs(acos(dot(normalize(fragToFlashlight),normalize(flashLightDir))))/(deg2Rad(10)),0,1);
	component = pow(component, 4.0/3.0);
	int check = (dot(vnorm,-normalize(fragToFlashlight))>=0)?1:0;
	float distFactor = 1/(1+.001*dot(fragToFlashlight,fragToFlashlight));

	vec4 tcol = texture(texcol, ftextex);
	vec4 coutt = tcol*getLightIntensity(dot(fragToLight,fragToLight))*costheta 
		 + ((enableFlashlight) ? tcol*flashLightIntensity*component*check*distFactor : vec4(0.0))
		 + tcol*ambIntensity;
	//cout = vec4(coutt.rgb, tcol.a);
	cout = vec4(coutt.r, coutt.g, coutt.b, tcol.a);
	//cout = vec4(ftextex.x/8,ftextex.x/8,ftextex.x/8,1.0);
}