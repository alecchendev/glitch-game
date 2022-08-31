#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glitch/shader.h>
#include <glitch/camera.h>
#include <glitch/graphics.h>

#include <iostream>
#include <vector>
#include <algorithm>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
const char* WINDOW_TITLE = "Glitch Game";
const glm::vec4 BG_COL = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

// shaders
const std::string VERTEX_SHADER_PATH = "src/shaders/vertex.glsl";
const std::string FRAGMENT_SHADER_SOLID_COLOR_PATH = "src/shaders/f_color.glsl";
const std::string FRAGMENT_SHADER_TEXTURE_PATH = "src/shaders/fragment.glsl";

// images/textures
const std::string AWESOMEFACE_IMAGE_PATH = "src/images/awesomeface.png";
const std::string CONTAINER_IMAGE_PATH = "src/images/container.jpg";

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
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
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, WINDOW_TITLE, NULL, NULL);
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
    // Shader ourShader(VERTEX_SHADER_PATH.c_str(), FRAGMENT_SHADER_PATH.c_str());
    Shader ourShader(VERTEX_SHADER_PATH.c_str(), FRAGMENT_SHADER_TEXTURE_PATH.c_str());
    Shader solidShader(VERTEX_SHADER_PATH.c_str(), FRAGMENT_SHADER_SOLID_COLOR_PATH.c_str());

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    // float vertices[] = {
    //     -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
    //      0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
    //      0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    //     -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,

    //     -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    //      0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    //      0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    //     -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,

    //     -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    //     -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    //     -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    //     -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    //      0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    //      0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    //      0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    //      0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    //     -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    //      0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
    //      0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    //     -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    //     -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    //      0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    //      0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    //     -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    // };

    // // element indices
    // unsigned int indices[] = {
    //     0, 1, 2,
    //     2, 3, 0,

    //     4, 5, 6,
    //     6, 7, 4,

    //     8, 9, 10,
    //     10, 11, 8,

    //     12, 13, 14,
    //     14, 15, 12,

    //     16, 17, 18,
    //     18, 19, 16,

    //     20, 21, 22,
    //     22, 23, 20,
    // };
    gfx::TextureBlock sample_cube(
        glm::vec3(-0.5f, -0.5f, -0.5f),
        glm::vec3(1.0f, 1.0f, 1.0f)
    );
    std::vector<float> vertices_vec = sample_cube.vertices();
    float vertices[gfx::N_CUBE_TEXTURE_VERTICES];
    std::copy(vertices_vec.begin(), vertices_vec.end(), vertices);
    std::vector<unsigned int> indices_vec = sample_cube.indices();
    unsigned int indices[gfx::N_CUBE_INDICES];
    std::copy(indices_vec.begin(), indices_vec.end(), indices);

    // world space positions of our cubes
    glm::vec3 cubePositions[] = {
        glm::vec3( 0.0f,  0.0f,  0.0f),
    };

    // create vertex array object
    gfx::VAO vao;
    vao.addVertexBuffer(sizeof(vertices), vertices, GL_STATIC_DRAW);
    vao.addElementBuffer(sizeof(indices), indices, GL_STATIC_DRAW);
    vao.addVertexAttribute(0, 3, 5, 0); // vertex positions
    vao.addVertexAttribute(1, 3, 5, 3); // texture coords


    // solid color cube
    float length = 0.8f;
    gfx::SolidColorBlock orange_cube(
        glm::vec3(-2.0f, 0.0f, 0.0f),
        glm::vec3(length, length, length)
    );

    // get vertex data
    std::vector<float> orange_vtx_vec = orange_cube.vertices();
    float orange_vertices[gfx::N_CUBE_TEXTURE_VERTICES];
    std::copy(orange_vtx_vec.begin(), orange_vtx_vec.end(), orange_vertices);
    std::vector<unsigned int> orange_idx_vec = orange_cube.indices();
    unsigned int orange_indices[gfx::N_CUBE_INDICES];
    std::copy(orange_idx_vec.begin(), orange_idx_vec.end(), orange_indices);

    // create vao from block
    gfx::VAO orange_vao;
    orange_vao.addVertexBuffer(sizeof(orange_vertices), orange_vertices, GL_STATIC_DRAW);
    orange_vao.addElementBuffer(sizeof(orange_indices), orange_indices, GL_STATIC_DRAW);
    orange_vao.addVertexAttribute(0, 3, 3, 0); // vertex positions





    // create a block, get vertex + index data from block, create vao
    // float length = 3.0f;
    // Block ground = Block(
    //     glm::vec3(-length/2, -length/4, -length/2),
    //     glm::vec3(length, length/4, length)
    // );
    // float ground_vertices[gfx::N_CUBE_SOLID_COLOR_VERTICES * 3];
    // unsigned int ground_indices[gfx::N_CUBE_INDICES * 3];
    // ground.fillVertices(ground_vertices);
    // ground.fillIndices(ground_indices);


    // load and create a texture 
    // -------------------------
    gfx::Texture container_tx(
        GL_TEXTURE_2D,
        {
            { GL_TEXTURE_WRAP_S, GL_REPEAT },
            { GL_TEXTURE_WRAP_T, GL_REPEAT },
            { GL_TEXTURE_MIN_FILTER, GL_LINEAR },
            { GL_TEXTURE_MAG_FILTER, GL_LINEAR },
        },
        CONTAINER_IMAGE_PATH,
        false
    );
    gfx::Texture awesomeface_tx(
        GL_TEXTURE_2D,
        {
            { GL_TEXTURE_WRAP_S, GL_REPEAT },
            { GL_TEXTURE_WRAP_T, GL_REPEAT },
            { GL_TEXTURE_MIN_FILTER, GL_LINEAR },
            { GL_TEXTURE_MAG_FILTER, GL_LINEAR },
        },
        AWESOMEFACE_IMAGE_PATH,
        true
    );

    // tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
    // -------------------------------------------------------------------------------------------
    ourShader.use();
    ourShader.setInt("texture1", 0);
    ourShader.setInt("texture2", 1);

    solidShader.use();
    solidShader.setVec4("color", glm::vec4(1.0f, 0.5f, 0.2f, 1.0f));


    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        // --------------------
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(BG_COL.x, BG_COL.y, BG_COL.z, BG_COL.w);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 

        // bind textures on corresponding texture units
        container_tx.activeBindTexture(GL_TEXTURE0);
        awesomeface_tx.activeBindTexture(GL_TEXTURE1);

        // activate shader
        ourShader.use();

        // pass projection matrix to shader (note that in this case it could change every frame)
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        ourShader.setMat4("projection", projection);

        // camera/view transformation
        glm::mat4 view = camera.GetViewMatrix();
        ourShader.setMat4("view", view);

        // render blocks
        vao.bind();
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, sample_cube.position());
        ourShader.setMat4("model", model);
        vao.drawElements(36);

        solidShader.use();
        solidShader.setMat4("projection", projection);
        solidShader.setMat4("view", view);

        orange_vao.bind();
        glm::mat4 orange_model = glm::mat4(1.0f);
        orange_model = glm::translate(orange_model, orange_cube.position());
        solidShader.setMat4("model", orange_model);
        orange_vao.drawElements(36);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    vao.deallocate();

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
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