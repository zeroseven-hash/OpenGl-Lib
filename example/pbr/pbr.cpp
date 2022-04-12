#include<glad/glad.h>
#include <GLFW/glfw3.h>

#include"VertexArrayBuffer.h"
#include"Renderer.h"
#include"Texture.h"
#include"glm/glm.hpp"

using namespace OGL;
void RenderSphere();
int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;
    glfwWindowHint(GLFW_SAMPLES, 4);
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


    glEnable(GL_DEPTH_TEST);
    Ref<Shader> shader = Shader::Create("../../example/pbr/pbr.glsl");
    glm::vec3 light_pos[4] =
    {
        glm::vec3(-10.0f,10.0f,10.0f),
        glm::vec3(10.0f,10.0f,10.0f),
        glm::vec3(-10.0f,-10.0f,10.0f),
        glm::vec3(10.0f,-10.0f,10.0f),
    };
    glm::vec3 light_col[4] =
    {
        glm::vec3(300.0f, 300.0f, 300.0f),
        glm::vec3(300.0f, 300.0f, 300.0f),
        glm::vec3(300.0f, 300.0f, 300.0f),
        glm::vec3(300.0f, 300.0f, 300.0f)
    };

    int row = 7;
    int col = 7;
    float spaceing = 2.5f;

    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)650/ (float)480, 0.1f, 100.0f);
    glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, 23.0f), glm::vec3(0.0f, 0.0f, 2.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 model(1.0f);
    shader->Bind();
    shader->SetMat4("u_projection", projection);
    shader->SetMat4("u_view", view);
    shader->SetFloat3("u_camera_pos", glm::vec3(0.0f, 0.0f, 23.0f));

    //shader->SetMat4("view", view);
    /* Loop until the user closes the window */
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        if (shader->Compile("../../example/pbr/pbr.glsl"))
        {
            shader->Reset("../../example/pbr/pbr.glsl");
            shader->Bind();
            shader->SetMat4("u_projection", projection);
            shader->SetMat4("u_view", view);
            shader->SetFloat3("u_camera_pos", glm::vec3(0.0f, 0.0f, 23.0f));
            for (int i = 0;i < 4;i++) shader->SetFloat3("u_light_col[" + std::to_string(i) + "]", light_col[i]);
        }
        for (unsigned int i = 0; i < sizeof(light_pos) / sizeof(light_pos[0]); ++i)
        {
            glm::vec3 newPos = light_pos[i] + glm::vec3(sin(glfwGetTime() * 5.0) * 5.0, 0.0, 0.0);
            newPos = light_pos[i];
            shader->Bind();
            shader->SetFloat3("u_light_pos[" + std::to_string(i) + "]", newPos);

            model = glm::mat4(1.0f);
            model = glm::translate(model, newPos);
            model = glm::scale(model, glm::vec3(0.5f));
            shader->Bind();
            shader->SetMat4("model", model);
            RenderSphere();
        }
        
        glm::mat4 model = glm::mat4(1.0f);
        for (int r = 0;r < row; r++)
        {
            shader->SetFloat("metallic", (float)r / float(row));
            for (int c = 0;c < col;c++)
            {
                shader->SetFloat("roughness", glm::clamp((float)c / float(col), 0.05f, 1.0f));
                model = glm::mat4(1.0f);
                model = glm::mat4(1.0f);
                model = glm::translate(model, glm::vec3(
                    (c - (col / 2)) * spaceing,
                    (r - (row / 2)) * spaceing,
                    0.0f
                ));
                shader->Bind();
                shader->SetMat4("u_model", model);
                RenderSphere();
            }
        }


        

        ///* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}


Ref<VertexArrayBuffer> sphere=nullptr;
void RenderSphere()
{
    if (!sphere)
    {
        sphere = VertexArrayBuffer::Create();
        std::vector<glm::vec3> positions;
        std::vector<glm::vec2> uv;
        std::vector<glm::vec3> normals;
        std::vector<unsigned int> indices;

        const unsigned int X_segments = 64;
        const unsigned int Y_segments = 64;
        const float PI = 3.14159265359;
        for (unsigned int x = 0;x <= X_segments;x++)
        {
            for (unsigned int y = 0;y <= Y_segments;y++)
            {
                float x_segment = (float)x / (float)X_segments;
                float y_segment = (float)y / (float)Y_segments;

                float xpos = std::cos(x_segment * 2.0f * PI) * std::sin(y_segment * PI);
                float ypos = std::cos(y_segment * PI);
                float zpos = std::sin(x_segment * 2.0f * PI) * std::sin(y_segment * PI);

                positions.push_back(glm::vec3(xpos, ypos, zpos));
                uv.push_back(glm::vec2(x_segment, y_segment));
                normals.push_back(glm::vec3(xpos, ypos, zpos));
            }
        }

        bool odd_row = false;
        for (unsigned int y = 0; y < Y_segments; ++y)
        {
            if (!odd_row) // even rows: y == 0, y == 2; and so on
            {
                for (unsigned int x = 0; x <= X_segments; ++x)
                {
                    indices.push_back(y * (X_segments + 1) + x);
                    indices.push_back((y + 1) * (X_segments + 1) + x);
                }
            }
            else
            {
                for (int x = X_segments; x >= 0; --x)
                {
                    indices.push_back((y + 1) * (X_segments + 1) + x);
                    indices.push_back(y * (X_segments + 1) + x);
                }
            }
            odd_row = !odd_row;
        }

        std::vector<float> data;
        for (unsigned int i = 0; i < positions.size(); ++i)
        {
            data.push_back(positions[i].x);
            data.push_back(positions[i].y);
            data.push_back(positions[i].z);
            if (normals.size() > 0)
            {
                data.push_back(normals[i].x);
                data.push_back(normals[i].y);
                data.push_back(normals[i].z);
            }
            if (uv.size() > 0)
            {
                data.push_back(uv[i].x);
                data.push_back(uv[i].y);
            }
        }

        Ref<VertexBuffer> vbo = VertexBuffer::Create(data.data(), data.size() * sizeof(float));
        BufferLayout layout =
        {
            {"Position",OpenGLDataType::Float,3},
            {"Normal",OpenGLDataType::Float,3},
            {"TexCoords",OpenGLDataType::Float,2}
        };
        vbo->SetLayout(layout);

        Ref<IndexBuffer> ibo = IndexBuffer::Create(indices.data(), indices.size());
        sphere->SetIndexBuffer(ibo);
        sphere->SetVertexBuffer(vbo);
    }

    sphere->Bind();
    sphere->GetIndexBuffer()->Bind();
    glDrawElements(GL_TRIANGLE_STRIP, sphere->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, 0);
}