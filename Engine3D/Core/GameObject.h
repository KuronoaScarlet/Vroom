#pragma once

#include <vector>
#include <string>

#include "rapidjson-1.1.0/include/rapidjson/prettywriter.h"
#include "rapidjson-1.1.0/include/rapidjson/document.h"

#include "Algorithm/Random/LCG.h"

typedef rapidjson::PrettyWriter<rapidjson::StringBuffer> JSONWriter;
typedef rapidjson::Value JSONReader;

class Component;
class ComponentTransform;

class GameObject {

public:

	GameObject();
	GameObject(const std::string name);

	~GameObject();

	void Update(float dt);
	void OnGui();

	template<class T> T* CreateComponent()
	{
		T* newComponent = new T(this);
		return newComponent;
	}

	template<class T> T* GetComponent()
	{
		T* component = nullptr; 
		for (Component* c : components)
		{
			component = dynamic_cast<T*>(c);
			if (component)
				break;
		}
		return component;
	}

	void DeleteComponent(Component* component);
	void AddComponent(Component* component);
	void AttachChild(GameObject* child);
	void RemoveChild(GameObject* child);
	void PropagateTransform();
	void EraseGameObject();

	std::string name;
	GameObject* parent = nullptr;
	ComponentTransform* transform = nullptr;
	std::vector<GameObject*> children;
	std::vector<Component*> components;
	
	bool active = true;
	bool isSelected = false;

	unsigned __int32 UUID;
	void GenerateUUID();

	void Save(JSONWriter& writer);
	void Load(const JSONReader& reader);

	LCG uidGen;
};

