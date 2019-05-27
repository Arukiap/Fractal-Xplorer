#version 410 core

//Ray Marching constants
const int MAX_MARCHING_STEPS = 128;
const float MIN_DIST = 0.0;
const float MAX_DIST = 100.0;
const float EPSILON = 0.0005;
const float FOV = 120.0;

//Fractal constants
float POWER = 11;
const float BAILOUT = 50.0;
const int ITERATIONS = 10;
const int SIERPISNKI_ITERATIONS = 20;
const int COLORITERATIONS = 5;

//Shader constants
const float shadowIntensity = 1.5; // From 0.0 to 1.0 how strong you want the shadows to be
const float shadowDiffuse = 1.0 - shadowIntensity;
const float diffuseStrength = 1.5; // The higher the value the more bright the object gets if using normal lighting
const float orbitStrength = 0.80; // The higher the value the more bright the object gets
vec4 orbitTrap = vec4(MAX_DIST); // Orbit trapping in order to shade or color fractals
int currentSteps;


vec2 mouseDelta;

in vec4 gl_FragCoord;

varying float vSelectedFractal;
varying float vSystemTime;
varying vec2 vSystemResolution;
varying vec3 vCamera_pos;
varying vec2 vMouse_delta;

/*
 * Sphere distance estimator function where sphere.w represents the radius of the sphere
 * Used for debug and testing
 */
float sphereSDF(vec3 samplePoint) {
    vec4 sphere = vec4(0.0,1.0,5.0,1.0);
    return length(samplePoint-sphere.xyz) - sphere.w;
}

/*
 * Simple xy plane distance estimator function
 * Used for debug and testing
 */
float planeSDF(vec3 samplePoint) {
    return 0.2-samplePoint.z;
}

/*
 * Signed distance function for the estimation of the mandelbulb set
 */
float mandelbulbSDF(vec3 pos, bool isLight) {
	if(!isLight) orbitTrap = vec4(MAX_DIST);
	vec3 z = pos;
	float dr = 1.0;
	float r = 0.0;
	for (int i = 0; i < ITERATIONS ; i++) {
		r = length(z);

		if (r>BAILOUT) break;

		// convert to polar coordinates
		float theta = acos(z.z/r);
		float phi = atan(z.y,z.x);
		dr =  pow( r, POWER-1.0 )*POWER*dr + 1.0;
		
		// scale and rotate the point
		float zr = pow( r,POWER);
		theta = theta*POWER;
		phi = phi*POWER;
		
		// convert back to cartesian coordinates
		z = zr*vec3(sin(theta)*cos(phi), sin(phi)*sin(theta), cos(theta));

		z+=pos;

		if (i<COLORITERATIONS && !isLight) orbitTrap = min(orbitTrap,abs(vec4(z.x,z.y,z.z,r*r)));
	}
	return 0.5*log(r)*r/dr;
}

/*
 * Signed distance function for the estimation of the 3D Sierpinski Tetrahedron fractal
 */
float sierpinskiSDF(vec3 z, bool isLight)
{
	if(!isLight) orbitTrap = vec4(MAX_DIST);
	vec3 a1 = vec3(0.5,0.5,-0.5);
	vec3 a2 = vec3(-0.5,-0.5,-0.5);
	vec3 a3 = vec3(0.5,-0.5,0.5);
	vec3 a4 = vec3(-0.5,0.5,0.5);
	vec3 c;
	int n = 0;
	float dist, d;
	while (n < SIERPISNKI_ITERATIONS) {
		 c = a1; dist = length(z-a1);
	     d = length(z-a2); if (d < dist) { c = a2; dist=d; }
		 d = length(z-a3); if (d < dist) { c = a3; dist=d; }
		 d = length(z-a4); if (d < dist) { c = a4; dist=d; }
		z = 2.0*z-c*(2.0-1.0);
		float r = dot(z,z);
		if (n<COLORITERATIONS && !isLight) orbitTrap = min(orbitTrap,abs(vec4(z.x,z.y,z.z,r)));
		n++;
	}

	return length(z) * pow(2.0, float(-n));
}

/*
 * Signed distance function for the estimation of Julia quaternion set
 */
float juliaSDF(vec3 pos, bool isLight) {
	if(!isLight) orbitTrap = vec4(MAX_DIST);
	vec4 p = vec4(pos, 0.0);
	vec4 dp = vec4(1.0,0.0,0.0,0.0);
	for (int i = 0; i < ITERATIONS; i++) {
		dp = 2.0* vec4(p.x*dp.x-dot(p.yzw, dp.yzw), p.x*dp.yzw+dp.x*p.yzw+cross(p.yzw, dp.yzw));
		p = vec4(p.x*p.x-dot(p.yzw, p.yzw), vec3(2.0*p.x*p.yzw))+0.30;
		float p2 = dot(p,p);
		if (i<COLORITERATIONS && !isLight) orbitTrap = min(orbitTrap, abs(vec4(p.xyz,p2)));
		if (p2 > BAILOUT) break;
	}
	float r = length(p);
	return  0.5 * r * log(r) / length(dp);
}

float mandelboxSDF(vec3 pos, bool isLight) {
  if(!isLight) orbitTrap = vec4(MAX_DIST);
  float SCALE = 2.8;
  float MR2 = 0.2;

  vec4 scalevec = vec4(SCALE, SCALE, SCALE, abs(SCALE)) / MR2;
  float C1 = abs(SCALE-1.0), C2 = pow(abs(SCALE), float(1-ITERATIONS));

  // distance estimate
  vec4 p = vec4(pos.xyz, 1.0), p0 = vec4(pos.xyz, 1.0);  // p.w is knighty's DEfactor
  
  for (int i=0; i<ITERATIONS; i++) {
    p.xyz = clamp(p.xyz, -1.0, 1.0) * 2.0 - p.xyz;  // box fold: min3, max3, mad3
    float r2 = dot(p.xyz, p.xyz);  // dp3
	if (i<COLORITERATIONS && !isLight) orbitTrap = min(orbitTrap, abs(vec4(p.xyz,r2)));
    p.xyzw *= clamp(max(MR2/r2, MR2), 0.0, 1.0);  // sphere fold: div1, max1.sat, mul4
    p.xyzw = p*scalevec + p0;  // mad4
  }
  return ((length(p.xyz) - C1) / p.w) - C2;
}


/*
 * Returns a rotation matrix for a rotation of theta degrees in the z axis.
 */
mat4 rotateZaxis(float theta) {
    float c = cos(theta);
    float s = sin(theta);

    return mat4(
        vec4(c, -s, 0, 0),
        vec4(s, c, 0, 0),
        vec4(0, 0, 1, 0),
        vec4(0, 0, 0, 1)
    );
}

/*
 * Returns a rotation matrix for a rotation of theta degrees in the y axis.
 */
mat4 rotateYaxis(float theta) {
    float c = cos(theta);
    float s = sin(theta);

    return mat4(
        vec4(c, 0, s, 0),
        vec4(0, 1, 0, 0),
        vec4(-s, 0, c, 0),
        vec4(0, 0, 0, 1)
    );
}

/*
 * Returns a rotation matrix for a rotation of theta degrees in the x axis.
 */
mat4 rotateXaxis(float theta) {
    float c = cos(theta);
    float s = sin(theta);

    return mat4(
        vec4(1, 0, 0, 0),
        vec4(0, c, -s, 0),
        vec4(0,s, c, 0),
        vec4(0, 0, 0, 1)
    );
}

/*
 * Represents the current scene as a conjunction of all SDFunctions we want to represent.
 */
float sceneSDF(vec3 samplePoint, bool isLight) {

	vec3 fractalPoint = ((rotateXaxis(-vMouse_delta.y*0.005)*
							rotateYaxis(-vMouse_delta.x*0.005)*
							rotateYaxis(0)*
							vec4(samplePoint,1.0))).xyz;

	if(abs(vSelectedFractal - 1.0) < 0.1){
		return mandelbulbSDF(fractalPoint,isLight);
	}
	if(abs(vSelectedFractal - 2.0) < 0.1){
		return sierpinskiSDF(fractalPoint,isLight);
	}
	if(abs(vSelectedFractal - 3.0) < 0.1){
		return juliaSDF(fractalPoint,isLight);
	}

	return mandelboxSDF(fractalPoint,isLight);
}

/*
 * Ray marching algorithm.
 * Returns aprox. distance to the scene from a certain point with a certain direction.
 */
float rayMarch(vec3 from, vec3 direction, bool isLight) {
	float totalDistance = 0.0;
	int steps;
	for (steps=0; steps < MAX_MARCHING_STEPS; steps++) {
		vec3 p = from + totalDistance * direction;
		float distance = sceneSDF(p,isLight);
		totalDistance += distance;
		if (distance > MAX_DIST || distance < EPSILON) break;
	}
	currentSteps = steps;
	return totalDistance;
}

/*
 * Helper function to find the direction of the ray to march to.
 */
vec3 rayDirection(float fov, vec2 size, vec2 fragCoord){
    vec2 xy = fragCoord - size / 2.0;
    float z = size.y / tan(radians(fov)/2.0);
    return normalize(vec3(xy,z));
}

/*
 * Returns an aprox. normal vector to the given point in space.
 * Useful for lighting.
 */
vec3 getNormal(vec3 samplePoint, bool isLight){
    float distanceToPoint = sceneSDF(samplePoint,isLight);
    vec2 e = vec2(.01,0); //epsilon vector to facilitate calculating the normal

    vec3 n = distanceToPoint - vec3(
        sceneSDF(samplePoint-e.xyy,isLight),
        sceneSDF(samplePoint-e.yxy,isLight),
        sceneSDF(samplePoint-e.yyx,isLight)
    );

    return normalize(n);
}

/*
 * Returns the amount of diffuse for a certain pixel.
 * Currently not being used to light fractals, instead we simply use orbital trap.
 */
float getLight(vec3 samplePoint){
    vec3 lightPosition = vec3(10.0,10.0,-10.0);
    vec3 light = normalize(lightPosition-samplePoint);
    vec3 normal = getNormal(samplePoint,true);

    float dif = clamp(dot(normal,light)*diffuseStrength,0.0,1.0);

	// march a bit above the point else we get 0 distance from rayMarch
    float distanceToLightSource = rayMarch(samplePoint+normal*EPSILON*2.0,light,true); 

	// if distance to light source is less then the actual distance, this means we have an object in between and need to apply shadow on it
    if(distanceToLightSource < length(lightPosition-samplePoint)) dif *= shadowDiffuse;

    return dif;
}

void main(){	

	// returns for each pixel the direction of the ray to march
    vec3 dir = rayDirection(FOV,vSystemResolution,gl_FragCoord.xy); 

	// defines where the camera/eye is in space
	vec3 eye = vCamera_pos;    
	//vec3 eye = vec3(0.0,0.0,-4.0); 
    float marchedDistance = rayMarch(eye,dir,false);

	if(marchedDistance >= MAX_DIST){
		float glow = currentSteps/3;
		gl_FragColor = mix(vec4(0.0,0.0,0.0,0.0),vec4(1.0,1.0,1.0,1.0),glow*0.05);
		//gl_FragColor = vec4(0.612,0.816,1.0,0.0);
	} else {
		
		// get intersection point in scene and retrieve the diffuse we need to apply
		vec3 p = eye + dir * marchedDistance; 
		float diffuse = getLight(p);

		vec4 baseColor = vec4(1.0,orbitTrap.z,orbitTrap.x,1.0)*orbitTrap.w*0.6+diffuse*0.6+0.2;//vec4(orbitTrap.xzy,1.0)*orbitTrap.w;//+diffuse*0.3;
		gl_FragColor = mix(baseColor,vec4(0.0,0.0,0.0,0.0),clamp(marchedDistance*0.25,0.0,1.0));

	}
}