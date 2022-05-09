#include "instancingRenderer.h"
#include "ModelRenderer.h"
#include <iostream>
#include <glm/gtc/type_ptr.hpp>
#include "Utils.h"

namespace GE {

	instancingRenderer::instancingRenderer()
	{
		/*pos_x = pos_y = pos_z = 0.0f;
		rot_x = rot_y = rot_z = 0.0f;
		scale_x = scale_y = scale_z = 1.0f;*/

		programId = 0;
		vertexPos3DLocation = 0;
		vertexUVLocation = 0;
		vetexNormalLocation = 0;
		instatanceTransLocation = 0;

	
		viewUninformId = 0;
		projectionUniformId = 0;


	}

	instancingRenderer::~instancingRenderer()
	{
	}

	//void displayShaderCompilerError(GLint shaderID) {   // ---------------need to make this function accesable by other classes so they can also report errors
	//	// the length of the error message
	//	GLint msgLen = 0;

	//	glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &msgLen);

	//	// only display error if more then 1 char was returned
	//	if (msgLen > 1) {
	//		// creates a msg buffer to store the msg
	//		GLchar* msg = new GLchar[msgLen + 1];

	//		// get the error msg from OpenGL to the msg buffer
	//		glGetShaderInfoLog(shaderID, msgLen, NULL, msg);

	//		// display the error

	//		std::cerr << "Error compiling shader: " << msg << std::endl;

	//		// deletes the memorry for the msg
	//		delete[] msg;

	//		////////-------------------------------------------------------------------------- test
	//	}
	//}

	void instancingRenderer::Init()
	{
		// create the vertext shader
		GLint vertexShader = glCreateShader(GL_VERTEX_SHADER);

		// shader code
		const GLchar* V_ShaderCode[] = {
			"#version 330\n"
			"in vec3 vertexPos3D;\n"
			"in vec2 vUV;\n"
			"in vec3 vertexNormal;\n"
			"out vec2 UV;\n"
			"out vec3 out_normal;\n"
			"out vec3 fragPosition;\n"
			"out float fog_amount;\n"

			"in mat4 instance_transform;\n"
			//"uniform mat4 transform;\n"
			"uniform mat4 view;\n"
			"uniform mat4 projection;\n"
			"uniform float fog_start;\n"
			"uniform float fog_range;\n"
			"uniform float fog_density;\n"
			"void main(){\n"
			"vec4 v = vec4(vertexPos3D.xyz, 1);\n"
			"vec4 posInWorld = instance_transform * v;\n"
			"v = projection * view * posInWorld;\n"
			"gl_Position = v;\n"
			"UV = vUV;\n"
			"out_normal = vec3(instance_transform * vec4(vertexNormal, 1.0f));\n"
			"fragPosition = vec3(instance_transform * v);\n"
			// fog calculation
			"vec4 pos_rel_eye = view * posInWorld;\n"
			"float distance = length(pos_rel_eye.xyz);\n"
			// linear fog
			//"fog_amount = (distance - fog_start) / fog_range;\n"
			// exponential fog
			"fog_amount = 1.0f - exp(-distance * fog_density);\n"
			"fog_amount = clamp(fog_amount,0.0f,1.0f);\n"
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
			//displayShaderCompilerError(vertexShader);

			return;
		}

		// Do the same for the fragment shader
		GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		// Fragment shader source code
		const GLchar* F_ShaderCode[] = {
		  "#version 330\n"
		  "in vec2 UV;\n"
		  "in vec3 fragPosition;\n"
		  "in vec3 out_normal;\n"
		  "in float fog_amount;\n"
		  "out vec4 fragmentColor;\n"
		  "uniform sampler2D sampler;\n"
		  "uniform vec3 lightColour;\n"
		  "uniform vec3 lightPosition;\n"
		  "uniform vec3 viewPosition;\n"
		  "uniform vec3 fog_colour;\n"
		  "void main()\n"
		  "{\n"
			// Ambient Light
			"float ambientStrength = 1.0f;\n"
			"vec3 ambient = ambientStrength * lightColour;\n"
			// Diffuse Lighting
			"vec3 normal = normalize(out_normal);\n"
			"vec3 lightDirection = normalize(lightPosition - fragPosition);\n"
			"float diff = max(dot(normal, lightDirection), 0.0f);\n"
			"vec3 diffuse = diff * lightColour;\n"
			// Specular Lighting
			"float specularStrength = 0.2f;\n"
			"vec3 viewDir = normalize(viewPosition - fragPosition);\n"
			"vec3 reflectDirection = reflect(-lightDirection, normal);\n"
			"float spec = pow(max(dot(viewDir, reflectDirection), 0.0f), 32);\n"
			"vec3 specular = specularStrength * spec * lightColour;\n"
			// Texture
			"vec3 textureColour = texture(sampler, UV).xyz;\n"
			// Combining them together
			"vec3 result = (ambient + diffuse) * textureColour + specular;\n"
			"fragmentColor = vec4(result, 1.0f);\n"
			"fragmentColor = mix(fragmentColor, vec4(fog_colour, 1.0f), fog_amount);\n"
			//"fragmentColour = texture(sampler, UV).rgba;\n"
		  "}\n"
		};

		glShaderSource(fragmentShader, 1, F_ShaderCode, NULL);

		// Compile it

		glCompileShader(fragmentShader);

		// Check for errors. Code is same as above of getting status
		// and displaying error message, if necessary

		isShaderCompiledOK = GL_FALSE;
		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isShaderCompiledOK);

		if (isShaderCompiledOK != GL_TRUE) {
			std::cerr << "Unable to compile fragment shader" << std::endl;
			//displayShaderCompilerError(fragmentShader);
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
		// Now get a link to the vertexPos3D so we can link the attribute
		// to our vertices when rendering
		vertexPos3DLocation = glGetAttribLocation(programId, "vertexPos3D");

		// Check for errors
		if (vertexPos3DLocation == -1) {
			std::cerr << "Problem getting vertex3DPos" << std::endl;
		}

		vertexUVLocation = glGetAttribLocation(programId, "vUV");

		if (vertexUVLocation == -1) {
			std::cerr << "Problem getting vUV" << std::endl;
		}

		vetexNormalLocation = glGetAttribLocation(programId, "vertexNormal");

		if (vetexNormalLocation == -1) {
			std::cerr << "Problem getting vertexNormal" << std::endl;
		}

		instatanceTransLocation = glGetAttribLocation(programId, "instance_transform");

		if (instatanceTransLocation == -1) {
			std::cerr << "Problem getting transfrom location" << std::endl;
		}


		// links the uniforms to the memebr fields
		//transformUniformId = glGetUniformLocation(programId, "transform");
		viewUninformId = glGetUniformLocation(programId, "view");
		projectionUniformId = glGetUniformLocation(programId, "projection");
		samplerId = glGetUniformLocation(programId, "sampler");

		// set light
		lightColourId = glGetUniformLocation(programId, "lightColour");
		lightPositionId = glGetUniformLocation(programId, "lightPosition");
		viewPositionId = glGetUniformLocation(programId, "viewPosition");

		// set fog
		fogColourId = glGetUniformLocation(programId, "fog_colour");
		fogStartId = glGetUniformLocation(programId, "fog_start");
		fogStartId = glGetUniformLocation(programId, "fog_range");
		fogDensityId = glGetUniformLocation(programId, "fog_density");

		glUseProgram(programId);

		glUniform1f(fogStartId, 200.0f);
		glUniform1f(fogRangeId, 1000.0f);
		// used for expoential
		glUniform1f(fogDensityId, 0.006);
		glm::vec3 fog_colour = glm::vec3(0.8470f, 0.72549f, 0.3961f);
		glUniform3fv(fogColourId, 1, glm::value_ptr(fog_colour));
		glUseProgram(0);

	}

	

	void instancingRenderer::instancedData(const std::vector<modelInstance>& instancePositions) {
		instance_matrix = new glm::mat4[instancePositions.size()];

		int index = 0;
		for (auto& instance : instancePositions) {
			// calculate the transformation matrix
			instance_matrix[index] = glm::mat4(1.0f);
			instance_matrix[index] = glm::translate(instance_matrix[index], glm::vec3(instance.posX , instance.posY, instance.posZ));
			instance_matrix[index] = glm::rotate(instance_matrix[index], glm::radians(instance.rotX), glm::vec3(1.0f, 0.0f, 0.0f));
			instance_matrix[index] = glm::rotate(instance_matrix[index], glm::radians(instance.rotY), glm::vec3(0.0f, 1.0f, 0.0f));
			instance_matrix[index] = glm::rotate(instance_matrix[index], glm::radians(instance.rotZ), glm::vec3(0.0f, 0.0f, 1.0f));
			instance_matrix[index] = glm::scale(instance_matrix[index], glm::vec3(instance.scaleX, instance.scaleY, instance.scaleZ));

			index++;
		}
		glGenBuffers(1, &instanceMatBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, instanceMatBuffer);
		glBufferData(GL_ARRAY_BUFFER, instancePositions.size() * sizeof(glm::mat4), &instance_matrix[0], GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void instancingRenderer::Draw(Camera* cam, Model* model, std::vector<modelInstance> instancePositions)
	{
		glEnable(GL_CULL_FACE);

		
		

		glm::mat4 viewMat = cam->getViewMatrix();
		glm::mat4 projectionMat = cam->getProjectionMatrix();

		std::cout << glGetError();

		// Select the program into the rendering context
		glUseProgram(programId);

		// set the uniforms in the shader
		//glUniformMatrix4fv(transformUniformId, 1, GL_FALSE, glm::value_ptr(transformationMat));
		glUniformMatrix4fv(viewUninformId, 1, GL_FALSE, glm::value_ptr(viewMat));
		glUniformMatrix4fv(projectionUniformId, 1, GL_FALSE, glm::value_ptr(projectionMat));

		// Select the vertex buffer object into the context
		glBindBuffer(GL_ARRAY_BUFFER, model->getVerticies());

		// Define the structure of a vertex for OpenGL to select values from vertex buffer
		// and store in vertexPos3DLocation attribute
		// and store in the vcolour attribute
		glEnableVertexAttribArray(vertexPos3DLocation);
		glVertexAttribPointer(vertexPos3DLocation, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, x));

		glEnableVertexAttribArray(vertexUVLocation);
		glVertexAttribPointer(vertexUVLocation, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, u));

		glEnableVertexAttribArray(vetexNormalLocation);
		glVertexAttribPointer(vetexNormalLocation, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normalX));


		glBindBuffer(GL_ARRAY_BUFFER, 0);
		
		
		

		glUniform3f(lightColourId, 255.0f / 255.0f, 255.0f / 255.0f, 255.0f / 255.0f);
		glUniform3f(lightPositionId, 0.0f, 1000.0f, 6000.0f);
		glUniform3f(viewPositionId, cam->getPosX(), cam->getPosY(), cam->getPosZ());


		glBindBuffer(GL_ARRAY_BUFFER, instanceMatBuffer);

		glEnableVertexAttribArray(instatanceTransLocation);
		glVertexAttribPointer(instatanceTransLocation, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4) * 4, (void*)0);

		glEnableVertexAttribArray(instatanceTransLocation + 1);
		glVertexAttribPointer(instatanceTransLocation + 1, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4) * 4, (void*)(sizeof(glm::vec4)));

		glEnableVertexAttribArray(instatanceTransLocation + 2);
		glVertexAttribPointer(instatanceTransLocation + 2, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4) * 4, (void*)(2 * sizeof(glm::vec4)));

		glEnableVertexAttribArray(instatanceTransLocation + 3);
		glVertexAttribPointer(instatanceTransLocation + 3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4) * 4, (void*)(3 * sizeof(glm::vec4)));

		

		glVertexAttribDivisor(instatanceTransLocation, 1);
		glVertexAttribDivisor(instatanceTransLocation + 1, 1);
		glVertexAttribDivisor(instatanceTransLocation + 2, 1);
		glVertexAttribDivisor(instatanceTransLocation + 3, 1);

		glBindBuffer(GL_ARRAY_BUFFER, 0);

		// selet the texture
		glActiveTexture(GL_TEXTURE0);
		glUniform1i(samplerId, 0);
		glBindTexture(GL_TEXTURE_2D, material->getTextureName());

		// Draw the triangle
		//glDrawArrays(GL_TRIANGLES, 0, model->getNumVertices());
		glDrawArraysInstanced(GL_TRIANGLES, 0, model->getNumVertices(), instancePositions.size()-1);
		// Unselect the attribute from the context
		glBindTexture(GL_TEXTURE_2D, 0);

		glDisableVertexAttribArray(vertexPos3DLocation);
		glDisableVertexAttribArray(vertexUVLocation);
		glDisableVertexAttribArray(vetexNormalLocation);

		glDisableVertexAttribArray(instatanceTransLocation);
		glDisableVertexAttribArray(instatanceTransLocation + 1);
		glDisableVertexAttribArray(instatanceTransLocation + 2);
		glDisableVertexAttribArray(instatanceTransLocation + 3);
		// Unselect the program from the context
		glUseProgram(0);

		glDisable(GL_CULL_FACE);
	}
	void instancingRenderer::Destroy()
	{
		glDeleteProgram(programId);

	}
}