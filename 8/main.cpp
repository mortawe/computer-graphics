// Include standard headers
#include <stdio.h>
#include <stdlib.h>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>

GLFWwindow *window;

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "config.h"
#include "control.cpp"
#include "shader_m.h"
#include "texture.cpp"
#include "box.cpp"

int WINDOW_SIZE = 900;

int initMain() {
    if (!glfwInit()) {
        return -1;
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow( WINDOW_SIZE, WINDOW_SIZE, "Simple example", NULL, NULL);
    if( window == NULL ){
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    glewExperimental = true;
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        getchar();
        glfwTerminate();
        return -1;
    }
    return 0;
}

int main(  ){
    if (initMain()) {
      return -1;
    }

    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    glfwSetKeyCallback(window, keyCallback);

    glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    Shader lightingShader("shaders/shader.vs", "shaders/shader.fs");
    Shader lampShader("shaders/lamp.vs", "shaders/lamp.fs");
    Shader boxShader("shaders/box.vs", "shaders/box.fs");

    Box box = Box{};
    box.init(multy);
    box.initVAO();
    pyramid = new Pyramid(4, 10, 3, 40, 1);
    pyramid->count();

    unsigned int lightVAO;
    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);

    glBindBuffer(GL_ARRAY_BUFFER, pyramid->VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *) 0);

    glm::mat4 Projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
    glm::mat4 View = glm::lookAt(
            glm::vec3(0, 0, 10),
            glm::vec3(0, 0, 0),
            glm::vec3(0, 1, 0)
    );
    lightingShader.setMat4("projection", Projection);
    lightingShader.setMat4("view", View);

    glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

    uint texture = readTexture();
    if (!texture) {
        cout << "Image could not be opened\n";
        return -5;
    }
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    do {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glBindTexture(GL_TEXTURE_2D, texture);
        boxShader.use();
        glm::mat4 Model = glm::mat4(1.0f);
        boxShader.setMat4("projection", Projection);
        boxShader.setMat4("view", View);
        boxShader.setMat4("model", Model);

        glBindVertexArray(box.VAO);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        glDrawArrays(GL_TRIANGLE_STRIP, 4, 4);
        glDrawArrays(GL_TRIANGLE_STRIP, 8, 4);
        glDrawArrays(GL_TRIANGLE_STRIP, 12, 4);
        glDrawArrays(GL_TRIANGLE_STRIP, 16, 4);
        glDrawArrays(GL_TRIANGLE_STRIP, 20, 4);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        lightingShader.use();
        lightingShader.setVec3("objectColor", 1.0f, 0.5f, 1.0f);
        lightingShader.setVec3("lightColor", lightColor);
        lightingShader.setVec3("lightPos", lightPos);
        lightingShader.setVec3("viewPos", 0.0, 0.0, 10);

        Model = glm::translate(Model, glm::vec3(pyramid->translationX, pyramid->translationY, pyramid->translationZ));
        Model = glm::rotate(Model, pyramid->rotationY, glm::vec3(0.0f, 1.0f, 0.0f));
        Model = glm::rotate(Model, pyramid->rotationX, glm::vec3(1.0f, 0.0f, 0.0f));
        Model = glm::rotate(Model, pyramid->rotationZ, glm::vec3(0.0f, 0.0f, 1.0f));
        lightingShader.setMat4("projection", Projection);
        lightingShader.setMat4("view", View);
        lightingShader.setMat4("model", Model);

        glBindVertexArray(pyramid->VAO);
        glDrawArrays(GL_TRIANGLES, 0, pyramid->layersN * pyramid->vertNum * 2 * 3 * 3 );

        lampShader.use();
        lampShader.setMat4("projection", Projection);
        lampShader.setMat4("view", View);
        Model = glm::mat4(1.0f);
        Model = glm::translate(Model, lightPos);
        Model = glm::scale(Model, glm::vec3(0.2f));
        lampShader.setMat4("model", Model);

        glBindVertexArray(lightVAO);

        glDrawArrays(GL_TRIANGLES, 0, 36);
        if (isMotionOn) {
            pyramid->animate();
        }

        glfwSwapBuffers(window);
        glfwPollEvents();

    }
    while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
           glfwWindowShouldClose(window) == 0 );

    glDeleteVertexArrays(1, &pyramid->VAO);
    glDeleteVertexArrays(1, &lightVAO);
    glDeleteBuffers(1, &pyramid->VBO);

    glfwTerminate();

    return 0;
}

