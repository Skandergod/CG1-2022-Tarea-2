
#include <iostream>
#include <cmath>
#include <vector>

// GLEW
// #ifdef GLEW_STATIC
// #  define GLEWAPI extern
// #else
// #  ifdef GLEW_BUILD
// #    define GLEWAPI extern __declspec(dllexport)
// #  else
// #    define GLEWAPI extern __declspec(dllimport)
// #  endif
// #endif

#include <../GL/glew.h>

// pueden utilizar este después
// #include <GL/glad.h>

// GLFW
#include <../GLFW/glfw3.h>

// GLM Mathematics
#define GLM_FORCE_RADIANS// force everything in radian
#include <../glm/glm.hpp>
#include <../glm/gtc/matrix_transform.hpp>
#include <../glm/gtc/type_ptr.hpp>

// Other includes
#include "Shader.h"

using namespace std;

// Function prototypes
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void do_movement(GLfloat delta);

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;

// Camera
GLfloat lastX  =  WIDTH  / 2.0;
GLfloat lastY  =  HEIGHT / 2.0;
bool    keys[1024];

// Deltatime
GLfloat deltaTime = 0.0f;	// Time between current frame and last frame
GLfloat lastFrame = 0.0f;  	// Time of last frame

GLfloat zDist = -3.0f;
GLfloat angle = 0.0f;

class My3DObject
{
public:
    int size;

    // data =  x,y,z,nx,ny,nz,x,y,z,nx,ny,nz, .... 
    My3DObject(GLfloat* data, int size)
    {
        this->size = size;
        // First, set the container's VAO (and VBO)
        glGenVertexArrays(1, &containerVAO);
        glBindVertexArray(containerVAO);

        glGenBuffers(1, &VBO);
        // We only need to bind to the VBO (to link it with glVertexAttribPointer), no need to fill it; the VBO's data already contains all we need.
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);

        // Position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);
        // Normal attribute
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
        glEnableVertexAttribArray(1);
        // Color attribute
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
        glEnableVertexAttribArray(2);
    }

    void draw()
    {
        bind();
        glDrawArrays(GL_TRIANGLES, 0, size/36);
    }

private:
    GLuint VBO, containerVAO;

    void bind()
    {
        glBindVertexArray(containerVAO);
    }

};


// The MAIN function, from here we start the application and run the game loop
int main()
{
    // Init GLFW
    glfwInit();
    // Set all the required options for GLFW, (GLSL version required = 3.0)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    // Create a GLFWwindow object that we can use for GLFW's functions
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "proyecto 1 CG1 - UCV", nullptr, nullptr);
    glfwMakeContextCurrent(window);

    // Set the required callback functions
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, mouse_callback);

    // Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
    glewExperimental = GL_TRUE;
    // Initialize GLEW to setup the OpenGL Function pointers
    glewInit();

    // OpenGL options
    glEnable(GL_DEPTH_TEST);

    // Build and compile our shader program
    Shader basic_shader("color_shader.vs", "color_shader.frag");


    // Set up vertex data (and buffer(s)) and attribute pointers
    GLfloat cube_vertices[] =
    {
        // back
        0.5,0.5,-0.5, 0.0f,  0.0f, -1.0f, 1,0,0,
        0.5,-0.5,-0.5, 0.0f,  0.0f, -1.0f,1,0,0,
       -0.5,0.5,-0.5,  0.0f,  0.0f, -1.0f,1,0,0,
       -0.5,0.5,-0.5,0.0f,  0.0f, -1.0f,1,0,0,
        0.5,-0.5,-0.5, 0.0f,  0.0f, -1.0f,1,0,0,
       -0.5,-0.5,-0.5, 0.0f,  0.0f, -1.0f,1,0,0,

       // front 
      -0.5,0.5,0.5,  0.0f,  0.0f,  1.0f,0,1,0,
      -0.5,-0.5,0.5,  0.0f,  0.0f,  1.0f,0,1,0,
       0.5,-0.5,0.5,  0.0f,  0.0f,  1.0f,0,1,0,
      -0.5,0.5,0.5,  0.0f,  0.0f,  1.0f,0,1,0,
       0.5,-0.5,0.5, 0.0f,  0.0f,  1.0f,0,1,0,
       0.5,0.5,0.5,  0.0f,  0.0f,  1.0f,0,1,0,

       // left
       -0.5,0.5,-0.5, -1.0f,  0.0f,  0.0f,0,0,1,
       -0.5,-0.5,-0.5, -1.0f,  0.0f,  0.0f,0,0,1,
       -0.5,-0.5,0.5, -1.0f,  0.0f,  0.0f,0,0,1,
       -0.5,0.5,-0.5, -1.0f,  0.0f,  0.0f,0,0,1,
       -0.5,-0.5,0.5, -1.0f,  0.0f,  0.0f,0,0,1,
       -0.5,0.5,0.5, -1.0f,  0.0f,  0.0f,0,0,1,



       // right
        0.5,0.5,0.5, 1.0f,  0.0f,  0.0f, 1,0,1,
        0.5,-0.5,0.5, 1.0f,  0.0f,  0.0f,1,0,1,
        0.5,-0.5,-0.5, 1.0f,  0.0f,  0.0f,1,0,1,
        0.5,0.5,0.5, 1.0f,  0.0f,  0.0f,1,0,1,
        0.5,-0.5,-0.5, 1.0f,  0.0f,  0.0f,1,0,1,
        0.5,0.5,-0.5, 1.0f,  0.0f,  0.0f,1,0,1,


        // bottom
       -0.5,-0.5,0.5, 0.0f, -1.0f,  0.0f,1,1,0,
       -0.5,-0.5,-0.5, 0.0f, -1.0f,  0.0f,1,1,0,
        0.5,-0.5,-0.5,0.0f, -1.0f,  0.0f,1,1,0,
       -0.5,-0.5,0.5, 0.0f, -1.0f,  0.0f,1,1,0,
        0.5,-0.5,-0.5, 0.0f, -1.0f,  0.0f,1,1,0,
        0.5,-0.5,0.5, 0.0f, -1.0f,  0.0f,1,1,0,


        // top
        -0.5,0.5,-0.5, 0.0f,  1.0f,  0.0f, 1,1,1,
        -0.5,0.5,0.5, 0.0f,  1.0f,  0.0f, 1,1,1,
         0.5,0.5,0.5, 0.0f,  1.0f,  0.0f, 1,1,1,
        -0.5,0.5,-0.5,  0.0f,  1.0f,  0.0f, 1,1,1,
         0.5,0.5,0.5, 0.0f,  1.0f,  0.0f,1,1,1,
         0.5,0.5,-0.5, 0.0f,  1.0f,  0.0f,1,1,1,
    };

    My3DObject cube(cube_vertices, sizeof(cube_vertices));



    // Game loop
    // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);


    basic_shader.Use();
    // Get the uniform locations
    GLint objectColorLoc = glGetUniformLocation(basic_shader.Program, "objectColor");
    GLint modelLoc = glGetUniformLocation(basic_shader.Program, "model");
    GLint viewLoc = glGetUniformLocation(basic_shader.Program, "view");
    GLint projLoc = glGetUniformLocation(basic_shader.Program, "projection");

    while (!glfwWindowShouldClose(window))
    {
        // Calculate deltatime of current frame
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        int w, h;
        glfwGetFramebufferSize(window, &w, &h);

        // Define the viewport dimensions
        glViewport(0, 0, w, h);

        // Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
        glfwPollEvents();
        do_movement(deltaTime);

        // Clear the colorbuffer
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        static float x_angle = 0.0f;
        x_angle += deltaTime;

        // Create camera transformations
        glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        glm::mat4 projection = glm::perspective(60.0f * 3.14159f / 180.0f, float(w)/float(h), 0.1f, 100.0f);
        glm::mat4 model =
            glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, zDist)) *
            glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f, 1.0f, 0.0f)) *
            glm::rotate(glm::mat4(1.0f), x_angle, glm::vec3(1.0f, 0.0f, 0.0f)) *
            glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f));

        // Use cooresponding shader when setting uniforms/drawing objects
        glUniform3f(objectColorLoc, 1.0f, 1.0f, 1.0f);

        // Pass the matrices to the shader
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

        cube.draw();

        glBindVertexArray(0);

        // Swap the screen buffers
        glfwSwapBuffers(window);
    }

    // Terminate GLFW, clearing any resources allocated by GLFW.
    glfwTerminate();
    return 0;
}

// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
    if (key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS)
            keys[key] = true;
        else if (action == GLFW_RELEASE)
            keys[key] = false;
    }
}

void do_movement(GLfloat delta)
{
    // Camera controls
    if (keys[GLFW_KEY_W])
    {
        zDist += delta;
    }
    if (keys[GLFW_KEY_S])
    {
        zDist -= delta;
    }
    if (keys[GLFW_KEY_A])
    {
        angle -= delta;
    }
    if (keys[GLFW_KEY_D])
    {
        angle += delta;
    }
}

bool firstMouse = true;
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    GLfloat xoffset = xpos - lastX;
    // Reversed since y-coordinates go from bottom to left
    GLfloat yoffset = lastY - ypos;  
    //glfwSetCursorPos(window, WIDTH/2, HEIGHT/2);

    lastX = WIDTH / 2;// xpos;
    lastY = HEIGHT / 2; // ypos;

}

