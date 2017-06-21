
#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include "esUtil.h"
#include "Texture2D.h"

typedef struct
{
   // Handle to a program object
   GLuint programObject;

    GLint position;
    GLint texcoord;
   // Sampler location
   GLint samplerLoc;

   // Texture handle
   GLuint textureId;

} UserData;

///
// Create a simple 2x2 texture image with four different colors
//
GLuint CreateSimpleTexture2D( )
{
   // Texture object handle
   GLuint textureId =0;

   // 2x2 Image, 3 bytes per pixel (R, G, B)
   GLubyte pixels[4 * 3] =
   {
      255,   0,   0, // Red
        0, 255,   0, // Green
        0,   0, 255, // Blue
      255, 255,   0  // Yellow
   };

   // Use tightly packed data
   glPixelStorei ( GL_UNPACK_ALIGNMENT, 1 );

   // Generate a texture object
   glGenTextures ( 1, &textureId );

   // Bind the texture object
   glBindTexture ( GL_TEXTURE_2D, textureId );

   // Load the texture
   glTexImage2D ( GL_TEXTURE_2D, 0, GL_RGB, 2, 2, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels );

   // Set the filtering mode
   glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
   glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );

   return textureId;

}


///
// Initialize the shader and program object
//
int Init ( ESContext *esContext )
{
    esContext->userData = malloc ( sizeof ( UserData ) );
    UserData *userData = (UserData *)esContext->userData;
   char vShaderStr[] =
      "#version 300 es                            \n"
      "in vec4 a_position;   \n"
      "in vec2 a_texCoord;   \n"
      "out vec2 v_texCoord;                       \n"
      "void main()                                \n"
      "{                                          \n"
      "   gl_Position = a_position;               \n"
      "   v_texCoord = a_texCoord;                \n"
      "}                                          \n";

   char fShaderStr[] =
      "#version 300 es                                     \n"
      "precision mediump float;                            \n"
      "in vec2 v_texCoord;                                 \n"
      "out vec4 outColor;             \n"
      "uniform sampler2D s_texture;                        \n"
      "void main()                                         \n"
      "{                                                   \n"
      "  outColor = texture( s_texture, v_texCoord );      \n"
      "}                                                   \n";

   // Load the shaders and get a linked program object
   userData->programObject = esLoadProgram ( vShaderStr, fShaderStr );

    userData->position = glGetAttribLocation(userData->programObject, "a_position");
    userData->texcoord = glGetAttribLocation(userData->programObject, "a_texCoord");
   // Get the sampler location
   userData->samplerLoc = glGetUniformLocation ( userData->programObject, "s_texture" );
    CheckGLError("Reticle program");
   // Load the texture
   userData->textureId = CreateSimpleTexture2D ();

   glClearColor ( 1.0f, 1.0f, 1.0f, 0.0f );
    CheckGLError("Reticle program");
   return TRUE;
}

///
// Draw a triangle using the shader pair created in Init()
//
void Draw ( ESContext *esContext )
{
   UserData *userData = (UserData *)esContext->userData;
   GLfloat vVertices[] = {
           -0.5f,  0.5f, 0.0f, 0.0f,  0.0f,
           -0.5f, -0.5f, 0.0f, 0.0f,  1.0f,
           0.5f, -0.5f, 0.0f, 1.0f,  1.0f,
           0.5f,  0.5f, 0.0f, 1.0f,  0.0f
   };
   GLushort indices[] = { 0, 1, 2, 0, 2, 3 };

   // Set the viewport
//   glViewport ( 0, 0, esContext->width, esContext->height );
//    glViewport ( 0, 0, 2048, 2048 );

   // Clear the color buffer
//   glClear ( GL_COLOR_BUFFER_BIT );

   // Use the program object
   glUseProgram ( userData->programObject );

   // Load the vertex position
    glEnableVertexAttribArray ( userData->position );
   glVertexAttribPointer ( userData->position, 3, GL_FLOAT, GL_FALSE, 5 * sizeof ( GLfloat ), vVertices );
   // Load the texture coordinate
    glEnableVertexAttribArray ( userData->texcoord );
   glVertexAttribPointer ( userData->texcoord, 2, GL_FLOAT, GL_FALSE, 5 * sizeof ( GLfloat ), &vVertices[3] );


   // Bind the texture
   glActiveTexture ( GL_TEXTURE0 );
   glBindTexture ( GL_TEXTURE_2D, userData->textureId );

   // Set the sampler texture unit to 0
   glUniform1i ( userData->samplerLoc, 0 );

   glDrawElements ( GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, indices );
//    glDrawArrays(GL_TRIANGLES, 0, 4);
    glDisableVertexAttribArray(userData->position);
    glDisableVertexAttribArray(userData->texcoord);

//   pthread_t seft = pthread_self();
//   LOGE( "draw thread id=%u", seft  );

}

///
// Cleanup
//
void ShutDown ( ESContext *esContext )
{
   UserData *userData = (UserData *)esContext->userData;

   // Delete texture object
   glDeleteTextures ( 1, &userData->textureId );

   // Delete program object
   glDeleteProgram ( userData->programObject );
}
