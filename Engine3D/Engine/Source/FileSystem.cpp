#include "FileSystem.h"
#include "Application.h"
#include "Globals.h"
#include "ModelImporter.h"
#include "ResourceManager.h"
#include "ModuleEditor.h"
#include "GameObject.h"

#include "assimp/cimport.h"
#include "AssimpDefs.h"
#include "IL/il.h"
#include "Resource.h"

#include <vector>
#include <stack>

#include "SDL/include/SDL_filesystem.h"
#include "Profiling.h"

FileSystem::FileSystem(const char* assetsPath) : name("FileSystem")
{
	char* basePath = SDL_GetBasePath();
	PHYSFS_init(basePath);
	SDL_free(basePath);

	// workaround VS string directory mess
	AddPath(".");

	if (0 && assetsPath != nullptr)
		AddPath(assetsPath);

	// Dump list of paths
	DEBUG_LOG("FileSystem Operations base is [%s] plus:", GetBasePath());
	DEBUG_LOG(GetReadPaths());

	// enable us to write in the game's dir area
	if (PHYSFS_setWriteDir(".") == 0)
		DEBUG_LOG("File System error while creating write dir: %s\n", PHYSFS_getLastError());

	// Make sure standard paths exist
	const char* dirs[] = {
		RESOURCES_FOLDER, SETTINGS_FOLDER, LIBRARY_FOLDER, TEXTURES_FOLDER, MESHES_FOLDER, SCENES_FOLDER, MODELS_FOLDER, ANIMATIONS_FOLDER, BONES_FOLDER
	};

	for (uint i = 0; i < sizeof(dirs) / sizeof(const char*); ++i)
	{
		if (PHYSFS_exists(dirs[i]) == 0)
			PHYSFS_mkdir(dirs[i]);
	}

	// Generate IO interfaces
	CreateAssimpIO();

	ilInit();
	
	texExtension = { ".png", ".jpg", ".dds", ".tga"};
	modelExtension = { ".obj", ".fbx", ".3DS", ".FBX"};
}

FileSystem::~FileSystem()
{
	RELEASE(assimpIO);
	aiDetachAllLogStreams();
	PHYSFS_deinit();
}

bool FileSystem::Init(JsonParsing& node)
{
	DEBUG_LOG("Loading File System");
	bool ret = true;

	// Ask SDL for a write dir
	char* writePath = SDL_GetPrefPath(app->GetOrganizationName(), app->GetAppName());

	// Trun this on while in game mode
	//if(PHYSFS_setWriteDir(write_path) == 0)
		//LOG("File System error while creating write dir: %s\n", PHYSFS_getLastError());


	SDL_free(writePath);

	return ret;
}

bool FileSystem::CleanUp()
{
	return true;
}

bool FileSystem::AddPath(const char* path)
{
	bool ret = false;

	if (PHYSFS_mount(path, nullptr, 1) == 0)
		DEBUG_LOG("File System error while adding a path or zip: %s\n", PHYSFS_getLastError());
	else
		ret = true;

	return ret;
}

uint FileSystem::Load(const char* file, char** buffer)
{
	uint ret = 0;

	PHYSFS_file* fsFile = PHYSFS_openRead(file);

	if (fsFile != nullptr)
	{
		PHYSFS_sint32 size = (PHYSFS_sint32)PHYSFS_fileLength(fsFile);

		if (size > 0)
		{
			*buffer = new char[size];
			uint readed = (uint)PHYSFS_read(fsFile, *buffer, 1, size);
			if (readed != size)
			{
				DEBUG_LOG("File System error while reading from file %s: %s\n", file, PHYSFS_getLastError());
				RELEASE_ARRAY(buffer);
			}
			else
				ret = readed;
		}

		if (PHYSFS_close(fsFile) == 0)
			DEBUG_LOG("File System error while closing file %s: %s\n", file, PHYSFS_getLastError());
	}
	else
		DEBUG_LOG("File System error while opening file %s: %s\n", file, PHYSFS_getLastError());

	return ret;
}

uint FileSystem::Save(const char* file, const void* buffer, unsigned int size, bool append)
{
	unsigned int ret = 0;

	bool overWrite = PHYSFS_exists(file) != 0;
	PHYSFS_file* fsFile = (append) ? PHYSFS_openAppend(file) : PHYSFS_openWrite(file);

	if (fsFile != nullptr)
	{
		uint written = (uint)PHYSFS_write(fsFile, (const void*)buffer, 1, size);
		if (written != size)
			DEBUG_LOG("File System error while writing to file %s: %s", file, PHYSFS_getLastError());
		else
		{
			if (append == true)
				DEBUG_LOG("Added %u data to [%s%s]", size, PHYSFS_getWriteDir(), file);
			else if (overWrite == false)
				DEBUG_LOG("New file created [%s%s] of %u bytes", PHYSFS_getWriteDir(), file, size);

			ret = written;
		}

		if (PHYSFS_close(fsFile) == 0)
			DEBUG_LOG("File System error while closing file %s: %s", file, PHYSFS_getLastError());
	}
	else
		DEBUG_LOG("File System error while opening file %s: %s", file, PHYSFS_getLastError());

	return ret;
}

const char* FileSystem::GetReadPaths() const
{
	static char paths[512];

	paths[0] = '\0';

	char** path;
	for (path = PHYSFS_getSearchPath(); *path != nullptr; ++path)
	{
		strcat_s(paths, 512, *path);
		strcat_s(paths, 512, "\n");
	}

	return paths;
}

void FileSystem::LoadFile(std::string& path)
{
	std::string extension = path.substr(path.find_last_of(".", path.length()));
	std::list<std::string>::iterator s;
	std::list<std::string>::iterator end = modelExtension.end();
	
	for (s = modelExtension.begin(); s != end; ++s)
	{
		if (*s == extension)
		{
			RG_PROFILING_FUNCTION("Loading Model");
			ModelImporter::LoadModel(path);
			return;
		}
	}

	end = texExtension.end();

	for (s = texExtension.begin(); s != end; ++s)
	{
		if (*s == extension)
		{
			RG_PROFILING_FUNCTION("Loading Texture");
			// When mouse picking fixed, this will work correctly
			//app->editor->GetGO()->GetComponent<MaterialComponent>()->SetTexture(ResourceManager::GetInstance()->IsTextureLoaded(path));
			return;
		}
	}
}

void FileSystem::ImportFiles(std::string& path)
{
	std::stack<std::string> dirsStack;
	dirsStack.push(path);

	while (!dirsStack.empty())
	{
		// First we get the item at the top of the stack
		std::string dir = dirsStack.top();
		
		// Then we create the vectors of files and dirs
		std::vector<std::string> files;
		std::vector<std::string> dirs;

		// We get the files and dirs of the current directory, that it's the top of the stack
		DiscoverFilesAndDirs(dir.c_str(), files, dirs);

		// We iterate all the files on the current directory
		for (std::vector<std::string>::iterator it = files.begin(); it != files.end(); ++it)
		{
			CheckExtension(*it);
		}

		// Now we pop the current directory we don't need it anymore
		dirsStack.pop();

		// Finally we iterate all the dirs on the current directory and we push them to the stack
		for (std::vector<std::string>::iterator it = dirs.begin(); it != dirs.end(); ++it)
		{
			dirsStack.push(*it);
		}
	}
}

void FileSystem::LoadFiles()
{
	std::stack<std::string> dirsStack;
	dirsStack.push(std::string("Library/"));

	while (!dirsStack.empty())
	{
		std::string dir = dirsStack.top();

		std::vector<std::string> files;
		std::vector<std::string> dirs;

		DiscoverFilesAndDirs(dir.c_str(), files, dirs);

		for (std::vector<std::string>::iterator it = files.begin(); it != files.end(); ++it)
		{
			if ((*it).find(".vrmesh") != std::string::npos)
			{
				
			}
			else if ((*it).find(".dds") != std::string::npos)
			{

			}
		}

		dirsStack.pop();

		for (std::vector<std::string>::iterator it = dirs.begin(); it != dirs.end(); ++it)
		{
			dirsStack.push(*it);
		}
	}
}

void FileSystem::ImportFromOutside(std::string& source, std::string& destination)
{
	char buffer[8192];
	size_t size;

	FILE* file = nullptr;
	fopen_s(&file, source.c_str(), "rb");
	
	std::string name = source;
	GetFilenameWithExtension(name);

	name = destination + name;
	PHYSFS_file* dest = PHYSFS_openWrite(name.c_str());

	if (file && dest)
	{
		while (size = fread_s(buffer, 8192, 1, 8192, file))
		{
			PHYSFS_write(dest, buffer, 1, size);
		}

		fclose(file);
		PHYSFS_close(dest);
	}
}

ResourceType FileSystem::CheckExtension(std::string& path)
{
	std::string extension = path.substr(path.find_last_of(".", path.length()));
	std::list<std::string>::iterator s;
	std::list<std::string>::iterator end = modelExtension.end();

	if (extension.data() == std::string(".knekro")) return ResourceType::SCENE;

	for (s = modelExtension.begin(); s != end; ++s)
	{
		if (*s == extension)
		{
			RG_PROFILING_FUNCTION("Importing Model");
			return ResourceType::MODEL;
		}
	}

	end = texExtension.end();

	for (s = texExtension.begin(); s != end; ++s)
	{
		if (*s == extension)
		{
			RG_PROFILING_FUNCTION("Importing Texture");
			return ResourceType::TEXTURE;
		}
	}

	return ResourceType::NONE;
}

void FileSystem::DiscoverFilesAndDirs(const char* directory, std::vector<std::string>& fileList, std::vector<std::string>& dirList)
{
	char** rc = PHYSFS_enumerateFiles(directory);
	char** i;

	std::string dir(directory);

	for (i = rc; *i != nullptr; ++i)
	{
		if (PHYSFS_isDirectory((dir + *i).c_str()))
			dirList.push_back(dir + *i + "/");
		else
			fileList.push_back(dir + *i);
	}

	PHYSFS_freeList(rc);
}

void FileSystem::DiscoverFiles(const char* directory, std::vector<std::string>& fileList)
{
	char** rc = PHYSFS_enumerateFiles(directory);
	char** i;

	std::string dir(directory);

	for (i = rc; *i != nullptr; ++i)
	{
		if (!PHYSFS_isDirectory((dir + *i).c_str()))
			fileList.push_back(*i);
	}

	PHYSFS_freeList(rc);
}

void FileSystem::DiscoverDirs(const char* directory, std::vector<std::string>& dirList)
{
	char** rc = PHYSFS_enumerateFiles(directory);
	char** i;

	std::string dir(directory);

	for (i = rc; *i != nullptr; ++i)
	{
		if (PHYSFS_isDirectory((dir + *i).c_str()))
			dirList.push_back(dir + *i + "/");
	}

	PHYSFS_freeList(rc);
}

void FileSystem::NormalizePath(std::string& path)
{
	for (int i = 0; i < path.length(); ++i)
		if (path[i] == '\\') path[i] = '/';
}

void FileSystem::GetRelativeDirectory(std::string& path)
{
	NormalizePath(path);

	if (path.find("/") != std::string::npos)
	{
		path = path.substr(0, path.length() - 1);
		path = path.substr(path.find_last_of("/") + 1, path.length());
	}
}

void FileSystem::GetFilenameWithExtension(std::string& path)
{
	NormalizePath(path);

	if (path.find("/") != std::string::npos)
		path = path.substr(path.find_last_of("/") + 1, path.length());
}

void FileSystem::GetFilenameWithoutExtension(std::string& path)
{
	NormalizePath(path);

	if (path.find("/") != std::string::npos)
	{
		path = path.substr(path.find_last_of("/") + 1, path.length());
	}
	path = path.substr(0, path.find_last_of("."));
}

bool FileSystem::RemoveFile(const char* file)
{
	bool ret = false;

	if (file != nullptr)
	{
		if (PHYSFS_delete(file) == 0)
		{
			LOG("File deleted: [%s]", file);
			ret = true;
		}
		else
			LOG("File System error while trying to delete [%s]: ", file, PHYSFS_getLastError());
	}

	return ret;
}

void FileSystem::CreateDir(const char* directory)
{
	PHYSFS_mkdir(directory);
}

void FileSystem::CreateAssimpIO()
{
	assimpIO = new aiFileIO;
	assimpIO->OpenProc = AssimpOpen;
	assimpIO->CloseProc = AssimpClose;

	aiLogStream stream;
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr);
	aiAttachLogStream(&stream);
}

void FileSystem::SplitPath(std::string full_path, std::string* path, std::string* filename, std::string* extension)
{
	NormalizePath(full_path);
	uint pos_slash = full_path.find_last_of('/');
	uint pos_dot = full_path.find_last_of('.');

	if (path != nullptr)
	{
		if (pos_slash < full_path.length())
			*path = full_path.substr(0, pos_slash + 1);
		else
			path->clear();
	}
	if (filename != nullptr)
	{
		if (pos_slash < full_path.length())
			*filename = full_path.substr(pos_slash + 1, pos_dot - pos_slash - 1);
		else
			*filename = full_path.substr(0, pos_dot);
	}
	if (extension != nullptr)
	{
		if (pos_dot < full_path.length())
			*extension = full_path.substr(pos_dot + 1);
		else
			extension->clear();
	}
}