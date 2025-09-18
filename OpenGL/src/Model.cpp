#include "Model.h"


Model::Model()
{
	path = "";
}

Model::Model(std::string& path)
{
	this->path = path;
	loadModel();
}


void Model::loadModel()
{
	Assimp::Importer importer;

	const aiScene* scene = importer.ReadFile(path, aiProcess_FlipUVs | aiProcess_GenSmoothNormals | aiProcess_Triangulate | aiProcess_JoinIdenticalVertices);

	if (!scene) {
		printf("%s", importer.GetErrorString());
		return;
	}
		
	aiNode* rootNode = scene->mRootNode;

	loadNode(rootNode, scene);

	loadMaterial(scene);

}

void Model::loadNode(aiNode* node, const aiScene* scene)
{
	for (size_t i = 0; i < node->mNumMeshes; i++) {
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		loadMesh(mesh, scene);
	}

	for (size_t i = 0; i < node->mNumChildren; i++) {
		aiNode* childNode = node->mChildren[i];
		loadNode(childNode, scene);
	}
}

void Model::loadMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<float> vertices;
	std::vector<unsigned int> indices;

	for (size_t i = 0; i < mesh->mNumVertices; i++) {
			
		vertices.insert(vertices.end(), { mesh->mVertices[i].x, mesh->mVertices[i].y,mesh->mVertices[i].z});

		if (mesh->mTextureCoords[0]) {
			vertices.insert(vertices.end(), { mesh->mTextureCoords[0][i].x,mesh->mTextureCoords[0][i].y});
		}
		else {
			vertices.insert(vertices.end(), { 0.0f,0.0f });
		}

		vertices.insert(vertices.end(), { mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z});

	}

	for (size_t i = 0; i < mesh->mNumFaces; i++) {
		
		aiFace face = mesh->mFaces[i];
		
		for (size_t j = 0; j < face.mNumIndices; j++) {
			indices.push_back(face.mIndices[j]);
		}
	}

	meshes.push_back(new VertexBuffer(&vertices[0], &indices[0], vertices.size(), indices.size()));

	meshToTex.push_back(mesh->mMaterialIndex);

}

void Model::loadMaterial(const aiScene* scene)
{
	
	for (size_t i = 0; i < scene->mNumMaterials; i++) {
		aiMaterial* material = scene->mMaterials[i];
		
		textures[i] = nullptr;

		if (material->GetTextureCount(aiTextureType_DIFFUSE)) {
			aiString aiPath;
			if (material->GetTexture(aiTextureType_DIFFUSE, 0, &aiPath) == AI_SUCCESS) {
				int index = std::string(aiPath.data).rfind("\\");
				std::string tName = std::string(aiPath.data).substr(index + 1);
				std::string tPath = std::string("C:/Users/Meate/source/repos/OpenGL/OpenGL/src/Textures/") + tName;
				Texture* texture = new Texture(tPath.c_str());

				if (texture->loadTexture()) {
					printf("Model texture %s is loaded sucessfully.",tName.c_str());
				}
				else{
					printf("Model texture %s can't be loaded. Using default texture.",tName.c_str());
					delete texture;
					texture = new Texture("C:/Users/Meate/source/repos/OpenGL/OpenGL/src/Textures/brick.png");
				}
				textures[i] = texture;
			}
		}

	}
}

void Model::renderModel()
{
	for (size_t i = 0; i < meshes.size(); i++) {
		unsigned int materialIndex = meshToTex[i];
		textures[materialIndex]->useTexture();
		meshes[i]->render();
	}
}

void Model::clearModel()
{
	for (int i = 0; i < meshes.size(); i++) {
		delete meshes[i];
		meshes[i] = nullptr;
	}

	for (int i = 0; i < textures.size(); i++) {
		delete textures[i];
		textures[i] = nullptr;
	}

	for (int i = 0; i < meshToTex.size(); i++) {
		meshToTex[i] = 0;
	}

}

Model::~Model()
{
	clearModel();

}