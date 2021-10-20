/*
    XMLDriver.cpp

    Driver program for XML parsing examples.
*/

#include <iostream>
#include <string>

#include <libxml/xpath.h>
#include <libxml/xpathInternals.h>

int main(int argc, const char* argv[]) {

    // number of arguments is 0..1
    if (argc > 2) {
        std::cerr << "Usage: " << argv[0] << " <XML_filename>" << '\n'
                  << "       " << argv[0] << '\n';
        return 1;
    }

    // input is filename, or stdin as "-"
    std::string xmlFilename = argc == 2 ? argv[1] : "-";

    // open the srcML file with no encoding change and no options
    xmlDoc* xmlDoc(xmlReadFile(xmlFilename.c_str(), nullptr, 0));
    if (!xmlDoc) {
        return -1;
    }

    xmlFreeDoc(xmlDoc);

    return 0;
}
