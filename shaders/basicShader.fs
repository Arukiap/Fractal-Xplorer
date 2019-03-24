#version 410 core

const int MAX_MARCHING_STEPS = 255;
const float MIN_DIST = 0.0;
const float MAX_DIST = 100.0;
const float EPSILON = 0.01;

in vec4 gl_FragCoord;

varying vec3 vPos;
varying vec2 vTexCoord;

float sphereSDF(vec3 samplePoint) {
    vec4 sphere = vec4(0.0,1.0,-10.0,1.0);
    return length(samplePoint-sphere.xyz) - sphere.w;
}

float planeSDF(vec3 samplePoint) {
    return samplePoint.y;
}

float spheresDE(vec3 z)
{
  z.xy = mod((z.xy),1.0)-vec2(0.5); // instance on xy-plane
  return length(z)-0.3;             // sphere DE
}

float sceneSDF(vec3 samplePoint) {
    return min(planeSDF(samplePoint),sphereSDF(samplePoint));
}

float trace(vec3 from, vec3 direction) {
	float totalDistance = 0.0;
	int steps;
	for (steps=0; steps < MAX_MARCHING_STEPS; steps++) {
		vec3 p = from + totalDistance * direction;
		float distance = sceneSDF(p);
		totalDistance += distance;
		if (distance > MAX_DIST || distance < EPSILON) break;
	}
	return totalDistance;
}

vec3 rayDirection(float fov, vec2 size, vec2 fragCoord){
    vec2 xy = fragCoord - size / 2.0;
    float z = size.y / tan(radians(fov)/2.0);
    return normalize(vec3(xy,-z));
}

vec3 getNormal(vec3 samplePoint){
    float distanceToPoint = sceneSDF(samplePoint);
    vec2 e = vec2(.01,0); //epsilon vector to facilitate calculating the normal

    vec3 n = distanceToPoint - vec3(
        sceneSDF(samplePoint-e.xyy),
        sceneSDF(samplePoint-e.yxy),
        sceneSDF(samplePoint-e.yyx)
    );

    return normalize(n);
}

float getLight(vec3 samplePoint){
    vec3 lightPosition = vec3(0,5.0,-8.0);
    vec3 light = normalize(lightPosition-samplePoint);
    vec3 normal = getNormal(samplePoint);

    float dif = dot(normal,light);
    return dif;
}

void main(){
    vec3 dir = rayDirection(45.0,vec2(1920,1080),gl_FragCoord.xy); // returns for each pixel the direction of the ray to march
    vec3 eye = vec3(0.0, 1.0, 0.0); // defines where the camera/eye is

    float marchedDistance = trace(eye,dir);
    //float color = marchedDistance/20.0;

    vec3 p = eye + dir * marchedDistance; //intersection point

    float diffuse = getLight(p);

    vec3 color = vec3(diffuse);

    gl_FragColor = vec4(color,0.0);
}