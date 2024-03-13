#include <glad/glad.h>
#include <GL/glew.h>

#include <GLFW/glfw3.h>
#include <limits>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader_m.h"
#include "camera.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include "generate.h"
#include <vector>

#include <iostream>

using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

// data
vector<glm::vec3> vertices; // 网格中的顶点
vector<unsigned int> indices; // 三角形   
vector<float> scalarData;
vector<glm::vec3> normals;

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// lighting
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

void calculateFPS(GLFWwindow* window)
{
    static double previousTime = glfwGetTime();
    static int frameCount = 0;

    double currentTime = glfwGetTime();
    double deltaTime = currentTime - previousTime;
    frameCount++;

    // 每秒更新一次帧率
    if (deltaTime >= 1.0)
    {
        double fps = frameCount / deltaTime;
        std::cout << "FPS: " << fps << std::endl;
        frameCount = 0;
        previousTime = currentTime;
    }
}

/* void renderScene()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // 生成随机点并绘制
    glBegin(GL_POINTS);
    for (int i = 0; i < 10000000; i++)
    {
        float x = static_cast<float>(rand()) / RAND_MAX * 2.0f - 1.0f;
        float y = static_cast<float>(rand()) / RAND_MAX * 2.0f - 1.0f;
        glVertex2f(x, y);
    }
    glEnd();
}
 */

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // build and compile our shader zprogram
    // ------------------------------------
    Shader renderShader("shader/contour.vs", "shader/contour.fs");

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    generateSphereVertices(1, 30, 30, vertices, normals, scalarData);
    generateSphereIndices(30, 30, indices);
    // first, configure the cube's VAO (and VBO)
    unsigned int VBO, VAO, EBO, normalVBO;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO); // 生成顶点缓冲区对象
    glGenBuffers(1, &EBO); // 生成三角形缓冲区对象
    glGenBuffers(1, &normalVBO); // 生成法线缓冲区对象

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO); // 绑定顶点缓冲区对象
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW); // 将顶点数据复制到顶点缓冲区对象中
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0); // 解绑顶点缓冲区对象

    glBindBuffer(GL_ARRAY_BUFFER, normalVBO);  // 绑定法线缓冲区对象
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), normals.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, 0); // 解绑法线缓冲区对象


    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); // 绑定三角形缓冲区对象
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int) * 3, indices.data(), GL_STATIC_DRAW); // 将三角形数据复制到三角形缓冲区对象中
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // 不能解绑ebo

    glBindVertexArray(0);     // 解绑顶点数组对象

    
    float maxX = std::numeric_limits<float>::lowest();
    float minX = std::numeric_limits<float>::max();

    for (const auto& vertex : vertices) {
        if (vertex.x > maxX) {
            maxX = vertex.x;
        }
        if (vertex.x < minX) {
            minX = vertex.x;
        }
    }





    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {

    calculateFPS(window);

        // per-frame time logic
        // --------------------
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        processInput(window);
        // renderScene();
        // render
        // ------
        glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);





        // be sure to activate shader when setting uniforms/drawing objects
        renderShader.use();
        //lightingShader.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
        renderShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
        renderShader.setVec3("lightPos", lightPos);
        renderShader.setVec3("viewPos", camera.Position);

        // view/projection transformations
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        renderShader.setMat4("projection", projection);
        renderShader.setMat4("view", view);

        // world transformation
        glm::mat4 model = glm::mat4(1.0f);
        renderShader.setMat4("model", model);


        // render the cube
        glBindVertexArray(VAO); // 绑定顶点数组对象
        glDrawElements(GL_TRIANGLES, indices.size() * 3, GL_UNSIGNED_INT, 0); // 绘制三角形


        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &normalVBO);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}


// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

