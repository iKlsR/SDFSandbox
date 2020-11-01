#version 330 core
uniform vec2 iResolution;
uniform float u_time;

out vec4 FragColor;

void main() {
    vec2 st = gl_FragCoord.xy / iResolution.xy;
    vec3 color = vec3(0.);
    color = vec3(st.x,st.y,abs(sin(u_time)));

    FragColor = vec4(color, 1.);
}
