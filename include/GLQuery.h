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

#ifndef GLP_QUERY_H
#define GLP_QUERY_H

#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>

#include <boost/utility.hpp>

#include "GLCheckError.h"

namespace glp {

template<GLenum TARGET>
class Query : boost::noncopyable {
public:
	Query()
	{
		GLP_CHECKED_CALL(glGenQueries(1, &id);)
	}
	
	void begin()
	{
		GLP_CHECKED_CALL(glBeginQuery(TARGET, id);)
	}
	
	void end()
	{
		GLP_CHECKED_CALL(glEndQuery(TARGET);)
	}
	
	bool available()
	{
		GLuint result;
		GLP_CHECKED_CALL(glGetQueryObjectuiv(id, GL_QUERY_RESULT_AVAILABLE, &result);)
		return result == GL_TRUE;
	}

	GLuint result()
	{
		GLuint result;
		GLP_CHECKED_CALL(glGetQueryObjectuiv(id, GL_QUERY_RESULT, &result);)
		return result;
	}
	
	operator GLuint() const { return id; }
	
	~Query()
	{
		GLP_CHECKED_CALL(glDeleteQueries(1, &id);)
	}
private:
	GLuint id;
};

}

#endif
