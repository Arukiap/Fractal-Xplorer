#version 410 core

const int MAX_MARCHING_STEPS = 255;
const float MIN_DIST = 0.0;
const float MAX_DIST = 100.0;
const float EPSILON = 0.01;

in vec4 gl_FragCoord;

varying vec3 vPos;
varying vec2 vTexCoord;

float sphereSDF(vec3 samplePoint) {
    vec4 sphere = vec4(0.0,1.0,5.0,1.0);
    return length(samplePoint-sphere.xyz) - sphere.w;
}

float planeSDF(vec3 samplePoint) {
    return samplePoint.y;
}

float DE(vec3 z)
{
  z.xy = mod((z.xy),1.0)-vec2(0.5); // instance on xy-planex
  return length(z-vec3(0,0,-10.0))-0.3;             // sphere DE
}

float mandelbulbDistance(vec3 pos) {
	vec3 z = pos;
	float dr = 1.0;
	float r = 0.0;
	for (int i = 0; i < 8 ; i++) {
		r = length(z);
		if (r>100.0) break;
		
		// convert to polar coordinates
		float theta = acos(z.z/r);
		float phi = atan(z.y,z.x);
		dr =  pow( r, 4.0-1.0)*4.0*dr + 1.0;
		
		// scale and rotate the point
		float zr = pow( r,4.0);
		theta = theta*4.0;
		phi = phi*4.0;
		
		// convert back to cartesian coordinates
		z = zr*vec3(sin(theta)*cos(phi), sin(phi)*sin(theta), cos(theta));
		z+=pos;
	}
	return 0.5*log(r)*r/dr;
}

float fractalDistance(vec3 z)
{
	vec3 a1 = vec3(1,1,-1);
	vec3 a2 = vec3(-1,-1,-1);
	vec3 a3 = vec3(1,-1,1);
	vec3 a4 = vec3(-1,1,1);
	vec3 c;
	int n = 0;
	float dist, d;
	while (n < 10) {
		 c = a1; dist = length(z-a1);
	     d = length(z-a2); if (d < dist) { c = a2; dist=d; }
		 d = length(z-a3); if (d < dist) { c = a3; dist=d; }
		 d = length(z-a4); if (d < dist) { c = a4; dist=d; }
		z = 2.0*z-c*(2.0-1.0);
		n++;
	}

	return length(z) * pow(2.0, float(-n));
}

float sceneSDF(vec3 samplePoint) {
    return mandelbulbDistance(samplePoint);
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
    return normalize(vec3(xy,z));
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
    vec3 lightPosition = vec3(10.0,50.0,-50.0);
    vec3 light = normalize(lightPosition-samplePoint);
    vec3 normal = getNormal(samplePoint);

    float dif = clamp(dot(normal,light),0.0,1.0);

    float distanceToLightSource = trace(samplePoint+normal*EPSILON*2.0,light); //march a bit above the point else we get 0 distance from trace

    if(distanceToLightSource < length(lightPosition-samplePoint)) dif *= 0.5;

    return dif;
}

void main(){
    vec3 dir = rayDirection(45.0,vec2(1920,1080),gl_FragCoord.xy); // returns for each pixel the direction of the ray to march
    vec3 eye = vec3(0.0, 0.0, -6.0); // defines where the camera/eye is

    float marchedDistance = trace(eye,dir);
    //float color = marchedDistance/20.0;

    vec3 p = eye + dir * marchedDistance; //intersection point

    float diffuse = getLight(p);

    vec3 color = vec3(diffuse);

    gl_FragColor = vec4(color,0.0);
}