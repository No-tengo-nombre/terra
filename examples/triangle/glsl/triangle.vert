#version 450

layout (location = 0) out vec3 frag_color;

vec2 positions[] = {
    vec2(0.0, 0.5),
    vec2(0.5, 0.0),
    vec2(-0.5, 0.0),
};

vec3 colors[] = {
    vec3(1.0, 0.0, 0.0),
    vec3(0.0, 1.0, 0.0),
    vec3(0.0, 0.0, 1.0),
};

void main() {
    gl_Position = vec4(positions[gl_VertexIndex], 0.0, 1.0);
    frag_color = colors[gl_VertexIndex];
}
