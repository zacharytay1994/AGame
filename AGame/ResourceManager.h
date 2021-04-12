/******************************************************************************/
/*!
\file		ResourceManager.h
\author 	HCMR
\par    	email: nil
\date   	April 12, 2021
\brief		Allocates and Frees resources for AGame

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/
#pragma once
#include <unordered_map>
#include <stack>
#include <string>
#include <queue>
#include "AEEngine.h"
#include "zMath.h"
#include <iostream>
#include <vector>
#include <string>
#include "music.h"

static constexpr float cursor_particle_scale = 5.0f;

// forward deckaratuibs
struct Com_Tilemap;
struct Com_TilePosition;

//render pack 
struct RenderPack {
	int					_layer{ 0 };
	AEMtx33				_transform{ 0 };
	AEGfxVertexList*	_mesh{ nullptr };
	AEGfxTexture*		_texture{ nullptr };
	float				_offset_x;
	float				_offset_y;
	float				r{ 1.f };
	float				g{ 1.f };
	float				b{ 1.f };
	float				a{ 1.0f };
};

//text pack
struct TextPack {
	int			_layer{ 0 };
	char		_font{ 0 };
	std::string _text{ "loren ipsum" };
	Vec2f		_position{ 0.0f,0.0f };
	float		_scale{ 1.0f };
	float		_r{ 0.0f }, _g{ 0.0f }, _b{ 0.0f };
};

struct RM_Compare {
	bool operator()(RenderPack* lhs, RenderPack* rhs) const { return lhs->_layer > rhs->_layer; }
};

struct CursorParticle {
	Vec2f	_position{ -1000.0f,-1000.0f };
	float	_scale{ 1.0f };
	float	_dimension{ cursor_particle_scale };
	float	_a{ 1.0f };
};

struct ResourceManager {
	static ResourceManager& Instance();
	void GetResource(AEGfxTexture*& tex, AEGfxVertexList*& mesh, const std::string& texturename, const int& row, const int&col/*, const int& frames*/);
	void FreeResources();
	float _screen_shake{ 0.0f };
	float _dampening{ 10.0f };
	AEGfxVertexList* _cursor_mesh{ nullptr };

	//<-- Level Select
	struct tilemap_identifier {
		std::string _name;
		std::string _binary;
		std::string _map;
	};
	int _tilemap_id{ 0 };
	std::vector<tilemap_identifier> _tilemap_names;
	std::vector<AEGfxTexture*> _tilemap_images;
	//-->


	//<-- Level Select Normal
	struct tilemap_identifier2 {
		std::string _name;
		std::string _binary;
		std::string _map;
	};
	int _tilemap_id2{ 0 };
	std::vector<tilemap_identifier> _tilemap_names2;
	std::vector<AEGfxTexture*> _tilemap_images2;
	//-->

	int _cursor_particle_count = 200;
	std::vector<CursorParticle> _cursor_particles;
	std::vector<Vec2f> _scene_transition;
	float _panel_timer{ 3.14159f };
private:
	ResourceManager();
	void Initialize();
	std::string asset_path = "../bin/Assets/";
	std::string texture_path = "Textures/";
	std::string font_path = "Fonts/";
	std::string tilemap_path = "Tilemaps/";

	//<-- Level Select
	std::string _known_tilemaps = "tilemaps.txt";
	int _tilemap_count{ 0 };
	int _current_tilemap{ -1 };
	//-->

	//<-- Level Select
	std::string _known_tilemaps2 = "leveltilemaps.txt";
	int _tilemap_count2{ 0 };
	int _current_tilemap2{ -1 };
	//-->


	std::vector<RenderPack*> _render_queue_vector;
	std::vector<TextPack*> _text_pack;

	//Fmod (Music)
	FMOD::System* sound_system;
	
	// Sounds
	FMOD::Sound* sound1; // BGM
	FMOD::Sound* soundWalk;
	FMOD::Sound* soundShoot;
	FMOD::Sound* soundStab;
	FMOD::Sound* soundBoom;
	FMOD::Sound* soundEnemyDeath;
	FMOD::Sound* soundLaserBomb;
	FMOD::Sound* soundGrunt;
	
	// Channels required for sound
	FMOD::Channel* channel = 0;  // BGM
	FMOD::Channel* channelWalkingPlayer = 0;
	FMOD::Channel* channelGunEffect = 0;
	FMOD::Channel* channelMeleeEffect = 0;
	FMOD::Channel* channelBoomEffect = 0;
	FMOD::Channel* channelEnemyDeath = 0;
	FMOD::Channel* channelLaserBomb = 0;
	FMOD::Channel* channelGrunt = 0;
	
	FMOD_RESULT       result;
	unsigned int      version;
	void* extradriverdata = 0;
	bool playing = FALSE;
	bool mute = FALSE;


public:
	void Update(const float& dt);
	// render queue
	void DrawQueue(RenderPack* pack);
	void DrawStackText(TextPack& pack);
	void FlushDraw();
	void FlushDrawText();
	void FlushDrawTextLayer(int layer);
	void ResetRenderQueue();
	void ResetTextStack();
	// texture functions
	AEGfxTexture* LoadTexture(const std::string& name, const std::string& path);
	AEGfxTexture* GetTexture(const std::string& name);
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

	void ReadTilemapNames();
	std::string SwitchTilemap(const int& val);
	void ReadTilemapNames2();
	std::string SwitchTilemap2(const int& val);

	// music and sound effect
	void CreateMusic();
	void UpdateAndPlayMusic();
	void ToggleMuteMusic(int setting = -1);
	void WalkingSound();
	void ShootingSound(float pitch = 1.f);
	void StabbingSound();
	void BoomSound();
	void EnemyDeathSound();
	void BombSound();
	void PlayerDamageSound();
	void FreeMusic();

	AEMtx33 ScreenShake();
	void DrawCursor();
	void CursorParticlesUpdate(const float& dt);
	void AddCursorParticle();
	void DrawScenePanels(const float& dt);
};