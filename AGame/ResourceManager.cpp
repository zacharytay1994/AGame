#include "ResourceManager.h"
#include <assert.h>

ResourceManager& ResourceManager::Instance()
{
	static ResourceManager instance;
	return instance;
}

void ResourceManager::GetResource(AEGfxTexture* tex, AEGfxVertexList* mesh, const std::string& texturename, const std::string& meshname)
{
	assert(_textures.find(texturename) != _textures.end());
	assert(_meshes.find(meshname) != _meshes.end());
	tex = _textures[texturename];
	mesh = _meshes[meshname];
}

void ResourceManager::LoadTexture(const std::string& name, const std::string& path)
{
	_textures[name] = AEGfxTextureLoad(path.c_str());
	// make sure texture is successfully loaded
	assert(_textures[name]);
}

ResourceManager::ResourceManager()
{
	Initialize();
}

void ResourceManager::Initialize()
{
	// load all textures
	// load all meshes
	// start with basic sprite mesh, square mesh
	AEGfxMeshStart();
	AEGfxTriAdd(
		-0.5f, 0.5f, 0xFFFF0000, 0.0f, 0.0f,
		-0.5f, -0.5f, 0xFFFF0000, 0.0f, 1.0f,
		0.5f, 0.5f, 0xFFFFFFFF, 1.0f, 0.0f
	);
	AEGfxTriAdd(
		0.5f, 0.5f, 0xFFFF0000, 1.0f, 0.0f,
		-0.5f, -0.5f, 0xFFFF0000, 0.0f, 1.0f,
		0.5f, -0.5f, 0xFFFFFFFF, 1.0f, 1.0f
	);
	_meshes["square"] = AEGfxMeshEnd();
	// make sure mesh is successfully loaded
	assert(_meshes["square"]);
}
