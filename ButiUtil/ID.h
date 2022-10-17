#pragma once
#ifndef _BUTI_ID_H
#define _BUTI_ID_H

#include<memory>
#include<map>
#include<vector>
#include<string>
namespace ButiEngine {
static const std::string noneIDStr = "none";
template <typename T>
class ID {
public:
	ID() {}
	ID(Value_ptr<std::string> arg_id) { id =* (arg_id); }
	ID(std::string arg_str) { id = arg_str; }

	const std::string& GetID()const {
		return IsEmpty()? noneIDStr: id;
	}
	bool operator == (const ID& other) const {
		return IsEmpty() ? false: id==other.id;
	}
	bool operator != (const ID& other)const {
		return other.id != id;
	}
	bool IsEmpty() const {
		return !id.size();
	}
	template<class Archive>
	void serialize(Archive& archive)
	{
		ARCHIVE_BUTI(id);
	}
private:
	std::string id ;
};
template <typename T> const std::string& TypeToDraggingObjectType() { static std::string output = "Other"; return output; }
template <typename T>
class IDContainer {
public:
	void Clear() {
		m_map_vlp_resource.clear();
	}
	ID<T> GetTag(const std::string& arg_key)const {
		return ID<T>(make_value<std::string>(arg_key));
	}
	Value_ptr<T> GetValue(const ID<T>& arg_tag) {
		if (arg_tag.IsEmpty()) {
			std::cout << arg_tag.GetID() << ": This tag is unregisted." << std::endl;
			return FailedValue();
		}
		if (m_map_vlp_resource.count(arg_tag.GetID()))
		{
			return m_map_vlp_resource.at(arg_tag.GetID());
		}
		else
		{
			return FailedValue();
		}
	}
	Value_ptr<T> FailedValue() {

		if (m_map_vlp_resource.size()) {
			return m_map_vlp_resource.begin()->second;
		}
		else {
			std::cout << "ResourceContainer is Empty." << std::endl;
			return nullptr;
		}

	}
	Value_ptr<T> GetValue(const std::string& arg_key) {

		return m_map_vlp_resource.at(arg_key);
	}

	bool Exist(const ID<T>& arg_tag)const {
		if (arg_tag.IsEmpty()) {
			return false;
		}
		return m_map_vlp_resource.count(arg_tag.GetID());
	}
	ID<T> AddValue(Value_ptr<T> arg_value, const std::string& arg_key) {
		ID<T> output(make_value<std::string>(arg_key));
		if (m_map_vlp_resource.count(arg_key)) {
			return output;
		}
		m_map_vlp_resource.emplace(arg_key, arg_value);
		return output;
	}

	bool ContainValue(const std::string& arg_key) {
		if (m_map_vlp_resource.count(arg_key)) {
			return true;
		}
		return false;
	}
	void Remove(const std::string& arg_key) {
		if (!m_map_vlp_resource.count(arg_key)) {
			return;
		}
		m_map_vlp_resource.erase(arg_key);
	}
	void Remove(ID<T> arg_id) {
		Remove(arg_id.GetID());
	}

	void ShowGUI(const std::string& arg_exclusionWord = "") {

		GUI::BeginChild("##Tag"+TypeToDraggingObjectType<T>(), Vector2(0, 0), true);
		GUI::Input("searchStr##"+std::string( typeid(T).name()), m_searchStr);
		if (m_searchStr.size() <= 0) {

			for (auto itr : m_map_vlp_resource) {
				GUI::Button(Util::ToUTF8(StringHelper::Remove(itr.first, arg_exclusionWord)));

				if (GUI::BeginDragDropSource())
				{
					GUI::SetDragDropPayload(TypeToDraggingObjectType<T>().c_str(), itr.first.c_str(), (itr.first.size() + 1) * sizeof(char));
					GUI::EndDragDropSource();
				}
			}
		}
		else {
			for (auto itr : m_map_vlp_resource) {
				if (!StringHelper::Contains(itr.first, m_searchStr)) {
					continue;
				}
				GUI::Button(Util::ToUTF8(itr.first));
				if (GUI::BeginDragDropSource())
				{
					GUI::SetDragDropPayload(TypeToDraggingObjectType<T>().c_str(), itr.first.c_str(), (itr.first.size() + 1) * sizeof(char));
					GUI::EndDragDropSource();
				}
			}
		}

		GUI::EndChild();
	}

	List< Value_ptr<T>> GetResources() const {
		List< Value_ptr<T>> output;
		output.Reserve(m_map_vlp_resource.size());
		for (auto itr : m_map_vlp_resource) {
			output.Add(itr.second);
		}

		return output;
	}
	List< std::string> GetResourceNames()const {
		List< std::string> output;
		output.Reserve(m_map_vlp_resource.size());
		for (auto itr : m_map_vlp_resource) {
			output.Add(itr.first);
		}

		return output;
	}
	List< ID<T>> GetResourceTags()const {
		List< ID<T>> output;
		output.Reserve(m_map_vlp_resource.size());
		for (auto itr : m_map_vlp_resource) {
			output.Add(ID<T>(itr.first));
		}
		return output;
	}

	std::vector< std::string> GetResourceNames_vec()const {
		std::vector< std::string> output;
		output.reserve(m_map_vlp_resource.size());
		for (auto itr : m_map_vlp_resource) {
			output.push_back(itr.first);
		}

		return output;
	}
	std::vector< ID<T>> GetResourceTags_vec()const {
		std::vector< ID<T>> output;
		output.reserve(m_map_vlp_resource.size());
		for (auto itr : m_map_vlp_resource) {
			output.push_back(ID<T>(itr->first));
		}

		return output;
	}
	std::int32_t GetSize()const {
		return m_map_vlp_resource.size();
	}
private:
	std::map< std::string, Value_ptr<T>> m_map_vlp_resource;
	std::string m_searchStr;

};
template <typename T>
class IDManager {
public:
	~IDManager() {
		Clear();
	}
	void Clear() {
		map_values.clear();
	}
	ID<T> GetTag(const std::string& arg_key) {
		if (!map_values.count(arg_key)) {
			return AddTag(arg_key);
		}
		return map_values.at(arg_key);
	}
	ID<T> AddTag(const std::string& arg_key) {
		if (map_values.count(arg_key)) {
			return map_values.at(arg_key);
		}
		ID<T> output(arg_key);

		map_values.emplace(arg_key, output);
		return output;
	}
	std::string GetIDName(ID<T> arg_id) {
		if (arg_id.IsEmpty()) {
			return "none";
		}
		return arg_id.GetID();
	}
	void Remove(const std::string& arg_key) {
		if (!map_values.count(arg_key)) {
			return;
		}
		map_values.erase(arg_key);
	}
	void Remove(ID<T> arg_id) {
		if (!map_values.count(arg_id.GetID())) {
			return;
		}
		map_values.erase(arg_id.GetID());
	}
	template<class Archive>
	void serialize(Archive& archive)
	{
		ARCHIVE_BUTI(map_values);
	}
private:
	std::unordered_map<std::string, ID<T>> map_values;

};
}

namespace std {

template<typename T>
struct hash<ButiEngine::ID<T>> {
public:
	size_t operator()(const ButiEngine::ID<T>& data)const {

		std::size_t seed = 0;
		ButiEngine::hash_combine(seed, data.GetID());


		return seed;

	}
};
}

#endif // !_BUTI_ID_H