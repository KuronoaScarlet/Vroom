#include "Animation.h"
#include "Globals.h"

#include "AnimationImporter.h"
#include "Application.h"
#include "FileSystem.h"
#include "AnimationImporter.h"
#include "glew/include/GL/glew.h"

#include "Profiling.h"

Animation::Animation(uint uid, std::string& assets, std::string& library) : Resource(uid, ResourceType::ANIMATION, assets, library)
{
	std::string metaPath = ANIMATIONS_FOLDER + std::string("animation_") + std::to_string(uid) + ".meta";
	name = assets;
	app->fs->GetFilenameWithoutExtension(name);
	name = name.substr(name.find_first_of("__") + 2, name.length());
}
Animation::Animation(unsigned int  uid, ResourceType type)
{

}

Animation::~Animation()
{

}
