#ifndef __ANIMATIONIMPORTER_H__
#define __ANIMATIONIMPORTER_H__

#include "assimp/anim.h"
#include "assimp/mesh.h"

struct aiAnimation;
struct aiBone;

class JsonParsing;
class Animation;
struct ModelParameters;

namespace AnimationImporter
{
	void ImportAnimation(aiAnimation* animation, unsigned int uid, JsonParsing& json, std::string& path);
	void SaveAnimation(Animation* animation, unsigned int uid, std::string& path);

}

#endif
