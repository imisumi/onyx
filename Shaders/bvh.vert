#version 460 core

void main() {
    // Just pass through a dummy vertex
    // The geometry shader will generate all the actual geometry
    gl_Position = vec4(0.0, 0.0, 0.0, 1.0);
}