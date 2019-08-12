//
// Created by liao xiangsen on 2019/8/12.
//

#include <object.hpp>
#include <voxelizer.hpp>
#include "scene.hpp"

CONETRACER_NAMESPACE_BEGIN

void Scene::voxelize(int voxelsPerSide, float voxelSize) {
    int numVoxels = voxelsPerSide * voxelsPerSide * voxelsPerSide, voxelsPerPlane = voxelsPerSide * voxelsPerSide;
    GLubyte* data = new GLubyte[numVoxels * 4];
    memset(data, 0, sizeof(GLubyte) * numVoxels * 4);

    // voxelize
    for (auto &obj: objs) {
        auto meshPtr = obj->meshPtr;
        vx_mesh_t *vxMesh = vx_mesh_alloc(meshPtr->numVertices, meshPtr->numFaces * 3);
        // Can be optimized by memcpy?
        for (unsigned int i = 0; i < meshPtr->numVertices; i++) {
            vxMesh->vertices[i].x = meshPtr->vertices[i].x;
            vxMesh->vertices[i].y = meshPtr->vertices[i].y;
            vxMesh->vertices[i].z = meshPtr->vertices[i].z;
        }
        for (unsigned int i = 0; i < meshPtr->numFaces; i++) {
            vxMesh->indices[3 * i] = meshPtr->indices[i].x;
            vxMesh->indices[3 * i + 1] = meshPtr->indices[i].y;
            vxMesh->indices[3 * i + 2] = meshPtr->indices[i].z;
        }
        float precF = voxelSize * 0.1f, resF_1 = 1.0f / voxelSize;
        vx_point_cloud_t* result = vx_voxelize_pc(vxMesh, voxelSize, voxelSize, voxelSize, precF);
        // result->vertices is point's pos, trans to index in voxel grids
        for(size_t i = 0; i < result->nvertices; i++) {
            vx_vertex_t *p = &(result->vertices[i]);
            int x = (int)(p->x * resF_1), y = (int)(p->y * resF_1), z = (int)(p->z * resF_1);
            int offset = 4 * (x + y * voxelsPerSide + z * voxelsPerPlane);
            data[offset] = 1;
            data[offset + 1] = 1;
            data[offset + 2] = 1;
            data[offset + 3] = 1;
        }
        vx_point_cloud_free(result);
    }

    // bind voxel grids to 3D texture
    glEnable(GL_TEXTURE_3D);
    voxelTex.size = voxelsPerSide;
    glGenTextures(1, &voxelTex.textureID);
    glBindTexture(GL_TEXTURE_3D, voxelTex.textureID);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA8, voxelsPerSide, voxelsPerSide, voxelsPerSide, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_3D);
    delete[] data;
}

CONETRACER_NAMESPACE_END