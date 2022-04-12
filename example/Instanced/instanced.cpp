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
    };

    std::vector<V> vertices = {
        { glm::vec4{ -0.25, 0.25, 0.0f, 1}, glm::vec4{0, 1, 0, 1}},
        { glm::vec4{ -0.25, -0.25, 0.0f, 1}, glm::vec4{0, 0, 1, 1}},
        { glm::vec4{  0.25,  0.25, 0.0f, 1}, glm::vec4{1, 0, 0, 1}},
        { glm::vec4{0.25,-0.25,0.0f,1},glm::vec4{0.5,0.5,0.5,1}}
    };
    
    OGL::Ref < OGL::VertexBuffer> vbo = OGL::VertexBuffer::Create(vertices.data(), sizeof(V) * vertices.size());
    OGL::BufferLayout layout =
    {
        {"a_Position",OGL::OpenGLDataType::Float,4},
        {"a_Color",OGL::OpenGLDataType::Float,4},
    };

    std::vector<glm::mat4> models;
    auto m1 = glm::translate(glm::mat4(1.0f), glm::vec3(-0.5f, 0.5f, 0.0f));
    auto m2 = glm::translate(glm::mat4(1.0f), glm::vec3(0.5f, -0.5f, 0.0f));
    models.push_back(m1);
    models.push_back(m2);
    OGL::Ref<OGL::VertexBuffer> instanced_buffer = OGL::VertexBuffer::Create(models.data(), sizeof(glm::mat4) * 2);
    OGL::BufferLayout i_layout =
    {
        {"a_row1",OGL::OpenGLDataType::Float,4,1},
        {"a_row2",OGL::OpenGLDataType::Float,4,1},
        {"a_row3",OGL::OpenGLDataType::Float,4,1},
        {"a_row4",OGL::OpenGLDataType::Float,4,1},
    };
    instanced_buffer->SetLayout(i_layout);


    vbo->SetLayout(layout);
    std::vector<uint32_t> indices = { 0,1,2,1,3,2 };
    OGL::Ref<OGL::IndexBuffer>ibo = OGL::IndexBuffer::Create(indices.data(), 6);

    OGL::Ref<OGL::VertexArrayBuffer> vao = OGL::VertexArrayBuffer::Create();
    vao->SetVertexBuffer(vbo);
    vao->SetVertexBuffer(instanced_buffer, true);
    vao->SetIndexBuffer(ibo);
    OGL::Ref<OGL::Shader> shader = OGL::Shader::Create("../../example/instanced/instanced.glsl");

   

    
    
    //shader->SetMat4("view", view);
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);
        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        //OGL::Renderer::Draw(*shader, vao, glm::mat4(1.0f));
        shader->Bind();
        vao->Bind();
        glDrawElementsInstanced(GL_TRIANGLES, vao->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr, 2);

        ///* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}