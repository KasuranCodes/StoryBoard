//#vertex
#version 300 es
in vec2 pos;
void main() {
    gl_Position = vec4(pos, 0.0, 1.0);
}

//#fragment
#version 300 es
precision mediump float;
out vec4 frag_color;
void main() {
    frag_color = vec4(1.0, 1.0, 0.0, 1.0);
}
