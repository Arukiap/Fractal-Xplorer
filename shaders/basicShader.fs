#version 410 core

const int MAX_MARCHING_STEPS = 255;
const float MIN_DIST = 0.0;
const float MAX_DIST = 100.0;
const float EPSILON = 0.0001;

in vec4 gl_FragCoord;

varying vec3 vPos;
varying vec2 vTexCoord;

float sphereSDF(vec3 samplePoint) {
    return length(samplePoint) - 1.0;
}

float sceneSDF(vec3 samplePoint) {
    return sphereSDF(samplePoint);
}

float shortestDistanceToSurface(vec3 eye, vec3 marchingDirection, float start, float end) {
    float depth = start;
    for (int i = 0; i < MAX_MARCHING_STEPS; i++) {
        float dist = sceneSDF(eye + depth * marchingDirection);
        if (dist < EPSILON) {
			return depth;
        }
        depth += dist;
        if (depth >= end) {
            return end;
        }
    }
    return end;
}

vec3 rayDirection(float fov, vec2 size, vec2 fragCoord){
    vec2 xy = fragCoord - size / 2.0;
    float z = size.y / tan(radians(fov)/2.0);
    return normalize(vec3(xy,-z));
}

void main(){
    vec3 dir = rayDirection(45.0.0,vec2(800,800),gl_FragCoord.xy); // returns for each pixel the direction of the ray to march
    vec3 eye = vec3(0.0, 0.0, 5.0); // defines where the camera/eye is
    float dist = shortestDistanceToSurface(eye, dir, MIN_DIST, MAX_DIST);

    if (dist > MAX_DIST - EPSILON) {
        // Didn't hit anything
        gl_FragColor = vec4(0.0, 0.0, 0.0, 0.0);
		return;
    }
    
    gl_FragColor = vec4(0.0, 0.6, 0.2, 1.0);
}