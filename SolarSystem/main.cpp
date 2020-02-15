
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.hpp"
#include "Camera.hpp"
#include "Model.h"

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void draw(Model loadedModel, float degree, glm::vec3 translate, glm::vec3 scale, Shader shader);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 20.0f));

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
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
    
    // build and compile shaders
    // -------------------------
    Shader ourShader("/Users/parsa/Desktop/University/SolarSystem/SolarSystem/VertexShader.glsl",
                     "/Users/parsa/Desktop/University/SolarSystem/SolarSystem/FragmentShader.glsl");
    
    // load models
    // -----------
    Model jupiter("/Users/parsa/Desktop/University/SolarSystem/SolarSystem/Resources/Jupiter/jupiter.obj");
    Model neptune("/Users/parsa/Desktop/University/SolarSystem/SolarSystem/Resources/Neptune/moon.obj");
    Model pluto("/Users/parsa/Desktop/University/SolarSystem/SolarSystem/Resources/Pluto/pluto.obj");
    Model uranus("/Users/parsa/Desktop/University/SolarSystem/SolarSystem/Resources/Uranus/Uranus.obj");
    Model venus("/Users/parsa/Desktop/University/SolarSystem/SolarSystem/Resources/Venus/jupiter.obj");
    Model mars("/Users/parsa/Desktop/University/SolarSystem/SolarSystem/Resources/Mars/mars.obj");
    Model earth("/Users/parsa/Desktop/University/SolarSystem/SolarSystem/Resources/Earth/Earth.obj");
    Model sun("/Users/parsa/Desktop/University/SolarSystem/SolarSystem/Resources/Sun/jupiter.obj");
    Model saturn("/Users/parsa/Desktop/University/SolarSystem/SolarSystem/Resources/Saturn/saturn_final.blend.obj");
    Model mercury("/Users/parsa/Desktop/University/SolarSystem/SolarSystem/Resources/Mercury/Uranus.obj");
    Model milkyWay("/Users/parsa/Desktop/University/SolarSystem/SolarSystem/Resources/Milky Way/jupiter.obj");
    
    // draw in wireframe
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    
    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        // --------------------
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        float now = glfwGetTime()/3;
        
        // input
        // -----
        processInput(window);
        
        // render
        // ------
        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        // don't forget to enable shader before setting uniforms
        ourShader.use();
        
        // view/projection transformations
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        ourShader.setMat4("projection", projection);
        ourShader.setMat4("view", view);
        
        // render the loaded models
        draw(milkyWay, glm::radians(0.0), glm::vec3(-50.0f, -50.0f, -50.0f), glm::vec3(50.0f, 50.0f, 50.0f), ourShader);
        draw(sun, glm::radians(0.0), glm::vec3(-1.2f, -1.2f, -1.2f), glm::vec3(1.2f, 1.2f, 1.2f), ourShader);
        draw(mercury, glm::radians(now*48), glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.3f, 0.3f, 0.3f), ourShader);
        draw(venus, glm::radians(now*35), glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3(0.3f, 0.3f, 0.3f), ourShader);
        draw(earth, glm::radians(now*30), glm::vec3(0.0f, 0.0f, 8.0f), glm::vec3(0.3f, 0.3f, 0.3f), ourShader);
        draw(mars, glm::radians(now*24), glm::vec3(0.0f, 0.0f, 10.0f), glm::vec3(0.3f, 0.3f, 0.3f), ourShader);
        draw(jupiter, glm::radians(now*13), glm::vec3(0.0f, 0.0f, 12.0f), glm::vec3(0.3f, 0.3f, 0.3f), ourShader);
        draw(saturn, glm::radians(now*9.5), glm::vec3(0.0f, 0.0f, 13.0f), glm::vec3(0.3f, 0.3f, 0.3f), ourShader);
        draw(uranus, glm::radians(now*7), glm::vec3(0.0f, 0.0f, 14.0f), glm::vec3(0.3f, 0.3f, 0.3f), ourShader);
        draw(neptune, glm::radians(now*5.5), glm::vec3(0.0f, 0.0f, 16.0f), glm::vec3(0.3f, 0.3f, 0.3f), ourShader);
        draw(pluto, glm::radians(now*5), glm::vec3(0.0f, 0.0f, 20.0f), glm::vec3(0.3f, 0.3f, 0.3f), ourShader);
        
        
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// Draw spheres
void draw(Model loadedModel,float degree, glm::vec3 translate, glm::vec3 scale, Shader shader)
{
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::rotate(model, degree, glm::vec3(0, 1, 0));
    model = glm::translate(model, translate);
    model = glm::scale(model, scale);
    shader.setMat4("model", model);
    loadedModel.Draw(shader);
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
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
    if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
        camera.ProcessKeyboard(ZOOMIN, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
        camera.ProcessKeyboard(ZOOMOUT, deltaTime);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}
