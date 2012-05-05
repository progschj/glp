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

#ifndef GLP_VERTEX_ARRAY_H
#define GLP_VERTEX_ARRAY_H

#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>

#include <boost/utility.hpp>

#include "GLVertexBuffer.h"

namespace glp {
    
class VertexArray : boost::noncopyable {
public:
    VertexArray()
        : vbo_size(0), ibo_size(0), ibo_type(GL_FALSE)
    {
        GLP_CHECKED_CALL(glGenVertexArrays(1, &vao);)
    }

    void bind()
    {
        GLP_CHECKED_CALL(glBindVertexArray(vao);)
    }

    template<class T>
    void attach(VertexBuffer<T> &vbo)
    {
        this->bind();
        vbo.bind();
        this->unbind();
        vbo.unbind();
        if(vbo.getDivisor() == 0)
            vbo_size = vbo.size();
    }
    
    template<class T>
    void attach(IndexBuffer<T> &ibo)
    {
        this->bind();
        ibo.bind();
        this->unbind();
        ibo.unbind();
        ibo_type = TypeToGLConstant<T>::value;
        ibo_size = ibo.size();
    }
    
    void setVertexCount(size_t n) { vbo_size = n; }
    
    void draw(GLenum primitives)
    {
        this->bind();
        
        if(ibo_type == GL_FALSE)
            GLP_CHECKED_CALL(glDrawArrays(primitives, 0, vbo_size);)
        else
            GLP_CHECKED_CALL(glDrawElements(primitives, ibo_size, ibo_type, 0);)
            
        this->unbind();
    }

    void drawInstanced(GLenum primitives, GLsizei primcount)
    {
        this->bind();
        
        if(ibo_type == GL_FALSE)
            GLP_CHECKED_CALL(glDrawArraysInstanced(primitives, 0, vbo_size, primcount);)
        else
            GLP_CHECKED_CALL(glDrawElementsInstanced(primitives,  ibo_size,  ibo_type, 0, primcount);)
        
        this->unbind();
    }

    void draw(GLenum primitives, GLuint begin, GLuint end)
    {
        this->bind();
        
        if(ibo_type == GL_FALSE)
            GLP_CHECKED_CALL(glDrawArrays(primitives, begin, end-begin);)
        else
            GLP_CHECKED_CALL(glDrawRangeElements(primitives, 0, vbo_size, end-begin, ibo_type, static_cast<GLubyte*>(0)+begin);)
        
        this->unbind();
    }

    void unbind()
    {
        GLP_CHECKED_CALL(glBindVertexArray(0);)
    }
    
    ~VertexArray()
    {
        GLP_CHECKED_CALL(glDeleteVertexArrays(1, &vao);)
    }
private:
    GLuint vao;
    size_t vbo_size;
    size_t ibo_size;
    GLenum ibo_type;
};

}
#endif

    


