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

#ifndef GLSL_SHADER_H
#define GLSL_SHADER_H

#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>

#include <string>
#include <stdexcept>
#include <vector>

#include <boost/utility.hpp>

#include "GLCheckError.h"

namespace glp {
    
class GLSL_shader_exception : public std::runtime_error
{
public:
    GLSL_shader_exception(std::string what)
    : runtime_error(what)
    { }
};

class ShaderProgram {
public:
    ShaderProgram()
     :  compiled(false),
        has_geometry_shader(false), has_vertex_shader(false),
        has_fragment_shader(false), linked(false) { }

    void setGeometryShaderSource(const std::string &source);
    void setVertexShaderSource(const std::string &source);
    void setFragmentShaderSource(const std::string &source);
    void compileProgram();
    void linkProgram();
    void bindAttributeLocation(const GLuint id, const std::string &name);
    void bindFragDataLocation(const GLuint id, const std::string &name);
    void setTransformFeedbackVaryings(std::vector<const char*>);
    void bindProgram();
    void unbindProgram();
    void deleteProgram();
    
    GLint getUniformLocation(const std::string &name);
    void setUniform(const std::string &name, GLint val1);
    void setUniform(const std::string &name, GLint val1, GLint val2);
    void setUniform(const std::string &name, GLint val1, GLint val2, GLint val3);
    void setUniform(const std::string &name, GLint val1, GLint val2, GLint val3, GLint val4);
    void setUniform(const std::string &name, GLfloat val1);
    void setUniform(const std::string &name, GLfloat val1, GLfloat val2);
    void setUniform(const std::string &name, GLfloat val1, GLfloat val2, GLfloat val3);
    void setUniform(const std::string &name, GLfloat val1, GLfloat val2, GLfloat val3, GLfloat val4);
    void setUniform(const std::string &name, int size, GLint *val, int count = 1);
    void setUniform(const std::string &name, int size, GLfloat *val, int count = 1);
    void setUniformMatrix(const std::string &name, int width, int height, GLfloat *val, int count = 1);
    void setUniform(GLint location, GLint val1);
    void setUniform(GLint location, GLint val1, GLint val2);
    void setUniform(GLint location, GLint val1, GLint val2, GLint val3);
    void setUniform(GLint location, GLint val1, GLint val2, GLint val3, GLint val4);
    void setUniform(GLint location, GLfloat val1);
    void setUniform(GLint location, GLfloat val1, GLfloat val2);
    void setUniform(GLint location, GLfloat val1, GLfloat val2, GLfloat val3);
    void setUniform(GLint location, GLfloat val1, GLfloat val2, GLfloat val3, GLfloat val4);
    void setUniform(GLint location, int size, GLint *val, int count = 1);
    void setUniform(GLint location, int size, GLfloat *val, int count = 1);
    void setUniformMatrix(GLint location, int width, int height, GLfloat *val, int count = 1);
    
    std::vector<unsigned char> getBinary();
    void loadBinary(std::vector<unsigned char>&);
    
    ~ShaderProgram();
private:
    GLuint geometry_shader, vertex_shader, fragment_shader, shader_program;
    std::string vertex_source, fragment_source, geometry_source;
    bool compiled, has_geometry_shader, has_vertex_shader, has_fragment_shader, linked;
    
    void check_shader_compile_status(GLuint);
    void check_program_link_status(GLuint);
};


}
#endif
