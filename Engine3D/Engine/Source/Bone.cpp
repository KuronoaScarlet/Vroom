#include "Bone.h"
#include "Globals.h"

#include "AnimationImporter.h"
#include "Application.h"
#include "FileSystem.h"

#include "glew/include/GL/glew.h"

#include "Profiling.h"

Bone::Bone(uint uid, std::string& assets, std::string& library) : Resource(uid, ResourceType::BONE, assets, library)
{
	std::string metaPath = BONES_FOLDER + std::string("bone_") + std::to_string(uid) + ".meta";
	AnimationImporter::CreateMetaBones(metaPath, assetsPath, uid);
	name = assets;
	app->fs->GetFilenameWithoutExtension(name);
	name = name.substr(name.find_first_of("__") + 2, name.length());
}

Bone::~Bone()
{
	RELEASE_ARRAY(weights);
}