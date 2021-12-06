#include "Globals.h"
#include "Application.h"
#include "ModuleScene.h"
#include "glew.h"
#include "ImGui/imgui.h"
#include "ModuleImport.h"
#include "ModuleTextures.h"
#include "ModuleCamera3D.h"
#include "Component.h"
#include <stack>
#include <queue>
#include "ModuleViewportFrameBuffer.h"
#include "CameraComponent.h"
#include "ModuleEditor.h"
#include "ModuleRenderer3D.h"

#include "rapidjson-1.1.0/include/rapidjson/rapidjson.h"

ModuleScene::ModuleScene(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

bool ModuleScene::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;
	
	root = new GameObject("Root");
	camera = CreateGameObject("Camera", root);
	camera->CreateComponent<CameraComponent>();
	camera->GetComponent<CameraComponent>()->Start();
	assets = new File("Assets");
	assets->path = assets->name;
	assets->ReadFiles();

	//Loading house and textures since beginning
	App->import->LoadGeometry("Assets/Models/street_env.fbx");
	
	return ret;
}

bool ModuleScene::CleanUp()
{
	std::stack<GameObject*> S;
	for (GameObject* child : root->children)	
	{
		S.push(child);
	}
	root->children.clear();

	while (!S.empty())
	{
		GameObject* go = S.top();
		S.pop();
		for (GameObject* child : go->children)
		{
			S.push(child);
		}
		go->children.clear();
		delete go;
	}

//	delete root;

	return true;
}

update_status ModuleScene::Update(float dt)
{
	App->viewportBuffer->PreUpdate(dt);
	std::queue<GameObject*> S;
	for (GameObject* child : root->children)
	{
		S.push(child);
	}

	while (!S.empty())
	{
		GameObject* go = S.front();
		go->Update(dt);
		S.pop();
		for (GameObject* child : go->children)
		{
			S.push(child);
		}
	}
	if(App->renderer3D->drawRayLine) App->renderer3D->DrawRayCast(App->camera->picking);
	App->editor->DrawGrid();
	App->viewportBuffer->PostUpdate(dt);
	if (camera != nullptr)
	{
		camera->GetComponent<CameraComponent>()->PreUpdate(dt);
		std::queue<GameObject*> S;
		for (GameObject* child : root->children)
		{
			S.push(child);
		}

		while (!S.empty())
		{
			GameObject* go = S.front();
			go->Update(dt);
			S.pop();
			for (GameObject* child : go->children)
			{
				S.push(child);
			}
		}
		App->viewportBufferGame->PostUpdate(dt);
	}

	return UPDATE_CONTINUE;
}

GameObject* ModuleScene::CreateGameObject(GameObject* parent) {

	GameObject* temp = new GameObject();
	if (parent)
		parent->AttachChild(temp);
	else
		root->AttachChild(temp);
	return temp;
}
GameObject* ModuleScene::CreateGameObject(const std::string name, GameObject* parent)
{
	GameObject* temp = new GameObject(name);
	if (parent)
		parent->AttachChild(temp);
	else
		root->AttachChild(temp);
	return temp;
}

void ModuleScene::Save()
{
	rapidjson::StringBuffer sb;
	JSONWriter writer(sb);

	writer.StartObject();
	writer.String("GameObjects");
	writer.StartArray();
	root->Save(writer);
	writer.EndArray();
	writer.EndObject();
	App->fileSystem->CreateDir("Library/Scenes/");
	if (App->fileSystem->Save("Library/Scenes/vroomScene.knekro", sb.GetString(), strlen(sb.GetString()), false))
	{
		LOG("Scene saved.");
	}
	else
	{
		LOG("Scene not saved.");
	}
}

void ModuleScene::Load(const char* file)
{
	char* buffer = nullptr;
	if (App->fileSystem->Load(file, &buffer))
	{
		rapidjson::Document document;
		if (document.Parse<rapidjson::kParseStopWhenDoneFlag>(buffer).HasParseError()==false)
		{
			const rapidjson::Value reader = document.GetObjectJSON();

			if (reader.HasMember("GameObjects"))
			{
				auto& a = reader["GameObjects"];
				if (a.IsArray())
				{
					for (int i = 0; i < a.MemberCount(); ++i)
					{
						GameObject n;
						n.Load(reader);
						root->AttachChild(&n);
					}
				}
			}

			LOG("Engine config loaded");
		}
	}
	RELEASE_ARRAY(buffer);
}
