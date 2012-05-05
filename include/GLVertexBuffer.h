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

#ifndef VBO_H
#define VBO_H

#include <vector>
#include <boost/fusion/include/for_each.hpp>
#include <boost/fusion/include/size.hpp>
#include <boost/fusion/include/value_at.hpp>
#include <boost/mpl/int.hpp>
#include <boost/type_traits.hpp>

#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>

#include "vector_traits.h"

#include "TypeToGLConstant.h"

#include "GLBuffer.h"

#include "GLCheckError.h"
        
namespace glp {
    
template<class T>
class IndexBuffer;

template<class V>
class VertexBuffer : public Buffer<V, GL_ARRAY_BUFFER> {
public:
    typedef Buffer<V, GL_ARRAY_BUFFER> base_type;

    VertexBuffer(size_t);
    VertexBuffer(size_t, GLenum);

    void map()
    {
        base_type::map(GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);
    }
    
    void bind();
    void unbind();
    void setBaseAttrib(GLuint i) { base_attrib = i; }   
    void setDivisor(GLuint i) { divisor = i; }  
    GLuint getBaseAttrib() const  { return base_attrib; }
    GLuint getDivisor() const  { return divisor; }
    GLuint getAttributeCount() const
    { return boost::fusion::result_of::size<V>::type::value; }
    GLuint getNextAttrib() const
    { return base_attrib+boost::fusion::result_of::size<V>::type::value; }
    
private:
    GLuint vao;
    GLuint base_attrib, divisor;
};

struct VertexAttribPred {
    
    typedef void result_type;
    
    template<class T>
    VertexAttribPred(T &t, int &c) : count(c), size(sizeof(T))
    {
        base_ptr = reinterpret_cast<char*>(&t);
    }
    
    template<class T>
    void operator()(T &t) const
    {
        static_assert(vector_traits<T>::dimension<=4, "vertex attribute cannot have more than 4 elements");
        if(boost::is_integral<typename vector_traits<T>::element_type>::type::value)
            GLP_CHECKED_CALL(glVertexAttribIPointer(
                    count++, 
                    vector_traits<T>::dimension,
                    TypeToGLConstant<
                        typename vector_traits<T>::element_type
                    >::value,
                    size,
                    (char*)(reinterpret_cast<char*>(&t)-base_ptr)
                );)
        else
            GLP_CHECKED_CALL(glVertexAttribPointer(
                    count++,
                    vector_traits<T>::dimension, 
                    TypeToGLConstant<
                        typename vector_traits<T>::element_type
                    >::value, 
                    GL_FALSE, 
                    size, 
                    (char*)(reinterpret_cast<char*>(&t)-base_ptr)
                );)
    }
    char *base_ptr;
    int &count;
    int size;
};

template<class V>   
VertexBuffer<V>::VertexBuffer(size_t s)
    : Buffer<V, GL_ARRAY_BUFFER>(s, GL_DYNAMIC_DRAW),
    base_attrib(0), divisor(0)
{
}

template<class V>   
VertexBuffer<V>::VertexBuffer(size_t s, GLenum usage)
    : Buffer<V, GL_ARRAY_BUFFER>(s, usage),
    base_attrib(0), divisor(0)
{
}

template<class V>   
void VertexBuffer<V>::bind()
{
    base_type::bind();
    
    {
        V tmp;
        int c = base_attrib;
        VertexAttribPred p(tmp, c);
        boost::fusion::for_each(tmp, p);
    }

    const unsigned size = boost::fusion::result_of::size<V>::type::value;
    for(unsigned i=0;i<size;++i)
    {
        GLP_CHECKED_CALL(glEnableVertexAttribArray(base_attrib+i);)
        GLP_CHECKED_CALL(glVertexAttribDivisor(base_attrib+i, divisor);)
    }   
}

template<class V>   
void VertexBuffer<V>::unbind()
{
    base_type::unbind();
    const unsigned size = boost::fusion::result_of::size<V>::type::value;
    for(unsigned i=0;i<size;++i)
    {
        GLP_CHECKED_CALL(glDisableVertexAttribArray(base_attrib+i);)
        GLP_CHECKED_CALL(glVertexAttribDivisor(base_attrib+i, 0);)
    }
}

template<class T>
class IndexBuffer : public Buffer<T, GL_ELEMENT_ARRAY_BUFFER> {
public:
    typedef Buffer<T, GL_ELEMENT_ARRAY_BUFFER> base_type;
    
    IndexBuffer(size_t s) : base_type(s, GL_STATIC_DRAW) { }
    IndexBuffer(size_t s, GLenum usage) : base_type(s, usage) { }
    
    void map()
    {
        base_type::map(GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);
    }
};

}
#endif
