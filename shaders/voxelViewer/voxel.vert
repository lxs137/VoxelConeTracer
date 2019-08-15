#version 330 core

out vec4 color;

uniform int VoxelsPerSide;

uniform sampler3D VoxelsTexture;

void main() {
    // center of voxel
    vec3 pos;

    // vertexIDs = [0, 1, 2,..., n-1]
    pos.x = gl_VertexID % VoxelsPerSide;
    pos.z = (gl_VertexID / VoxelsPerSide) % VoxelsPerSide;
    pos.y = gl_VertexID / (VoxelsPerSide * VoxelsPerSide);

    color = texture(VoxelsTexture, pos / VoxelsPerSide);
    gl_Position = vec4(pos - VoxelsPerSide * 0.5, 1);
}