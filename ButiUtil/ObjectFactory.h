#pragma once
#include"Util.h"
#include"ButiMemorySystem/ButiMemorySystem/ButiPtr.h"
#ifdef _BUTIENGINEBUILD
#include"ButiEngineHeader/Header/Device/GUIWindowReaction.h"
#endif // _BUTIENGINEBUILD

#include<vector>
#include<map>
namespace ButiEngine {

class IObject : public enable_value_from_this<IObject>
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
	inline Value_ptr<T> GetThis() {
		auto Ptr = dynamic_value_ptr_cast<T>(value_from_this());
		if (Ptr) {
			return Ptr;
		}
		else {
			std::wstring str(L"Can't cast this to");
			str += Util::GetWStringTypeName<T>();
			str += L" type.";
			throw ButiException(
				str,
				L"if( ! dynamic_value_ptr_cast<T>(value_from_this()) )",
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
		auto Ptr = dynamic_value_ptr_cast<T>(value_from_this());
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
class ObjectFactory {
public:
	template<typename T, typename... Ts>
	static inline Value_ptr<T> Create(Ts&&... params) {
		Value_ptr<T> output = make_value<T>(params...);
		if constexpr (std::is_base_of_v<IObject,T>) {
			output->PreInitialize();
			output->Initialize();
			output->SetIsCereal(false);
		}
		return output;
	};
	template<typename T>
	static inline Value_ptr<T> CreateCopy(const T& arg_value) {
		return make_value<T>(arg_value);
	};


	template<typename T, typename... Ts>
	static inline Value_ptr<T> CreateFromCereal(const std::string& arg_filePath) {
		Value_ptr<T> output= Value_ptr<T>();
		if (Util::ExistFile(arg_filePath))
			InputCereal(output, arg_filePath);
		else {
			output= make_value<T>();
		}
		if constexpr (std::is_base_of_v<IObject,T>) {
			output->PreInitialize();
			output->Initialize();
			output->SetIsCereal(false);
		}
		return output;
	};

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

	void Regist(Value_ptr<T> arg_vlp_registObj) {
		if (map_objectIndexPtr.count(arg_vlp_registObj)) {
			return;
		}
		vec_objectIndex.push_back(new std::int32_t(vec_objectIndex.size()));
		map_objectIndexPtr.emplace(arg_vlp_registObj, vec_objectIndex.back());
		vec_vlp_Objects.push_back(arg_vlp_registObj);
	}
	void UnRegist(Value_ptr<T> arg_vlp_unregistObj) {
		if (!map_objectIndexPtr.count(arg_vlp_unregistObj)) {
			return;
		}

		auto itr = vec_vlp_Objects.begin();
		std::int32_t index = *map_objectIndexPtr.at(arg_vlp_unregistObj);
		itr += index;
		vec_vlp_Objects.erase(itr);
		map_objectIndexPtr.erase(arg_vlp_unregistObj);
		delete vec_objectIndex.at(index);

		for (auto indexItr = vec_objectIndex.erase(vec_objectIndex.begin() + index), end = vec_objectIndex.end(); indexItr != end; indexItr++) {
			*(*indexItr) -= 1;
		}
	}
	template<void (T::* Method)() >
	void CallFunction() {

		auto currentVec = vec_vlp_Objects;
		for (auto itr = currentVec.begin(); itr != currentVec.end(); itr++) {
			//
			
			((*itr).get()->*Method)();

		}
	}

private:
	std::vector< Value_ptr<T>>vec_vlp_Objects;
	std::map<Value_ptr<T>, std::int32_t*> map_objectIndexPtr;
	std::vector<std::int32_t*> vec_objectIndex;
};

}