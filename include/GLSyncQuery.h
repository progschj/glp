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

#ifndef GL_SYNC_QUERY_H
#define GL_SYNC_QUERY_H

#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <boost/utility.hpp>

#include "GLCheckError.h"

namespace glp {

class SyncQuery : boost::noncopyable {
public:
	SyncQuery() : signaled_state(true)
	{
	}
	
	void fence()
	{
		if(!signaled_state) glDeleteSync(sync);
		sync = glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0);
		signaled_state = false;
	}

	bool signaled()
	{
		if(signaled_state)
			return true;
			
		GLint status;
		GLsizei length;
		GLP_CHECKED_CALL(glGetSynciv(sync, GL_SYNC_STATUS, sizeof(status), &length, &status);)
		signaled_state = (status == GL_SIGNALED);
		if(signaled_state)
			GLP_CHECKED_CALL(glDeleteSync(sync);)
		return signaled_state;
	}
	
	operator bool()
	{
		return signaled();
	}
	
	GLenum wait(GLuint64 timeout)
	{
		GLenum result;
		GLP_CHECKED_CALL(result = glClientWaitSync(sync, GL_SYNC_FLUSH_COMMANDS_BIT, timeout);)
		if(result == GL_ALREADY_SIGNALED || result == GL_CONDITION_SATISFIED)
		{
			signaled_state = true;
			GLP_CHECKED_CALL(glDeleteSync(sync);)
		}
		return result;
	} 
	
	~SyncQuery()
	{
		if(!signaled_state) GLP_CHECKED_CALL(glDeleteSync(sync);)
	}
private:
	GLsync sync;
	bool signaled_state;
};


}

#endif
