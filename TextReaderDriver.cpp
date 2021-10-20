/*
    SAX2Driver.cpp

    Driver program for XML parsing examples.
*/

#include <iostream>
#include <string>
#include <string.h>

#include <libxml/xmlreader.h>

#include <unordered_map>
#include <vector>
#include <algorithm>
#include <locale>

// element counting
// global for use by callback
std::unordered_map<std::string, int> elementMap;

int main(int argc, const char* argv[]) {

    // number of arguments is 0..1
    if (argc > 2) {
        std::cerr << "Usage: " << argv[0] << " <XML_filename>" << '\n'
                  << "       " << argv[0] << '\n';
        return 1;
    }

    // input is filename, or stdin as "-"
    std::string xmlFilename = argc == 2 ? argv[1] : "-";

    // create a reader from the filename
    xmlTextReaderPtr reader = xmlNewTextReaderFilename(xmlFilename.c_str());
    if (reader == 0)
        return 1;

    // read and process the nodes
    while (true) {

        // read the next node
        if (xmlTextReaderRead(reader) != 1)
            break;

        // process an element node
        if (xmlTextReaderNodeType(reader) == XML_READER_TYPE_ELEMENT) {

            // increment element count
            ++elementMap[(const char*)(xmlTextReaderConstName(reader))];
        }
    }
    xmlFreeTextReader(reader);

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
