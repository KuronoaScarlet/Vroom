#include "GameObject.h"
#include "Application.h"
#include "ModuleScene.h"
#include "ModuleEditor.h"
#include "ModuleFileSystem.h"
#include "ComponentTransform.h"
#include "ImGui/imgui.h"

GameObject::GameObject() {

	name = name + ("GameObject");
	parent = nullptr;

	transform = CreateComponent<ComponentTransform>();

	active = true;
}

GameObject::GameObject(const std::string name) : name(name) 
{
	transform = CreateComponent<ComponentTransform>();

	active = true;
}

GameObject::~GameObject() {

	for (size_t i = 0; i < components.size(); i++) {
		RELEASE(components[i]);
	}

	components.clear();

	for (GameObject* go : children)
	{
		RELEASE(go);
	}

	parent = nullptr;
}

void GameObject::Update(float dt) 
{
	for (Component* component : components)
	{
		component->Update(dt);
	}
	if (isSelected)
	{
		if (this->name != "Camera")
			this->GetComponent<ComponentMesh>()->drawOBB = true;
	}
	else
	{
		if (this->name != "Camera")
			this->GetComponent<ComponentMesh>()->drawOBB = false;
	}
}

void GameObject::OnGui()
{
	if (App->scene->root != this)
	{
		ImGui::Text("%s", name.c_str());
		ImGui::Separator();

		for (Component* component : components)
		{
			component->OnGui();
		}
	}
}

void GameObject::DeleteComponent(Component* component) {

	auto componentIt = std::find(components.begin(), components.end(), component);
	if (componentIt != components.end())
	{
		components.erase(componentIt);
		components.shrink_to_fit();
	}
}

void GameObject::AddComponent(Component* component)
{
	components.push_back(component);
}

void GameObject::EraseGameObject()
{
	for (int i = 0; i < App->scene->root->children.size(); i++)
	{
		if (App->scene->root->children.at(i) == App->editor->gameobjectSelected)
		{
			std::vector<GameObject*> L;
			for (int i = 0; i < App->scene->root->children.size(); i++)
			{
				if (App->scene->root->children.at(i) != App->editor->gameobjectSelected)
				{
					L.push_back(App->scene->root->children.at(i));
				}
			}
			App->editor->gameobjectSelected = nullptr;
			App->scene->root->children.clear();
			for (int i = 0; i < L.size(); i++)
			{
				App->scene->root->children.push_back(L.at(i));
			}
		}
	}
}

void GameObject::GenerateUUID()
{
	UUID = 0;
}

void GameObject::AttachChild(GameObject* child)
{
	child->parent = this;
	children.push_back(child);
	child->transform->NewAttachment();
	child->PropagateTransform();
}

void GameObject::RemoveChild(GameObject* child)
{
	auto it = std::find(children.begin(), children.end(), child);
	if (it != children.end())
	{
		children.erase(it);
	}
}

void GameObject::PropagateTransform()
{
	for (GameObject* go : children)
	{
		go->transform->OnParentMoved();
	}
}

void GameObject::Save(JSONWriter& writer)
{
	writer.StartObject();

	writer.String("name");
	writer.String(name.c_str());

	writer.String("UUID");
	writer.Int(UUID);

	writer.String("parent UUID");
	writer.Int((parent != nullptr) ? parent->UUID : 0);

	writer.String("components");
	writer.StartArray();
	for (size_t i = 0; i < components.size(); ++i)
	{
		components[i]->Save(writer);
	}
	writer.EndArray();

	writer.EndObject();

	if (!children.empty())
		for (size_t i = 0; i < children.size(); ++i)
	{
		children[i]->Save(writer);
	}
}
