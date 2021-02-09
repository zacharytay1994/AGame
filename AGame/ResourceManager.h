#pragma once
#include <unordered_map>
#include <string>
#include "AEEngine.h"

struct ResourceManager {
	static ResourceManager& Instance();
	void GetResource(AEGfxTexture*& tex, AEGfxVertexList*& mesh, const std::string& texturename, const std::string& meshname);
private:
	ResourceManager();
	void Initialize();
	void LoadTexture(const std::string& name, const std::string& path);
	std::unordered_map<std::string, AEGfxTexture*>		_textures;
	std::unordered_map<std::string, AEGfxVertexList*>	_meshes;
};