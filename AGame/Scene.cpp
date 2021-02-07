#include "Scene.h"
#include "SceneDeclarations.h"
#include "CSHeaderDef.h"
#include <iostream>

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

	// Registering all components for the game
	ComponentDescription_DB::Instance().RegisterComponent<Position>();

	// Registering all systems for the game

	// Registering scenes
	/*AddScene("Test Scene", TS1_Initialize, TS1_Update, TS1_Exit);
	AddScene("Test Scene 2", TS2_Initialize, TS2_Update, TS2_Exit);*/
	AddScene<TestScene>("Test Scene");
	AddScene<TestScene2>("Test Scene 2");
}

void SceneManager::ChangeScene(const std::string& name) {
	assert(_scenes.find(name) != _scenes.end());
	if (_current_scene) {
		_current_scene->Exit();
		std::cout << "SCENE |" << _current_scene_name << "| exited." << std::endl;
		// reload the scene into memory
		_scenes[_current_scene_name] = std::make_shared<Scene>();
	}
	_current_scene = _scenes[name];
	_current_scene_name = name;
	_current_scene->Initialize();
	std::cout << "SCENE |" << _current_scene_name << "| initialized." << std::endl;
}

//void SceneManager::AddScene(const std::string& name/*, scene_initialize init, scene_update update, scene_exit exit*/)
//}

void SceneManager::Update(const float& dt)
{
	if (_current_scene) {
		_current_scene->Update(dt);
	}
}

void Scene::Initialize()
{
}

void Scene::Update(const float& dt)
{
}

void Scene::Exit()
{
}
