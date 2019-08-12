//
// Created by liao xiangsen on 2019/8/11.
//
#include "utils.hpp"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

CONETRACER_NAMESPACE_BEGIN

std::shared_ptr<Scene> loadScene(const string &path, const string &resDir, const glm::vec3 &center, const double scale) {
    Assimp::Importer importer;

    const aiScene *scene = importer.ReadFile(path,
                                             aiProcess_Triangulate |
                                             aiProcess_CalcTangentSpace |
                                             aiProcess_JoinIdenticalVertices);

    if (!scene) {
        cerr << "Assimp load scene (" << path << ") err: " << importer.GetErrorString() << endl;
        return nullptr;
    }

    auto scenePtr = std::make_shared<Scene>();

    for(unsigned int m = 0; m < scene->mNumMeshes; m++) {
        aiMesh* mesh = scene->mMeshes[m];
        auto meshPtr = Mesh::build(mesh);
        auto matPtr = Material::build(scene->mMaterials[mesh->mMaterialIndex], resDir);
        auto objPtr = std::make_shared<RenderObj>(meshPtr, matPtr);
        scenePtr->objs.push_back(std::make_shared<RenderObj>(meshPtr, matPtr));
    }

    return scenePtr;
}

Texture2D loadTexture(const string &dir, const string &filename) {
    Texture2D tex;

    string path = dir + "/" + filename;
    // replace all '\' with '/'
    std::replace(path.begin(), path.end(), '\\', '/');

    const char *pathStr = path.c_str();
    GLubyte* textureData = stbi_load(pathStr, &tex.width, &tex.height, &tex.componentsPerPixel, 0);

    if(!textureData) {
        cerr << "Couldn't load image: " << pathStr << endl;
        return tex;
    }

    // Generate a texture ID and bind to it
    glGenTextures(1, &tex.textureID);
    glBindTexture(GL_TEXTURE_2D, tex.textureID);

    if(tex.componentsPerPixel == 4) {
        glTexImage2D(GL_TEXTURE_2D,    // Type of texture
                     0,                // Mipmap level (0 being the top level i.e. full size)
                     GL_RGBA,          // Internal format
                     tex.width,        // Width of the texture
                     tex.height,       // Height of the texture,
                     0,                // Border in pixels
                     GL_RGBA,          // Data format
                     GL_UNSIGNED_BYTE, // Type of texture data
                     textureData);     // The image data to use for this texture
    }
    else if(tex.componentsPerPixel == 3) {
        glTexImage2D(GL_TEXTURE_2D,    // Type of texture
                     0,                // Mipmap level (0 being the top level i.e. full size)
                     GL_RGB,           // Internal format
                     tex.width,        // Width of the texture
                     tex.height,       // Height of the texture,
                     0,                // Border in pixels
                     GL_RGB,           // Data format
                     GL_UNSIGNED_BYTE, // Type of texture data
                     textureData);     // The image data to use for this texture

    }else if(tex.componentsPerPixel == 1) {
        glTexImage2D(GL_TEXTURE_2D,    // Type of texture
                     0,                // Mipmap level (0 being the top level i.e. full size)
                     GL_RED,           // Internal format
                     tex.width,        // Width of the texture
                     tex.height,       // Height of the texture,
                     0,                // Border in pixels
                     GL_RED,           // Data format
                     GL_UNSIGNED_BYTE, // Type of texture data
                     textureData);     // The image data to use for this texture
    }

    if(tex.componentsPerPixel == 4 || tex.componentsPerPixel == 3 || tex.componentsPerPixel == 1) {
        // Specify our minification and magnification filters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glGenerateMipmap(GL_TEXTURE_2D);
    }

    // Check for OpenGL texture creation errors
    GLenum glError = glGetError();
    if(glError) {
        cerr << "There was an error loading the texture: "<< pathStr << endl;
        switch (glError) {
            case GL_INVALID_ENUM:
                cerr << "Invalid enum." << endl;
                break;
            case GL_INVALID_VALUE:
                cerr << "Invalid value." << endl;
                break;
            case GL_INVALID_OPERATION:
                cerr << "Invalid operation." << endl;
                break;
            default:
                cerr << "Unrecognised GLenum." << endl;
                break;
        }
    }

    stbi_image_free(textureData);

    return tex;
}

CONETRACER_NAMESPACE_END