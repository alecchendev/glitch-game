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
#include <glitch/player.h>

#include <iostream>
#include <vector>
#include <algorithm>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
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

// camera mode
enum class CameraMode {
    FirstPerson,
    ThirdPerson
};
CameraMode camera_mode = CameraMode::ThirdPerson;

// camera
Camera camera(glm::vec3(0.0f, 0.5f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// create player
Player player(
    glm::vec3(0.0f, 0.0f, -1.0f),
    glm::vec3(0.0f, 0.5f, 3.0f),
    glm::vec3(0.7f, 0.7f, 0.7f)
);

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
    glfwSetKeyCallback(window, key_callback);

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

    // Create blocks

    // player block
    gfx::TextureBlock player_block(
        player.position(),
        player.hurtboxSize()
    );
    gfx::VAO player_vao;
    player_vao.initFromBlock(player_block);


    // sample texture block
    gfx::TextureBlock sample_cube(
        glm::vec3(-0.5f, 0.5f, -1.0f),
        glm::vec3(1.0f, 1.0f, 1.0f)
    );
    gfx::VAO vao;
    vao.initFromBlock(sample_cube);

    // solid color cube
    float length = 0.8f;
    gfx::SolidColorBlock orange_cube(
        glm::vec3(-2.0f, 0.0f, 0.0f),
        glm::vec3(length, length, length)
    );
    gfx::VAO orange_vao;
    orange_vao.initFromBlock(orange_cube);

    // ground
    float ground_length = 5.0f;
    gfx::SolidColorBlock ground_block(
        glm::vec3(-ground_length/2, -ground_length/5, -ground_length/2),
        glm::vec3(ground_length, ground_length/5, ground_length)
    );
    gfx::VAO ground_vao;
    ground_vao.initFromBlock(ground_block);

    // purple
    float purple_length = 1.0f;
    gfx::SolidColorBlock purple_block(
        glm::vec3(0.0f, 0.0f, 10.0f),
        glm::vec3(purple_length, purple_length, purple_length)
    );
    gfx::VAO purple_vao;
    purple_vao.initFromBlock(purple_block);

    // green
    float green_length = 1.0f;
    gfx::SolidColorBlock green_block(
        glm::vec3(-10.0f, 0.0f, 0.0f),
        glm::vec3(green_length, green_length, green_length)
    );
    gfx::VAO green_vao;
    green_vao.initFromBlock(green_block);

    // blue
    float blue_length = 1.0f;
    gfx::SolidColorBlock blue_block(
        glm::vec3(10.0f, 0.0f, 0.0f),
        glm::vec3(blue_length, blue_length, blue_length)
    );
    gfx::VAO blue_vao;
    blue_vao.initFromBlock(blue_block);

    // // add blocks to vector to render
    // std::vector<gfx::VAO&> world_objects;
    // world_objects.push_back(orange_vao);

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

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        // --------------------
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input + game logic
        // ------------------
        processInput(window);
        player_block.setPosition(player.position());

        // render
        // ------
        glClearColor(BG_COL.x, BG_COL.y, BG_COL.z, BG_COL.w);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 

        // bind textures on corresponding texture units
        container_tx.activeBindTexture(GL_TEXTURE0);
        awesomeface_tx.activeBindTexture(GL_TEXTURE1);

        // pass projection matrix to shader (note that in this case it could change every frame)
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

        // camera/view transformation
        glm::mat4 view = camera.GetViewMatrix();

        // render blocks
        ourShader.use();
        ourShader.setMat4("projection", projection);
        ourShader.setMat4("view", view);

        // don't draw player if in first person mode
        if (camera_mode == CameraMode::ThirdPerson) {
            player_vao.bind();
            glm::mat4 player_model = glm::mat4(1.0f);
            player_model = glm::translate(player_model, player_block.position());
            player_model = glm::rotate(player_model, -player.yaw(), glm::vec3(0.0f, 1.0f, 0.0f));
            player_model = glm::translate(player_model, - 0.5f * player_block.size());
            ourShader.setMat4("model", player_model);
            vao.drawElements(36);
        }

        // Texture block
        vao.bind();
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, sample_cube.position());
        ourShader.setMat4("model", model);
        vao.drawElements(36);

        // Config solid color shader
        solidShader.use();
        solidShader.setMat4("projection", projection);
        solidShader.setMat4("view", view);

        // Set color and draw blocks
        solidShader.setVec4("color", glm::vec4(1.0f, 0.5f, 0.2f, 1.0f));
        orange_vao.bind();
        glm::mat4 orange_model = glm::mat4(1.0f);
        orange_model = glm::translate(orange_model, orange_cube.position());
        solidShader.setMat4("model", orange_model);
        orange_vao.drawElements(36);

        solidShader.setVec4("color", glm::vec4(0.8f, 0.8f, 0.8f, 1.0f));
        ground_vao.bind();
        glm::mat4 ground_model = glm::mat4(1.0f);
        ground_model = glm::translate(ground_model, ground_block.position());
        solidShader.setMat4("model", ground_model);
        ground_vao.drawElements(36);

        solidShader.setVec4("color", glm::vec4(0.8f, 0.0f, 0.8f, 1.0f));
        purple_vao.bind();
        glm::mat4 purple_model = glm::mat4(1.0f);
        purple_model = glm::translate(purple_model, purple_block.position());
        solidShader.setMat4("model", purple_model);
        purple_vao.drawElements(36);

        solidShader.setVec4("color", glm::vec4(0.0f, 0.8f, 0.5f, 1.0f));
        green_vao.bind();
        glm::mat4 green_model = glm::mat4(1.0f);
        green_model = glm::translate(green_model, green_block.position());
        solidShader.setMat4("model", green_model);
        green_vao.drawElements(36);

        solidShader.setVec4("color", glm::vec4(0.0f, 0.2f, 0.8f, 1.0f));
        blue_vao.bind();
        glm::mat4 blue_model = glm::mat4(1.0f);
        blue_model = glm::translate(blue_model, blue_block.position());
        solidShader.setMat4("model", blue_model);
        blue_vao.drawElements(36);

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

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    // toggle camera mode
    if (key == GLFW_KEY_C && action == GLFW_PRESS) {
        if (camera_mode == CameraMode::ThirdPerson) {
            camera_mode = CameraMode::FirstPerson;
            camera.followFront(player.position());
            camera.ProcessMouseMovement(0, 0);
        } else if (camera_mode == CameraMode::FirstPerson) {
            camera_mode = CameraMode::ThirdPerson;
            camera.followBehind(player.position(), player.front());
        }
    }
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }

    if (camera_mode == CameraMode::ThirdPerson) {
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
            player.move(PlayerMovement::Forward, deltaTime);
            camera.followBehind(player.position(), player.front());
        }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
            player.move(PlayerMovement::Backward, deltaTime);
            camera.followBehind(player.position(), player.front());
        }
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
            player.move(PlayerMovement::Left, deltaTime);
            camera.followBehind(player.position(), player.front());
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
            player.move(PlayerMovement::Right, deltaTime);
            camera.followBehind(player.position(), player.front());
        }
    } else if (camera_mode == CameraMode::FirstPerson) {
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
            player.move(PlayerMovement::Forward, deltaTime);
            camera.followFront(player.position());
        }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
            player.move(PlayerMovement::Backward, deltaTime);
            camera.followFront(player.position());
        }
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
            player.move(PlayerMovement::Left, deltaTime);
            camera.followFront(player.position());
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
            player.move(PlayerMovement::Right, deltaTime);
            camera.followFront(player.position());
        }
    }
    
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

    if (camera_mode == CameraMode::FirstPerson) {
        camera.ProcessMouseMovement(xoffset, yoffset);
        player.turn(camera.Yaw);
    } else if (camera_mode == CameraMode::ThirdPerson) {
        // do nothing
        camera.ProcessMouseMovement(xoffset, 0);
        player.turn(camera.Yaw);
        camera.followBehind(player.position(), player.front());
    }
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}
