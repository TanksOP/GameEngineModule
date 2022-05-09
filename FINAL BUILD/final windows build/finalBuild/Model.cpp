#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include<vector>
#include <iostream>
#include "Model.h"

namespace GE {
	bool Model::loadFromFile(const char* filename) {
		// temp vector to hold vertices from the file
		std::vector<Vertex> loadedVertices;

		// create importer
		Assimp::Importer imp;

		// load the model into scene obj
		const aiScene* pScene = imp.ReadFile(filename, aiProcessPreset_TargetRealtime_Quality | aiProcess_FlipUVs);

		// check it was opened ok
		if (!pScene) {
			std::cerr << "There was a problem opening the file" << std::endl;
			return false;
		}

		// uses three loops
		// process all of the meshes
		// process the faces in the mesh
		// process vertices in the faces

		// get the vertices from the model
		for (int MeshIdx = 0; MeshIdx < pScene->mNumMeshes; MeshIdx++) {
			const aiMesh* mesh = pScene->mMeshes[MeshIdx];

			for (int faceIdx = 0; faceIdx < mesh->mNumFaces; faceIdx++) {
				const aiFace& face = mesh->mFaces[faceIdx];
				//extract a vertex from the mesh's vertx array for each point in the face
				// 3 as there are three verticies in a triagle face
				for (int vertIdx = 0; vertIdx < 3; vertIdx++) {
					// extract the pos and texture co-ord from the index number
					const aiVector3D* pos = &mesh->mVertices[face.mIndices[vertIdx]];


					// get the normals
					aiVector3D normal = { 0,0,0 };
					if (mesh->HasNormals()) {
						normal = mesh->mNormals[MeshIdx];
					}

					// get uvs for vertex					
					aiVector3D uv = { 0,0,0 };
					if (mesh->HasTextureCoords(0)) {
						 uv = mesh->mTextureCoords[0][face.mIndices[vertIdx]];
					}
					

					// creates a new object in shap array bassed on extract vertex's
					loadedVertices.emplace_back(Vertex(pos->x, pos->y, pos->z, uv.x, uv.y, normal.x, normal.y, normal.z));
				}
			}
		}
		// creates a vertex buffer for rendeerign from the vertex array

		numVertices = loadedVertices.size();
		
		// copy the vertices into a memory buffer
		glGenBuffers(1, &vbo);

		// create the buffer
		glBindBuffer(GL_ARRAY_BUFFER, vbo);

		// copy vertex data to the buffer
		glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(Vertex), loadedVertices.data(), GL_STATIC_DRAW);

		// removes the buffer
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		return true;

	}

}
