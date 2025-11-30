//#vertex
@vs vs_tri
in vec2 position;
layout(binding=0) uniform u_color0 {
    vec4 u_color;
};
out vec4 color;

void main() {
    gl_Position = vec4(position, 0.0, 1.0);
    color = u_color;
}
@end

//#fragment
@fs fs_tri
precision mediump float;
in vec4 color;
out vec4 frag_color;

void main() {
    frag_color = color;
}
@end

@program triangleshader vs_tri fs_tri