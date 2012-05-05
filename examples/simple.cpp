#define GL_GLEXT_PROTOTYPES
#include <GL/glfw.h>

#include <GL/gl.h>
#include <GL/glu.h>

#include <vector>
#include <iostream>
#include <string>

#include <boost/fusion/include/vector.hpp>

// defining GLP_DEBUG results in a lot of OpenGL error checks in the
// wrapper.
//#define GLP_DEBUG

#include "MathVector.h"
#include "MathMatrix.h"
#include "GraphicsMatrices.h"

#include "GLShaderProgram.h"
#include "GLVertexBuffer.h"
#include "GLFrameBuffer.h"
#include "GLTexture.h"
#include "GLPixelBuffer.h"
#include "GLSyncQuery.h"
#include "GLVertexArray.h"
#include "GLQuery.h"
#include "GLCheckError.h"

namespace fusion = boost::fusion;

// define a vertex format. This can be any fusion sequence including
// an adapted struct
typedef fusion::vector<
			Vector<GLfloat,3>,
			Vector<GLfloat,3>,
			Vector<GLfloat,4>
		> PositionNormalColor;

int main(int argc, char *argv[])
{
	// create OpenGL context and window using GLFW (2.7+)
	glfwInit();
	
	int width = 1024;
	int height = 768;
	float aspect = float(width)/height;

	// we want a 3.3 core profile
	glfwOpenWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, 3);
	glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR, 3);
	
	glfwOpenWindow(width, height, 0, 0, 0, 0, 0, 0, GLFW_WINDOW);
	glfwSwapInterval(1); // vsync
	glfwDisable(GLFW_MOUSE_CURSOR);


	
	// create and fill a vertex buffer	
	glp::VertexBuffer<PositionNormalColor> vbo(4);
	vbo.map();
	vbo[0] = PositionNormalColor(	Vector<GLfloat,3>(-1,-1,0),
									Vector<GLfloat,3>(0,0,-1),
									Vector<GLfloat,4>(1,0,0,1));
									
	vbo[1] = PositionNormalColor(	Vector<GLfloat,3>( 1,-1,0),
									Vector<GLfloat,3>(0,0,-1),
									Vector<GLfloat,4>(0,1,0,1));
									
	vbo[2] = PositionNormalColor(	Vector<GLfloat,3>(-1, 1,0),
									Vector<GLfloat,3>(0,0,-1),
									Vector<GLfloat,4>(0,0,1,1));
									
	vbo[3] = PositionNormalColor(	Vector<GLfloat,3>( 1, 1,0),
									Vector<GLfloat,3>(0,0,-1),
									Vector<GLfloat,4>(0,0,0,1));
	vbo.unmap();

	// create and fill an index buffer
	glp::IndexBuffer<GLuint> ibo(6);
	ibo.map();
	ibo[0] = 0; ibo[1] = 1; ibo[2] = 2; // triangle 0
	ibo[3] = 2; ibo[4] = 1; ibo[5] = 3; // triangle 1
	ibo.unmap();

	// create a vertex array object and attach our vbo and ibo
	glp::VertexArray vao;
	vao.attach(vbo);
	vao.attach(ibo);
	
	// create a shader program...
	glp::ShaderProgram shader;
	
	// only shaders we define sources for will be attached
	
	shader.setVertexShaderSource(
		"#version 330\n"
		"in vec4 position;\n"
		"in vec3 normal;\n"
		"in vec4 color;\n"
		"out vec4 fcolor;\n"
		"void main() {\n"
		"	fcolor = color;\n"
		"	gl_Position = position;\n"
		"}\n"
	);
	
	shader.setFragmentShaderSource(
		"#version 330\n"
		"in vec4 fcolor;\n"
		"out vec4 FragColor;\n"
		"void main() {\n"
		"	FragColor = fcolor;\n"
		"}\n"
	);
	
	shader.compileProgram();
	
	// attribute locations get implicitly numbered from 0 by the
	// vbo wrapper unless otherwise specified (see setBaseAttrib)
	shader.bindAttributeLocation(0, "position");
	shader.bindAttributeLocation(1, "normal");
	shader.bindAttributeLocation(2, "color");
	
	// there is no gl_FragColor in opengl 3.3 core profile.
	shader.bindFragDataLocation(0, "FragColor");
	
	shader.linkProgram();

	

	bool running = true;

	while(running)
	{	
		float dt = 1/60.f;	

		if(glfwGetKey(GLFW_KEY_ESC))
		{
			running = false;
		}

		// draw our stuff
		shader.bindProgram();
		
		vao.draw(GL_TRIANGLES);
		
		// check for errors at least once a frame
		glp::checkGlErrors(); 
				
		glfwSwapBuffers();
		
	}

	glfwTerminate();
	return 0;
}
