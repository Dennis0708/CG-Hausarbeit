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

Model::Model() : pMeshes(NULL), MeshCount(0), pMaterials(NULL), MaterialCount(0)
{

}

Model::Model(const char* ModelFile, Vector& position, float size) : BaseModel(position, size), pMeshes(NULL), MeshCount(0), pMaterials(NULL), MaterialCount(0)
{
	bool ret = load(ModelFile, size);
	if (!ret)
		throw std::exception();
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

bool Model::load(const char* ModelFile, float size)
{
	const aiScene* pScene = aiImportFile(ModelFile, aiProcessPreset_TargetRealtime_Fast | aiProcess_TransformUVCoords);

	if (pScene == NULL || pScene->mNumMeshes <= 0)
		return false;

	Filepath = ModelFile;
	Path = Filepath;
	size_t pos = Filepath.rfind('/');
	if (pos == std::string::npos)
		pos = Filepath.rfind('\\');
	if (pos != std::string::npos)
		Path.resize(pos + 1);

	loadMeshes(pScene, size);
	loadMaterials(pScene);
	loadNodes(pScene);

	return true;
}

void Model::loadMeshes(const aiScene* pScene, float size)
{
	calcBoundingBox(pScene, BoundingBox, size);

	MeshCount = pScene->mNumMeshes;
	pMeshes = new Mesh[MeshCount];

	for (unsigned int meshNumber = 0; meshNumber < MeshCount; meshNumber++) {
		pMeshes[meshNumber].MaterialIdx = pScene->mMeshes[meshNumber]->mMaterialIndex;
		pMeshes[meshNumber].VB.begin();
		for (unsigned int vertixNumber = 0; vertixNumber < pScene->mMeshes[meshNumber]->mNumVertices; vertixNumber++) {
			if (pScene->mMeshes[meshNumber]->HasNormals()) {
				aiVector3D& normal = pScene->mMeshes[meshNumber]->mNormals[vertixNumber];
				pMeshes[meshNumber].VB.addNormal(normal.x, normal.y, normal.z);
			}

			if (pScene->mMeshes[meshNumber]->HasTextureCoords(0)) {
				aiVector3D& texture = pScene->mMeshes[meshNumber]->mTextureCoords[0][vertixNumber];
				pMeshes[meshNumber].VB.addTexcoord0(texture.x, -texture.y);
			}

			aiVector3D& vertex = pScene->mMeshes[meshNumber]->mVertices[vertixNumber];
			pMeshes[meshNumber].VB.addVertex(vertex.x, vertex.y, vertex.z);

		}
		pMeshes[meshNumber].VB.end();

		pMeshes[meshNumber].IB.begin();
		for (unsigned int faceNumber = 0; faceNumber < pScene->mMeshes[meshNumber]->mNumFaces; faceNumber++) {
			for (unsigned int indexNumber = 0; indexNumber < pScene->mMeshes[meshNumber]->mFaces[faceNumber].mNumIndices; indexNumber++) {
				pMeshes[meshNumber].IB.addIndex(pScene->mMeshes[meshNumber]->mFaces[faceNumber].mIndices[indexNumber]);
			}
		}
		pMeshes[meshNumber].IB.end();
	}
}

void Model::loadMaterials(const aiScene* pScene)
{
	MaterialCount = pScene->mNumMaterials;
	pMaterials = new Material[MaterialCount];

	for (unsigned int materialNumber = 0; materialNumber < MaterialCount; materialNumber++) {
		aiMaterial* aimaterial = pScene->mMaterials[materialNumber];
		aiColor4D color;
		aiGetMaterialColor(aimaterial, AI_MATKEY_COLOR_AMBIENT, &color);
		pMaterials[materialNumber].AmbColor = Color(color.r, color.g, color.b);
		aiGetMaterialColor(aimaterial, AI_MATKEY_COLOR_DIFFUSE, &color);
		pMaterials[materialNumber].DiffColor = Color(color.r, color.g, color.b);
		aiGetMaterialColor(aimaterial, AI_MATKEY_COLOR_SPECULAR, &color);
		pMaterials[materialNumber].SpecColor = Color(color.r, color.g, color.b);

		aiString aiS;
		aiGetMaterialTexture(aimaterial, aiTextureType_DIFFUSE, 0, &aiS);
		pMaterials[materialNumber].DiffTex = Texture().LoadShared((Path + aiS.C_Str()).c_str());
	}
}

void Model::calcBoundingBox(const aiScene* pScene, AABB& Box, float size)
{
	aiVector3D min, max;

	min = pScene->mMeshes[0]->mVertices[0];
	max = min;

	for (unsigned int meshNumber = 0; meshNumber < pScene->mNumMeshes; meshNumber++) {
		for (unsigned int vertixNumber = 0; vertixNumber < pScene->mMeshes[meshNumber]->mNumVertices; vertixNumber++) {
			aiVector3D vertice = pScene->mMeshes[meshNumber]->mVertices[vertixNumber];
			if (vertice.x < min.x)
				min.x = vertice.x;
			if (vertice.y < min.y)
				min.y = vertice.y;
			if (vertice.z < min.z)
				min.z = vertice.z;
			if (vertice.x > max.x)
				max.x = vertice.x;
			if (vertice.y > max.y)
				max.y = vertice.y;
			if (vertice.z > max.z)
				max.z = vertice.z;
		}
	}

	min *= size;
	max *= size;

	Box = { min.x, min.y, min.z, max.x, max.y, max.z };
}

void Model::loadNodes(const aiScene* pScene)
{
	deleteNodes(&RootNode);
	copyNodesRecursive(pScene->mRootNode, &RootNode);
}

void Model::copyNodesRecursive(const aiNode* paiNode, Node* pNode)
{
	pNode->Name = paiNode->mName.C_Str();
	pNode->Trans = convert(paiNode->mTransformation);

	if (paiNode->mNumMeshes > 0)
	{
		pNode->MeshCount = paiNode->mNumMeshes;
		pNode->Meshes = new int[pNode->MeshCount];
		for (unsigned int i = 0; i < pNode->MeshCount; ++i)
			pNode->Meshes[i] = (int)paiNode->mMeshes[i];
	}

	if (paiNode->mNumChildren <= 0)
		return;

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

Matrix Model::convert(const aiMatrix4x4& m)
{
	return Matrix(m.a1, m.a2, m.a3, m.a4,
		m.b1, m.b2, m.b3, m.b4,
		m.c1, m.c2, m.c3, m.c4,
		m.d1, m.d2, m.d3, m.d4);
}



