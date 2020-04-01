#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <stdio.h>

static void error_callback(int error, const char* description){
    fputs(description, stderr);
}

int color = 0;
bool pressed = false;
float angle = 0.1;

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
    if (key == GLFW_KEY_ENTER && action == GLFW_PRESS) {
        color = (color + 1) % 3;
    }
}

static void mouse_callback(GLFWwindow* window, int key, int action, int mods){
    if (key == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        pressed = true;
    }
    if (key == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
        pressed = false;
    }
}

int main(void)
{
    GLFWwindow* window;
    glfwSetErrorCallback(error_callback);
    if (!glfwInit())
        exit(EXIT_FAILURE);
    window = glfwCreateWindow(640, 480, "Simple example", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_callback);
    glfwSetMouseButtonCallback(window, mouse_callback);
    while (!glfwWindowShouldClose(window))
    {
            float ratio;
            int width, height;
            glfwGetFramebufferSize(window, &width, &height);
            ratio = width / (float) height;
            glViewport(0, 0, width, height);
            glClear(GL_COLOR_BUFFER_BIT);
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            glOrtho(-ratio, ratio, -1.f, 1.f, 1.f, -1.f);
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
            if (pressed){
                glRotatef((float) angle * 50.f, 0.f, 0.f, 1.f);
                angle += 0.01;
            } else {
                glRotatef((float) angle * 50.f, 0.f, 0.f, 1.f);
            }
            glBegin(GL_TRIANGLES);
            switch (color) {
                case 0:
                    glColor3f(0.f, 1.f, 0.f);
                    break;
                case 1: glColor3f(1.f, 0.f, 0.f);
                     break;

                default: glColor3f(0.f, 0.f, 1.f);
            }
            glVertex3f(-0.6f, -0.4f, 0.f);
            glVertex3f(0.6f, -0.4f, 0.f);
            glVertex3f(0.f, 0.6f, 0.f);
            glEnd();
            glfwSwapBuffers(window);
            glfwPollEvents();
    }
    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}
