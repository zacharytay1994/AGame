#include "Scene.h"
#include "SceneDeclarations.h"

SceneManager::SceneManager()
{
	Initialize();
}

SceneManager& SceneManager::Instance()
{
    static SceneManager instance;
    return instance;
}

void SceneManager::Initialize() {
	AddScene("Test Scene", TS1_Initialize, TS1_Update, TS1_Exit);
	AddScene("Test Scene 2", TS2_Initialize, TS2_Update, TS2_Exit);
}

void SceneManager::ChangeScene(const std::string& name) {
	assert(_scenes.find(name) != _scenes.end());
	if (_current_scene) {
		_current_scene->_exit();
		// reload the scene into memory
		_scenes[_current_scene_name] = std::make_shared<Scene>(_current_scene->_initialize, _current_scene->_update, _current_scene->_exit);
	}
	_current_scene = _scenes[name];
	_current_scene_name = name;
	_current_scene->_initialize();
}

void SceneManager::AddScene(const std::string& name, scene_initialize init, scene_update update, scene_exit exit) {
	assert(_scenes.find(name) == _scenes.end());
	_scenes[name] = std::make_shared<Scene>(init, update, exit);
}

void SceneManager::Update(const float& dt)
{
	if (_current_scene) {
		_current_scene->_update(dt);
	}
}
