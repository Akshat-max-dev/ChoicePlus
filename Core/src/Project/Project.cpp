#include"Project.h"

#include"src/Log.h"

namespace ChoicePlus
{
	Project::Project(const std::string& name, const std::string& directory)
	{
		mProjectName = name;
		mProjectFolder = directory + "\\" + name;
		mProjectFolder.append("\\");
		std::filesystem::create_directory(std::filesystem::path(directory + name));

		std::ofstream projfile(mProjectFolder + mProjectName + ".cpproj", std::ios::out | std::ios::binary);
		cpassert(!projfile.is_open() && projfile.bad());
		projfile.close();
	}

	Project::Project(const std::string& location)
	{
		mProjectName = std::filesystem::path(location).stem().string();
		mProjectFolder = location.substr(0, location.find_last_of('\\') + 1);

		std::ifstream projfile(location, std::ios::in | std::ios::binary);

		if (!projfile.is_open() && projfile.bad())
		{
			std::string msg = "Failed To Open Project " + mProjectName + " {e}";
			CONSOLE(msg.c_str());
			return;
		}

		uint32_t numberofscenes;
		projfile.read((char*)&numberofscenes, sizeof(numberofscenes));

		mSceneNames.resize(numberofscenes);

		uint32_t activescenenamesize;
		projfile.read((char*)&activescenenamesize, sizeof(activescenenamesize));
		std::string activescene;
		activescene.resize(activescenenamesize);
		projfile.read((char*)activescene.data(), activescenenamesize);

		mActiveScene = activescene;

		for (auto& scenename : mSceneNames)
		{
			uint32_t scenenamesize;
			projfile.read((char*)&scenenamesize, sizeof(scenenamesize));
			scenename.resize(scenenamesize);
			projfile.read((char*)scenename.data(), scenenamesize);
		}

		projfile.close(); 
	}

	void Project::AddScene(Scene* scene)
	{
		if (scene)
		{
			mSceneNames.emplace_back(scene->Name());
			std::filesystem::create_directory(std::filesystem::path(mProjectFolder + scene->Name()));
			std::unique_ptr<SceneContainer> scenecontainer = std::make_unique<SceneContainer>();
			scenecontainer->ContainScene(scene, mProjectFolder + scene->Name());
		}
	}
}