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

#ifndef GL_BUFFER_H
#define GL_BUFFER_H

#include <stdexcept>
#include <boost/utility.hpp>

#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>

#include "GLCheckError.h"

namespace glp {

template<class T, GLenum TARGET>
class Buffer : boost::noncopyable {
public:
    typedef T value_type;
    typedef T& reference;
    typedef const T& const_reference;
    typedef T* iterator;
    typedef const T* const_iterator;
    typedef const ptrdiff_t difference_type;
    typedef size_t size_type;


    Buffer(size_type s, GLenum usage)
        : size_(s), host_ptr(0)
    {
        GLP_CHECKED_CALL(glGenBuffers(1, &buffer);)
        GLP_CHECKED_CALL(glBindBuffer(TARGET, buffer);)
        GLP_CHECKED_CALL(glBufferData(TARGET, size_*sizeof(value_type), 0, usage);)
        GLP_CHECKED_CALL(glBindBuffer(TARGET, 0);)
    }
    
    void map(GLbitfield access)
    {
        if(host_ptr)
            return;
        GLP_CHECKED_CALL(glBindBuffer(TARGET, buffer);)
        GLP_CHECKED_CALL(
        host_ptr = reinterpret_cast<value_type*>(
                    glMapBufferRange(TARGET, 0, 
                                     size_*sizeof(value_type),
                                     access)
                                    );
        )
        GLP_CHECKED_CALL(glBindBuffer(TARGET, 0);)
    }
    
    bool isMapped() const { return host_ptr != 0; }
    
    inline reference operator[](size_t i)
    {
        check_mapped();
        return host_ptr[i];
    }

    inline const_reference operator[](size_t i) const
    {
        check_mapped();
        return host_ptr[i];
    }
    
    inline value_type* data() { check_mapped(); return host_ptr; }
    inline const value_type* data() const { check_mapped(); return host_ptr; }
    inline iterator begin() { check_mapped(); return host_ptr; }
    inline const_iterator begin() const { check_mapped(); return host_ptr; }
    inline iterator end() { check_mapped(); return host_ptr+size_; }
    inline const_iterator end() const { check_mapped(); return host_ptr+size_; }
    
    inline size_type size() const { return size_; }
    
    inline void bind()
    {
        check_unmapped();
        GLP_CHECKED_CALL(glBindBuffer(TARGET, buffer);)
    }
    
    inline void unbind()
    {
        GLP_CHECKED_CALL(glBindBuffer(TARGET, 0);)  
    }
    
    void unmap()
    {
        if(!host_ptr)
            return;
        GLP_CHECKED_CALL(glBindBuffer(TARGET, buffer);)
        GLP_CHECKED_CALL(glUnmapBuffer(TARGET);)
        GLP_CHECKED_CALL(glBindBuffer(TARGET, 0);)
        host_ptr = 0;   
    }

    operator GLuint() const { return buffer; }
    GLuint getBuffer() const { return buffer; }

    virtual ~Buffer()
    {
        GLP_CHECKED_CALL(glDeleteBuffers(1, &buffer);)
    }
protected:
    inline void check_mapped() const
    {   
        if(!host_ptr)
            throw exception("Buffer not mapped");
    }

    inline void check_unmapped() const
    {   
        if(host_ptr)
            throw exception("Buffer mapped");
    }

    GLuint buffer;
    size_type size_;
    value_type *host_ptr;
};


}

#endif
