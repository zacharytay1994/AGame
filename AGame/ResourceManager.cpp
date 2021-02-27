#include "ResourceManager.h"
#include <assert.h>

#define MY_PI 3.142f

ResourceManager& ResourceManager::Instance()
{
	static ResourceManager instance;
	return instance;
}

void ResourceManager::GetResource(AEGfxTexture*& tex, AEGfxVertexList*& mesh, const std::string& texturename, const int& row, const int& col, const int& frames)
{
	assert(_textures.find(texturename) != _textures.end());
	tex = _textures[texturename];
	float key = row * MY_PI + col;
	if (_meshes.find(key) != _meshes.end()) {
		mesh = _meshes[key];
	}
	else {
		mesh = CreateMesh(row,col);
	}
}

void ResourceManager::LoadTexture(const std::string& name, const std::string& path)
{
	_textures[name] = AEGfxTextureLoad(path.c_str());
	// make sure texture is successfully loaded
	assert(_textures[name]);
}

void ResourceManager::FreeResources()
{
	for (auto m : _meshes) {
		AEGfxMeshFree(m.second);
	}
	for (auto t : _textures) {
		AEGfxTextureUnload(t.second);
	}
}

AEGfxVertexList* ResourceManager::CreateMesh(const int& row, const int& col)
{
	float key = row*MY_PI+col, x = 1.0f / col, y = 1.0f / row;
	AEGfxMeshStart();
	AEGfxTriAdd(
		-0.5f, 0.5f, 0xFFFF0000, 0.0f, 0.0f,
		-0.5f, -0.5f, 0xFFFF0000, 0.0f, y,
		0.5f, 0.5f, 0xFFFFFFFF, x, 0.0f
	);
	AEGfxTriAdd(
		0.5f, 0.5f, 0xFFFF0000, x, 0.0f,
		-0.5f, -0.5f, 0xFFFF0000, 0.0f, y,
		0.5f, -0.5f, 0xFFFFFFFF, x, y
	);
	_meshes[key] = AEGfxMeshEnd();
	return _meshes[key];
}

ResourceManager::ResourceManager()
{
	Initialize();
}

void ResourceManager::Initialize()
{
	// load all textures
	LoadTexture("test", "../bin/Assets/test2.png");
	LoadTexture("test2", "../bin/Assets/testguy.png");
	// load all meshes
	// start with basic sprite mesh, square mesh
	//AEGfxMeshStart();
	//AEGfxTriAdd(
	//	-0.5f, 0.5f, 0xFFFF0000, 0.0f, 0.0f,
	//	-0.5f, -0.5f, 0xFFFF0000, 0.0f, 1.0f,
	//	0.5f, 0.5f, 0xFFFFFFFF, 1.0f, 0.0f
	//);
	//AEGfxTriAdd(
	//	0.5f, 0.5f, 0xFFFF0000, 1.0f, 0.0f,
	//	-0.5f, -0.5f, 0xFFFF0000, 0.0f, 1.0f,
	//	0.5f, -0.5f, 0xFFFFFFFF, 1.0f, 1.0f
	//);
	//// mesh signature 1 by 1 default sprite
	//_meshes[{1, 1}] = AEGfxMeshEnd();
}
