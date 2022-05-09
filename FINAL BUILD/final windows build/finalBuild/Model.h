#pragma once
#include <GL/glew.h>

namespace GE {

	struct Vertex {
		float x, y, z;
		float u, v;
		float normalX, normalY, normalZ;

		Vertex(float _x, float _y, float _z, float _u, float _v, float _normalX, float _normalY, float _normalZ) {
			x = _x;
			y = _y;
			z = _z;

			u = _u;
			v = _v;
			normalX = _normalX;
			normalY = _normalY;
			normalZ = _normalZ;

		}
		Vertex() {
			x = 0.0f;
			y = 0.0f;
			z = 0.0f;

			u = 0.0f;
			v = 0.0f;

			normalX = 0.0f;
			normalY = 0.0f;
			normalZ = 0.0f;
		}
	};

	class Model
	{
	public:
		Model() {
			vbo = 0;
			numVertices = 0;
		}
		~Model() {
			glDeleteBuffers(1, &vbo);
		}

		bool loadFromFile(const char* filename);

		GLuint getVerticies() {
			return vbo;
		}

		int getNumVertices() {
			return numVertices;
		}
	private:
		GLuint vbo;
		int numVertices;
	};
}

