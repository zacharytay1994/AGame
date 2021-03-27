#include "ResourceManager.h"
#include "CSHeaderDef.h"
#include <assert.h>
#include <fstream>
#include <algorithm>

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
	//std::priority_queue <RenderPack*, RM_Compare> _somequeue( _render_queue_vector );
	//std::make_heap<RenderPack*>(_render_queue_vector.begin(), _render_queue_vector.end(), RM_Compare);
	std::sort(_render_queue_vector.begin(), _render_queue_vector.end(), cmp);
	std::sort(_text_pack.begin(), _text_pack.end(), cmp2);
	/*while (!_render_queue_vector.empty()) {
		const RenderPack& rp = *_render_queue.top();
		AEGfxSetTransform(const_cast<RenderPack&>(rp)._transform.m);
		AEGfxTextureSet(rp._texture, rp._offset_x, rp._offset_y);
		AEGfxMeshDraw(rp._mesh, AEGfxMeshDrawMode::AE_GFX_MDM_TRIANGLES);
		_render_queue.pop();
	}*/
	int i = -10000000;
	for (auto r : _render_queue_vector) {
		if (r->_layer > i) {
			i = r->_layer;
			FlushDrawTextLayer(i);
		}
		AEMtx33Concat(&r->_transform, &shake, &r->_transform);
		AEGfxSetTransform(r->_transform.m);
		AEGfxTextureSet(r->_texture, r->_offset_x, r->_offset_y);
		AEGfxMeshDraw(r->_mesh, AEGfxMeshDrawMode::AE_GFX_MDM_TRIANGLES);
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
	//_render_queue = std::priority_queue <RenderPack*, std::vector<RenderPack*>, RM_Compare>();
}

void ResourceManager::ResetTextStack()
{
	_text_pack = std::vector<TextPack*>();
}

void ResourceManager::LoadTexture(const std::string& name, const std::string& path)
{
	_textures[name] = AEGfxTextureLoad((asset_path + texture_path + path).c_str());
	// make sure texture is successfully loaded
	assert(_textures[name]);
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
		AEGfxTextureUnload(t.second);
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

//void ResourceManager::WriteFloorMapBin(const std::string& path, Com_Tilemap& tilemap)
//{
//}

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

	//result = sound_system->createSound(Common_MediaPath("drumloop.wav"), FMOD_DEFAULT, 0, &sound1);
	result = sound_system->createSound("../bin/Assets/Sound/SoulFly.wav", FMOD_DEFAULT, 0, &sound1);
	result = sound1->setMode(FMOD_LOOP_NORMAL); 



	//result = sound_system->createSound(Common_SoundPath("drumloop.wav"), FMOD_DEFAULT, 0, &sound1);
	std::cout << "Sound load";

}

void ResourceManager::UpdateAndPlayMusic() 
{
	//std::cout<< AEFrameRateControllerGetFrameTime()<<std::endl;
	//std::cout << "Sound update" << std::endl;

	//mute
	if (AEInputCheckTriggered(AEVK_M)) {
		mute = !mute;
		//channel->getMute(&mute);
		//channel->setMute(&mute);
	}
	if (!mute) {
		//Play sound
		//Player jump
		if (!playing)
		{
			result = sound_system->playSound(sound1, 0, false, &channel);
			std::cout << "sound played";
			//ERRCHECK(result);
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

void ResourceManager::FreeMusic() 
{
	result = sound1->release();
	std::cout << "Freed" << std::endl;
}

//void ResourceManager::WriteFloorMapTxt(const std::string& path, Com_Tilemap& tilemap)
//{
//}

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
	// load all fonts
	LoadFont("courier", "COURIER.TTF", 20);
}

bool RenderComparator(RenderPack* p1, RenderPack* p2)
{
	return p1->_layer < p2->_layer;
}
