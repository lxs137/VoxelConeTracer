//
// Created by liao xiangsen on 2019/8/11.
//

#include "application.hpp"

#include <glm/gtc/matrix_transform.hpp>

#include "utils.hpp"

CONETRACER_NAMESPACE_BEGIN

TracerApp::TracerApp(int width, int height, GLFWwindow *window): winWidth(width), winHeight(height), windowPtr(window) {
    ctrlPtr = std::make_shared<Controller>(window, width, height);
    mode = TRACERAPP_DRAW_MODE::RAW_VOXELS_MODE;
}

bool TracerApp::initContext() {
    cout << "Compile and link shader..." << endl;
    GLuint voxelVertID = loadAndCompileShader("../shaders/voxelViewer/voxel.vert", GL_VERTEX_SHADER),
        voxelGeomID = loadAndCompileShader("../shaders/voxelViewer/voxel.geom", GL_GEOMETRY_SHADER),
        voxelFragID = loadAndCompileShader("../shaders/voxelViewer/voxel.frag", GL_FRAGMENT_SHADER);
    if (voxelVertID == 0 || voxelGeomID == 0 || voxelFragID == 0
        || (voxelShaderID = linkProgram(voxelVertID, voxelGeomID, voxelFragID)) == 0) {
        cerr << "init shader err." << endl;
        return false;
    }

    cout << "Loading and voxelize scene..." << endl;
    double timeStart = glfwGetTime();
//    scenePtr = loadScene("../data/models/cube/cube.obj", "../data/models/cube", glm::vec3(0.0f), 0.05f);
    scenePtr = loadScene("../data/models/teapot/teapot.obj", "../data/models/teapot", glm::vec3(0.0f), 0.05f);
    auto gridsPtr = scenePtr->voxelize(VOXELS_PER_SIDE, VOXELS_SIZE);
    cout << "voxels density: " << gridsPtr->getDensity() << endl;
    GLubyte* voxelizeData = new GLubyte[VOXELS_PER_SIDE * VOXELS_PER_SIDE * VOXELS_PER_SIDE * 4];
    for (auto p : gridsPtr->voxels) {
        int offset = 4 * gridsPtr->getOffset(p.x, p.y, p.z);
        voxelizeData[offset] = 250;
        voxelizeData[offset + 1] = 0;
        voxelizeData[offset + 2] = 0;
        voxelizeData[offset + 3] = 1;
    }
    // bind voxel grids to 3D texture
    glEnable(GL_TEXTURE_3D);
    voxelTex.size = VOXELS_PER_SIDE;
    glGenTextures(1, &voxelTex.textureID);
    glBindTexture(GL_TEXTURE_3D, voxelTex.textureID);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA8, VOXELS_PER_SIDE, VOXELS_PER_SIDE, VOXELS_PER_SIDE, 0, GL_RGBA, GL_UNSIGNED_BYTE, voxelizeData);
    glGenerateMipmap(GL_TEXTURE_3D);
    delete[] voxelizeData;
    cout << "load and voxelize scene cost: " << (glfwGetTime() - timeStart) << " seconds." << endl;

    switch (mode) {
        case RAW_VOXELS_MODE:
            int numVoxels = voxelTex.size * voxelTex.size * voxelTex.size;
            unsigned char *emptyVBO = new unsigned char[numVoxels];
            glGenBuffers(1, &voxelVBOID);
            glGenVertexArrays(1, &voxelVAOID);
            glBindVertexArray(voxelVAOID);
            glBindBuffer(GL_ARRAY_BUFFER, voxelVBOID);
            glBufferData(GL_ARRAY_BUFFER, numVoxels * sizeof(unsigned char), emptyVBO, GL_STATIC_DRAW);

            glVertexAttribPointer(0, 1, GL_BYTE, GL_FALSE, 0, 0);
            glEnableVertexAttribArray(0);
            delete []emptyVBO;
            break;
    }

    return true;
}

void TracerApp::drawVoxels() {
    glUseProgram(voxelShaderID);

    int numVoxels = voxelTex.size * voxelTex.size * voxelTex.size;
    glUniform1f(glGetUniformLocation(voxelShaderID, "VoxelSize"), VOXELS_SIZE);
    glUniform1i(glGetUniformLocation(voxelShaderID, "VoxelsPerSide"), voxelTex.size);
    glm::mat4 modelMatrix = glm::translate(glm::scale(glm::mat4(1.0f), glm::vec3(VOXELS_SIZE)), glm::vec3(0, 0, 0));
    glm::mat4 viewMatrix = ctrlPtr->cameraPtr->getViewMatrix();
    glm::mat4 modelViewMatrix = viewMatrix * modelMatrix;
    glm::mat4 projectionMatrix = ctrlPtr->cameraPtr->getProjectionMatrix();

    cout << "modelViewMat: " << modelViewMatrix << endl;
    cout << "projectionMat: " << projectionMatrix << endl;
    glm::vec4 point = projectionMatrix * modelMatrix * (glm::vec4(36, 25, 4, 0) + glm::vec4(-0.5, 0.5, 0.5, 0));
    cout << "transform: " << point;

    glUniformMatrix4fv(glGetUniformLocation(voxelShaderID, "ModelViewMatrix"), 1, GL_FALSE, &modelViewMatrix[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(voxelShaderID, "ProjectionMatrix"), 1, GL_FALSE, &projectionMatrix[0][0]);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_3D, voxelTex.textureID);
    glUniform1i(glGetUniformLocation(voxelShaderID, "VoxelsTexture"), 0);

    glBindVertexArray(voxelVAOID);
    glDrawArrays(GL_POINTS, 0, numVoxels);

    glBindVertexArray(0);
    glUseProgram(0);
}

void TracerApp::draw(float loopCostTime) {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, winWidth, winHeight);
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    switch (mode) {
        case RAW_VOXELS_MODE:
            drawVoxels();
            break;
    }
    ctrlPtr->loopUpdate(loopCostTime);
}

CONETRACER_NAMESPACE_END