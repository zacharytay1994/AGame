#pragma once
#include <unordered_map>
#include <string>
#include "AEEngine.h"

struct ResourceManager {
	static ResourceManager& Instance();
	void GetResource(AEGfxTexture*& tex, AEGfxVertexList*& mesh, const std::string& texturename, const int& row, const int&col, const int& frames);
	void FreeResources();
private:
	ResourceManager();
	void Initialize();
	void LoadTexture(const std::string& name, const std::string& path);
	AEGfxVertexList* CreateMesh(const int& x, const int& y);				// gets a mesh based on how many frames there are
	std::unordered_map<std::string, AEGfxTexture*>		_textures;
	std::unordered_map<float, AEGfxVertexList*>	_meshes;
}; 