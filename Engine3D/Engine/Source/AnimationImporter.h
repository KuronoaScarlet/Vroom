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
	bool AnimationImport(aiAnimation* animation, unsigned int uid, std::string& newpath);
	bool BonesImport(aiBone* bone, uint UID, std::string& newpath);
}

#endif
