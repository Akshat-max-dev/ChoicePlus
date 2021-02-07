#include"SceneContainer.h"

#include"src/Log.h"
#include"src/Scene/SceneObject.h"
#include<glm/gtc/type_ptr.hpp>

namespace ChoicePlus
{
	void SceneContainer::ContainScene(Scene* scene, const std::string& scenepath)
	{
		std::ofstream container(scenepath, std::ios::out | std::ios::binary);
		if (!container.is_open() && container.bad())
		{
			std::string msg = "Failed to save scene " + scene->mName + "{e}";
			CONSOLE(msg.c_str());
			return;
		}

		uint32_t namesize = (uint32_t)scene->mName.size();
		container.write((char*)&namesize, sizeof(namesize));
		container.write((char*)scene->mName.data(), namesize);

		uint32_t sceneobjectssize = (uint32_t)scene->mSceneObjects.size();
		container.write((char*)&sceneobjectssize, sizeof(sceneobjectssize));

		for (auto& object : scene->mSceneObjects)
		{
			uint32_t proptypesize;
			std::string proptype;

			auto modelprop = object->GetProperty<Model>();
			if (modelprop)
			{
				proptype = "Model";
				proptypesize = (uint32_t)proptype.size();
				container.write((char*)&proptypesize, sizeof(proptypesize));
				container.write((char*)proptype.data(), proptypesize);

				uint32_t srcfilesize = (uint32_t)modelprop->mSrcFile.size();
				container.write((char*)&srcfilesize, sizeof(srcfilesize));
				container.write((char*)modelprop->mSrcFile.data(), srcfilesize);
			}

			auto transformprop = object->GetProperty<Transform>();
			if (transformprop)
			{
				proptype = "Transform";
				proptypesize = (uint32_t)proptype.size();
				container.write((char*)&proptypesize, sizeof(proptypesize));
				container.write((char*)proptype.data(), proptypesize);

				container << transformprop->Position.x << " " << transformprop->Position.y << " " << transformprop->Position.z << " ";
				glm::vec3 rotation = glm::degrees(transformprop->Rotation);
				container << rotation.x << " " << rotation.y << " " << rotation.z << " ";
				container << transformprop->Scale.x << " " << transformprop->Scale.y << " " << transformprop->Scale.z;
			}
		}

		container.close();
	}

	static std::string ReadPropertyType(std::ifstream& containedscene)
	{
		uint32_t proptypesize;
		containedscene.read((char*)&proptypesize, sizeof(proptypesize));
		std::string proptype;
		proptype.resize(proptypesize);
		containedscene.read((char*)proptype.data(), proptypesize);
		return proptype;
	}

	Scene* SceneContainer::LoadContainedScene(const std::string& scenepath)
	{
		std::ifstream containedscene(scenepath, std::ios::in | std::ios::binary);

		if (!containedscene.is_open() && containedscene.bad())
		{
			std::string msg = "Failed to load Scene " + std::filesystem::path(scenepath).stem().string();
			msg.append("{e}");
			CONSOLE(msg.c_str());
			return nullptr;
		}

		uint32_t namesize;
		containedscene.read((char*)&namesize, sizeof(namesize));
		std::string scenename;
		scenename.resize(namesize);
		containedscene.read((char*)scenename.data(), namesize);

		Scene* scene = new Scene(scenename);

		uint32_t objectssize;
		containedscene.read((char*)&objectssize, sizeof(objectssize));

		scene->mSceneObjects.resize(objectssize);
		for (auto& object : scene->mSceneObjects)
		{
			object = new SceneObject();
			
			std::string proptype = ReadPropertyType(containedscene);

			if (proptype == "Model")
			{
				uint32_t srcfilesize;
				containedscene.read((char*)&srcfilesize, sizeof(srcfilesize));
				std::string srcfile;
				srcfile.resize(srcfilesize);
				containedscene.read((char*)srcfile.data(), srcfilesize);

				object->AddProperty<Model>(Load(srcfile));
				proptype = ReadPropertyType(containedscene);
			}
			
			if (proptype == "Transform")
			{
				Transform* transform = new Transform();
				glm::vec3 rotation;
				containedscene >> transform->Position.x >> transform->Position.y >> transform->Position.z >> 
					rotation.x >> rotation.y >> rotation.z >> 
					transform->Scale.x >> transform->Scale.y >> transform->Scale.z;
				transform->Rotation = glm::radians(rotation);

				object->AddProperty<Transform>(transform);
			}
		}

		containedscene.close();

		return scene;
	}
}