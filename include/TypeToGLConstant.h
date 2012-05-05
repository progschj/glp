#ifndef TYPE_TO_GL_CONSTANT
#define TYPE_TO_GL_CONSTANT

#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
namespace glp {
	
template<class T>
struct TypeToGLConstant { };

template<>
struct TypeToGLConstant<GLfloat> { static const GLenum value = GL_FLOAT; };

template<>
struct TypeToGLConstant<GLdouble> { static const GLenum value = GL_DOUBLE; };

template<>
struct TypeToGLConstant<GLbyte> { static const GLenum value = GL_BYTE; };

template<>
struct TypeToGLConstant<GLubyte> { static const GLenum value = GL_UNSIGNED_BYTE; };

template<>
struct TypeToGLConstant<GLshort> { static const GLenum value = GL_SHORT; };

template<>
struct TypeToGLConstant<GLushort> { static const GLenum value = GL_UNSIGNED_SHORT; };

template<>
struct TypeToGLConstant<GLint> { static const GLenum value = GL_INT; };

template<>
struct TypeToGLConstant<GLuint> { static const GLenum value = GL_UNSIGNED_INT; };

GLenum uint2attachment(GLuint i)
{
#define GLPUINT2ATTACHMENT_MACRO(arg) case arg: return GL_COLOR_ATTACHMENT##arg ;
	switch(i)
	{
		GLPUINT2ATTACHMENT_MACRO(0)
		GLPUINT2ATTACHMENT_MACRO(1)
		GLPUINT2ATTACHMENT_MACRO(2)
		GLPUINT2ATTACHMENT_MACRO(3)
		GLPUINT2ATTACHMENT_MACRO(4)
		GLPUINT2ATTACHMENT_MACRO(5)
		GLPUINT2ATTACHMENT_MACRO(6)
		GLPUINT2ATTACHMENT_MACRO(7)
		default: return i;
	}
#undef GLPUINT2ATTACHMENT_MACRO
}

GLenum uint2texture(GLuint i)
{
#define GLPUINT2TEXTURE_MACRO(arg) case arg: return GL_TEXTURE##arg ;
	switch(i)
	{
		GLPUINT2TEXTURE_MACRO(0)
		GLPUINT2TEXTURE_MACRO(1)
		GLPUINT2TEXTURE_MACRO(2)
		GLPUINT2TEXTURE_MACRO(3)
		GLPUINT2TEXTURE_MACRO(4)
		GLPUINT2TEXTURE_MACRO(5)
		GLPUINT2TEXTURE_MACRO(6)
		GLPUINT2TEXTURE_MACRO(7)
		default: return i;
	}
#undef GLPUINT2TEXTURE_MACRO
}

}
#endif
