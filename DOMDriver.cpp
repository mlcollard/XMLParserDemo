/*
    DOMDriver.cpp

    Driver program for XML parsing examples.
*/

#include <iostream>
#include <string>

#include <libxml/xpath.h>
#include <libxml/xpathInternals.h>

#include <unordered_map>
#include <vector>
#include <algorithm>
#include <locale>
#include <stack>

// element counting
// global for use by callback
std::unordered_map<std::string, int> elementMap;

// static void storeElementNames(xmlNode* a_node) {

//     std::stack<xmlNode*> nodes;
//     nodes.push(a_node);
//     while (!nodes.empty()) {
//         auto cur_node = nodes.top();
//         nodes.pop();
//         for (; cur_node; cur_node = cur_node->next) {
//             if (cur_node->type == XML_ELEMENT_NODE) {

//                // increment element count
//                 ++elementMap[(const char*)(cur_node->name)];
//             }

//             nodes.push(cur_node->children);
//         }
//     }
// }

int main(int argc, const char* argv[]) {

    // number of arguments is 0..1
    if (argc > 2) {
        std::cerr << "Usage: " << argv[0] << " <XML_filename>" << '\n'
                  << "       " << argv[0] << '\n';
        return 1;
    }

    // input is filename, or stdin as "-"
    std::string xmlFilename = argc == 2 ? argv[1] : "-";

    // open the XML file with no encoding change and no options
    xmlDoc* xmlDoc(xmlReadFile(xmlFilename.c_str(), nullptr, 0));
    if (!xmlDoc) {
        return 1;
    }

    // update element count as we visit all nodes
    auto root = xmlDocGetRootElement(xmlDoc);
    std::stack<xmlNode*> nodes;
    nodes.push(root);
    while (!nodes.empty()) {
        auto cur_node = nodes.top();
        nodes.pop();
        for (; cur_node; cur_node = cur_node->next) {
            if (cur_node->type == XML_ELEMENT_NODE) {

               // increment element count
                ++elementMap[(const char*)(cur_node->name)];
            }

            nodes.push(cur_node->children);
        }
    }

    xmlFreeDoc(xmlDoc);

    // create a sorted by count container of the elements
    std::vector<std::pair<std::string, int>> elements;
    std::copy(elementMap.begin(), elementMap.end(), std::back_inserter(elements));
    std::sort(elements.begin(), elements.end(), [](const auto &l, const auto &r) {

            if (l.second != r.second)
                return l.second > r.second;

            return l.first < r.first;
    });
 
    // output table of element frequency
    std::cout << "|Element|Count|\n";
    std::cout << "|---|---:|\n";
    std::cout.imbue(std::locale(""));
    for (const auto& element : elements) {
        std::cout << "| " << element.first << " | " << element.second << " |\n";
    }

    return 0;
}
