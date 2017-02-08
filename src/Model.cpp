//
//  Model.cpp
//  ogl4
//
//  Created by Philipp Lensing on 21.09.16.
//  Copyright © 2016 Philipp Lensing. All rights reserved.
//

#include "Model.h"
#include "phongshader.h"
#include <list>
#include <assert.h>
#include <fstream>

#define MODEL_BOX_SIZE 5.0f

Model::Model() : pMeshes(NULL), MeshCount(0), pMaterials(NULL), MaterialCount(0)
{
}

Model::Model(const char* ModelFile, bool FitSize) : pMeshes(NULL), MeshCount(0), pMaterials(NULL), MaterialCount(0)
{
	bool ret = load(ModelFile, FitSize);
	if (!ret) throw std::exception();
}

Model::~Model()
{
	delete[] pMeshes;
	delete[] pMaterials;
	deleteNodes(&RootNode);
}

void Model::deleteNodes(Node* pNode)
{
	if (!pNode)
		return;
	for (unsigned int i = 0; i < pNode->ChildCount; ++i)
		deleteNodes(&(pNode->Children[i]));
	if (pNode->ChildCount > 0)
		delete[] pNode->Children;
	if (pNode->MeshCount > 0)
		delete[] pNode->Meshes;
}

bool Model::load(const char* ModelFile, bool FitSize)
{
	const aiScene* pScene = aiImportFile(ModelFile, aiProcessPreset_TargetRealtime_Fast | aiProcess_TransformUVCoords | aiProcess_FlipUVs);

	if (pScene == NULL || pScene->mNumMeshes <= 0) return false;

	Filepath = ModelFile;
	Path = Filepath;
	size_t pos = Filepath.rfind('/');
	if (pos == std::string::npos)
		pos = Filepath.rfind('\\');
	if (pos != std::string::npos)
		Path.resize(pos + 1);

	loadMeshes(pScene, FitSize);
	loadMaterials(pScene);
	loadNodes(pScene);

	aiReleaseImport(pScene);

	return true;
}

void Model::loadMeshes(const aiScene* pScene, bool FitSize)
{
	float factor = 1.0f;
	if (FitSize) {
		calcBoundingBox(pScene, BoundingBox);
		Vector size = BoundingBox.size();
		factor = MODEL_BOX_SIZE / std::max({ size.X, size.Y, size.Z });
	}

	MeshCount = pScene->mNumMeshes;
	pMeshes = new Mesh[pScene->mNumMeshes];

	for (unsigned int i = 0; i < pScene->mNumMeshes; i++) {

		Mesh& m = pMeshes[i];
		aiMesh& aim = *pScene->mMeshes[i];

		m.MaterialIdx = aim.mMaterialIndex;

		m.VB.begin();

		for (unsigned int j = 0; j < aim.mNumVertices; j++) {
			// Normale
			if (aim.HasNormals()) m.VB.addNormal(convertVec(aim.mNormals[j]));
			// Textur koordinaten
			if (aim.HasTextureCoords(0) && aim.mNumUVComponents[0] == 2) {
				m.VB.addTexcoord0(convertVec(aim.mTextureCoords[0][j]));
			}
			// Vertex
			m.VB.addVertex(convertVec(aim.mVertices[j]) * factor);
		}

		m.VB.end();

		m.IB.begin();

		for (unsigned int j = 0; j < aim.mNumFaces; j++) {
			assert(aim.mFaces[j].mNumIndices == 3);

			m.IB.addIndex(aim.mFaces[j].mIndices[0]);
			m.IB.addIndex(aim.mFaces[j].mIndices[1]);
			m.IB.addIndex(aim.mFaces[j].mIndices[2]);
		}

		m.IB.end();
	}
}

void Model::loadMaterials(const aiScene* pScene)
{

	MaterialCount = pScene->mNumMaterials;
	pMaterials = new Material[pScene->mNumMaterials];

	for (unsigned int i = 0; i < pScene->mNumMaterials; i++) {

		Material& m = pMaterials[i];
		aiMaterial& aim = *pScene->mMaterials[i];

		aiColor3D diff;
		aiColor3D spec;
		aiColor3D amb;
		float exp = 0.0f;

		aim.Get(AI_MATKEY_COLOR_DIFFUSE, diff);
		aim.Get(AI_MATKEY_COLOR_SPECULAR, spec);
		aim.Get(AI_MATKEY_COLOR_AMBIENT, amb);
		aim.Get(AI_MATKEY_SHININESS, exp);

		m.DiffColor = Color(diff.r, diff.g, diff.b);
		m.SpecColor = Color(spec.r, spec.g, spec.b);
		m.AmbColor = Color(amb.r, amb.g, amb.b);

		if (aim.GetTextureCount(aiTextureType_DIFFUSE) == 1) {

			aiString aiTexName;
			aim.Get(AI_MATKEY_TEXTURE(aiTextureType_DIFFUSE, 0), aiTexName);

			std::string texName(aiTexName.C_Str());
			std::string texPath = Path + texName;

			m.DiffTex = Texture::LoadShared(texPath.c_str());
		}
	}

}

void Model::calcBoundingBox(const aiScene* pScene, AABB& Box)
{

	float maxf = std::numeric_limits<float>::max();
	float minf = std::numeric_limits<float>::min();

	Vector min(maxf, maxf, maxf);
	Vector max(minf, minf, minf);

	for (unsigned int i = 0; i < pScene->mNumMeshes; i++) {

		aiMesh& aim = *pScene->mMeshes[i];

		for (unsigned int j = 0; j < aim.mNumVertices; j++) {
			aiVector3D& v = aim.mVertices[j];
			min.X = std::min(min.X, v.x);
			min.Y = std::min(min.Y, v.y);
			min.Z = std::min(min.Z, v.z);

			max.X = std::max(max.X, v.x);
			max.Y = std::max(max.Y, v.y);
			max.Z = std::max(max.Z, v.z);
		}
	}

	Box.Min = min;
	Box.Max = max;

}

void Model::loadNodes(const aiScene* pScene)
{
	deleteNodes(&RootNode);
	copyNodesRecursive(pScene->mRootNode, &RootNode);
}

void Model::copyNodesRecursive(const aiNode* paiNode, Node* pNode)
{
	pNode->Name = paiNode->mName.C_Str();
	pNode->Trans = convertMat(paiNode->mTransformation);

	if (paiNode->mNumMeshes > 0)
	{
		pNode->MeshCount = paiNode->mNumMeshes;
		pNode->Meshes = new int[pNode->MeshCount];
		for (unsigned int i = 0; i < pNode->MeshCount; ++i)
			pNode->Meshes[i] = (int)paiNode->mMeshes[i];
	}

	if (paiNode->mNumChildren <= 0) return;

	pNode->ChildCount = paiNode->mNumChildren;
	pNode->Children = new Node[pNode->ChildCount];
	for (unsigned int i = 0; i < paiNode->mNumChildren; ++i)
	{
		copyNodesRecursive(paiNode->mChildren[i], &(pNode->Children[i]));
		pNode->Children[i].Parent = pNode;
	}
}

void Model::applyMaterial(unsigned int index)
{
	if (index >= MaterialCount)
		return;

	PhongShader* pPhong = dynamic_cast<PhongShader*>(shader());
	if (!pPhong) {
		//std::cout << "Model::applyMaterial(): WARNING Invalid shader-type. Please apply PhongShader for rendering models.\n";
		return;
	}

	Material* pMat = &pMaterials[index];
	pPhong->ambientColor(pMat->AmbColor);
	pPhong->diffuseColor(pMat->DiffColor);
	pPhong->specularExp(pMat->SpecExp);
	pPhong->specularColor(pMat->SpecColor);
	pPhong->diffuseTexture(pMat->DiffTex);
}

void Model::draw(const BaseCamera& Cam)
{
	if (!pShader) {
		std::cout << "BaseModel::draw() no shader found" << std::endl;
		return;
	}
	pShader->modelTransform(transform());

	std::list<Node*> DrawNodes;
	DrawNodes.push_back(&RootNode);

	while (!DrawNodes.empty())
	{
		Node* pNode = DrawNodes.front();
		Matrix GlobalTransform;

		if (pNode->Parent != NULL)
			pNode->GlobalTrans = pNode->Parent->GlobalTrans * pNode->Trans;
		else
			pNode->GlobalTrans = transform() * pNode->Trans;

		pShader->modelTransform(pNode->GlobalTrans);

		for (unsigned int i = 0; i < pNode->MeshCount; ++i)
		{
			Mesh& mesh = pMeshes[pNode->Meshes[i]];
			mesh.VB.activate();
			mesh.IB.activate();
			applyMaterial(mesh.MaterialIdx);
			pShader->activate(Cam);
			glDrawElements(GL_TRIANGLES, mesh.IB.indexCount(), mesh.IB.indexFormat(), 0);
			mesh.IB.deactivate();
			mesh.VB.deactivate();
		}
		for (unsigned int i = 0; i < pNode->ChildCount; ++i)
			DrawNodes.push_back(&(pNode->Children[i]));

		DrawNodes.pop_front();
	}
}

Matrix Model::convertMat(const aiMatrix4x4& m)
{
	return Matrix(m.a1, m.a2, m.a3, m.a4,
		m.b1, m.b2, m.b3, m.b4,
		m.c1, m.c2, m.c3, m.c4,
		m.d1, m.d2, m.d3, m.d4);
}

Vector Model::convertVec(const aiVector3D v) {
	return Vector(v.x, v.y, v.z);
}

