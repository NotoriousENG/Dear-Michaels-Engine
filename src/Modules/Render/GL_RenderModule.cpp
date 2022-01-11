#include "GL_RenderModule.h"

#include <stdio.h>
#include <glad/glad.h>
#include <stdlib.h>

#include <string>

void GL_RenderModule::Init(void* proc, int w, int h)
{
    this->w = w;
    this->h = h;

    // Check OpenGL properties
    printf("OpenGL loaded\n");
    gladLoadGLLoader((GLADloadproc) proc);
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

    glViewport(0, 0, w, h);
    glClearColor(0.5f, 1.0f, 0.85f, 1.0f);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_DEPTH_TEST);

    // generate frame buffer
    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

    // color attatchment texture
    glGenTextures(1, &textureColorbuffer);
    glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1920, 1080, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);

    // create a renderbuffer object for depth and stencil attachment (we won't be sampling these)
    unsigned int rbo;
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 1920, 1080); // use a single renderbuffer object for both a depth AND stencil buffer.
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo); // now actually attach it
    // now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        printf("ERROR::FRAMEBUFFER:: Framebuffer is not complete!\n");
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void GL_RenderModule::Update()
{   
    // bind to framebuffer and draw scene as we normally would to color texture
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    glEnable(GL_DEPTH_TEST); // enable depth testing (is disabled for rendering screen-space quad)

    // make sure we clear the framebuffer's content
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // render objects here
    //
    // ----------------------------------------------------------

    // now bind back to default framebuffer and copy the output of our custom framebuffer,
    // any post processing can be done on the framebuffer before this step
    // this will get drawn over by the editor so our original framebuffer address is given to ImGui
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBlitNamedFramebuffer(framebuffer, 0, 0, 0, 1920, 1080, 0, 0, w, h, GL_COLOR_BUFFER_BIT, GL_NEAREST);
    
    // call winbdow update afterwards to swap buffers, get events, etc
}

void GL_RenderModule::Shutdown()
{
    // window handles destroying the openGL context
}

void GL_RenderModule::openglCallbackFunction(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
    (void)source; (void)type; (void)id;
    (void)severity; (void)length; (void)userParam;
    fprintf(stderr, "%s\n", message);
    if (severity == GL_DEBUG_SEVERITY_HIGH) {
        fprintf(stderr, "Aborting...\n");
        abort();
    }
}

void GL_RenderModule::framebuffer_size_callback(int width, int height)
{
    w = width;
    h = height;
    glViewport(0, 0, width, height);
}

unsigned int GL_RenderModule::GetTextureColorBuffer()
{
    return textureColorbuffer;
}

//void GL_RenderModule::DrawMesh(rm::Mesh mesh, rm::Material material, rm::Shader shader)
//{
//    if (shader == material->Shader.get())
//    {
//        material->SetUniforms();
//
//        // bind appropriate textures
//        unsigned int diffuseNr = 1;
//        unsigned int specularNr = 1;
//        unsigned int normalNr = 1;
//        unsigned int heightNr = 1;
//        for (unsigned int i = 0; i < material->Textures.size(); i++)
//        {
//            glActiveTexture(GL_TEXTURE0 + i); // active proper texture unit before binding
//            // retrieve texture number (the N in diffuse_textureN)
//            std::string number;
//            std::string name = material->Textures[i]->type;
//            if (name == "texture_diffuse")
//                number = std::to_string(diffuseNr++);
//            else if (name == "texture_specular")
//                number = std::to_string(specularNr++); // transfer unsigned int to stream
//            else if (name == "texture_normal")
//                number = std::to_string(normalNr++); // transfer unsigned int to stream
//            else if (name == "texture_height")
//                number = std::to_string(heightNr++); // transfer unsigned int to stream
//
//            // now set the sampler to the correct texture unit
//            glUniform1i(glGetUniformLocation(shader->ID, (name + number).c_str()), i);
//            // and finally bind the texture
//            glBindTexture(GL_TEXTURE_2D, material->Textures[i]->ID);
//        }
//
//        if (material->Textures.size() == 0)
//        {
//            shader->SetBool("useTexture", false);
//            glBindTexture(GL_TEXTURE_2D, 0);
//        }
//    }
//
//    // draw mesh
//    glBindVertexArray(VAO);
//    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
//    glBindVertexArray(0);
//
//    // always good practice to set everything back to defaults once configured.
//    glActiveTexture(GL_TEXTURE0);
//}