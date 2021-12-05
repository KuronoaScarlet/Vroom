#pragma once

#include "Component.h"

struct TextureObject;

class ComponentMaterial : public Component {

public:

	ComponentMaterial(GameObject* parent);

	void SetTexture(const TextureObject& texture);
	void OnGui() override;
	inline uint GetTextureId() const { return textureId; }
	void Save(JSONWriter& writer);
	void Load(const JSONReader& reader);

private:

	std::string textureName;
	uint textureId = 0, width = 0, height = 0;
};