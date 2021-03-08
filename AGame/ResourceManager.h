#pragma once
#include <unordered_map>
#include <stack>
#include <string>
#include <queue>
#include "AEEngine.h"
#include "zMath.h"

// forward deckaratuibs
struct Com_Tilemap;

struct RenderPack {
	int					_layer{ 0 };
	AEMtx33				_transform{ 0 };
	AEGfxVertexList*	_mesh{ nullptr };
	AEGfxTexture*		_texture{ nullptr };
	float				_offset_x;
	float				_offset_y;
	//bool operator>(const RenderPack& rhs) const { return _layer > rhs._layer; }
};

struct TextPack {
	char		_font;
	std::string _text{ "loren ipsum" };
	Vec2f		_position{ 0.0f,0.0f };
	float		_scale{ 1.0f };
	float		_r{ 1.0f }, _g{ 0.0f }, _b{ 1.0f };
};

struct RM_Compare {
	bool operator()(RenderPack lhs, RenderPack rhs) { return lhs._layer > rhs._layer; }
};


struct ResourceManager {
	static ResourceManager& Instance();
	void GetResource(AEGfxTexture*& tex, AEGfxVertexList*& mesh, const std::string& texturename, const int& row, const int&col, const int& frames);
	void FreeResources();
private:
	ResourceManager();
	void Initialize();
	std::string asset_path = "../bin/Assets/";
	std::string texture_path = "Textures/";
	std::string font_path = "Fonts/";
	std::string tilemap_path = "Tilemaps/";

	std::priority_queue <RenderPack, std::vector<RenderPack>, RM_Compare> _render_queue;
	std::stack <TextPack*> _text_pack;
public:
	// render queue
	void DrawQueue(RenderPack pack);
	void DrawStackText(TextPack& pack);
	void FlushDraw();
	void FlushDrawText();
	void ResetRenderQueue();
	void ResetTextStack();
	// texture functions
	void LoadTexture(const std::string& name, const std::string& path);
	void LoadFont(const std::string& name, const std::string& path, int size);
	char GetFont(const std::string& name);
	AEGfxVertexList* CreateMesh(const int& x, const int& y);				// gets a mesh based on how many frames there are
	std::unordered_map<std::string, AEGfxTexture*>		_textures;
	std::unordered_map<float, AEGfxVertexList*>			_meshes;
	std::unordered_map<std::string, char>				_fonts;

	// tilemap functions
	void ReadTilemapBin(const std::string& path, Com_Tilemap& tilemap);
	void WriteTilemapBin(const std::string& path,  Com_Tilemap& tilemap);
	void ReadTilemapTxt(const std::string& path, Com_Tilemap& tilemap);
	void WriteTilemapTxt(const std::string& path,  Com_Tilemap& tilemap);

	void ReadFloorMapBin(const std::string& path, Com_Tilemap& tilemap);
	void WriteFloorMapBin(const std::string& path, Com_Tilemap& tilemap);
	void ReadFloorMapTxt(const std::string& path, Com_Tilemap& tilemap);
	void WriteFloorMapTxt(const std::string& path, Com_Tilemap& tilemap);
}; 