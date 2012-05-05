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

#ifndef GL_PIXEL_BUFFER_H
#define GL_PIXEL_BUFFER_H

#include <stdexcept>

#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>

#include "GLBuffer.h"
#include "GLTexture.h"

namespace glp {

template<class T>
class PixelUnpackBuffer : public Buffer<T,GL_PIXEL_UNPACK_BUFFER> {
public:
	typedef Buffer<T,GL_PIXEL_UNPACK_BUFFER> base_type;

	PixelUnpackBuffer(size_t s)
		: base_type(s, GL_DYNAMIC_DRAW)
	{ }
	
	void map()
	{
		base_type::map(GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);
	}
	
	void copyTo(Texture2D &texture)
	{
		copyTo(texture, 0);
	}
	
	void copyTo(Texture2D &texture, GLint level)
	{
		this->check_unmapped();
		GLenum format = texture.getFormat();
		GLsizei components = getComponents(format);
		GLsizei width = std::max(1,texture.getWidth()/(1<<level));
		GLsizei height = std::max(1,texture.getHeight()/(1<<level));
		
		if(this->size() != size_t(components*width*height))
			throw std::runtime_error("PixelUnpackBuffer does not match texture size");
		

		this->bind();
		texture.bind();
		GLP_CHECKED_CALL(
		glTexSubImage2D(GL_TEXTURE_2D, level, 0, 0, width, height,
					getDataFormat(format),
					TypeToGLConstant<
						typename boost::remove_const<T>::type
					>::value,
					0);
		)
		
		this->unbind();
	}
	
	void copyTo(Texture2D &texture, GLint offsetx, GLint offsety, GLsizei width, GLsizei height)
	{
		copyTo(texture, offsetx, offsety, width, height, 0);
	}
	
	void copyTo(Texture2D &texture, GLint offsetx, GLint offsety, GLsizei width, GLsizei height, GLint level)
	{
		this->check_unmapped();
		GLenum format = texture.getFormat();
		GLsizei components = getComponents(format);
		
		if(this->size() != size_t(components*width*height))
			throw std::runtime_error("PixelUnpackBuffer does not match texture size");
		

		this->bind();
		texture.bind();
		
		GLP_CHECKED_CALL(
		glTexSubImage2D(GL_TEXTURE_2D, level, offsetx, offsety, width, height,
					getDataFormat(format),
					TypeToGLConstant<
						typename boost::remove_const<T>::type
					>::value,
					0);
		)
		
		this->unbind();
	}

	void copyTo(Texture3D &texture)
	{
		copyTo(texture, 0);
	}
	
	void copyTo(Texture3D &texture, GLint level)
	{
		this->check_unmapped();
		GLenum format = texture.getFormat();
		GLsizei components = getComponents(format);
		GLsizei width = std::max(1,texture.getWidth()/(1<<level));
		GLsizei height = std::max(1,texture.getHeight()/(1<<level));
		GLsizei depth = std::max(1,texture.getDepth()/(1<<level));
		
		if(this->size() != size_t(components*width*height*depth))
			throw std::runtime_error("PixelUnpackBuffer does not match texture size");
		

		this->bind();
		texture.bind();
		GLP_CHECKED_CALL(
		glTexSubImage3D(GL_TEXTURE_3D, level, 0, 0, 0, width, height, depth,
					getDataFormat(format),
					TypeToGLConstant<
						typename boost::remove_const<T>::type
					>::value,
					0);
		)
		
		this->unbind();
	}
	
	void copyTo(Texture3D &texture, GLint offsetx, GLint offsety, GLint offsetz, GLsizei width, GLsizei height, GLsizei depth)
	{
		copyTo(texture, offsetx, offsety, offsetz, width, height, depth, 0);
	}
	
	void copyTo(Texture3D &texture, GLint offsetx, GLint offsety, GLint offsetz, GLsizei width, GLsizei height, GLsizei depth, GLint level)
	{
		this->check_unmapped();
		GLenum format = texture.getFormat();
		GLsizei components = getComponents(format);
		
		if(this->size() != size_t(components*width*height*depth))
			throw std::runtime_error("PixelUnpackBuffer does not match texture size");
		

		this->bind();
		texture.bind();
		
		GLP_CHECKED_CALL(
		glTexSubImage3D(GL_TEXTURE_3D, level, offsetx, offsety, offsetz, width, height, depth,
					getDataFormat(format),
					TypeToGLConstant<
						typename boost::remove_const<T>::type
					>::value,
					0);
		)
		
		this->unbind();
	}
};




template<class T>
class PixelPackBuffer : public Buffer<T,GL_PIXEL_PACK_BUFFER>  {
public:
	typedef Buffer<T,GL_PIXEL_PACK_BUFFER> base_type;
	
	PixelPackBuffer(size_t s)
		: base_type(s, GL_STREAM_READ)
	{ }
	
	void map()
	{
		base_type::map(GL_MAP_READ_BIT);
	}

	void readPixels(GLenum source, GLint  x,  GLint  y,  GLsizei  width,  GLsizei  height,  GLenum  format)
	{
		GLsizei components = getComponents(format);
		if(this->size() != size_t(components*width*height))
			throw std::runtime_error("PixelPackBuffer does not match rectangle size");
			
		glReadBuffer(source);
		this->bind();
		GLP_CHECKED_CALL(
		glReadPixels(x, y, width, height,
			format,
			TypeToGLConstant<
						typename boost::remove_const<T>::type
					>::value,
			0);
		)
		this->unbind();
	}
};

}

#endif
