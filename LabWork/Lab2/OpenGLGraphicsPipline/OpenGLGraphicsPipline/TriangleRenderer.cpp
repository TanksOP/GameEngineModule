#include "TriangleRenderer.h"
#include <iostream>

namespace GE {

	GLfloat vertexData[] = {

		-1.0f, 0.0f,
		1.0f, 0.0f,
		0.0f, 1.0f,

		-1.0f, 0.0f,
		0.0f, -1.0f,
		1.0f, 0.0f
	};

	TriangleRenderer::TriangleRenderer()
	{
	}

	TriangleRenderer::~TriangleRenderer()
	{
	}

	

	// helper function to display any shader compile errors if somthinggoes wrong
	void displayShaderCompilerError(GLint shaderID) {   // ---------------need to make this function accesable by other classes so they can also report errors
		// the length of the error message
		GLint msgLen = 0;

		glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &msgLen);

		// only display error if more then 1 char was returned
		if (msgLen > 1) {
			// creates a msg buffer to store the msg
			GLchar* msg = new GLchar[msgLen + 1];

			// get the error msg from OpenGL to the msg buffer
			glGetShaderInfoLog(shaderID, msgLen, NULL, msg);

			// display the error
			std::cerr << "here"<< std::endl;
			std::cerr << "Error compiling shader: " << msg << std::endl;

			// deletes the memorry for the msg
			delete[] msg;
		}
	}

	// creates and complies the shaders, creats projecrs links to it creats a vertix buffer obj
	void TriangleRenderer::Init()
	{
		// create the vertext shader
		GLint vertexShader = glCreateShader(GL_VERTEX_SHADER);

		// shader code
		const GLchar* V_ShaderCode[] = {
			"#version 140\n"
			"in vec2 vertexPos2D;\n"
			"void main(){\n"
			"gl_Position = vec4(vertexPos2D.x, vertexPos2D.y, 0, 1);\n"
			"}\n" };
		// copy the source to OpenGL for compliation
		glShaderSource(vertexShader, 1, V_ShaderCode, NULL);

		// compile the code
		glCompileShader(vertexShader);

		// check compile errors
		GLint isShaderCompiledOK = GL_FALSE;

		// get the compile status from OpenGL
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isShaderCompiledOK);

		// has the shader failed to compile
		if (isShaderCompiledOK != GL_TRUE) {
			std::cerr << "Unable to compile vertex shader" << std::endl;
			displayShaderCompilerError(vertexShader);

			return;
		}

		// Do the same for the fragment shader
		GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		// Fragment shader source code
		const GLchar* F_ShaderCode[] = {
		  "#version 140\n"
		  "out vec4 fragmentColour;\n"
		  "void main()\n"
		  "{\n"
		  "fragmentColour = vec4(1.0, 0.0, 0.0, 1.0);\n"
		  "}\n"
		};

		// Transfer the shader code

		glShaderSource(fragmentShader, 1, F_ShaderCode, NULL);

		// Compile it

		glCompileShader(fragmentShader);

		// Check for errors. Code is same as above of getting status
		// and displaying error message, if necessary

		isShaderCompiledOK = GL_FALSE;
		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isShaderCompiledOK);

		if (isShaderCompiledOK != GL_TRUE) {
			std::cerr << "Unable to compile fragment shader" << std::endl;
			displayShaderCompilerError(fragmentShader);
			return;
		}

		// Create the program object
		programId = glCreateProgram();

		// Attach shaders to the program object
		glAttachShader(programId, vertexShader);
		glAttachShader(programId, fragmentShader);

		// Now link the program to create an executable program we
		// and use to render the object
		// Program executable will exist in graphics memory

		glLinkProgram(programId);
		// Check for linking errors
		GLint isProgramLinked = GL_FALSE;
		glGetProgramiv(programId, GL_LINK_STATUS, &isProgramLinked);
		if (isProgramLinked != GL_TRUE) {
			std::cerr << "Failed to link program" << std::endl;
		}
		// Now get a link to the vertexPos2D so we can link the attribute
		// to our vertices when rendering
		vertexPos2DLocation = glGetAttribLocation(programId, "vertexPos2D");
		// Check for errors
		if (vertexPos2DLocation == -1) {
			std::cerr<< "Problem getting vertex2DPos" << std::endl;
		}
		// Create the vertex buffer object
		glGenBuffers(1, &vboTriangle);
		glBindBuffer(GL_ARRAY_BUFFER, vboTriangle);
		// Transfer vertices to graphics memory
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);
	}

	void TriangleRenderer::Update()
	{

	}
	void TriangleRenderer::Draw()
	{
		// Select the program into the rendering context
		glUseProgram(programId);
		// Define the structure of a vertex for OpenGL to select values from vertex buffer
		// and store in vertexPos2DLocation attribute
		glVertexAttribPointer(vertexPos2DLocation, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), nullptr);
		// Enable the attribute to be passed vertices from the vertex buffer object
		glEnableVertexAttribArray(vertexPos2DLocation);
		// Select the vertex buffer object into the context
		glBindBuffer(GL_ARRAY_BUFFER, vboTriangle);
		// Draw the triangle
		glDrawArrays(GL_TRIANGLES, 0, 6);
		// Unselect the attribute from the context
		glDisableVertexAttribArray(vertexPos2DLocation);
		// Unselect the program from the context
		glUseProgram(0);
	}

	void TriangleRenderer::Destroy()
	{
		glDeleteProgram(programId);
		glDeleteBuffers(1, &vboTriangle);
	}
}