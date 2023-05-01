#include "stdafx.h"
#include "ParserXML.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include "rapidXML/rapidxml.hpp"
#include "rapidXML/rapidxml_print.hpp"


void ParserXML::TestRapidXml() {

	rapidxml::xml_document<> doc;
	std::ifstream file("Resources/XMLs/exemplu_xml.xml");
	std::stringstream buffer;
	buffer << file.rdbuf();
	file.close();
	std::string content(buffer.str());

	doc.parse<0>(&content[0]);

	rapidxml::xml_node<>* pRoot = doc.first_node();
	for (rapidxml::xml_node<>* pNode = pRoot->first_node("obiect"); pNode; pNode = pNode->next_sibling())
	{
		//Atributele
		for (rapidxml::xml_attribute<>* pAttr = pNode->first_attribute(); pAttr; pAttr = pAttr->next_attribute())
		{
			//AIci luam atributele. exemlpu: <obiect id="100" nume="zar">  atributele: nume 'id', value '100' si nume 'nume', value 'zar'
			std::cout << "Attr: " << pAttr->name() << " " << pAttr->value() << std::endl;
		}

		//Copii
		for (rapidxml::xml_node<>* resourceNode = pNode->first_node(); resourceNode; resourceNode = resourceNode->next_sibling())
		{
			//Atributele
			for (rapidxml::xml_attribute<>* pAttr = resourceNode->first_attribute(); pAttr; pAttr = pAttr->next_attribute())
			{
				//AIci luam atributele. exemlpu: <obiect id="100" nume="zar">  atributele: nume 'id', value '100' si nume 'nume', value 'zar'
				std::cout << "Attr: " <<  pAttr->name() << " " << pAttr->value() << std::endl;
			}
			std::cout << "Node: " << resourceNode->name() << " " << resourceNode->value() << std::endl;
		}

	}

	rapidxml::xml_node<>* obiect = doc.allocate_node(rapidxml::node_element, "obiect");
	obiect->append_attribute(doc.allocate_attribute("id", "33"));
	obiect->append_attribute(doc.allocate_attribute("nume", "add_from_code"));
	pRoot->append_node(obiect);

	rapidxml::xml_node<>* child = doc.allocate_node(rapidxml::node_element, "model","10");
	child->append_attribute(doc.allocate_attribute("info", "ceva"));
	obiect->append_node(child);

	child = doc.allocate_node(rapidxml::node_element, "texture", "123");
	child->append_attribute(doc.allocate_attribute("folder", "undeva"));
	obiect->append_node(child);
	

	std::string xml_as_string;
	rapidxml::print(std::back_inserter(xml_as_string), doc);

	// Save to file
	std::ofstream file_stored("Resources/XMLs/Modified.xml");
	file_stored << doc;
	file_stored.close();
	doc.clear();

}