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

#ifndef GLP_CHECK_ERRORS_H
#define GLP_CHECK_ERRORS_H

#define GL_GLEXT_PROTOTYPES

#include <GL/gl.h>
#include <GL/glu.h>

#include <stdexcept>
#include <string>

namespace glp {

class exception : public std::runtime_error
{
public:
    exception(GLenum c, const std::string &what)
    : runtime_error(what), code(c)
    { }
    exception(const std::string &what)
    : runtime_error(what), code(0)
    { }
    GLenum code;
};

inline void checkGlErrors()
{
    GLenum error = glGetError();
    if(error != GL_NO_ERROR)
    {
        throw exception(error, reinterpret_cast<const char*>(gluErrorString(error)));
    }
}

inline void checkGlErrors(std::string location)
{
    GLenum error = glGetError();
    if(error != GL_NO_ERROR)
    {
        throw exception(error, std::string(reinterpret_cast<const char*>(gluErrorString(error))) + " after: " + location);
    }
}

#ifdef GLP_DEBUG
    #define GLP_CHECKED_CALL(arg) { arg  glp::checkGlErrors(__FILE__ " : " #arg  " " ); }
#else
    #define GLP_CHECKED_CALL(arg) arg 
#endif

}

#endif
