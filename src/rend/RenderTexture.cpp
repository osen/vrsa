#include "RenderTexture.h"
#include "Context.h"
#include "Exception.h"

namespace rend
{

RenderTexture::~RenderTexture()
{
  GLuint delId = id;
  glDeleteTextures(1, &delId);
  context->pollForError();

  delId = fboId;
  glDeleteFramebuffers(1, &delId); 
  context->pollForError();

  delId = rboId;
  glDeleteRenderbuffers(1, &delId); 
  context->pollForError();
}

GLuint RenderTexture::getId()
{
  if(dirty)
  {
    glBindFramebuffer(GL_FRAMEBUFFER, fboId);

    glBindTexture(GL_TEXTURE_2D, id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, size.x, size.y, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, id, 0);
    glBindTexture(GL_TEXTURE_2D, 0);

    glBindRenderbuffer(GL_RENDERBUFFER, rboId);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, size.x, size.y);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rboId);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
      throw Exception("FrameBuffer is not complete");
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    dirty = false;
  }

  return id;
}

ivec2 RenderTexture::getSize() const
{
  return size;
}

int RenderTexture::getWidth() const
{
  return size.x;
}

int RenderTexture::getHeight() const
{
  return size.y;
}

void RenderTexture::setSize(unsigned int width, unsigned int height)
{
  dirty = true;
  size = ivec2(width, height);
}

}
