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

GLuint RenderTexture::getTexId()
{
  return id;
}

void RenderTexture::clear()
{
  glBindFramebuffer(GL_FRAMEBUFFER, getId());
  context->pollForError();

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  context->pollForError();

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  context->pollForError();
}

GLuint RenderTexture::getId()
{
  if(dirty)
  {
    glBindFramebuffer(GL_FRAMEBUFFER, fboId);
    context->pollForError();

    glBindTexture(GL_TEXTURE_2D, id); context->pollForError();
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, size.x, size.y, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    context->pollForError();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    context->pollForError();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    context->pollForError();
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, id, 0);
    context->pollForError();
    glBindTexture(GL_TEXTURE_2D, 0);
    context->pollForError();

    glBindRenderbuffer(GL_RENDERBUFFER, rboId);
    context->pollForError();
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, size.x, size.y);
    context->pollForError();
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rboId);
    context->pollForError();
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    context->pollForError();

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
      throw Exception("FrameBuffer is not complete");
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    context->pollForError();
    dirty = false;
  }

  return fboId;
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
