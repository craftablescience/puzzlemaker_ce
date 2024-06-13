#version 150

attribute vec3 vPos;
attribute vec2 vUV;

uniform mat4 uMVP;

out float fDepth;
out vec2 fUVMesh;

void main() {
    vec4 position = uMVP * vec4(vPos, 1.0);
    gl_Position = position;
    fUVMesh = vUV;

    // Unused right now
    fDepth = position.w;
}
