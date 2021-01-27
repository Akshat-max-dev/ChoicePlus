#pragma once
#include"c+pch.h"

#include"Property/Model.h"
#include"Property/Transform.h"

#include"src/Log.h"


namespace ChoicePlus
{
	class SceneObject
	{
	public:
		const std::string& Name()const { return mName; }

		template<typename T>
		void AddProperty(const T& property_p) { static_assert(false); }

		template<>
		void AddProperty<Model>(const Model& model)
		{
			if (!HasProperty("Model"))
			{
				mName = model.Name();
				mProperties.insert({ "Model", model });
			}
			else CONSOLE("Property Already Exists{e}");
		}

		template<>
		void AddProperty<Transform>(const Transform& transform)
		{
			if (!HasProperty("Transform"))mProperties.insert({ "Transform", transform });
			else CONSOLE("Property Already Exists{e}");
		}

		template<typename T>
		T* GetProperty() { static_assert(false); }

		template<>
		Model* GetProperty<Model>()
		{
			auto it = mProperties.find("Model");
			if (it != mProperties.end())return std::any_cast<Model>(&it->second);
			return nullptr;
		}

		template<> 
		Transform* GetProperty<Transform>()
		{
			auto it = mProperties.find("Transform");
			if (it != mProperties.end())return std::any_cast<Transform>(&it->second);
			return nullptr;
		}
	private:
		bool HasProperty(const std::string& of)
		{
			if (mProperties.find(of) != mProperties.end())
				return true;
			return false;
		}
	private:
		std::string mName = "Object";
		std::map<std::string, std::any> mProperties;
	};

}