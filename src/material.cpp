//
// Created by liao xiangsen on 2019/8/12.
//

#include "material.hpp"

#include "common.hpp"
#include "utils.hpp"

CONETRACER_NAMESPACE_BEGIN

shared_ptr<Material> Material::build(const aiMaterial *mat, const std::string &resDir) {
    auto matPtr = std::make_shared<Material>();

    aiString name;
    mat->Get(AI_MATKEY_NAME, name);
    matPtr->name = name.data;

    aiColor3D color;
    mat->Get(AI_MATKEY_COLOR_AMBIENT, color);
    matPtr->ambientColor = glm::vec3(color.r, color.g, color.b);

    mat->Get(AI_MATKEY_COLOR_SPECULAR, color);
    matPtr->specularColor = glm::vec3(color.r, color.g, color.b);

    mat->Get(AI_MATKEY_COLOR_DIFFUSE, color);
    matPtr->diffuseColor = glm::vec3(color.r, color.g, color.b);

    mat->Get(AI_MATKEY_COLOR_DIFFUSE, color);
    matPtr->emissiveColor = glm::vec3(color.r, color.g, color.b);

    mat->Get(AI_MATKEY_OPACITY, matPtr->opacity);
    mat->Get(AI_MATKEY_SHININESS, matPtr->shininess);

    if(mat->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
        cout << "  diffuseTexture loaded" << endl;
        aiString texturePath;
        if(mat->GetTexture(aiTextureType_DIFFUSE, 0, &texturePath) == AI_SUCCESS) {
            matPtr->diffuseTex = loadTexture(resDir, texturePath.data);
        }
    }

    if(mat->GetTextureCount(aiTextureType_AMBIENT) > 0) {
        cout << "  specularTexture loaded" << endl;
        aiString texturePath;
        if(mat->GetTexture(aiTextureType_AMBIENT, 0, &texturePath) == AI_SUCCESS) {
            matPtr->specularTex = loadTexture(resDir, texturePath.data);
        }
    }

    if(mat->GetTextureCount(aiTextureType_HEIGHT) > 0) {
        cout << "  heightTexture loaded" << endl;
        aiString texturePath;
        if(mat->GetTexture(aiTextureType_HEIGHT, 0, &texturePath) == AI_SUCCESS) {
            matPtr->heightTex = loadTexture(resDir, texturePath.data);
        }
    }

    if(mat->GetTextureCount(aiTextureType_OPACITY) > 0) {
        cout << "  maskTexture loaded" << endl;
        aiString texturePath;
        if(mat->GetTexture(aiTextureType_OPACITY, 0, &texturePath) == AI_SUCCESS) {
            matPtr->maskTex = loadTexture(resDir, texturePath.data);
        }
    }

    return matPtr;
}

void Material::bindShader(GLuint shader) const {
    glUseProgram(shader);

    glUniform3f(glGetUniformLocation(shader, "AmbientColor"), ambientColor.r, ambientColor.g, ambientColor.b);
    glUniform3f(glGetUniformLocation(shader, "DiffuseColor"), diffuseColor.r, diffuseColor.g, diffuseColor.b);
    glUniform3f(glGetUniformLocation(shader, "SpecularColor"), specularColor.r, specularColor.g, specularColor.b);

    glUniform1f(glGetUniformLocation(shader, "Shininess"), shininess);
    glUniform1f(glGetUniformLocation(shader, "Opacity"), opacity);

    glActiveTexture(GL_TEXTURE0 + DIFFUSE_TEXTURE);
    glBindTexture(GL_TEXTURE_2D, diffuseTex.textureID);
    glUniform1i(glGetUniformLocation(shader, "DiffuseTexture"), DIFFUSE_TEXTURE);
    glUniform2f(glGetUniformLocation(shader, "DiffuseTextureSize"), diffuseTex.width, diffuseTex.height);

    glActiveTexture(GL_TEXTURE0 + SPECULAR_TEXTURE);
    glBindTexture(GL_TEXTURE_2D, specularTex.textureID);
    glUniform1i(glGetUniformLocation(shader, "SpecularTexture"), SPECULAR_TEXTURE);
    glUniform2f(glGetUniformLocation(shader, "SpecularTextureSize"), specularTex.width, specularTex.height);

    glActiveTexture(GL_TEXTURE0 + MASK_TEXTURE);
    glBindTexture(GL_TEXTURE_2D, maskTex.textureID);
    glUniform1i(glGetUniformLocation(shader, "MaskTexture"), MASK_TEXTURE);
    glUniform2f(glGetUniformLocation(shader, "MaskTextureSize"), maskTex.width, maskTex.height);

    glActiveTexture(GL_TEXTURE0 + HEIGHT_TEXTURE);
    glBindTexture(GL_TEXTURE_2D, heightTex.textureID);
    glUniform1i(glGetUniformLocation(shader, "HeightTexture"), HEIGHT_TEXTURE);
    glUniform2f(glGetUniformLocation(shader, "HeightTextureSize"), heightTex.width, heightTex.height);
}

CONETRACER_NAMESPACE_END