#include "Application.h"
#include "ModuleTextures.h"
#include "ImGui/imgui.h"
#include "ComponentMaterial.h"

ComponentMaterial::ComponentMaterial(GameObject* parent) : Component(parent) {}

void ComponentMaterial::SetTexture(const TextureObject& texture)
{
	textureName = texture.name;
	textureId = texture.id;
	width = texture.width;
	height = texture.height;
}

void ComponentMaterial::OnGui()
{
	if (ImGui::CollapsingHeader("Material"))
	{
		if (textureId != 0)
		{
			ImGui::Text("Name: %s", textureName.c_str());
			ImGui::Image((ImTextureID)textureId, ImVec2(128, 128), ImVec2(0, 1), ImVec2(1, 0));
			ImGui::Text("Size: %d x %d", width, height);
		}
	}
}

void ComponentMaterial::Save(JSONWriter& writer)
{
	writer.StartObject();
	writer.String("material");
	writer.StartArray();

	writer.String("name");
	writer.String(textureName.c_str());

	writer.String("width");
	writer.Uint(width);
	writer.String("height");
	writer.Uint(height);

	writer.EndArray();
	writer.EndObject();
}

void ComponentMaterial::Load(const JSONReader& reader)
{
	if (reader.HasMember("name"))
		textureName = reader["name"].GetString();
}

