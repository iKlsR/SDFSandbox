#version 330 core
out vec4 col;
uniform vec2 iResolution;
uniform float u_time;
in vec2 TexCoord;


#define MAX_STEPS 100
#define MAX_DIST 100.
#define SURF_DIST .01

float sphereSDF(vec3 p, vec3 pos, float radius) {
    return length(p - pos) - radius;
}

float planeSDF(vec3 p) {
    return p.y;
}

%1

//float GetDist(vec3 p) {
//    float sphereDist = sphereSDF(p, vec3(0, 1, 6), 1);
//    float planeDist = planeSDF(p);

//    float d = min(sphereDist, planeDist);
//    return d;
//}

float RayMarch(vec3 ro, vec3 rd) {
    float dO = 0.;
    for (int i = 0; i < MAX_STEPS; i++) {
        vec3 p = ro + rd * dO;
        float dS = GetDist(p);
        dO += dS;
        if (dO > MAX_DIST || dS < SURF_DIST) break;
    }

    return dO;
}

out vec4 FragColor;

void main() {
//    vec2 st = gl_FragCoord.xy / iResolution.xy;
//    vec3 color = vec3(0.);
//    color = vec3(st.x,st.y,abs(sin(u_time)));
//    col = vec4(color, 1.0);

    vec2 uv = (gl_FragCoord.xy - .5 * iResolution.xy) / iResolution.y;
    vec3 col = vec3(0);
    vec3 ro = vec3(0, 1, 0);
    vec3 rd = normalize(vec3(uv.x, uv.y, 1));

    float d = RayMarch(ro, rd);
    d /= 6.;
    col = vec3(d);

    FragColor = vec4(col, 1.);
}
