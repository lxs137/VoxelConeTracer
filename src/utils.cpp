//
// Created by liao xiangsen on 2019/8/11.
//
#include "utils.hpp"

#include <fstream>

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

GLuint loadAndCompileShader(const string &path, GLenum shaderType) {
    std::string shaderCode;
    std::ifstream inFile(path, std::ios::in);
    if (!inFile) {
        cerr << "Error open GLSL file: " << path << endl;
        return 0;
    }

    if (inFile.is_open()) {
        std::string line;
        while(getline(inFile, line))
            shaderCode += "\n" + line;
        inFile.close();
    } else {
        cerr << "Couldn't open shader: " << path << endl;
        return 0;
    }

    GLuint shaderID = glCreateShader(shaderType);
    if (0 == shaderID) {
        cerr << "Error creating" << shaderType << "shader." << endl;
    }

    const char *sourceStrPtr = shaderCode.c_str();
    glShaderSource(shaderID, 1, &sourceStrPtr, nullptr);
    glCompileShader(shaderID);

    GLint err, logLen;
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &err);
    glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &logLen);
    if (GL_FALSE == err && logLen > 0) {
        std::vector<char> errorMessage(logLen + 1);
        glGetShaderInfoLog(shaderID, logLen, nullptr, &errorMessage[0]);
        cerr << "OpenGL shader info log: " << &errorMessage[0] << endl;
        return 0;
    }

    return (GL_FALSE == err) ? 0 : shaderID;
}

GLuint linkProgram(GLuint vertShaderID, GLuint fragShaderID, GLuint geomShaderID) {
    GLuint programID = glCreateProgram();
    glAttachShader(programID, vertShaderID);
    glAttachShader(programID, fragShaderID);
    if (geomShaderID) {
        glAttachShader(programID, geomShaderID);
    }
    glLinkProgram(programID);

    // Check the program
    GLint err, logLen;
    glGetProgramiv(programID, GL_LINK_STATUS, &err);
    glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &logLen);
    if (GL_FALSE == err && logLen > 0) {
        std::vector<char> errorMessage(logLen + 1);
        glGetShaderInfoLog(programID, logLen, nullptr, &errorMessage[0]);
        cerr << "OpenGL shader info log: " << &errorMessage[0] << endl;
        return 0;
    }

    glDeleteShader(vertShaderID);
    glDeleteShader(fragShaderID);
    if (geomShaderID) {
        glDeleteShader(geomShaderID);
    }

    return (GL_FALSE == err) ? 0 : programID;
}

std::ostream& operator<<(std::ostream& os, const glm::mat4& mat) {
    os << "Mat[" << endl
       << "  " << mat[0][0] << ", " << mat[0][1] << ", " << mat[0][2] << ", " << mat[0][3] << endl
       << "  " << mat[1][0] << ", " << mat[1][1] << ", " << mat[1][2] << ", " << mat[1][3] << endl
       << "  " << mat[2][0] << ", " << mat[2][1] << ", " << mat[2][2] << ", " << mat[2][3] << endl
       << "  " << mat[3][0] << ", " << mat[3][1] << ", " << mat[3][2] << ", " << mat[3][3] << endl
       << "]" << endl;
    return os;
}

std::ostream& operator<<(std::ostream& os, const glm::vec3& vec) {
    os << "Vec3[ " << vec[0] << ", " << vec[1] << ", " << vec[2] << " ]" << endl;
    return os;
}

std::ostream& operator<<(std::ostream& os, const glm::vec4& vec) {
    os << "Vec4[ " << vec[0] << ", " << vec[1] << ", " << vec[2] << ", " << vec[3] << " ]" << endl;
    return os;
}

CONETRACER_NAMESPACE_END