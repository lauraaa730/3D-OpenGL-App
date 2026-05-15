//----------------------------------------------------------------------------------------
/**
 * \file    singlemesh.cpp
 * \author  Laura Katerina Dudkova
 * \date    2026/05/11
 * \brief   Implementation of the SingleMesh class for textured objects and Assimp model loading.
 */

#include <iostream>
#include "singlemesh.h"


void SingleMesh::update(float elapsedTime, const glm::mat4* parentModelMatrix) {

	//handle animated UV coordinates for specific objects
	if (hasAnimatedTexture) {
		glm::mat4 matrix = glm::mat4(1.0f); //reset
		matrix = glm::translate(matrix, glm::vec3(0.0f, elapsedTime * -0.5f, 0.0f));
		matrix = glm::scale(matrix, glm::vec3(sin(elapsedTime) * 0.2f + 1.0)); //to scale from 0.8 to 1.2
		setUVMatrix(matrix);
		setAlpha(0.5f);
	}

	//propagate the update to the base class and children
	ObjectInstance::update(elapsedTime, parentModelMatrix);
}

void SingleMesh::draw(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix)
{
	if (initialized && (shaderProgram != nullptr)) {
		glUseProgram(shaderProgram->program);

		glm::mat4 PVM = projectionMatrix * viewMatrix * globalModelMatrix;

		//send material uniforms
		glUniform1i(shaderProgram->locations.hasTexture, 1); //singlemeshes will always have texture
		glUniform3fv(shaderProgram->locations.matDiffuse, 1, glm::value_ptr(material->diffuse));
		glUniform3fv(shaderProgram->locations.matAmbient, 1, glm::value_ptr(material->ambient));
		glUniform3fv(shaderProgram->locations.matSpecular, 1, glm::value_ptr(material->specular));
		glUniform1f(shaderProgram->locations.matShininess, material->shininess);

		//calculate normal matrix for view-space lighting
		glm::mat4 normalMat4 = glm::transpose(glm::inverse(globalModelMatrix));
		glm::mat3 normalMat3 = glm::mat3(normalMat4);

		//send transformation matrices
		glUniformMatrix3fv(shaderProgram->locations.normalMatrix, 1, GL_FALSE, glm::value_ptr(normalMat3));
		glUniformMatrix4fv(shaderProgram->locations.PVMmatrix, 1, GL_FALSE, glm::value_ptr(PVM));
		glUniformMatrix4fv(shaderProgram->locations.Mmatrix, 1, GL_FALSE, glm::value_ptr(globalModelMatrix));
		glUniformMatrix4fv(shaderProgram->locations.UVMatrix, 1, GL_FALSE, glm::value_ptr(UVMatrix));

		//bind texture to sampler
		if (textureID != 0) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, textureID);
			glUniform1i(shaderProgram->locations.texSampler, 0); //tell sampler to use GL_TEXTURE0
		}

		//draw object
		glBindVertexArray(geometry->vertexArrayObject);
		glDrawElements(GL_TRIANGLES, geometry->numTriangles * 3, GL_UNSIGNED_INT, 0);

		glBindVertexArray(0);
	}
	else {
		std::cerr << "SingleMesh::draw(): Can't draw, mesh not initialized properly!" << std::endl;
	}
}

bool SingleMesh::loadSingleMesh(const std::string& fileName, ShaderProgram* shader, ObjectGeometry** geometry, ObjectMaterial** material) {
	Assimp::Importer importer;

	//unitize object in size (scale the model to fit into (-1..1)^3)
	importer.SetPropertyInteger(AI_CONFIG_PP_PTV_NORMALIZE, 1);

	//load asset from the file
	const aiScene* scn = importer.ReadFile(fileName.c_str(), 0
		| aiProcess_Triangulate             // triangulate polygons
		| aiProcess_PreTransformVertices    // transforms scene hierarchy into one root
		| aiProcess_GenSmoothNormals        // calculate normals per vertex
		| aiProcess_JoinIdenticalVertices);

	//abort if the loader fails
	if (scn == NULL) {
		std::cerr << "SingleMesh::loadSingleMesh(): assimp error - " << importer.GetErrorString() << std::endl;
		*geometry = NULL;
		return false;
	}

	*geometry = new ObjectGeometry;
	*material = new ObjectMaterial;

	//aggregate data from all meshes into these vectors before uploading to OpenGL
	std::vector<float> allVertices;
	std::vector<unsigned int> allIndices;
	std::vector<float> allNormals;
	std::vector<float> allTexCoords;

	//keeps track of index offsets so triangles don't break across separate sub-meshes
	unsigned int vertexOffset = 0;

	//loop through all sub-meshes and combine their data into the master vectors
	for (unsigned int m = 0; m < scn->mNumMeshes; ++m) {
		const aiMesh* mesh = scn->mMeshes[m];

		for (unsigned int i = 0; i < mesh->mNumVertices; ++i) {
			//positions
			allVertices.push_back(mesh->mVertices[i].x);
			allVertices.push_back(mesh->mVertices[i].y);
			allVertices.push_back(mesh->mVertices[i].z);

			//normals
			if (mesh->HasNormals()) {
				allNormals.push_back(mesh->mNormals[i].x);
				allNormals.push_back(mesh->mNormals[i].y);
				allNormals.push_back(mesh->mNormals[i].z);
			}

			//texture coordinates
			if (mesh->HasTextureCoords(0)) {
				allTexCoords.push_back(mesh->mTextureCoords[0][i].x);
				allTexCoords.push_back(mesh->mTextureCoords[0][i].y);
			}
			else {
				//fallback to keep arrays aligned if a sub-mesh lacks UVs
				allTexCoords.push_back(0.0f);
				allTexCoords.push_back(0.0f);
			}
		}

		//indices (offsetting by the number of vertices we've already processed)
		for (unsigned int f = 0; f < mesh->mNumFaces; ++f) {
			allIndices.push_back(mesh->mFaces[f].mIndices[0] + vertexOffset);
			allIndices.push_back(mesh->mFaces[f].mIndices[1] + vertexOffset);
			allIndices.push_back(mesh->mFaces[f].mIndices[2] + vertexOffset);
		}

		vertexOffset += mesh->mNumVertices;
	}

	//load material ---
	//always grab the material from the 0th mesh as a baseline
	const aiMaterial* mat = scn->mMaterials[scn->mMeshes[0]->mMaterialIndex];
	aiColor3D color(0.0f, 0.0f, 0.0f);

	//ambient
	if (mat->Get(AI_MATKEY_COLOR_AMBIENT, color) == AI_SUCCESS)
		(*material)->ambient = glm::vec3(color.r, color.g, color.b);
	else
		(*material)->ambient = glm::vec3(0.1f, 0.1f, 0.1f);

	//diffuse
	if (mat->Get(AI_MATKEY_COLOR_DIFFUSE, color) == AI_SUCCESS)
		(*material)->diffuse = glm::vec3(color.r, color.g, color.b);
	else
		(*material)->diffuse = glm::vec3(1.0f, 1.0f, 1.0f);

	//specular
	if (mat->Get(AI_MATKEY_COLOR_SPECULAR, color) == AI_SUCCESS)
		(*material)->specular = glm::vec3(color.r, color.g, color.b);
	else
		(*material)->specular = glm::vec3(0.5f, 0.5f, 0.5f);

	//shininess
	float shininess = 32.0f;
	mat->Get(AI_MATKEY_SHININESS, shininess);
	(*material)->shininess = shininess;


	//start loading to buffers ---
	//VAO
	glGenVertexArrays(1, &((*geometry)->vertexArrayObject));
	glBindVertexArray((*geometry)->vertexArrayObject);

	//1. position attribute
	bool validInit = false;

	//vbo
	glGenBuffers(1, &((*geometry)->vertexBufferObject));
	glBindBuffer(GL_ARRAY_BUFFER, (*geometry)->vertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)* allVertices.size(), allVertices.data(), GL_STATIC_DRAW);

	if ((shaderProgram != nullptr) && shaderProgram->initialized && (shaderProgram->locations.position != -1)) {

		glEnableVertexAttribArray(shader->locations.position);
		glVertexAttribPointer(shader->locations.position,
			3,
			GL_FLOAT,
			GL_FALSE,
			0,
			0);

		CHECK_GL_ERROR();

		validInit = true;
	}

	//ebo
	glGenBuffers(1, &((*geometry)->elementBufferObject));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, (*geometry)->elementBufferObject);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * allIndices.size(), allIndices.data(), GL_STATIC_DRAW);


	//2. normal attribute
	if (!allNormals.empty()) {

		glGenBuffers(1, &((*geometry)->normalBufferObject));
		glBindBuffer(GL_ARRAY_BUFFER, (*geometry)->normalBufferObject);
		glBufferData(GL_ARRAY_BUFFER, allNormals.size() * sizeof(float), allNormals.data(), GL_STATIC_DRAW);

		//connect to shader's normal attribute
		if (shader->locations.normal != -1) {
			glEnableVertexAttribArray(shader->locations.normal);
			glVertexAttribPointer(shader->locations.normal,
				3,
				GL_FLOAT,
				GL_FALSE,
				0,
				0);

			CHECK_GL_ERROR();
		}
	}

	//3. texture coordinates attribute
	if (!allTexCoords.empty()) {

		//create vbo for texture coordinates
		glGenBuffers(1, &((*geometry)->texCoordBufferObject));
		glBindBuffer(GL_ARRAY_BUFFER, (*geometry)->texCoordBufferObject);
		glBufferData(GL_ARRAY_BUFFER, allTexCoords.size() * sizeof(float), allTexCoords.data(), GL_STATIC_DRAW);

		//connect to shader's texCoord attribute
		if (shader->locations.texCoord != -1) {
			glEnableVertexAttribArray(shader->locations.texCoord);
			glVertexAttribPointer(shader->locations.texCoord, 2, GL_FLOAT, GL_FALSE, 0, 0);
		}
	}
	else {
		std::cout << "Warning: Loaded model has no texture coordinates!" << std::endl;
	}

	glBindVertexArray(0);

	//count is now total aggregated indices / 3
	(*geometry)->numTriangles = allIndices.size() / 3;

	return validInit;
}


SingleMesh::SingleMesh(const std::string& fileName, const std::string& textureName, ShaderProgram* shdrPrg) : ObjectInstance(shdrPrg), initialized(false)
{
	textureID = pgr::createTexture(textureName);
	if (!textureID) {
		std::cerr << "SingleMesh::SingleMesh(): couldnt create texture!" << std::endl;
	}
	if (!loadSingleMesh(fileName, shdrPrg, &geometry, &material)) {
		if (geometry == nullptr) {
			std::cerr << "SingleMesh::SingleMesh(): geometry not initialized!" << std::endl;
		}
		else {
			std::cerr << "SingleMesh::SingleMesh(): shaderProgram struct not initialized!" << std::endl;
		}
	}
	else {
		if ((shaderProgram != nullptr) && shaderProgram->initialized && (shaderProgram->locations.PVMmatrix != -1)) {
			initialized = true;
		}
		else {
			std::cerr << "SingleMesh::SingleMesh(): shaderProgram struct not initialized!" << std::endl;
		}
	}
}

SingleMesh::~SingleMesh() {

	if (geometry != nullptr) {
		glDeleteVertexArrays(1, &(geometry->vertexArrayObject));
		glDeleteBuffers(1, &(geometry->elementBufferObject));
		glDeleteBuffers(1, &(geometry->vertexBufferObject));
		glDeleteBuffers(1, &(geometry->normalBufferObject));
		glDeleteBuffers(1, &(geometry->texCoordBufferObject));

		delete geometry;
		geometry = nullptr;
	}

	if (material != nullptr) {
		delete material;
		material = nullptr;
	}

	initialized = false;
}