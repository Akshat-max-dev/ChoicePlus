#include"Project.h"

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