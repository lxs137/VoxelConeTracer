//
// Created by liao xiangsen on 2019/8/14.
//

#include "scene/grids.hpp"

#include "utils.hpp"

CONETRACER_NAMESPACE_BEGIN

Grids::Grids(int w, int h, int d, vx_point_cloud_t *points, float voxelSize_): width(w), height(h), depth(d),
                                                                             planeSize(w*h), voxelSize(voxelSize_) {
    float res_1 = 1.f / voxelSize;
    int halfWidth = (int)(width * 0.5f), halfHeight = (int)(height * 0.5f), halfDepth = (int)(depth * 0.5f);
    for(size_t i = 0; i < points->nvertices; i++) {
        vx_vertex_t *p = &(points->vertices[i]);
        int x = clamp((int)(p->x * res_1) + halfWidth, 0, width),
            y = clamp((int)(p->y * res_1) + halfHeight, 0, height),
            z = clamp((int)(p->z * res_1) + halfDepth, 0, depth);
        voxels.emplace_back(x, y, z);
        cout << "(" << x << ", " << y << ", " << z << ")" << endl;
    }
}

void Grids::addVoxels(vx_point_cloud_t *points) {
    float res_1 = 1.f / voxelSize;
    int halfWidth = (int)(width * 0.5f), halfHeight = (int)(height * 0.5f), halfDepth = (int)(depth * 0.5f);
    for(size_t i = 0; i < points->nvertices; i++) {
        vx_vertex_t *p = &(points->vertices[i]);
        int x = clamp((int)(p->x * res_1) + halfWidth, 0, width),
            y = clamp((int)(p->y * res_1) + halfHeight, 0, height),
            z = clamp((int)(p->z * res_1) + halfDepth, 0, depth);
        voxels.emplace_back(x, y, z);
        cout << "(" << x << ", " << y << ", " << z << ")" << endl;
    }
}

CONETRACER_NAMESPACE_END
