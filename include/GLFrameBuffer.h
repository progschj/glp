/*
 * GLP OpenGL Wrappers - Copyright (c) 2012 Jakob Progsch
 *
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 *    1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 *
 *    2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 *
 *    3. This notice may not be removed or altered from any source
 *    distribution.
 */

#ifndef GL_FBO_H
#define GL_FBO_H

#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>

#include <boost/utility.hpp>

#include "GLTexture.h"
namespace glp {
    
class Renderbuffer : boost::noncopyable  {
public:
    Renderbuffer(GLint f, GLsizei width, GLsizei height)
        : format(f)
    {
        GLP_CHECKED_CALL(glGenRenderbuffers(1, &rbf);)
        GLP_CHECKED_CALL(glBindRenderbuffer(GL_RENDERBUFFER, rbf);)
        GLP_CHECKED_CALL(glRenderbufferStorage(GL_RENDERBUFFER, format, width, height);)
    }

    GLenum getFormat() const
    {
        return format;
    }

    operator GLuint() const
    {
        return rbf;
    }
    
    ~Renderbuffer()
    {
        GLP_CHECKED_CALL(glDeleteRenderbuffers(1, &rbf);)
    }
private:
    GLenum format;
    GLuint rbf;
};

class RenderbufferMultisample : boost::noncopyable {
public:
    RenderbufferMultisample(GLint f, GLsizei width, GLsizei height, GLsizei s)
        : format(f), samples(s)
    {
        GLP_CHECKED_CALL(glGenRenderbuffers(1, &rbf);)
        GLP_CHECKED_CALL(glBindRenderbuffer(GL_RENDERBUFFER, rbf);)
        GLP_CHECKED_CALL(glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples, format, width, height);)
    }

    GLenum getFormat() const
    {
        return format;
    }
    
    GLsizei getSamples() const
    {
        return samples;
    }

    operator GLuint() const
    {
        return rbf;
    }
    
    ~RenderbufferMultisample()
    {
        GLP_CHECKED_CALL(glDeleteRenderbuffers(1, &rbf);)
    }
private:
    GLenum format;
    GLsizei samples;
    GLuint rbf;
};

class FramebufferObject : boost::noncopyable  {
public:
    FramebufferObject()
    {
        GLP_CHECKED_CALL(glGenFramebuffers(1, &fbo);)
    }
    
    void attachColor(GLuint i, const Texture2D &tex)
    {
        GLP_CHECKED_CALL(glBindFramebuffer(GL_FRAMEBUFFER, fbo);)
        GLP_CHECKED_CALL(glFramebufferTexture2D(GL_FRAMEBUFFER, uint2attachment(i), GL_TEXTURE_2D, tex, 0);)
        GLP_CHECKED_CALL(glBindFramebuffer(GL_FRAMEBUFFER, 0);)
    }
    
    void attachDepth(const Texture2D &tex)
    {
        GLenum format = tex.getFormat();
        if(format == GL_DEPTH_COMPONENT ||
           format == GL_DEPTH_COMPONENT16 ||
           format == GL_DEPTH_COMPONENT24 ||
           format == GL_DEPTH_COMPONENT32 ||
           format == GL_DEPTH_COMPONENT32F)
        {
            GLP_CHECKED_CALL(glBindFramebuffer(GL_FRAMEBUFFER, fbo);)
            GLP_CHECKED_CALL(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, tex, 0);)
            GLP_CHECKED_CALL(glBindFramebuffer(GL_FRAMEBUFFER, 0);)
        }
    }

    void attachColor(GLuint i, const Renderbuffer &rbf)
    {
        GLP_CHECKED_CALL(glBindFramebuffer(GL_FRAMEBUFFER, fbo);)
        GLP_CHECKED_CALL(glFramebufferRenderbuffer(GL_FRAMEBUFFER, uint2attachment(i), GL_RENDERBUFFER, rbf);)
        GLP_CHECKED_CALL(glBindFramebuffer(GL_FRAMEBUFFER, 0);)
    }
    
    void attachDepth(const Renderbuffer &rbf)
    {
        GLenum format = rbf.getFormat();
        if(format == GL_DEPTH_COMPONENT ||
           format == GL_DEPTH_COMPONENT16 ||
           format == GL_DEPTH_COMPONENT24 ||
           format == GL_DEPTH_COMPONENT32 ||
           format == GL_DEPTH_COMPONENT32F)
        {
            GLP_CHECKED_CALL(glBindFramebuffer(GL_FRAMEBUFFER, fbo);)
            GLP_CHECKED_CALL(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbf);)
            GLP_CHECKED_CALL(glBindFramebuffer(GL_FRAMEBUFFER, 0);)
        }
        else if(format == GL_DEPTH_STENCIL ||
                format == GL_DEPTH24_STENCIL8 ||
                format == GL_DEPTH32F_STENCIL8)
        {
            GLP_CHECKED_CALL(glBindFramebuffer(GL_FRAMEBUFFER, fbo);)
            GLP_CHECKED_CALL(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbf);)
            GLP_CHECKED_CALL(glBindFramebuffer(GL_FRAMEBUFFER, 0);)
        }
    }
    
    void attachColor(GLuint i, const Texture2DMultisample &tex)
    {
        GLP_CHECKED_CALL(glBindFramebuffer(GL_FRAMEBUFFER, fbo);)
        GLP_CHECKED_CALL(glFramebufferTexture2D(GL_FRAMEBUFFER, uint2attachment(i), GL_TEXTURE_2D_MULTISAMPLE, tex, 0);)
        GLP_CHECKED_CALL(glBindFramebuffer(GL_FRAMEBUFFER, 0);)
    }
    
    void attachDepth(const Texture2DMultisample &tex)
    {
        GLenum format = tex.getFormat();
        if(format == GL_DEPTH_COMPONENT ||
           format == GL_DEPTH_COMPONENT16 ||
           format == GL_DEPTH_COMPONENT24 ||
           format == GL_DEPTH_COMPONENT32 ||
           format == GL_DEPTH_COMPONENT32F)
        {
            GLP_CHECKED_CALL(glBindFramebuffer(GL_FRAMEBUFFER, fbo);)
            GLP_CHECKED_CALL(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D_MULTISAMPLE, tex, 0);)
            GLP_CHECKED_CALL(glBindFramebuffer(GL_FRAMEBUFFER, 0);)
        }
    }

    void attachColor(GLuint i, const RenderbufferMultisample &rbf)
    {
        GLP_CHECKED_CALL(glBindFramebuffer(GL_FRAMEBUFFER, fbo);)
        GLP_CHECKED_CALL(glFramebufferRenderbuffer(GL_FRAMEBUFFER, uint2attachment(i), GL_RENDERBUFFER, rbf);)
        GLP_CHECKED_CALL(glBindFramebuffer(GL_FRAMEBUFFER, 0);)
    }
    
    void attachDepth(const RenderbufferMultisample &rbf)
    {
        GLenum format = rbf.getFormat();
        if(format == GL_DEPTH_COMPONENT ||
           format == GL_DEPTH_COMPONENT16 ||
           format == GL_DEPTH_COMPONENT24 ||
           format == GL_DEPTH_COMPONENT32 ||
           format == GL_DEPTH_COMPONENT32F)
        {
            GLP_CHECKED_CALL(glBindFramebuffer(GL_FRAMEBUFFER, fbo);)
            GLP_CHECKED_CALL(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbf);)
            GLP_CHECKED_CALL(glBindFramebuffer(GL_FRAMEBUFFER, 0);)
        }
        else if(format == GL_DEPTH_STENCIL ||
                format == GL_DEPTH24_STENCIL8 ||
                format == GL_DEPTH32F_STENCIL8)
        {
            GLP_CHECKED_CALL(glBindFramebuffer(GL_FRAMEBUFFER, fbo);)
            GLP_CHECKED_CALL(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbf);)
            GLP_CHECKED_CALL(glBindFramebuffer(GL_FRAMEBUFFER, 0);)
        }
    }

    void bind()
    {
        GLP_CHECKED_CALL(glBindFramebuffer(GL_FRAMEBUFFER, fbo);)
    }
    
    void bind(GLuint a0)
    {
        GLP_CHECKED_CALL(glBindFramebuffer(GL_FRAMEBUFFER, fbo);)
        GLenum buff[1] = {uint2attachment(a0)};
        GLP_CHECKED_CALL(glDrawBuffers(1, buff);)
    }

    void bind(GLuint a0, GLuint a1)
    {
        GLP_CHECKED_CALL(glBindFramebuffer(GL_FRAMEBUFFER, fbo);)
        GLenum buff[2] = {uint2attachment(a0), uint2attachment(a1)};
        GLP_CHECKED_CALL(glDrawBuffers(2, buff);)
    }

    void bind(GLuint a0, GLuint a1, GLuint a2)
    {
        GLP_CHECKED_CALL(glBindFramebuffer(GL_FRAMEBUFFER, fbo);)
        GLenum buff[3] = {uint2attachment(a0), uint2attachment(a1), uint2attachment(a2)};
        GLP_CHECKED_CALL(glDrawBuffers(3, buff);)
    }
    
    void bind(GLuint a0, GLuint a1, GLuint a2, GLuint a3)
    {
        GLP_CHECKED_CALL(glBindFramebuffer(GL_FRAMEBUFFER, fbo);)
        GLenum buff[4] = {uint2attachment(a0), uint2attachment(a1), uint2attachment(a2), uint2attachment(a3)};
        GLP_CHECKED_CALL(glDrawBuffers(4, buff);)
    }
    
    void unbind()
    {
        GLP_CHECKED_CALL(glBindFramebuffer(GL_FRAMEBUFFER, 0);)
        GLenum buff[1] = {GL_BACK_LEFT};
        GLP_CHECKED_CALL(glDrawBuffers(1, buff);)
    }
    
    ~FramebufferObject()
    {
        GLP_CHECKED_CALL(glDeleteFramebuffers(1, &fbo);)
    }
private:
    GLuint fbo;
};

}

#endif
