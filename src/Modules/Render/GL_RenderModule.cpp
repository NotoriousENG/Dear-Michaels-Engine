#include "GL_RenderModule.h"

#include <stdio.h>
#include <glad/glad.h>
#include <stdlib.h>

#include <string>
#include <ResourceManagement/Mesh.h>
#include <ResourceManagement/Material.h>
#include <ResourceManagement/Shader.h>
#include <ResourceManagement/Texture.h>
#include <Scene/Scene.h>
#include <Scene/Components.h>
#include <Elements/Camera.h>
#include <Input/Input.h>

void GL_RenderModule::Init(void *proc, int w, int h)
{
    this->w = w;
    this->h = h;

    // Check OpenGL properties
    printf("OpenGL loaded\n");
    gladLoadGLLoader((GLADloadproc)proc);
    printf("Vendor:   %s\n", glGetString(GL_VENDOR));
    printf("Renderer: %s\n", glGetString(GL_RENDERER));
    printf("Version:  %s\n", glGetString(GL_VERSION));

    // Enable the debug callback
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(openglCallbackFunction, nullptr);
    glDebugMessageControl(
        GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, GL_FALSE);
    glDebugMessageControl(
        GL_DEBUG_SOURCE_API, GL_DEBUG_TYPE_ERROR, GL_DONT_CARE, 0, NULL, GL_TRUE);

    glEnable(GL_DEPTH_TEST);
    // glDisable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClearColor(0.25f, .25f, 0.25f, 1.0f);

    // generate frame buffer
    glGenFramebuffers(1, &framebuffer);

    // generate texture
    glGenTextures(1, &textureColorbuffer);

    // create a renderbuffer object for depth and stencil attachment (we won't be sampling these)
    glGenRenderbuffers(1, &rbo);

    FrameBufferResizeCallback(w, h);

    this->SpriteShader = rm::ResourceManager::Load<rm::Shader>("Assets/Shaders/Sprite.glsl");

    // Init Quad
    {
        // configure VAO/VBO
        unsigned int VBO;
        float vertices[] = {
            // pos      // tex
            -.5f, -.5f, 1.0f, 1.0f,
            .5f, -.5f, 0.0f, 1.0f,
            -.5f, .5f, 1.0f, 0.0f,

            -.5f, .5f, 1.0f, 0.0f,
            .5f, .5f, 0.0f, 0.0f,
            .5f, -.5f, 0.0f, 1.0f
        };

        glGenVertexArrays(1, &this->quadVAO);
        glGenBuffers(1, &VBO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glBindVertexArray(this->quadVAO);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }
}

void GL_RenderModule::Update()
{
    // bind to framebuffer and draw scene as we normally would to color texture
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    glEnable(GL_DEPTH_TEST); // enable depth testing (is disabled for rendering screen-space quad)

    // make sure we clear the framebuffer's content
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    { // render objects here
        auto view = Scene::Instance->registry.view<TransformComponent, StaticMeshComponent>();

        // use forward iterators and get only the components of interest
        for (auto entity : view)

        {
            auto [transform, mesh] = view.get<TransformComponent, StaticMeshComponent>(entity);

            auto shader = mesh.Model->meshes[0].material->Shader.get();

            shader->Use();
            shader->SetMatrix4("model", transform.transform);
            shader->SetMatrix4("view", Camera::Main.view);
            shader->SetMatrix4("projection", Camera::Main.projection);

            mesh.Model->Draw(shader);
        }
    }

    {
        auto view = Scene::Instance->registry.view<TransformComponent, SpriteComponent>();

        // use forward iterators and get only the components of interest
        for (auto entity : view)

        {
            auto [transform, sprite] = view.get<TransformComponent, SpriteComponent>(entity);

            auto shader = this->SpriteShader.get();

            shader->Use();
            shader->SetMatrix4("model", transform.transform);
            shader->SetMatrix4("view", Camera::Main.view);
            shader->SetMatrix4("projection", Camera::Main.projection);
            shader->SetVector3f("spriteColor", glm::vec3(1, 1, 1));

            glActiveTexture(GL_TEXTURE0);
            sprite.Sprite->Bind();

            glBindVertexArray(this->quadVAO);
            glDrawArrays(GL_TRIANGLES, 0, 6);
            glBindVertexArray(0);
        }
    }
    // ----------------------------------------------------------

    // now bind back to default framebuffer and copy the output of our custom framebuffer,
    // any post processing can be done on the framebuffer before this step
    // this will get drawn over by the editor so our original framebuffer address is given to ImGui
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
#ifndef EDITOR
    glBlitNamedFramebuffer(framebuffer, 0, 0, 0, w, h, 0, 0, w, h, GL_COLOR_BUFFER_BIT, GL_NEAREST);
#endif

    // call winbdow update afterwards to swap buffers, get events, etc
}

void GL_RenderModule::Shutdown()
{
    // window handles destroying the openGL context
}

void GL_RenderModule::openglCallbackFunction(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *userParam)
{
    (void)source;
    (void)type;
    (void)id;
    (void)severity;
    (void)length;
    (void)userParam;
    fprintf(stderr, "%s\n", message);
    if (severity == GL_DEBUG_SEVERITY_HIGH)
    {
        fprintf(stderr, "Aborting...\n");
        abort();
    }
}

unsigned int GL_RenderModule::GetTextureColorBuffer()
{
    return textureColorbuffer;
}

void GL_RenderModule::DrawMesh(rm::Mesh *mesh, rm::Shader *shader)
{
    if (shader == mesh->material->Shader.get())
    {
        mesh->material->SetUniforms();

        // bind appropriate textures
        unsigned int diffuseNr = 1;
        unsigned int specularNr = 1;
        unsigned int normalNr = 1;
        unsigned int heightNr = 1;
        for (unsigned int i = 0; i < mesh->material->Textures.size(); i++)
        {
            glActiveTexture(GL_TEXTURE0 + i); // active proper texture unit before binding
            // retrieve texture number (the N in diffuse_textureN)
            std::string number;
            std::string name = mesh->material->Textures[i]->type;
            if (name == "texture_diffuse")
                number = std::to_string(diffuseNr++);
            else if (name == "texture_specular")
                number = std::to_string(specularNr++); // transfer unsigned int to stream
            else if (name == "texture_normal")
                number = std::to_string(normalNr++); // transfer unsigned int to stream
            else if (name == "texture_height")
                number = std::to_string(heightNr++); // transfer unsigned int to stream

            // now set the sampler to the correct texture unit
            glUniform1i(glGetUniformLocation(shader->ID, (name + number).c_str()), i);
            // and finally bind the texture
            glBindTexture(GL_TEXTURE_2D, mesh->material->Textures[i]->ID);
        }

        if (mesh->material->Textures.size() == 0)
        {
            shader->SetBool("useTexture", false);
            glBindTexture(GL_TEXTURE_2D, 0);
        }
    }

    // draw mesh
    glBindVertexArray(mesh->VAO);
    glDrawElements(GL_TRIANGLES, mesh->indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    // always good practice to set everything back to defaults once configured.
    glActiveTexture(GL_TEXTURE0);
}

void GL_RenderModule::FrameBufferResizeCallback(int w, int h)
{
    this->w = w;
    this->h = h;
    glViewport(0, 0, w, h);

    Camera::Main.AspectRatio = (w / 800.0f) / (h / 800.0f);
    Camera::Main.UpdateCameraVectors();

    // Update GL Render Properties
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

    // color attatchment texture

    glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    // attatch it to the currently bound framebuffer object
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);

    // Renderbuffer begin:
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, w, h); // use a single renderbuffer object for both a depth AND stencil buffer.
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo); // now actually attach it

    // now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        printf("ERROR::FRAMEBUFFER:: Framebuffer is not complete!\n");
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
