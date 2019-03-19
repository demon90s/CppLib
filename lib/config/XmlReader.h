#pragma once

#include <memory>
#include <vector>
#include "rapidxml/rapidxml.hpp"
#include "rapidxml/rapidxml_utils.hpp"

// ��װRapidXml���򻯶�ȡ���õĲ���
class XmlReader
{
	typedef rapidxml::xml_document<> XmlDoc;
	typedef rapidxml::file<> XmlFile;
public:
	XmlReader(const char *file_name);

	/*
	 * ����xml��·����ȡֵ��"config/name"
	 * <config>
	 *     <name>Liudiwen</name>
	 * </config>
	 */
	const char *GetValue(const char *xml_path) const;

	/*
	 * ����xml��·������ȡ���һ���ڵ��ֵܵ�ֵ: "config/reading/data"
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
