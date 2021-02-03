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
		~SceneObject()
		{
			if (mModelProp.has_value())if (mModelProp.value())delete mModelProp.value();
			if (mTransformProp.has_value())if (mTransformProp.value())delete mTransformProp.value();
		}
		const std::string& Name()const { return mName; }

		template<typename T>
		void AddProperty(T* property_p) { static_assert(false); }

		template<>
		void AddProperty<Model>(Model* model)
		{
			if (!mModelProp.has_value())
			{
				mModelProp = { model };
				mName = mModelProp.value()->mName;
				Transform* transform = new Transform();
				AddProperty<Transform>(transform);
			}
			else CONSOLE("Property Already Exists{e}");
		}

		template<>
		void AddProperty<Transform>(Transform* transform)
		{
			if (!mTransformProp.has_value())mTransformProp = { transform };
			else CONSOLE("Property Already Exists{e}");
		}

		template<typename T>
		T* GetProperty() { static_assert(false); }

		template<>
		Model* GetProperty<Model>()
		{
			if (mModelProp.has_value())if (mModelProp.value()) { return mModelProp.value(); }
			return nullptr;
		}

		template<> 
		Transform* GetProperty<Transform>()
		{
			if (mTransformProp.has_value())if (mTransformProp.value()) { return mTransformProp.value(); }
			return nullptr;
		}
	private:
		std::string mName = "";
		std::optional<Model*> mModelProp;
		std::optional<Transform*> mTransformProp;
	};

}