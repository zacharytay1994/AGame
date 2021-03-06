/******************************************************************************/
/*!
\File Name		: Scene.h
\Project Name	: AGame
\Authors 		:
				Primary - Zachary Tay (100%)
				Secondary -
\brief		SceneManager system for all scenes

All content � 2021 DigiPen Institute of Technology Singapore. All
rights reserved.
*/
/******************************************************************************/
#pragma once
#include <unordered_map>
#include <string>
#include <assert.h>
#include <memory>

/*___________________________________________________________________________________
* Brief:	Scene class is mainly meant to be virtual.
*			To be overriden to create custom scenes.
* 
* Relation:	SceneManager
*			- std::unordered_map<std::string, std::shared_ptr<Scene>> _scenes;
____________________________________________________________________________________*/
class Scene {
	friend class SceneManager;
	virtual void Initialize();
	virtual void Update(const float& dt);
	virtual void Exit();
	// new functions
	virtual void Load();
	virtual void Draw(const float& dt);
	virtual void Unload();
public:
	Scene() {}
};

/*___________________________________________________________________________________
* Brief:	SceneManager class is the boss of all scenes. In charge of
*			loading and changing scenes. Register your new scenes declared in
*			SceneDeclarations.h here.
* 
* LookAt:	AddScene(), ChangeScene()
____________________________________________________________________________________*/
class SceneManager {
	int _gamerunning = 1;
	SceneManager();
	std::unordered_map<std::string, std::shared_ptr<Scene>> _scenes;	// container to all added scenes
	std::shared_ptr<Scene>	_current_scene = nullptr;					// current scene being processed, if nullptr, scene state won't update
	std::string				_current_scene_name = "";					// current scene name, assigned by AddScene()
	void Initialize();
	std::string				_next_scene = "";
	float					_delay{ 0.0f };
	bool					_lock{ false };
public:
	static SceneManager& Instance();
	void ChangeScene(const std::string& name);
	void ChangeSceneNow(const std::string& name);
	void Free();
	bool _pause{ false };
	bool _currentlyplaying{ false };
	bool _inlevel{ false };
	bool _musicmmute{ false };
	bool _fullscreen{ true };
	bool _settings_toggle{ false };
	void CheckGame(int& gamerunning) {
		gamerunning = _gamerunning;
	}
	void StopGame() {
		_gamerunning = 0;
	}
	/*___________________________________________________________________________________
	* Brief:	Creates a scene of type derived scene.
	*
	* Access:	public
	* 
	* Used e.g:	SceneManager::Instance().AddScene<MyDerivedScene>();
	*			- look at SceneDeclarations.h for example scenes and
	*			  SceneManager::Initialize() for example adding.
	* 
	* Called:	in SceneManager::Initialize()
	* 
	* arg[in]:	name
	*			- The name of the scene to change to,
	*			  assigned at SceneManager::AddScene()
	____________________________________________________________________________________*/
	template <typename T>
	void AddScene(const std::string& name) {
		assert(_scenes.find(name) == _scenes.end());
		_scenes[name] = std::make_shared<T>();
	}
	void Update(const float& dt);

	// new functions
	void RestartScene();
	void Load();
	void Draw(const float& dt);
	void Unload();
};