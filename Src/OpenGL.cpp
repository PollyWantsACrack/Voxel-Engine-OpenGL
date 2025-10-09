#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include "Shader.h"
#include "Camera.h"
#include "Texture.h"
#include "Vao.h"
#include "ChunkLoader.h"
#include <vector>
#include "UniformBuffer.h"
#include "Chunk.h"

//forward deklaracje funckji
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void windowHintSetup();
static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void loadGlad();
void configureGL(GLFWwindow* window);
GLFWwindow* setupWindow();

//Zmienne
//Zmienne liczniea czasu
double lastTime{ 0.0 };
double currentTime{ 0.0 };
double deltaTime{ 0.0 };
double framesPerSecond = 60.0;
double waitTime = 1 / framesPerSecond;
//zmienne wircholkow
int hori = 16;
int vert = 15;
int dept = 0;

//Stałe
const float SCR_WDTH = 1280.0f;
const float SCR_HGHT = 720.0f;

//macierze
glm::mat4 view = glm::mat4(1.0f);
glm::mat4 model = glm::mat4(1.0f);
glm::mat4 projection = glm::mat4(1.0f);



//wskazniki
Vao* vaoptr{};
Chunk* chunkptr{};
Camera* cameraptr{};


int main() {
    
    
    //wlasciwosci okna
    windowHintSetup();
    //towrznie okna
    GLFWwindow* window = setupWindow();
    //usaiwnie kosntkesy
    glfwMakeContextCurrent(window);

    //załaduj funcje opengl
    loadGlad();
    //konfiguracja opengl
    configureGL(window);


    Shader shader{ "C:/Users/Franek/source/repos/OpenGL/Shaders Code/vertexShader.txt","C:/Users/Franek/source/repos/OpenGL/Shaders Code/fragmentShader.txt"};
    Texture texture{ "C:/Users/Franek/source/repos/OpenGL/Assets/texture.jpg" };
    ChunkLoader loader{};
    UniformBuffer ubo{2,0,0};
    loader.genChunk(hori,vert,dept);
    Chunk chunk{};
    chunk.generateVerticesOfChunk();
    chunkptr = &chunk;
    
    Camera camera{};
	cameraptr = &camera;

    Vao vao1{ chunk.vertices};
    vaoptr = &vao1;
    vao1.setVertexAttribPointer(0, 3, 5, 0);
    vao1.setVertexAttribPointer(1, 2, 5, 3);

    
    
    
    
    

    

  
    
    while (!glfwWindowShouldClose(window))
    {
        
        //obliczniea czasu
        currentTime = glfwGetTime();
        deltaTime = currentTime - lastTime;
        if (deltaTime <= waitTime)
        {
            continue;
        }
        
        //zmienne dynamiczne
        model = glm::mat4(1.0f);
        projection = glm::mat4(1.0f);
        view = glm::mat4(1.0f);
       

        processInput(window);
        glClearColor(0.49f, 0.79f, 0.99f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //Rysowanie
        shader.useShader();    
        shader.uniformBufferBinding("Matrices",0);

        projection = glm::perspective(glm::radians(45.0f), SCR_WDTH / SCR_HGHT, 0.1f, 500.0f);
        ubo.loadDataToUboMat4(0, projection);

        view = camera.getLookAt();
        ubo.loadDataToUboMat4(1, view);
        

        glActiveTexture(GL_TEXTURE0);
        texture.bindTexture();
        shader.setIntUniform("textureWall", 0);
        shader.setMatrixUniform("model", model);
        shader.setMatrixUniform("view", view);
        shader.setMatrixUniform("projection", projection);
        vao1.bindVAO();
        glDrawArrays(GL_TRIANGLES, 0,chunk.vertices.size()/5);

        //std::cout << "Kamera jest w pozycji X:" << camera.getCameraPos().x << " Y: " << camera.getCameraPos().y << " Z: " << camera.getCameraPos().z << "\n";
        //zakoncznie petli
        glfwSwapBuffers(window);
        glfwPollEvents();
        lastTime = currentTime;
    }

    glfwTerminate();
    return 0;
}





////////////////////////////////////////////////////////////////
/////////////////          METODY        ///////////////////////
////////////////////////////////////////////////////////////////


//Zmiena romziaru okna
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}
//WSAD
void processInput(GLFWwindow* window)
{
 

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {        
        cameraptr->updateVectorKey(Camera::W,chunkptr);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        cameraptr->updateVectorKey(Camera::S,chunkptr);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        cameraptr->updateVectorKey(Camera::A,chunkptr);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        cameraptr->updateVectorKey(Camera::D,chunkptr);
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
    {
        cameraptr->updateVectorKey(Camera::SPACE,chunkptr);
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
    {
        cameraptr->updateVectorKey(Camera::LSHIFT,chunkptr);
    }
    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
    {
        cameraptr->updateVectorKey(Camera::KEY_2,chunkptr);
    }
    if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
    {
        cameraptr->updateVectorKey(Camera::KEY_3,chunkptr);
    }
    if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
    if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    
    

        
}
//KAMERA
static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
    cameraptr->updateVectorMouse(xpos, ypos);
}

//Ustawianie hintów okna
void windowHintSetup()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 8);
}

void loadGlad()
{
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
    }
}

void configureGL(GLFWwindow* window)
{
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_DEPTH_TEST);
    glViewport(0, 0, SCR_WDTH, SCR_HGHT);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glDisable(GL_CULL_FACE);

}
GLFWwindow* setupWindow()
{
    GLFWwindow* window = glfwCreateWindow(SCR_WDTH, SCR_HGHT, "DiggingDeepeer", NULL, NULL);
    if (window == NULL) {
        std::cout << "Window Cration Failedn";
        glfwTerminate();
    }
    return window;
}
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        chunkptr->removeTargetBlock(cameraptr->getCameraPos(), cameraptr->getCameraFront());
        chunkptr->generateVerticesOfChunk();
        (*vaoptr).bindVBO();
        glBufferData(GL_ARRAY_BUFFER, chunkptr->vertices.size() * sizeof(float), chunkptr->vertices.data(), GL_DYNAMIC_DRAW);
    }
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
    {
        chunkptr->addBlock(cameraptr->getCameraPos(), cameraptr->getCameraFront());
        chunkptr->generateVerticesOfChunk();
        (*vaoptr).bindVBO();
        glBufferData(GL_ARRAY_BUFFER, chunkptr->vertices.size() * sizeof(float), chunkptr->vertices.data(), GL_DYNAMIC_DRAW);
    }
        
}
