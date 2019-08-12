//
// Created by liao xiangsen on 2019/8/12.
//

#include "mesh.hpp"

CONETRACER_NAMESPACE_BEGIN

Mesh::Mesh() {
    hasTexCoord = hasNormal = hasTangentsAndBitangents = false;
}

shared_ptr<Mesh> Mesh::build(const aiMesh *mesh) {
    auto meshPtr = std::make_shared<Mesh>();

    meshPtr->hasTexCoord = mesh->HasTextureCoords(0);
    meshPtr->hasNormal = mesh->HasNormals();
    meshPtr->hasTangentsAndBitangents = mesh->HasTangentsAndBitangents();

    meshPtr->numVertices = mesh->mNumVertices;
    meshPtr->numFaces = mesh->mNumFaces;

    meshPtr->vertices.reserve(mesh->mNumVertices);
    for(unsigned int i = 0; i < mesh->mNumVertices; i++) {
        aiVector3D pos = mesh->mVertices[i];
        meshPtr->vertices.emplace_back(pos.x, pos.y, pos.z);
    }

    if(meshPtr->hasTexCoord) {
        meshPtr->uvs.reserve(mesh->mNumVertices);
        for(unsigned int i = 0; i < mesh->mNumVertices; i++) {
            aiVector3D uv = mesh->mTextureCoords[0][i];
            meshPtr->uvs.emplace_back(uv.x, -uv.y);
        }
    }

    if(meshPtr->hasNormal) {
        meshPtr->normals.reserve(mesh->mNumVertices);
        for(unsigned int i = 0; i < mesh->mNumVertices; i++) {
            aiVector3D n = mesh->mNormals[i];
            meshPtr->normals.emplace_back(n.x, n.y, n.z);
        }
    }

    if(meshPtr->hasTangentsAndBitangents) {
        meshPtr->tangents.reserve(mesh->mNumVertices);
        for(unsigned int i = 0; i < mesh->mNumVertices; i++) {
            aiVector3D t = mesh->mTangents[i];
            meshPtr->tangents.emplace_back(t.x, t.y, t.z);
        }

        meshPtr->bitangents.reserve(mesh->mNumVertices);
        for(unsigned int i = 0; i < mesh->mNumVertices; i++) {
            aiVector3D b = mesh->mBitangents[i];
            meshPtr->bitangents.emplace_back(b.x, b.y, b.z);
        }
    }

    meshPtr->indices.reserve(3 * mesh->mNumFaces);
    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        meshPtr->indices.emplace_back(face.mIndices[0], face.mIndices[1], face.mIndices[2]);
    }

    return meshPtr;
}

std::ostream& operator<<(std::ostream &os, const Mesh &mesh) {
    os << "Mesh["
        << "  " << (mesh.hasTexCoord ? "hasTex" : "noTex")
        << "  " << (mesh.hasNormal ? "hasNormal" : "noNormal")
        << "  " << (mesh.hasTangentsAndBitangents ? "hasTangentsAndBitangents" : "noTangentsAndBitangents")
        << "  vertices: " << mesh.numVertices
        << "  faces: " << mesh.numFaces
        << "]\n";
    return os;
}

CONETRACER_NAMESPACE_END