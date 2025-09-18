#pragma once

#include <string>
#include <vector>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include "VertexBuffer.h"
#include "Texture.h"

class Model
{
private:
	std::vector<VertexBuffer*> meshes;
	std::vector<Texture*> textures;
	std::string path;
	std::vector<int> meshToTex;
 
	void loadModel();
	void loadNode(aiNode* node, const aiScene* scene);
	void loadMesh(aiMesh* mesh, const aiScene* scene);
	void loadMaterial(const aiScene* scene);

public:
	Model();

	Model(std::string& path);

	~Model();

	void renderModel();
	void clearModel();
};

