#include "ResourceManager.h"
#include "CSHeaderDef.h"
#include <assert.h>
#include <fstream>

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
	_textures[name] = AEGfxTextureLoad((asset_path + texture_path + path).c_str());
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

void ResourceManager::ReadTilemapBin(const std::string& path, Com_Tilemap& tilemap)
{
	// open binary file
	std::ifstream file(asset_path + tilemap_path + path, std::ios::in | std::ios::binary);
	assert(file);
	// read width, height
	file.read((char*)&tilemap._width, sizeof(int));
	file.read((char*)&tilemap._height, sizeof(int));
	int size = tilemap._width * tilemap._height;
	tilemap._map.resize((size_t)size);
	for (int i = 0; i < size; ++i) {
		file.read((char*)&tilemap._map[i], sizeof(int));
		tilemap._map[i] -= '0';
	}
	file.close();
}

void ResourceManager::WriteTilemapBin(const std::string& path, Com_Tilemap& tilemap)
{
	// open binary file
	std::ofstream file(asset_path + tilemap_path + path, std::ios::out | std::ios::binary);
	assert(file);
	// write width, height
	file.write((char*)&tilemap._width, sizeof(int));
	file.write((char*)&tilemap._height, sizeof(int));
	int size = tilemap._width * tilemap._height;
	// write data
	for (int i = 0; i < size; ++i) {
		file.write((char*)&tilemap._map[i], sizeof(int));
	}
	file.close();
}

void ResourceManager::ReadTilemapTxt(const std::string& path, Com_Tilemap& tilemap)
{
	// open text file
	std::ifstream file(asset_path + tilemap_path + path);
	assert(file); 
	std::string line;
	// write width, height, size
	getline(file, line);
	tilemap._width = (int)std::stoi(line);
	getline(file, line);
	tilemap._height = (int)std::stoi(line);
	tilemap._map.resize((size_t)tilemap._height * (size_t)tilemap._width);
	for (size_t y = 0; y < (size_t)tilemap._height; ++y) {
		getline(file, line);
		for (size_t x = 0; x < (size_t)tilemap._width; ++x) {
			tilemap._map[x * (size_t)tilemap._height + y] = line[x]-'0';
		}
	}
	file.close();
}

void ResourceManager::WriteTilemapTxt(const std::string& path, Com_Tilemap& tilemap)
{
	// open text file
	std::ofstream file(asset_path + tilemap_path + path);
	assert(file);
	// write width, height, size
	file << tilemap._width << "\n";
	file << tilemap._height << "\n";
	for (size_t y = 0; y < (size_t)tilemap._height; ++y) {
		for (size_t x = 0; x < (size_t)tilemap._width; ++x) {
			file << tilemap._map[x * (size_t)tilemap._height + y];
		}
		file << "\n";
	}
	file.close();
}

void ResourceManager::ReadFloorMapBin(const std::string& path, Com_Tilemap& tilemap)
{
	// open binary file
	std::ifstream file(asset_path + tilemap_path + path, std::ios::in | std::ios::binary);
	assert(file);
	std::vector<int> temp;
	// read width, height
	file.read((char*)&tilemap._width, sizeof(int));
	file.read((char*)&tilemap._height, sizeof(int));
	int size = tilemap._width * tilemap._height;
	temp.resize((size_t)size);
	for (int i = 0; i < size; ++i) {
		file.read((char*)&temp[i], sizeof(int));
		temp[i] -= '0';
	}
	file.close();
	tilemap._floor_mask.resize((size_t)size);
	for (int y = 0; y < tilemap._height; ++y) {
		for (int x = 0; x < tilemap._width; ++x) {
			int i = 0, n = y - 1, s = y + 1, w = x - 1, e = x + 1;
			if (n >= 0 && temp[x * (size_t)tilemap._height + n]) { i |= 1; }
			if (s < tilemap._height && temp[x * (size_t)tilemap._height + s]) { i |= 8; }
			if (w >= 0 && temp[w * (size_t)tilemap._height + y]) { i |= 2; }
			if (e < tilemap._width && temp[e * (size_t)tilemap._height + y]) { i |= 4; }
			tilemap._floor_mask[x * (size_t)tilemap._height + y] = i;
		}
	}
}

void ResourceManager::WriteFloorMapBin(const std::string& path, Com_Tilemap& tilemap)
{
}

void ResourceManager::ReadFloorMapTxt(const std::string& path, Com_Tilemap& tilemap)
{
	// open text file
	std::ifstream file(asset_path + tilemap_path + path);
	assert(file);
	std::string line;
	std::vector<int> temp;
	// write width, height, size
	getline(file, line);
	tilemap._width = (int)std::stoi(line);
	getline(file, line);
	tilemap._height = (int)std::stoi(line);
	temp.resize((size_t)tilemap._height * (size_t)tilemap._width);
	tilemap._floor_mask.resize((size_t)tilemap._height * (size_t)tilemap._width);
	for (size_t y = 0; y < (size_t)tilemap._height; ++y) {
		getline(file, line);
		for (size_t x = 0; x < (size_t)tilemap._width; ++x) {
			temp[x * (size_t)tilemap._height + y] = line[x] - '0';
		}
	}
	for (size_t y = 0; y < (size_t)tilemap._height; ++y) {
		for (size_t x = 0; x < (size_t)tilemap._width; ++x) {
			if (!temp[x * (size_t)tilemap._height + y]) { 
				tilemap._floor_mask[x * (size_t)tilemap._height + y] = -1;
				continue; 
			}
			int i = 0, n = y - 1, s = y + 1, w = x - 1, e = x + 1;
			if (n >= 0 && temp[x * (size_t)tilemap._height + n]) { i |= 1; }
			if (s < tilemap._height && temp[x * (size_t)tilemap._height + s]) { i |= 8; }
			if (w >= 0 && temp[w * (size_t)tilemap._height + y]) { i |= 2; }
			if (e < tilemap._width && temp[e * (size_t)tilemap._height + y]) { i |= 4; }
			tilemap._floor_mask[x * (size_t)tilemap._height + y] = i;
		}
	}
	file.close();
}

void ResourceManager::WriteFloorMapTxt(const std::string& path, Com_Tilemap& tilemap)
{
}

ResourceManager::ResourceManager()
{
	Initialize();
}

void ResourceManager::Initialize()
{
	// load all textures
	LoadTexture("test", "test2.png");
	LoadTexture("test2", "testguy.png");
	LoadTexture("test3", "TestAlien.png");
	LoadTexture("tilemap", "tilemaptest.png");
}
