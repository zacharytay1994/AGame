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
	SceneManager();
	std::unordered_map<std::string, std::shared_ptr<Scene>> _scenes;	// container to all added scenes
	std::shared_ptr<Scene>	_current_scene = nullptr;					// current scene being processed, if nullptr, scene state won't update
	std::string				_current_scene_name = "";					// current scene name, assigned by AddScene()
	void Initialize();
public:
	static SceneManager& Instance();
	void ChangeScene(const std::string& name);
	void Free();
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