//
//  Model.hpp
//  ogl4
//
//  Created by Philipp Lensing on 21.09.16.
//  Copyright © 2016 Philipp Lensing. All rights reserved.
//

#ifndef Model_hpp
#define Model_hpp

#include "basemodel.h"
#include "vertexbuffer.h"
#include "indexbuffer.h"
#include "texture.h"
#include "aabb.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <string>

class Model : public BaseModel
{
public:
	Model();
	Model(const char* ModelFile, bool FitSize = false, float Size = 1.0);
	virtual ~Model();

	bool load(const char* ModelFile, bool FitSize = false, float Size = 1.0);
	virtual void draw(const BaseCamera& Cam);
	const AABB& boundingBox() const { return BoundingBox; }
	void overrideEmit(Color& c);
protected: // protected types
	struct Mesh
	{
		Mesh() {}
		VertexBuffer VB;
		IndexBuffer IB;
		int MaterialIdx;
	};
	struct Material
	{
		Material() :
			DiffTex(NULL),
			EmitTex(NULL),
			DiffColor(1, 1, 1),
			SpecColor(0.5f, 0.5f, 0.5f),
			AmbColor(0.2f, 0.2f, 0.2f),
			SpecExp(10.0f),
			EmitColor(0, 0, 0) {}
		Color DiffColor;
		Color SpecColor;
		Color AmbColor;
		float SpecExp;
		Color EmitColor;
		const Texture* DiffTex;
		const Texture* EmitTex;
	};
	struct Node
	{
		Node() : Parent(NULL), Children(NULL), ChildCount(0), MeshCount(0), Meshes(NULL) {}
		Matrix Trans;
		Matrix GlobalTrans;
		int* Meshes;
		unsigned int MeshCount;
		Node* Parent;
		Node* Children;
		unsigned int ChildCount;
		std::string Name;
	};

protected: // protected methods
	void loadMeshes(const aiScene* pScene, bool FitSize, float Size);
	void loadMaterials(const aiScene* pScene);
	void calcBoundingBox(const aiScene* pScene, AABB& Box);
	void loadNodes(const aiScene* pScene);
	void copyNodesRecursive(const aiNode* paiNode, Node* pNode);
	Matrix convertMat(const aiMatrix4x4& m);
	Vector convertVec(const aiVector3D v);
	void applyMaterial(unsigned int index);
	void deleteNodes(Node* pNode);

protected: // protected member variables
	Mesh* pMeshes;
	unsigned int MeshCount;
	Material* pMaterials;
	unsigned int MaterialCount;
	AABB BoundingBox;
	Color EmitColorOverride;
	bool hasEmitOverride;

	std::string Filepath; // stores pathname and filename
	std::string Path; // stores path without filename
	Node RootNode;

};

#endif /* Model_hpp */
