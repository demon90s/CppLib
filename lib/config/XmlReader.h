#pragma once

#include <memory>
#include <vector>
#include "rapidxml/rapidxml.hpp"
#include "rapidxml/rapidxml_utils.hpp"

// 封装RapidXml，简化读取配置的操作
class XmlReader
{
	typedef rapidxml::xml_document<> XmlDoc;
	typedef rapidxml::file<> XmlFile;
public:
	XmlReader(const char *file_name);

	/*
	 * 根据xml的路径获取值："config/name"
	 * <config>
	 *     <name>Liudiwen</name>
	 * </config>
	 */
	const char *GetValue(const char *xml_path) const;

	/*
	 * 根据xml的路径，获取最后一个节点兄弟的值: "config/reading/data"
	 * <config>
	 *     <reading>
	 *         <data>CppPrimer</data>
	 *         <data>Linux Programming</data>
	 *     </reading>
	 * </config>
	 */
	bool GetValue(const char *xml_path, std::vector<const char*> &value_list) const;

private:
	XmlDoc m_doc;
	std::unique_ptr<XmlFile> m_file;
	bool m_parsed = false;
};
