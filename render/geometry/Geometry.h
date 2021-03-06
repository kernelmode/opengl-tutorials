/* 
 * File:   Geometry.h
 * Author: kdubovikov
 *
 * Created on December 7, 2014, 3:39 PM
 */

#pragma once

#define GL_GLEXT_PROTOTYPES
#include "glfw/glfw3.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "shader/Shader.h"
#include "shader/ShaderManager.h"
#include "shader/CompilationResult.h"
#include "BufferManager.h"
#include "model/MeshLoader.h"

class Geometry {
protected:
    static const std::string MVP_UNIFORM_NAME;
    static const std::string VIEW_UNIFORM_NAME;
    static const std::string MODEL_UNIFORM_NAME;
    static const std::string VERTEX_BUFFER_NAME;
    static const std::string NORMAL_BUFFER_NAME;
    static const std::string UV_BUFFER_NAME;
    
    ShaderManager& shaderManager;
    BufferManager& bufferManager;
    
    std::vector<glm::vec3> verticies;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> uvs;
    
    GLuint vertexArrayId;
    int objectId;

    glm::mat4 modelMatrix;
    
    virtual void preRender();
    virtual void postRender();
    virtual void prepareBuffers();
public:
    Geometry(ShaderManager& shaderManager, BufferManager& bufferManager);
    void prepare();
    virtual void applyUniforms(const glm::mat4 &viewMatrix, const glm::mat4 &projectionMatrix);
    void translate(glm::vec3 to);
    virtual void render();
    ShaderManager& getShaderManager();
    glm::mat4& getModelMatrix();
    int getObjectId();
    ~Geometry();
};

