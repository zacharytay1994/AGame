/******************************************************************************/
/*!
\File Name		:	ResourceManager.cpp
\Project Name	:	AGame
\Authors 		:	
				Primary - Austen Ang (50%)
				Secondary - Zachary Tay (50%)
\brief		Contains all the functions which handles the resources of AGame

All content © 2021 DigiPen Institute of Technology Singapore. All
rights reserved.
*/
/******************************************************************************/

#include "ResourceManager.h"
#include "CSHeaderDef.h"
#include <assert.h>
#include <fstream>
#include <algorithm>
#include <math.h>

#define MY_PI 3.142f

ResourceManager& ResourceManager::Instance()
{
	static ResourceManager instance;
	return instance;
}

void ResourceManager::GetResource(AEGfxTexture*& tex, AEGfxVertexList*& mesh, const std::string& texturename, const int& row, const int& col/*, const int& frames*/)
{
	/*assert(_textures.find(texturename) != _textures.end());*/
	if (_textures.find(texturename) == _textures.end()) {
		LoadTexture(texturename, texturename);
	}
	tex = _textures[texturename];
	float key = row * MY_PI + col;
	if (_meshes.find(key) != _meshes.end()) {
		mesh = _meshes[key];
	}
	else {
		mesh = CreateMesh(row,col);
	}
}

void ResourceManager::Update(const float& dt)
{
	if (_screen_shake > 0.0f) {
		_screen_shake -= dt * _dampening;
	}
	else {
		_screen_shake = 0.0f;
	}
}

void ResourceManager::DrawQueue(RenderPack* pack)
{
	_render_queue_vector.push_back(pack);
}

void ResourceManager::DrawStackText(TextPack& pack)
{
	_text_pack.push_back(&pack);
}

void ResourceManager::FlushDraw()
{
	AEMtx33 shake = ScreenShake();
	AEGfxSetRenderMode(AEGfxRenderMode::AE_GFX_RM_TEXTURE);
	AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);
	auto cmp = [](RenderPack const* const a, RenderPack const* const b) { return a->_layer < b->_layer; };
	auto cmp2 = [](TextPack const* const a, TextPack const* const b) { return a->_layer > b->_layer; };
	std::sort(_render_queue_vector.begin(), _render_queue_vector.end(), cmp);
	std::sort(_text_pack.begin(), _text_pack.end(), cmp2);
	int i = -10000000;
	for (auto r : _render_queue_vector) {
		if (r->_layer > i) {
			i = r->_layer;
			FlushDrawTextLayer(i);
		}
		AEGfxSetTintColor(r->r, r->g, r->b, r->a);
		AEMtx33Concat(&r->_transform, &shake, &r->_transform);
		AEGfxSetTransform(r->_transform.m);
		AEGfxTextureSet(r->_texture, r->_offset_x, r->_offset_y);
		AEGfxMeshDraw(r->_mesh, AEGfxMeshDrawMode::AE_GFX_MDM_TRIANGLES);
		AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);
	}
	FlushDrawText();
	_render_queue_vector.resize(0);
}

void ResourceManager::FlushDrawText() {
	float width{ 0 };
	float height{ 0 };
	while (!_text_pack.empty()) {
		TextPack* pack = _text_pack.back();
		AEGfxGetPrintSize(pack->_font, const_cast<s8*>(pack->_text.c_str()), 1.0f, width, height);
		AEGfxPrint(pack->_font, const_cast<s8*>(pack->_text.c_str()), pack->_position.x - width/2.0f, pack->_position.y - height/2.0f, pack->_scale, pack->_r, pack->_g, pack->_b);
		_text_pack.pop_back(); 
	}
}

void ResourceManager::FlushDrawTextLayer(int layer)
{
	float width{ 0 };
	float height{ 0 };
	while (!_text_pack.empty() && _text_pack.back()->_layer < layer) {
		TextPack* pack = _text_pack.back();
		AEGfxGetPrintSize(pack->_font, const_cast<s8*>(pack->_text.c_str()), 1.0f, width, height);
		AEGfxPrint(pack->_font, const_cast<s8*>(pack->_text.c_str()), pack->_position.x - width/2.0f, pack->_position.y - height/2.0f, pack->_scale, pack->_r, pack->_g, pack->_b);
		_text_pack.pop_back();
	}
}

void ResourceManager::ResetRenderQueue()
{
	_render_queue_vector.resize(0);
}

void ResourceManager::ResetTextStack()
{
	_text_pack = std::vector<TextPack*>();
}

AEGfxTexture* ResourceManager::LoadTexture(const std::string& name, const std::string& path)
{
	if (_textures.find(name) != _textures.end()) { return _textures[name]; };
	_textures[name] = AEGfxTextureLoad((asset_path + texture_path + path).c_str());
	// make sure texture is successfully loaded
	if (!_textures[name]) { 
		std::cout << "TEXTURE NOT LOADED, PATH NOT FOUND!"; return nullptr; 
	}
	return _textures[name];
}

AEGfxTexture* ResourceManager::GetTexture(const std::string& name)
{
	if (_textures[name]) {
		return _textures[name];
	}
	return GetTexture("noimage");
}

void ResourceManager::LoadFont(const std::string& name, const std::string& path, int size)
{
	_fonts[name] = AEGfxCreateFont((asset_path + font_path + path).c_str(), size);
	assert(_fonts[name]);
}

char ResourceManager::GetFont(const std::string& name)
{
	assert(_fonts[name]);
	return _fonts[name];
}

void ResourceManager::FreeResources()
{
	for (auto m : _meshes) {
		AEGfxMeshFree(m.second);
	}
	for (auto t : _textures) {
		if (t.second) {
			AEGfxTextureUnload(t.second);
		}
	}
	for (auto f : _fonts) {
		AEGfxDestroyFont(f.second);
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
			int i = 0, n = (int)y - 1, s = (int)y + 1, w = (int)x - 1, e = (int)x + 1;
			if (n >= 0 && temp[x * (size_t)tilemap._height + n]) { i |= 1; }
			if (s < tilemap._height && temp[x * (size_t)tilemap._height + s]) { i |= 8; }
			if (w >= 0 && temp[w * (size_t)tilemap._height + y]) { i |= 2; }
			if (e < tilemap._width && temp[e * (size_t)tilemap._height + y]) { i |= 4; }
			tilemap._floor_mask[x * (size_t)tilemap._height + y] = (int)i;
		}
	}
	file.close();
}

AEMtx33 ResourceManager::ScreenShake()
{
	float r = AERandFloat();
	AEMtx33 trans;
	AEMtx33Trans(&trans, cos(r * 2 * PI) * _screen_shake, sin(r * 2 * PI) * _screen_shake);
	return trans;
}

void ResourceManager::DrawCursor()
{
	if (_cursor_mesh) {
		AEGfxSetRenderMode(AEGfxRenderMode::AE_GFX_RM_TEXTURE);
		AEMtx33 trans{ 0 }, scale{ 0 };
		int x, y;
		AEGfxTextureSet(GetTexture("cursor"), 0.0f,0.0f);
		AEInputGetCursorPosition(&x, &y);
		x -= (int)((float)AEGetWindowWidth() / 2.0f);
		y -= (int)((float)AEGetWindowHeight() / 2.0f);
		y *= -1;
		AEMtx33Trans(&trans, (float)x, (float)y);
		AEMtx33Scale(&scale, 100.0f, 100.0f);
		AEMtx33Concat(&trans, &trans, &scale);
		AEGfxSetTransform(trans.m);
		AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);
		AEGfxMeshDraw(_cursor_mesh, AEGfxMeshDrawMode::AE_GFX_MDM_TRIANGLES);
	}
}

void ResourceManager::CursorParticlesUpdate(const float& dt)
{
	int x, y;
	AEInputGetCursorPosition(&x, &y);
	x -= (int)((float)AEGetWindowWidth() / 2.0f);
	y -= (int)((float)AEGetWindowHeight() / 2.0f);
	y *= -1;
	if (_cursor_particle_count > 0) {
		AddCursorParticle();
		_cursor_particles.back()._position.x = (float)x;
		_cursor_particles.back()._position.y = (float)y;
		--_cursor_particle_count;
	}
	AEGfxSetRenderMode(AEGfxRenderMode::AE_GFX_RM_TEXTURE);
	AEGfxTextureSet(GetTexture("inkblob"), 0.0f, 0.0f);
	for (auto& p : _cursor_particles) {
		if (p._a <= 0.0f) {
			p._position.x = (float)x;
			p._position.y = (float)y;
			p._dimension = (0.5f + AERandFloat()) * cursor_particle_scale;
			p._scale = 1.0f;
			p._a = 1.0f;
		}
		else {
			p._position.y -= 5.0f * dt;
			p._scale -= 0.5f * dt;
			p._a -= 0.15f * dt;
		}
		// draw particle
		AEMtx33 trans{ 0 }, scale{ 0 };
		AEMtx33Trans(&trans, p._position.x, p._position.y);
		AEMtx33Scale(&scale, p._dimension * p._scale, p._dimension * p._scale);
		AEMtx33Concat(&trans, &trans, &scale);
		AEGfxSetTransform(trans.m);
		AEGfxSetTintColor(1.0f, 1.0f, 1.0f, p._a);
		AEGfxMeshDraw(_cursor_mesh, AEGfxMeshDrawMode::AE_GFX_MDM_TRIANGLES);
	}
}

void ResourceManager::AddCursorParticle()
{
	_cursor_particles.emplace_back();
}

void ResourceManager::DrawScenePanels(const float& dt)
{
	if (_panel_timer > 0.0f) {
		_panel_timer -= 3.0f * dt;
	}
	float height = (float)AEGetWindowHeight()/2.0f;
	float width = (float)AEGetWindowWidth()/2.0f;
	AEGfxSetRenderMode(AEGfxRenderMode::AE_GFX_RM_TEXTURE);
	AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);
	AEMtx33 trans{ 0 }, scale{ 0 };

	AEGfxTextureSet(GetTexture("LeftPanel"), 0.0f, 0.0f);
	AEMtx33Trans(&trans, _scene_transition[2].x + height * sin(_panel_timer), _scene_transition[2].y);
	AEMtx33Scale(&scale, width * 2.0f, height * 2.0f);
	AEMtx33Concat(&trans, &trans, &scale);
	AEGfxSetTransform(trans.m);
	AEGfxMeshDraw(_cursor_mesh, AEGfxMeshDrawMode::AE_GFX_MDM_TRIANGLES);

	AEGfxTextureSet(GetTexture("RightPanel"), 0.0f, 0.0f);
	AEMtx33Trans(&trans, _scene_transition[3].x - height * sin(_panel_timer), _scene_transition[3].y);
	AEMtx33Scale(&scale, width * 2.0f, height * 2.0f);
	AEMtx33Concat(&trans, &trans, &scale);
	AEGfxSetTransform(trans.m);
	AEGfxMeshDraw(_cursor_mesh, AEGfxMeshDrawMode::AE_GFX_MDM_TRIANGLES);

	AEGfxTextureSet(GetTexture("TopPanel"), 0.0f, 0.0f);
	AEMtx33Trans(&trans, _scene_transition[0].x, _scene_transition[0].y - height * sin(_panel_timer));
	AEMtx33Scale(&scale, width * 2.0f, height * 2.0f);
	AEMtx33Concat(&trans, &trans, &scale);
	AEGfxSetTransform(trans.m);
	AEGfxMeshDraw(_cursor_mesh, AEGfxMeshDrawMode::AE_GFX_MDM_TRIANGLES);

	AEGfxTextureSet(GetTexture("BottomPanel"), 0.0f, 0.0f);
	AEMtx33Trans(&trans, _scene_transition[1].x, _scene_transition[1].y + height * sin(_panel_timer));
	AEMtx33Scale(&scale, width * 2.0f, height * 2.0f);
	AEMtx33Concat(&trans, &trans, &scale);
	AEGfxSetTransform(trans.m);
	AEGfxMeshDraw(_cursor_mesh, AEGfxMeshDrawMode::AE_GFX_MDM_TRIANGLES);
}

void ResourceManager::ReadTilemapNames()
{
	// open file
	std::ifstream file(asset_path + tilemap_path + _known_tilemaps);
	if (file) {
		_tilemap_names.clear();
		_tilemap_images.clear();
		_tilemap_count = 0;
		std::string line;
		while (file >> line) {
			_tilemap_names.push_back({ line, "c_" + line,  "t_" + line }); 
			LoadTexture(line, "tilemaps/" + line + ".png");
			_tilemap_images.push_back(GetTexture(line));
			++_tilemap_count;
		}
	}
}

std::string ResourceManager::SwitchTilemap(const int& val)
{
	if (_tilemap_count <= 0) {
		std::cout << "ResourceManager:: No Levels Found." << std::endl;
		return "no level";
	}
	_tilemap_id += val;
	if (_tilemap_id < 0) {
		_tilemap_id += _tilemap_count;
	}
	else {
		_tilemap_id = _tilemap_id % _tilemap_count;
	}
	return _tilemap_names[_tilemap_id]._name;
}


void ResourceManager::ReadTilemapNames2()
{
	// open file
	std::ifstream file(asset_path + tilemap_path + _known_tilemaps2);
	if (file) {
		_tilemap_names2.clear();
		_tilemap_images2.clear();
		_tilemap_count2 = 0;
		std::string line;
		while (file >> line) {
			_tilemap_names2.push_back({ line, "c_" + line,  "t_" + line });
			LoadTexture(line, "tilemaps/" + line + ".png");
			_tilemap_images2.push_back(GetTexture(line));
			++_tilemap_count2;
		}
	}
}

std::string ResourceManager::SwitchTilemap2(const int& val)
{
	if (_tilemap_count2 <= 0) {
		std::cout << "ResourceManager:: No Levels Found." << std::endl;
		return "no level";
	}
	_tilemap_id2 += val;
	if (_tilemap_id2 < 0) {
		_tilemap_id2 += _tilemap_count2;
	}
	else {
		_tilemap_id2 = _tilemap_id2 % _tilemap_count2;
	}
	return _tilemap_names2[_tilemap_id2]._name;
}

void ResourceManager::CreateMusic()
{
	//Create system object and initailize it
	result = FMOD::System_Create(&sound_system);
	//ERRCHECK(result);


	result = sound_system->getVersion(&version);
	//ERRCHECK(result);


	if (version < FMOD_VERSION)
	{
		std::cout << "FMOD lib version" << version << "doesn't math hearder version" << FMOD_VERSION;
	}

	//init sound track
	result = sound_system->init(32, FMOD_INIT_NORMAL, extradriverdata);

	//load tracks
	//char* filePath = new char;

	result = sound_system->createSound("../bin/Assets/Sound/SoulFly.wav", FMOD_DEFAULT, 0, &sound1);
	result = sound1->setMode(FMOD_LOOP_NORMAL); 

	result = sound_system->createSound("../bin/Assets/Sound/WalkingPlayer1.wav", FMOD_DEFAULT, 0, &soundWalk);
	result = sound_system->createSound("../bin/Assets/Sound/Gun.wav", FMOD_DEFAULT, 0, &soundShoot);
	result = sound_system->createSound("../bin/Assets/Sound/Knife.wav", FMOD_DEFAULT, 0, &soundStab);
	result = sound_system->createSound("../bin/Assets/Sound/Boom.wav", FMOD_DEFAULT, 0, &soundBoom);
	result = sound_system->createSound("../bin/Assets/Sound/Death.wav", FMOD_DEFAULT, 0, &soundEnemyDeath);
	result = sound_system->createSound("../bin/Assets/Sound/LaserBomb.wav", FMOD_DEFAULT, 0, &soundLaserBomb);
	result = sound_system->createSound("../bin/Assets/Sound/Grunt.wav", FMOD_DEFAULT, 0, &soundGrunt);
	result = soundWalk->setMode(FMOD_LOOP_OFF);
	result = soundShoot->setMode(FMOD_LOOP_OFF);
	result = soundStab->setMode(FMOD_LOOP_OFF);
	result = soundEnemyDeath->setMode(FMOD_LOOP_OFF);
	result = soundLaserBomb->setMode(FMOD_LOOP_OFF);
	result = soundGrunt->setMode(FMOD_LOOP_OFF);

	std::cout << "Sound load";

}

/**************************************************************************/
	/*!
	  \brief
		Play walking sound
	*/
/**************************************************************************/
void ResourceManager::WalkingSound()
{
	if (SceneManager::Instance()._musicmmute == false) {
		result = sound_system->playSound(soundWalk, 0, false, &channelWalkingPlayer);
	}
}

/**************************************************************************/
	/*!
	  \brief
		Play shooting sound
	*/
/**************************************************************************/
void ResourceManager::ShootingSound(float pitch)
{
	if (SceneManager::Instance()._musicmmute == false) {
		result = sound_system->playSound(soundShoot, 0, false, &channelGunEffect);
		channelGunEffect->setPitch(pitch);
	}
}

/**************************************************************************/
	/*!
	  \brief
		Play stabbing sound
	*/
/**************************************************************************/
void ResourceManager::StabbingSound()
{
	if (SceneManager::Instance()._musicmmute == false) {
		result = sound_system->playSound(soundStab, 0, false, &channelMeleeEffect);
	}
}

/**************************************************************************/
	/*!
	  \brief
		Play boom sound
	*/
/**************************************************************************/
void ResourceManager::BoomSound()
{
	if (SceneManager::Instance()._musicmmute == false) {
		result = sound_system->playSound(soundBoom, 0, false, &channelBoomEffect);
	}
}

/**************************************************************************/
	/*!
	  \brief
		Play enemy death sound
	*/
/**************************************************************************/
void ResourceManager::EnemyDeathSound()
{
	if (SceneManager::Instance()._musicmmute == false) {
		result = sound_system->playSound(soundEnemyDeath, 0, false, &channelEnemyDeath);
	}
}

/**************************************************************************/
	/*!
	  \brief
		Play bomb sound
	*/
/**************************************************************************/
void ResourceManager::BombSound()
{
	if (SceneManager::Instance()._musicmmute == false) {
		result = sound_system->playSound(soundLaserBomb, 0, false, &channelLaserBomb);
	}
}

/**************************************************************************/
	/*!
	  \brief
		Play player damage sound
	*/
/**************************************************************************/
void ResourceManager::PlayerDamageSound()
{
	if (SceneManager::Instance()._musicmmute == false) {
		result = sound_system->playSound(soundGrunt, 0, false, &channelGrunt);
	}
}

void ResourceManager::UpdateAndPlayMusic() 
{
	//mute
	if (AEInputCheckTriggered(AEVK_M)) {
		mute = !mute;
		playing = true;
	}
	if (!mute) {
		//Play sound
		//Player jump
		if (!playing)
		{
			result = sound_system->playSound(sound1, 0, false, &channel);
		}

		result = sound_system->update();

		//system pause
		if (AEInputCheckTriggered(AEVK_P)) {
			bool paused;
			result = channel->getPaused(&paused);
			paused = !paused;
			result = channel->setPaused(paused);
		}

		if (channel) {

			result = channel->isPlaying(&playing);

		}

	}
}

/**************************************************************************/
	/*!
	  \brief
		Mutes and unmutes the music

	  \param setting
		-1 to switch to the opposite setting
		0 to unmute
		1 to mute
	*/
/**************************************************************************/
void ResourceManager::ToggleMuteMusic(int setting)
{
	bool paused;
	result = channel->getPaused(&paused);

	switch (setting)
	{
		case -1:
			paused = !paused;
			result = channel->setPaused(paused);
			break;
		case 0:
			if(paused != true)
				result = channel->setPaused(true);
			break;
		case 1:
			if(paused != false)
				result = channel->setPaused(false);
			break;
	}
}

void ResourceManager::FreeMusic() 
{
	result = sound1->release();
	result = soundWalk->release();
	result = soundShoot->release();
	std::cout << "Freed" << std::endl;
}

ResourceManager::ResourceManager()
{
	Initialize();
}

void ResourceManager::Initialize()
{
	// mouse cursor
	LoadTexture("cursor", "cursor.png");
	LoadTexture("inkblob", "inkblob.png");
	_cursor_mesh = CreateMesh(1, 1);

	// scene transition
	LoadTexture("TopPanel", "TopPanel.png");
	LoadTexture("BottomPanel", "BottomPanel.png");
	LoadTexture("RightPanel", "RightPanel.png");
	LoadTexture("LeftPanel", "LeftPanel.png");
	_scene_transition.push_back({ 0.0f,(float)AEGetWindowHeight() });
	_scene_transition.push_back({ 0.0f,-(float)AEGetWindowHeight() });
	_scene_transition.push_back({ -(float)AEGetWindowWidth(),0.0f });
	_scene_transition.push_back({ (float)AEGetWindowWidth(),0.0f });

	// load all textures
	LoadTexture("noimage", "noimage.png");
	LoadTexture("test3", "TestAlien.png");
	LoadTexture("tilemap", "tilemaptest.png");
	LoadTexture("skeleton", "temp1.png");
	LoadTexture("heroidle", "heroidle.png");
	LoadTexture("coolguy", "temp2.png");
	LoadTexture("box", "box.png");
	LoadTexture("kaboom", "kaboom.png");
	LoadTexture("menubutton", "testgui.png");
	// gui textures
	LoadTexture("menubackground", "mainmenubackground.png");
	LoadTexture("buttonsurface", "buttonsurface.png");
	LoadTexture("button1", "button1.png");
	LoadTexture("button2", "button2.png");
	LoadTexture("button3", "button3.png");
	LoadTexture("transparent", "transparent.png");
	LoadTexture("settings", "settingsbutton.png");
	LoadTexture("cross", "crossbutton.png");
	LoadTexture("background1", "background1.png");
	LoadTexture("gamelogo", "AGameLogo.png");
	LoadTexture("teamlogo", "TeamLogo.png");
	LoadTexture("zachary", "Zachary.png");
	LoadTexture("noel", "Noel.png");
	LoadTexture("austen", "Austen.png");
	LoadTexture("wilfred", "Wilfred.png");
	LoadTexture("faculty", "faculty.png");
	LoadTexture("instructorthomas", "instructor1.png");
	LoadTexture("instructordx", "instructor2.png");
	LoadTexture("digipensingapore", "digipensingapore.png");
	LoadTexture("president", "president.png");
	LoadTexture("executives1", "executives1.png");
	LoadTexture("executives2", "executives2.png");
	LoadTexture("executives3", "executives3.png");
	LoadTexture("ending", "ending.png");
	LoadTexture("endingfmod", "endingfmod.png");
	LoadTexture("bombparticles", "bombparticles.png");
	LoadTexture("blank", "blank.png");
	
	// Opening Scene
	LoadTexture("logo", "Digipen_.png");
	
	// load all fonts
	LoadFont("courier", "COURIER.TTF", 20);
}

bool RenderComparator(RenderPack* p1, RenderPack* p2)
{
	return p1->_layer < p2->_layer;
}
