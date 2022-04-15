#pragma once
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
		archive(id);
	}
private:
	std::string id ;
};

template <typename T>
class IDContainer {
public:
	void Clear() {
		map_shp_resource.clear();
	}
	ID<T> GetTag(const std::string& arg_key)const {
		return ID<T>(make_value<std::string>(arg_key));
	}
	Value_ptr<T> GetValue(const ID<T>& arg_tag) {
		if (arg_tag.IsEmpty()) {
			std::cout << arg_tag.GetID() << ": This tag is unregisted." << std::endl;
			return FailedValue();
		}
		if (map_shp_resource.count(arg_tag.GetID()))
		{
			return map_shp_resource.at(arg_tag.GetID());
		}
		else
		{
			return FailedValue();
		}
	}
	Value_ptr<T> FailedValue() {

		if (map_shp_resource.size()) {
			return map_shp_resource.begin()->second;
		}
		else {
			std::cout << "ResourceContainer is Empty." << std::endl;
			return nullptr;
		}

	}
	Value_ptr<T> GetValue(const std::string& arg_key) {

		return map_shp_resource.at(arg_key);
	}

	bool Exist(const ID<T>& arg_tag)const {
		if (arg_tag.IsEmpty()) {
			return false;
		}
		return map_shp_resource.count(arg_tag.GetID());
	}
	ID<T> AddValue(Value_ptr<T> arg_value, const std::string& arg_key) {
		ID<T> output(make_value<std::string>(arg_key));
		if (map_shp_resource.count(arg_key)) {
			return output;
		}
		map_shp_resource.emplace(arg_key, arg_value);
		return output;
	}

	bool ContainValue(const std::string& arg_key) {
		if (map_shp_resource.count(arg_key)) {
			return true;
		}
		return false;
	}
	void Remove(const std::string& arg_key) {
		if (!map_shp_resource.count(arg_key)) {
			return;
		}
		map_shp_resource.erase(arg_key);
	}
	void Remove(ID<T> arg_id) {
		Remove(arg_id.GetID());
	}

	template<class Archive>
	void serialize(Archive& archive)
	{
	}

	ID<T> ShowGUI(GUI::GuiIO& arg_io, const std::string& arg_exclusionWord = "") {

		GUI::Input("searchStr##"+std::string( typeid(T).name()), searchStr);


		ID<T> out;
		if (searchStr.size() <= 0) {

			for (auto itr = map_shp_resource.begin(), enditr = map_shp_resource.end(); itr != enditr; itr++) {
				if (GUI::Button(Util::ToUTF8(StringHelper::Remove(itr->first, arg_exclusionWord)).c_str())) {
					out = ID<T>(itr->first);
				}

				if (GUI::IsItemActive()) {
					auto p1 = arg_io.MouseClickedPos[0];
					auto p2 = arg_io.MousePos;
					GUI::Line(Vector2(p1.x, p1.y), Vector2(p2.x, p2.y), GUI::GetColorU32(GUI::GuiCol_::GuiCol_Button), 4.0f); // Draw a line between the button and the mouse cursor

					out = ID<T>(itr->first);
				}
			}
		}
		else {
			for (auto itr = map_shp_resource.begin(), enditr = map_shp_resource.end(); itr != enditr; itr++) {
				if (!StringHelper::Contains(itr->first, searchStr)) {
					continue;
				}
				if (GUI::Button(Util::ToUTF8(itr->first).c_str())) {
					out = ID<T>(itr->first);
				}

				if (GUI::IsItemActive()) {
					auto p1 = arg_io.MouseClickedPos[0];
					auto p2 = arg_io.MousePos;
					GUI::Line(Vector2(p1.x, p1.y), Vector2(p2.x, p2.y), GUI::GetColorU32(GUI::GuiCol_::GuiCol_Button), 4.0f); // Draw a line between the button and the mouse cursor

					out = ID<T>(itr->first);
				}
			}
		}
		return out;
	}

	std::vector< Value_ptr<T>> GetResources() const {
		std::vector< Value_ptr<T>> output;
		output.reserve(map_shp_resource.size());
		for (auto itr = map_shp_resource.begin(), end = map_shp_resource.end(); itr != end; itr++) {
			output.push_back(itr->second);
		}

		return output;
	}
	std::vector< std::string> GetResourceNames()const {
		std::vector< std::string> output;
		output.reserve(map_shp_resource.size());

		for (auto itr = map_shp_resource.begin(), end = map_shp_resource.end(); itr != end; itr++) {
			output.push_back(itr->first);
		}

		return output;
	}
	std::vector< ID<T>> GetResourceTags()const {
		std::vector< ID<T>> output;
		output.reserve(map_shp_resource.size());
		for (auto itr = map_shp_resource.begin(), end = map_shp_resource.end(); itr != end; itr++) {
			output.push_back(ID<T>(itr->first));
		}

		return output;
	}
private:
	std::map< std::string, Value_ptr<T>> map_shp_resource;
	std::string searchStr;

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
		archive(map_values);
	}
private:
	std::unordered_map<std::string, ID<T>> map_values;

};
}