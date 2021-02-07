#pragma once
#include <unordered_map>
#include <string>
#include <assert.h>
#include <memory>

//typedef void (*scene_initialize)(void);
//typedef void (*scene_update)(const float& dt);
//typedef void (*scene_exit)(void);

struct Scene {
	/*scene_initialize	_initialize;
	scene_update		_update;
	scene_exit			_exit;*/
	Scene(/*scene_initialize init, scene_update update, scene_exit exit*/) 
		/*:
		_initialize(init),
		_update(update),
		_exit(exit)*/
	{}
	virtual void Initialize();
	virtual void Update(const float& dt);
	virtual void Exit();
};

class SceneManager {
	SceneManager();
	std::unordered_map<std::string, std::shared_ptr<Scene>> _scenes;
	std::shared_ptr<Scene>	_current_scene = nullptr;
	std::string				_current_scene_name = "";
public:
	static SceneManager& Instance();
	void Initialize();
	void ChangeScene(const std::string& name);
	template <typename T>
	void AddScene(const std::string& name) {
		assert(_scenes.find(name) == _scenes.end());
		_scenes[name] = std::make_shared<T>();
	}
	void Update(const float& dt);
};