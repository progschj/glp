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

#ifndef GL_TEXTURE_H
#define GL_TEXTURE_H

#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>

#include <boost/type_traits.hpp>
#include <boost/utility.hpp>

#include "TypeToGLConstant.h"
#include "GLCheckError.h"

namespace glp {
    
GLenum getDataFormat(GLint format)
{
    switch(format)
    {
        case 1:
        case GL_COMPRESSED_RED:
        case GL_RED: 
        case GL_R8:           
        case GL_R16:            
        case GL_R8_SNORM:           
        case GL_R16_SNORM:            
        case GL_R16F:            
        case GL_R32F:
        case GL_R8I:            
        case GL_R8UI:            
        case GL_R16I:            
        case GL_R16UI:            
        case GL_R32I:            
        case GL_R32UI:            
            return GL_RED;
        
        case GL_DEPTH_COMPONENT:
        case GL_DEPTH_COMPONENT16:
        case GL_DEPTH_COMPONENT24:
        case GL_DEPTH_COMPONENT32F:
            return GL_DEPTH_COMPONENT;

        case GL_DEPTH24_STENCIL8:
        case GL_DEPTH32F_STENCIL8:
            return GL_DEPTH_STENCIL;
        
        case 2:
        case GL_COMPRESSED_RG:
        case GL_RG:           
        case GL_RG8:           
        case GL_RG16:           
        case GL_RG8_SNORM:           
        case GL_RG16_SNORM:           
        case GL_RG16F:           
        case GL_RG32F:
        case GL_RG8I:            
        case GL_RG8UI:            
        case GL_RG16I:            
        case GL_RG16UI:            
        case GL_RG32I:            
        case GL_RG32UI:          
            return GL_RG;
   
        case 3:
        case GL_COMPRESSED_RGB:
        case GL_COMPRESSED_SRGB:
        case GL_R3_G3_B2:
        case GL_RGB:
        case GL_RGB16F:
        case GL_RGB32F:
        case GL_RGB4:
        case GL_RGB5:
        case GL_RGB8:
        case GL_RGB8_SNORM:
        case GL_RGB10:
        case GL_RGB12:
        case GL_RGB16:
        case GL_RGB16_SNORM:
        case GL_SRGB:
        case GL_SRGB8:
        case GL_R11F_G11F_B10F:
        case GL_RGB8I:            
        case GL_RGB8UI:            
        case GL_RGB16I:            
        case GL_RGB16UI:            
        case GL_RGB32I:            
        case GL_RGB32UI:
            return GL_RGB;
        
        case 4:
        case GL_COMPRESSED_RGBA:
        case GL_COMPRESSED_SRGB_ALPHA:
        case GL_RGBA:
        case GL_RGBA16F:
        case GL_RGBA32F:
        case GL_RGBA2:
        case GL_RGBA4:
        case GL_RGB5_A1:
        case GL_RGBA8:
        case GL_RGBA8_SNORM:
        case GL_RGB10_A2:
        case GL_RGBA12:
        case GL_RGBA16:
        case GL_RGBA16_SNORM:
        case GL_SRGB_ALPHA: 
        case GL_SRGB8_ALPHA8:
        case GL_RGBA8I:            
        case GL_RGBA8UI:            
        case GL_RGBA16I:            
        case GL_RGBA16UI:            
        case GL_RGBA32I:            
        case GL_RGBA32UI:
        default:
            return GL_RGBA;   
    }
}

GLsizei getComponents(GLint format)
{
    switch(getDataFormat(format))
    {

        case GL_RG:                 return 2;
        case GL_RGB:                return 3;
        case GL_RGBA:               return 4;
        
        case GL_RED:
        case GL_DEPTH_COMPONENT:
        case GL_DEPTH_STENCIL:
        case GL_UNSIGNED_BYTE_3_3_2:
        case GL_UNSIGNED_BYTE_2_3_3_REV:
        case GL_UNSIGNED_SHORT_5_6_5:
        case GL_UNSIGNED_SHORT_5_6_5_REV:
        case GL_UNSIGNED_SHORT_4_4_4_4:
        case GL_UNSIGNED_SHORT_4_4_4_4_REV:
        case GL_UNSIGNED_SHORT_5_5_5_1:
        case GL_UNSIGNED_SHORT_1_5_5_5_REV:
        case GL_UNSIGNED_INT_8_8_8_8:
        case GL_UNSIGNED_INT_8_8_8_8_REV:
        case GL_UNSIGNED_INT_10_10_10_2:
        case GL_UNSIGNED_INT_2_10_10_10_REV:
        case GL_UNSIGNED_INT_24_8:
        case GL_UNSIGNED_INT_10F_11F_11F_REV:
        case GL_UNSIGNED_INT_5_9_9_9_REV:
                                    return 1;
        
        default:                    return 0;
    }
}

class Texture2D : boost::noncopyable  {
public:
    Texture2D(GLint f, GLsizei w, GLsizei h)
        : format(f), width(w), height(h)
    {
        GLP_CHECKED_CALL(glGenTextures(1, &tex);)
        GLP_CHECKED_CALL(glBindTexture(GL_TEXTURE_2D, tex);)
        GLP_CHECKED_CALL(glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, getDataFormat(format), GL_UNSIGNED_BYTE, 0);)
        GLP_CHECKED_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);)
        GLP_CHECKED_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);)
        GLP_CHECKED_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);)
        GLP_CHECKED_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);)
    }

    template<class T>
    Texture2D(GLint f, GLsizei w, GLsizei h, T *data)
        : format(f), width(w), height(h)
    {
        GLP_CHECKED_CALL(glGenTextures(1, &tex);)
        GLP_CHECKED_CALL(glBindTexture(GL_TEXTURE_2D, tex);)
        GLP_CHECKED_CALL(glTexImage2D(GL_TEXTURE_2D, 0, format,
                    width, height, 0, getDataFormat(format),
                    TypeToGLConstant<
                        typename boost::remove_const<T>::type
                    >::value,
                    data);)
        GLP_CHECKED_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);)
        GLP_CHECKED_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);)
        GLP_CHECKED_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);)
        GLP_CHECKED_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);)
    }
    
    void generateMipmap()
    {
        GLP_CHECKED_CALL(glBindTexture(GL_TEXTURE_2D, tex);)
        GLP_CHECKED_CALL(glGenerateMipmap(GL_TEXTURE_2D);)
        GLP_CHECKED_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);)
    }
    
    void setMinFilter(GLenum val)
    {
        GLP_CHECKED_CALL(glBindTexture(GL_TEXTURE_2D, tex);)
        GLP_CHECKED_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, val);) 
    }

    void setMagFilter(GLenum val)
    {
        GLP_CHECKED_CALL(glBindTexture(GL_TEXTURE_2D, tex);)
        GLP_CHECKED_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, val);) 
    }
    
    void setWrapS(GLenum val)
    {
        GLP_CHECKED_CALL(glBindTexture(GL_TEXTURE_2D, tex);)
        GLP_CHECKED_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, val);) 
    }
    
    void setWrapT(GLenum val)
    {
        GLP_CHECKED_CALL(glBindTexture(GL_TEXTURE_2D, tex);)
        GLP_CHECKED_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, val);) 
    }
    
    GLenum getFormat() const
    {
        return format;
    }
    
    GLsizei getWidth() const
    {
        return width;
    }
    
    GLsizei getHeight() const
    {
        return height;
    }

    void bind()
    {
        GLP_CHECKED_CALL(glBindTexture(GL_TEXTURE_2D, tex);)
    }
    
    void bind(GLuint active)
    {
        GLP_CHECKED_CALL(glActiveTexture(uint2texture(active));)
        GLP_CHECKED_CALL(glBindTexture(GL_TEXTURE_2D, tex);)
    }

    operator GLuint() const
    {
        return tex;
    }
    
    ~Texture2D()
    {
        GLP_CHECKED_CALL(glDeleteTextures(1, &tex);)
    }
private:
    GLenum format;
    GLsizei width, height;
    GLuint tex;
};


class Texture2DMultisample : boost::noncopyable  {
public:
    Texture2DMultisample(GLint f, GLsizei w, GLsizei h, GLsizei s)
        : format(f), width(w), height(h), samples(s)
    {
        GLP_CHECKED_CALL(glGenTextures(1, &tex);)
        GLP_CHECKED_CALL(glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, tex);)
        GLP_CHECKED_CALL(glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, format, width, height, GL_FALSE);)
    }
    
    GLenum getFormat() const
    {
        return format;
    }

    GLsizei getSamples() const
    {
        return samples;
    }
    
    GLsizei getWidth() const
    {
        return width;
    }
    
    GLsizei getHeight() const
    {
        return height;
    }

    void bind()
    {
        GLP_CHECKED_CALL(glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, tex);)
    }
    
    void bind(GLenum active)
    {
        GLP_CHECKED_CALL(glActiveTexture(active);)
        GLP_CHECKED_CALL(glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, tex);)
    }

    operator GLuint() const
    {
        return tex;
    }
    
    ~Texture2DMultisample()
    {
        GLP_CHECKED_CALL(glDeleteTextures(1, &tex);)
    }
private:
    GLenum format;
    GLsizei width, height, samples;
    GLuint tex;
};

class Texture3D : boost::noncopyable  {
public:
    Texture3D(GLint f, GLsizei w, GLsizei h, GLsizei d)
        : format(f), width(w), height(h), depth(d)
    {
        GLP_CHECKED_CALL(glGenTextures(1, &tex);)
        GLP_CHECKED_CALL(glBindTexture(GL_TEXTURE_3D, tex);)
        GLP_CHECKED_CALL(glTexImage3D(GL_TEXTURE_3D, 0, format, width, height, depth, 0, getDataFormat(format), GL_UNSIGNED_BYTE, 0);)
        GLP_CHECKED_CALL(glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);)
        GLP_CHECKED_CALL(glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);)
        GLP_CHECKED_CALL(glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_REPEAT);)
        GLP_CHECKED_CALL(glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_REPEAT);)
        GLP_CHECKED_CALL(glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_REPEAT);)
    }

    template<class T>
    Texture3D(GLint f, GLsizei w, GLsizei h, GLsizei d, T *data)
        : format(f), width(w), height(h), depth(d)
    {
        GLP_CHECKED_CALL(glGenTextures(1, &tex);)
        GLP_CHECKED_CALL(glBindTexture(GL_TEXTURE_3D, tex);)
        GLP_CHECKED_CALL(glTexImage3D(GL_TEXTURE_3D, 0, format,
                    width, height, depth, 0, getDataFormat(format),
                    TypeToGLConstant<
                        typename boost::remove_const<T>::type
                    >::value,
                    data);)
        GLP_CHECKED_CALL(glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);)
        GLP_CHECKED_CALL(glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);)
        GLP_CHECKED_CALL(glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_REPEAT);)
        GLP_CHECKED_CALL(glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_REPEAT);)
        GLP_CHECKED_CALL(glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_REPEAT);)
    }
    
    void generateMipmap()
    {
        GLP_CHECKED_CALL(glBindTexture(GL_TEXTURE_3D, tex);)
        GLP_CHECKED_CALL(glGenerateMipmap(GL_TEXTURE_3D);)
        GLP_CHECKED_CALL(glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);)
    }
    
    void setMinFilter(GLenum val)
    {
        GLP_CHECKED_CALL(glBindTexture(GL_TEXTURE_3D, tex);)
        GLP_CHECKED_CALL(glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, val);) 
    }

    void setMagFilter(GLenum val)
    {
        GLP_CHECKED_CALL(glBindTexture(GL_TEXTURE_3D, tex);)
        GLP_CHECKED_CALL(glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, val);) 
    }
    
    void setWrapS(GLenum val)
    {
        GLP_CHECKED_CALL(glBindTexture(GL_TEXTURE_3D, tex);)
        GLP_CHECKED_CALL(glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, val);) 
    }
    
    void setWrapT(GLenum val)
    {
        GLP_CHECKED_CALL(glBindTexture(GL_TEXTURE_3D, tex);)
        GLP_CHECKED_CALL(glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, val);) 
    }

    void setWrapR(GLenum val)
    {
        GLP_CHECKED_CALL(glBindTexture(GL_TEXTURE_3D, tex);)
        GLP_CHECKED_CALL(glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, val);) 
    }
    
    GLenum getFormat() const
    {
        return format;
    }
    
    GLsizei getWidth() const
    {
        return width;
    }
    
    GLsizei getHeight() const
    {
        return height;
    }
    
    GLsizei getDepth() const
    {
        return depth;
    }

    void bind()
    {
        GLP_CHECKED_CALL(glBindTexture(GL_TEXTURE_3D, tex);)
    }
    
    void bind(GLuint active)
    {
        GLP_CHECKED_CALL(glActiveTexture(uint2texture(active));)
        GLP_CHECKED_CALL(glBindTexture(GL_TEXTURE_3D, tex);)
    }

    operator GLuint() const
    {
        return tex;
    }
    
    ~Texture3D()
    {
        GLP_CHECKED_CALL(glDeleteTextures(1, &tex);)
    }
private:
    GLenum format;
    GLsizei width, height, depth;
    GLuint tex;
};

class BufferTexture : boost::noncopyable  {
public:
    BufferTexture(GLenum f)
        : format(f)
    {
        GLP_CHECKED_CALL(glGenTextures(1, &tex);)
        GLP_CHECKED_CALL(glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, tex);)
    }
    
    BufferTexture(GLenum f, GLuint buffer)
        : format(f)
    {
        GLP_CHECKED_CALL(glGenTextures(1, &tex);)
        GLP_CHECKED_CALL(glBindTexture(GL_TEXTURE_BUFFER, tex);)
        GLP_CHECKED_CALL(glTexBuffer(GL_TEXTURE_BUFFER, format, buffer);)
    }
    
    void attachBuffer(GLuint buffer)
    {
        GLP_CHECKED_CALL(glBindTexture(GL_TEXTURE_BUFFER, tex);)
        GLP_CHECKED_CALL(glTexBuffer(GL_TEXTURE_BUFFER, format, buffer);)
    }
    
    void detachBuffer()
    {
        GLP_CHECKED_CALL(glBindTexture(GL_TEXTURE_BUFFER, tex);)
        GLP_CHECKED_CALL(glTexBuffer(GL_TEXTURE_BUFFER, format, 0);)
    }
    
    GLenum getFormat() const
    {
        return format;
    }
    
    void bind()
    {
        GLP_CHECKED_CALL(glBindTexture(GL_TEXTURE_BUFFER, tex);)
    }
    
    void bind(GLuint active)
    {
        GLP_CHECKED_CALL(glActiveTexture(uint2texture(active));)
        GLP_CHECKED_CALL(glBindTexture(GL_TEXTURE_BUFFER, tex);)
    }

    operator GLuint() const
    {
        return tex;
    }
    
    ~BufferTexture()
    {
        GLP_CHECKED_CALL(glDeleteTextures(1, &tex);)
    }
private:
    GLenum format;
    GLuint tex;
};

}
#endif
