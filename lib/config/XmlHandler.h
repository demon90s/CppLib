#pragma once

#include <memory>
#include <vector>
#include <string>
#include "rapidxml/rapidxml.hpp"
#include "rapidxml/rapidxml_print.hpp"
#include "rapidxml/rapidxml_utils.hpp"
#include "common/NoCopy.h"
#include "common/string_functions.h"

// 封装RapidXml，简化配置的操作
// 通过一种路径表示xml节点, 比如 "config/name" 可以表示:
// <config>
// 		<name>Liudiwen</name>
// </config>

// 缺点:
// - 灵活性不足, 适用于一次性配置读取/设置操作

class XmlHandler
{
public:
	XmlHandler(const char* file_name = nullptr);
	~XmlHandler();

	bool Parse(const char *file_name = nullptr);

	const char* ErrorDesc() const {
		return lasterror_.c_str();
	}

	// 根据xml的路径获取值
	const char *GetValueStr(const char *xml_path) const;
	
	template<typename T>
	bool GetValue(const char *xml_path, T &v) const;

	// 根据xml的路径设置值
	bool SetValueStr(const char *xml_path, const char *value);

	template<typename T>
	bool SetValue(const char *xml_path, const T &v);

	// 根据xml的路径同步值, 如果路径不存在, 那么使用 init_value 初始化
	bool SyncValueStr(const char *xml_path, std::string &value, const char *init_value);

	template<typename T>
	bool SyncValue(const char *xml_path, T &value, const T &init_v);

	// 保存文件
	bool Save(const char *save_file = nullptr);

private:
	void MakeLastError(const std::string &error_info);

	NoCopy m_nocopy;

	typedef rapidxml::xml_document<> XmlDoc;
	typedef rapidxml::file<> XmlFile;

	XmlDoc doc_;
	XmlFile *file_;
	std::string filename_;
	bool parsed_;
	std::string lasterror_;
};

template<typename T>
bool XmlHandler::GetValue(const char *xml_path, T &v) const
{
	const char *value = this->GetValueStr(xml_path);
	if (!value)
		return false;

	bool ret = StringToBaseType(value, v);
	if (!ret) {
		const_cast<XmlHandler*>(this)->MakeLastError(StringFormat("string to %s false", xml_path));
	}
	return ret;
}

template<typename T>
bool XmlHandler::SetValue(const char *xml_path, const T &v)
{
	std::string v_str;
	if (!BaseTypeToString(v, v_str)) {
		this->MakeLastError(StringFormat("%s to string false", xml_path));
		return false;
	}
	return this->SetValueStr(xml_path, v_str.c_str());
}

template<typename T>
bool XmlHandler::SyncValue(const char *xml_path, T &value, const T &init_v)
{
	bool ret = this->GetValue(xml_path, value);
	if (ret) {
		return true;
	}

	return this->SetValue(xml_path, init_v);
}
