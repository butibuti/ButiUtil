#pragma once
#include<memory>
namespace ButiEngine {
class ObjectFactory {
public:
	template<typename T, typename... Ts>
	static inline std::shared_ptr<T> Create(Ts&&... params) {
		std::shared_ptr<T> Ptr = std::shared_ptr<T>(new T(params...));
		//仮想関数呼び出し
		Ptr->PreInitialize();
		Ptr->Initialize();
		Ptr->SetIsCereal(false);
		return Ptr;
	};
	template<typename T, typename... Ts>
	static inline std::shared_ptr<T> CreateCopy(T value) {
		std::shared_ptr<T> Ptr = std::make_shared<T>(value);
		return Ptr;
	};

	template<typename T, typename... Ts>
	static inline std::shared_ptr<T> CreateFromCereal(const std::string& arg_filePath) {
		std::shared_ptr<T> Ptr = std::shared_ptr<T>();
		if (Util::ExistFile(arg_filePath))
			InputCereal(Ptr, arg_filePath);
		else {
			Ptr = std::make_shared<T>();
		}
		//仮想関数呼び出し
		Ptr->PreInitialize();
		Ptr->Initialize();
		Ptr->SetIsCereal(true);
		return Ptr;
	};

};

struct GUIWindowReaction {
	bool isHovered = false;
	bool isClosed = false;
};
class IObject :public std::enable_shared_from_this<IObject>
{
	friend class ObjectFactory;
	bool isCereal = true;
protected:
	IObject() {}
	virtual ~IObject() {}

	void SetIsCereal(bool arg_created) {
		isCereal = arg_created;
	};
public:
	template<typename T>
	inline std::shared_ptr<T> GetThis() {
		auto Ptr = std::dynamic_pointer_cast<T>(shared_from_this());
		if (Ptr) {
			return Ptr;
		}
		else {
			std::wstring str(L"Can't cast this to");
			str += Util::GetWStringTypeName<T>();
			str += L" type.";
			throw ButiException(
				str,
				L"if( ! dynamic_pointer_cast<T>(shared_from_this()) )",
				L"IObject::GetThis()"
			);
		}
		return nullptr;

	}
	template<typename T>
	inline T* GetThis_ptr() {
		auto Ptr = dynamic_cast<T*>(this);
		if (Ptr) {
			return Ptr;
		}
		else {
			std::wstring str(L"Can't cast this to");
			str += Util::GetWStringTypeName<T>();
			str += L" type.";
			throw ButiException(
				str,
				L"if( ! dynamic_cast<T>(this) )",
				L"IObject::GetThis()"
			);
		}
		return nullptr;

	}
	template<typename T>
	inline bool IsThis() {
		auto Ptr = std::dynamic_pointer_cast<T>(shared_from_this());
		if (Ptr) {
			return true;
		}
		else {
			return false;
		}

	}
	template<typename T>
	inline bool IsThis_ptr() {
		auto Ptr = dynamic_cast<T*>(this);
		if (Ptr) {
			return true;
		}
		else {
			return false;
		}

	}
	bool IsCereal() { return isCereal; }
	virtual void PreInitialize() {}
	virtual void Initialize() {}
	virtual void ShowGUI() {  }
#ifdef BUTIGUI_H



	void RegistGUI() { GUI::RegistGUIObject(GetThis<IObject>()); }
	void UnRegistGUI() { GUI::UnRegistGUIObject(GetThis<IObject>()); }
	void RegistEditorGUI() { GUI::RegistEditorGUIObject(GetThis<IObject>()); }
	void UnRegistEditorGUI() { GUI::UnRegistEditorGUIObject(GetThis<IObject>()); }
#endif // BUTIGUI_H
};
template <typename T>
class ObjectRegistContainer {
public:
	~ObjectRegistContainer() {

		auto end = vec_objectIndex.end();
		for (auto indexItr = vec_objectIndex.begin(); indexItr != end; indexItr++) {
			delete* indexItr;
		}
	}

	void Regist(std::shared_ptr<T> arg_shp_registObj) {
		if (map_objectIndexPtr.count(arg_shp_registObj)) {
			return;
		}
		vec_objectIndex.push_back(new int(vec_objectIndex.size()));
		map_objectIndexPtr.emplace(arg_shp_registObj, vec_objectIndex.back());
		vec_shp_Objects.push_back(arg_shp_registObj);
	}
	void UnRegist(std::shared_ptr<T> arg_shp_unregistObj) {
		if (!map_objectIndexPtr.count(arg_shp_unregistObj)) {
			return;
		}

		auto itr = vec_shp_Objects.begin();
		int index = *map_objectIndexPtr.at(arg_shp_unregistObj);
		itr += index;
		vec_shp_Objects.erase(itr);
		map_objectIndexPtr.erase(arg_shp_unregistObj);
		delete vec_objectIndex.at(index);

		for (auto indexItr = vec_objectIndex.erase(vec_objectIndex.begin() + index), end = vec_objectIndex.end(); indexItr != end; indexItr++) {
			*(*indexItr) -= 1;
		}
	}
	template<void (T::* Method)() >
	void CallFunction() {

		auto currentVec = vec_shp_Objects;
		for (auto itr = currentVec.begin(); itr != currentVec.end(); itr++) {
			//
			
			((*itr).get()->*Method)();

		}
	}

private:
	std::vector< std::shared_ptr<T>>vec_shp_Objects;
	std::map<std::shared_ptr<T>, int*> map_objectIndexPtr;
	std::vector<int*> vec_objectIndex;
};

}