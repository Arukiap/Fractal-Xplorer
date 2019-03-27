#version 410 core

//Ray Marching constants
const int MAX_MARCHING_STEPS = 255;
const float MIN_DIST = 0.0;
const float MAX_DIST = 100.0;
const float EPSILON = 0.01;
const float FOV = 80.0;

//Fractal constants
float POWER = 8.0;
const float BAILOUT = 50.0;
const int ITERATIONS = 10;

//Shader constants
const float shadowIntensity = 0.5; // From 0.0 to 1.0 how strong you want the shadows to be
const float shadowDiffuse = 1.0 - shadowIntensity;
float minDistanceX = MAX_DIST; //used for orbit trap coloring
float minDistanceY = MAX_DIST; //used for orbit trap coloring
float minDistanceZ = MAX_DIST; //used for orbit trap coloring

in vec4 gl_FragCoord;

varying vec3 vPos;
varying vec2 vTexCoord;
varying float vSystemTime;
varying vec2 vSystemResolution;

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
    return samplePoint.y;
}

/*
 * Signed distance function for the estimation of the mandelbulb set
 */
float mandelbulbSDF(vec3 pos) {
	vec3 z = pos;
	float dr = 1.0;
	float r = 0.0;
	for (int i = 0; i < ITERATIONS ; i++) {
		r = length(z);

		float distanceToPlaneX = abs(pos.x);
		float distanceToPlaneY = abs(pos.y);
		float distanceToPlaneZ = abs(pos.z);

		if(distanceToPlaneX < minDistanceX){
			minDistanceX = distanceToPlaneX;
		}

		if(distanceToPlaneY < minDistanceY){
			minDistanceY = distanceToPlaneY;
		}

		if(distanceToPlaneZ < minDistanceZ){
			minDistanceZ = distanceToPlaneZ;
		}

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
	}
	return 0.5*log(r)*r/dr;
}

/*
 * Signed distance function for the estimation of the 3D Sierpinski Tetrahedron fractal
 */
float SierpinskiSDF(vec3 z)
{
	vec3 a1 = vec3(1,1,-1);
	vec3 a2 = vec3(-1,-1,-1);
	vec3 a3 = vec3(1,-1,1);
	vec3 a4 = vec3(-1,1,1);
	vec3 c;
	int n = 0;
	float dist, d;
	while (n < ITERATIONS) {
		 c = a1; dist = length(z-a1);
	     d = length(z-a2); if (d < dist) { c = a2; dist=d; }
		 d = length(z-a3); if (d < dist) { c = a3; dist=d; }
		 d = length(z-a4); if (d < dist) { c = a4; dist=d; }
		z = 2.0*z-c*(2.0-1.0);
		n++;
	}

	return length(z) * pow(2.0, float(-n));
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
 * Represents the current scene as a conjunction of all SDFunctions we want to represent.
 */
float sceneSDF(vec3 samplePoint) {
	float rotationAngle = 90.0;//vSystemTime*0.0005;
	vec3 fractalPoint = ((rotateYaxis(rotationAngle)* vec4(samplePoint,1.0))).xyz;
    return mandelbulbSDF(fractalPoint);
}

/*
 * Ray marching algorithm.
 * Returns aprox. distance to the scene from a certain point with a certain direction.
 */
float rayMarch(vec3 from, vec3 direction) {
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

/*
 * Returns the amount of diffuse for a certain pixel.
 */
float getLight(vec3 samplePoint){
    vec3 lightPosition = vec3(0.2,0.2,-6.0);
    vec3 light = normalize(lightPosition-samplePoint);
    vec3 normal = getNormal(samplePoint);

    float dif = clamp(dot(normal,light),0.0,1.0);

	// march a bit above the point else we get 0 distance from rayMarch
    float distanceToLightSource = rayMarch(samplePoint+normal*EPSILON*2.0,light); 

	// if distance to light source is less then the actual distance, this means we have an object in between and need to apply shadow on it
    if(distanceToLightSource < length(lightPosition-samplePoint)) dif *= shadowDiffuse;

    return dif;
}

void main(){

	POWER += vSystemTime*-0.0005;

	// returns for each pixel the direction of the ray to march
    vec3 dir = rayDirection(FOV,vSystemResolution,gl_FragCoord.xy); 

	// defines where the camera/eye is in space
    vec3 eye = vec3(0.0, 0.0,-4.0); 

    float marchedDistance = rayMarch(eye,dir);

	if(marchedDistance >= MAX_DIST){
		gl_FragColor = vec4(0.97,0.90,0.5,0.0);
	} else {
		// get intersection point in scene and retrieve the diffuse we need to apply
		vec3 p = eye + dir * marchedDistance; 
		float diffuse = getLight(p);

		vec3 colors = vec3(minDistanceZ,minDistanceY,minDistanceX);

		// shade our pixel accordingly
		vec3 diffuseVec = vec3(diffuse);

		gl_FragColor = vec4(diffuseVec*colors,0.0);
	}
}