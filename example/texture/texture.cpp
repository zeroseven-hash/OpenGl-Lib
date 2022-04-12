#define GLFW_INCLUDE_NONE
#include<glad/glad.h>
#include <GLFW/glfw3.h>

#include"VertexArrayBuffer.h"
#include"Renderer.h"
#include"Texture.h"
#include"glm/glm.hpp"

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
   
    if (!window)
    {
        glfwTerminate();
        return -1;
    }
    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        return 0;
    }
    
    struct V
    {
        
        glm::vec4 pos_;
        glm::vec4 color_;
        glm::vec2 coords;
    };

    std::vector<V> vertices = {
        { glm::vec4{ -0.5, 0.5, -1.5, 1}, glm::vec4{0, 1, 0, 1},glm::vec2{0,1} },
        { glm::vec4{ -0.5, -0.5, -1.5, 1}, glm::vec4{0, 0, 1, 1},glm::vec2{0,0}},
        { glm::vec4{  0.5,  0.5, -1.5, 1}, glm::vec4{1, 0, 0, 1},glm::vec2{1,1} },
        { glm::vec4{0.5,-0.5,-1.5,1},glm::vec4{0.5,0.5,0.5,1},glm::vec2{1,0}}
    };
    OGL::Ref < OGL::VertexBuffer> vbo = OGL::VertexBuffer::Create(vertices.data(), sizeof(V) * vertices.size());
    OGL::BufferLayout layout =
    {
        {"a_Position",OGL::OpenGLDataType::Float,4},
        {"a_Color",OGL::OpenGLDataType::Float,4},
        {"a_Coords",OGL::OpenGLDataType::Float,2},
    };
    vbo->SetLayout(layout);
    std::vector<uint32_t> indices = { 0,1,2,1,3,2 };
    OGL::Ref<OGL::IndexBuffer>ibo = OGL::IndexBuffer::Create(indices.data(), 6);
    
    OGL::Ref<OGL::VertexArrayBuffer> vao = OGL::VertexArrayBuffer::Create();
    vao->SetVertexBuffer(vbo);
    vao->SetIndexBuffer(ibo);
    OGL::Ref<OGL::Texture> texture = OGL::Texture::Create("../../example/texture/Texture.bmp");
    
    OGL::Ref<OGL::Shader> shader = OGL::Shader::Create("../../example/texture/meshshader_line.glsl");
    shader->Bind();
    shader->SetInt1("tex", 0);
    
    glm::mat4 model = glm::rotate(glm::mat4(1.0f), glm::radians(30.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    glm::mat4 perpective = glm::perspective(glm::radians(60.0f), 640 / 480.0f, 1.0f, 500.0f);
    //glm::mat4 view=glm::lookAt(glm::vec3(0.0f, 0.0f, 1.5f), glm::vec3(0.0f, 0.0f, -2.0f), glm::vec3(0.0f, 1.0f,0.0f));
    
    shader->SetMat4("pers", perpective);
    //shader->SetMat4("view", view);
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);
        glActiveTexture(GL_TEXTURE0);
        texture->Bind();
        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        float radius = 1.0f;
        float camX = sin(glfwGetTime()) * radius;
        float camZ = cos(glfwGetTime()) * radius;
        glm::mat4 view;
        view = glm::lookAt(glm::vec3(0.0f, -0.3, 0.0f), glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, 1.0, -1.0));
        shader->Bind();
        shader->SetMat4("view", view);

        OGL::Renderer::Draw(*shader, vao, glm::mat4(1.0f));
        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}