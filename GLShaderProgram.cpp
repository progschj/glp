#include "include/GLShaderProgram.h"

#include <boost/scoped_array.hpp>

#include <iostream>

namespace glp {

void ShaderProgram::check_shader_compile_status(GLuint obj)
{
	GLint status;
	glGetShaderiv(obj, GL_COMPILE_STATUS, &status);
	if(status == GL_FALSE)
	{
		GLint length;
		glGetShaderiv(obj, GL_INFO_LOG_LENGTH, &length);
		
		boost::scoped_array<char> log(new char[length]);
 
		glGetShaderInfoLog(obj, length, &length, log.get());
		
		GLSL_shader_exception e(log.get());
		throw(e);
	}
}

void ShaderProgram::check_program_link_status(GLuint obj)
{
	GLint status;
	glGetProgramiv(obj, GL_LINK_STATUS, &status);
	if(status == GL_FALSE)
	{
		GLint length;
		glGetProgramiv(obj, GL_INFO_LOG_LENGTH, &length);
 
		boost::scoped_array<char> log(new char[length]);
 
		glGetProgramInfoLog(obj, length, &length, log.get());
		
		GLSL_shader_exception e(log.get());
		throw(e);	
	}
}

void ShaderProgram::setGeometryShaderSource(const std::string &source)
{
	geometry_source = source;
	has_geometry_shader = true;
}

void ShaderProgram::setVertexShaderSource(const std::string &source)
{
	vertex_source = source;
	has_vertex_shader = true;
}

void ShaderProgram::setFragmentShaderSource(const std::string &source)
{
	fragment_source = source;
	has_fragment_shader = true;
}

void ShaderProgram::compileProgram()
{
	const char *source;
	int length;
	
	if(has_vertex_shader)	
	{
		vertex_shader = glCreateShader(GL_VERTEX_SHADER);
		source = vertex_source.c_str();
		length = vertex_source.size();
		glShaderSource(vertex_shader, 1, &source, &length);	
		glCompileShader(vertex_shader);
		check_shader_compile_status(vertex_shader);
	}
	
	if(has_geometry_shader)	
	{
		geometry_shader = glCreateShader(GL_GEOMETRY_SHADER);
		source = geometry_source.c_str();
		length = geometry_source.size();
		glShaderSource(geometry_shader, 1, &source, &length);	
		glCompileShader(geometry_shader);
		check_shader_compile_status(geometry_shader);
	}
	
	if(has_fragment_shader)	
	{	
		fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
		source = fragment_source.c_str();
		length = fragment_source.size();
		glShaderSource(fragment_shader, 1, &source, &length);	
		glCompileShader(fragment_shader);
		check_shader_compile_status(fragment_shader);
	}

	shader_program = glCreateProgram();
	if(has_geometry_shader)	
		glAttachShader(shader_program, geometry_shader);
	if(has_vertex_shader)	
		glAttachShader(shader_program, vertex_shader);
	if(has_fragment_shader)	
		glAttachShader(shader_program, fragment_shader);	
	
	compiled = true;
}

void ShaderProgram::linkProgram()
{
	glLinkProgram(shader_program);
	check_program_link_status(shader_program);
	linked = true;
}

void ShaderProgram::setTransformFeedbackVaryings(std::vector<const char*> varyings)
{
	glTransformFeedbackVaryings(shader_program, varyings.size(), varyings.data(), GL_INTERLEAVED_ATTRIBS);
}

void ShaderProgram::bindAttributeLocation(const GLuint id, const std::string &name)
{
	glBindAttribLocation(shader_program, id, name.c_str());
}

void ShaderProgram::bindFragDataLocation(const GLuint id, const std::string &name)
{
	glBindFragDataLocation(shader_program, id, name.c_str());
}

void ShaderProgram::bindProgram()
{
	glUseProgram(shader_program);
}

void ShaderProgram::unbindProgram()
{
	glUseProgram(0);
}

void ShaderProgram::deleteProgram()
{
	if(compiled)
	{
		unbindProgram();
		if(has_vertex_shader)		
			glDetachShader(shader_program, vertex_shader);
		if(has_geometry_shader)
			glDetachShader(shader_program, geometry_shader);
		if(has_fragment_shader)			
			glDetachShader(shader_program, fragment_shader);

		if(has_vertex_shader)		
			glDeleteShader(vertex_shader);
		if(has_geometry_shader)
			glDeleteShader(geometry_shader);
		if(has_fragment_shader)			
			glDeleteShader(fragment_shader);
		
		glDeleteProgram(shader_program);
		
		compiled = false;
		linked = false;
	}	
}
	
ShaderProgram::~ShaderProgram()
{
	deleteProgram();
}

GLint ShaderProgram::getUniformLocation(const std::string &name)
{
	return glGetUniformLocation(shader_program, name.c_str());
}

void ShaderProgram::setUniform(GLint location, GLint val1)
{
	glUniform1i(location, val1);
}

void ShaderProgram::setUniform(GLint location, GLint val1, GLint val2)
{
	glUniform2i(location, val1, val2);
}

void ShaderProgram::setUniform(GLint location, GLint val1, GLint val2, GLint val3)
{
	glUniform3i(location, val1, val2, val3);
}

void ShaderProgram::setUniform(GLint location, GLint val1, GLint val2, GLint val3, GLint val4)
{
	glUniform4i(location, val1, val2, val3, val4);
}

void ShaderProgram::setUniform(GLint location, GLfloat val1)
{
	glUniform1f(location, val1);
}

void ShaderProgram::setUniform(GLint location, GLfloat val1, GLfloat val2)
{
	glUniform2f(location, val1, val2);
}

void ShaderProgram::setUniform(GLint location, GLfloat val1, GLfloat val2, GLfloat val3)
{
	glUniform3f(location, val1, val2, val3);
}

void ShaderProgram::setUniform(GLint location, GLfloat val1, GLfloat val2, GLfloat val3, GLfloat val4)
{
	glUniform4f(location, val1, val2, val3, val4);
}

void ShaderProgram::setUniform(GLint location, int size, GLint *val, int count)
{
	switch(size)
	{
		case 1:
			glUniform1iv(location, count, val);
		return;
		case 2:
			glUniform2iv(location, count, val);
		return;
		case 3:
			glUniform3iv(location, count, val);
		return;
		case 4:
			glUniform4iv(location, count, val);
		return;
		default: return;
	}
}

void ShaderProgram::setUniform(GLint location, int size, GLfloat *val, int count)
{
	switch(size)
	{
		case 1:
			glUniform1fv(location, count, val);
		return;
		case 2:
			glUniform2fv(location, count, val);
		return;
		case 3:
			glUniform3fv(location, count, val);
		return;
		case 4:
			glUniform4fv(location, count, val);
		return;
		default: return;
	}
}

void ShaderProgram::setUniformMatrix(GLint location, int width, int height, GLfloat *val, int count)
{
	switch(width)
	{
		case 2:
		switch(height)
		{
			case 2:
				glUniformMatrix2fv(location, count, false, val);
			return;
			case 3:
				glUniformMatrix2x3fv(location, count, false, val);
			return;
			case 4:
				glUniformMatrix2x4fv(location, count, false, val);
			return;
			default: return;
		}
		case 3:
		switch(height)
		{
			case 2:
				glUniformMatrix3x2fv(location, count, false, val);
			return;
			case 3:
				glUniformMatrix3fv(location, count, false, val);
			return;
			case 4:
				glUniformMatrix3x4fv(location, count, false, val);
			return;
			default: return;
		}
		case 4:
		switch(height)
		{
			case 2:
				glUniformMatrix4x2fv(location, count, false, val);
			return;
			case 3:
				glUniformMatrix4x3fv(location, count, false, val);
			return;
			case 4:
				glUniformMatrix4fv(location, count, false, val);
			return;
			default: return;
		}
		default: return;
	}
}



void ShaderProgram::setUniform(const std::string &name, GLint val1)
{
	setUniform(getUniformLocation(name), val1);
}

void ShaderProgram::setUniform(const std::string &name, GLint val1, GLint val2)
{
	setUniform(getUniformLocation(name), val1, val2);
}

void ShaderProgram::setUniform(const std::string &name, GLint val1, GLint val2, GLint val3)
{
	setUniform(getUniformLocation(name), val1, val2, val3);
}

void ShaderProgram::setUniform(const std::string &name, GLint val1, GLint val2, GLint val3, GLint val4)
{
	setUniform(getUniformLocation(name), val1, val2, val3, val4);
}

void ShaderProgram::setUniform(const std::string &name, GLfloat val1)
{
	setUniform(getUniformLocation(name), val1);
}

void ShaderProgram::setUniform(const std::string &name, GLfloat val1, GLfloat val2)
{
	setUniform(getUniformLocation(name), val1, val2);
}

void ShaderProgram::setUniform(const std::string &name, GLfloat val1, GLfloat val2, GLfloat val3)
{
	setUniform(getUniformLocation(name), val1, val2, val3);
}

void ShaderProgram::setUniform(const std::string &name, GLfloat val1, GLfloat val2, GLfloat val3, GLfloat val4)
{
	setUniform(getUniformLocation(name), val1, val2, val3, val4);
}

void ShaderProgram::setUniform(const std::string &name, int size, GLint *val, int count)
{
	setUniform(getUniformLocation(name), size, val, count);
}

void ShaderProgram::setUniform(const std::string &name, int size, GLfloat *val, int count)
{
	setUniform(getUniformLocation(name), size, val, count);
}

void ShaderProgram::setUniformMatrix(const std::string &name, int width, int height, GLfloat *val, int count)
{
	setUniformMatrix(getUniformLocation(name), width, height, val, count);
}

std::vector<unsigned char> ShaderProgram::getBinary()
{
	if(!linked)
		return std::vector<unsigned char>();
	
	GLint length1, length2;
	glGetProgramiv(shader_program, GL_PROGRAM_BINARY_LENGTH, &length1);
	std::vector<unsigned char> binary(sizeof(GLenum)+length1);
	glGetProgramBinary(shader_program, length1, &length2,
					   reinterpret_cast<GLenum*>(binary.data()),
					   binary.data()+sizeof(GLenum));
	return binary;
}

void ShaderProgram::loadBinary(std::vector<unsigned char> &binary)
{
	if(compiled)
	{
		deleteProgram();
		vertex_source.clear();
		geometry_source.clear();
		fragment_source.clear();
		has_fragment_shader = false;
		has_vertex_shader = false;
		has_geometry_shader = false;
	}
	
	shader_program = glCreateProgram();
	glProgramBinary(shader_program,
					*reinterpret_cast<GLenum*>(binary.data()),
					binary.data()+sizeof(GLenum),
					binary.size()-sizeof(GLenum));
	check_program_link_status(shader_program);
	compiled = true;
	linked = true;
}

}
