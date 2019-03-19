#include "XmlReader.h"
#include "common/string_functions.h"
#include <cstring>
#include <iostream>

XmlReader::XmlReader(const char *file_name) : m_parsed(false)
{
	try
	{
		m_file.reset(new XmlFile(file_name));
		m_doc.parse<0>(m_file->data()); // 0 means default parse flags
	}
	catch (rapidxml::parse_error e)
	{
		std::cerr << "parse_error: " << e.what() << std::endl;
		return;
	}
	catch (std::runtime_error e)
	{
		std::cerr << "runtime_error: " << e.what() << std::endl;
		return;
	}

	m_parsed = true;
}

const char * XmlReader::GetValue(const char *xml_path) const
{
	if (!m_parsed) return nullptr;

	auto path_node = m_doc.first_node();
	if (!path_node) return nullptr;

	std::vector<std::string> node_list = SplitString(xml_path, "/");

	if (node_list.size() <= 1) // 必须包括根节点
		return nullptr;

	if (node_list[0] != path_node->name())
		return nullptr;

	size_t i = 1;
	for (; i < node_list.size(); ++i)
	{
		auto node_tmp = path_node->first_node(node_list[i].c_str());
		if (!node_tmp)
			return nullptr;

		path_node = node_tmp;
	}

	return path_node->value();
}

bool XmlReader::GetValue(const char *xml_path, std::vector<const char*> &value_list) const
{
	if (!m_parsed) return false;

	auto path_node = m_doc.first_node();
	if (!path_node) return false;

	std::vector<std::string> node_list = SplitString(xml_path, "/");

	if (node_list.size() <= 1) // 必须包括根节点
		return false;

	if (node_list[0] != path_node->name())
		return false;

	size_t i = 1;
	for (; i < node_list.size(); ++i)
	{
		auto node_tmp = path_node->first_node(node_list[i].c_str());
		if (!node_tmp)
			return false;

		path_node = node_tmp;
	}

	// 必须是最后一个element节点
	bool is_last_element = path_node->first_node() && path_node->first_node()->type() == rapidxml::node_data;
	if (!is_last_element)
		return false;

	while (path_node)
	{
		value_list.push_back(path_node->value());
		path_node = path_node->next_sibling();
	}

	return true;
}