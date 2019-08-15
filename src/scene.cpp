//
// Created by liao xiangsen on 2019/8/12.
//
#include "scene/scene.hpp"

#include "object.hpp"
#include "voxelizer.hpp"
#include "utils.hpp"

CONETRACER_NAMESPACE_BEGIN

shared_ptr<Grids> Scene::voxelize(int voxelsPerSide, float voxelSize) {
    int numVoxels = voxelsPerSide * voxelsPerSide * voxelsPerSide, voxelsPerPlane = voxelsPerSide * voxelsPerSide;
    GLubyte* data = new GLubyte[numVoxels * 4];
    memset(data, 0, sizeof(GLubyte) * numVoxels * 4);

    float precF = voxelSize * 0.1f;
    shared_ptr<Grids> gridsPtr = nullptr;
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
        vx_point_cloud_t* resultPoints = vx_voxelize_pc(vxMesh, voxelSize, voxelSize, voxelSize, precF);
        // resultPoints->vertices is point's pos, trans to index in voxel grids
        if (gridsPtr == nullptr) {
            gridsPtr = std::make_shared<Grids>(voxelsPerSide, voxelsPerSide, voxelsPerSide, resultPoints, voxelSize);
        } else {
            gridsPtr->addVoxels(resultPoints);
        }

        vx_mesh_free(vxMesh);
        vx_point_cloud_free(resultPoints);
    }

    return gridsPtr;
}

CONETRACER_NAMESPACE_END