/*
    XPathDriver.cpp

    Driver program for XML parsing examples using XPath.
*/

#include <iostream>
#include <string>

#include <libxml/tree.h>
#include <libxml/parser.h>
#include <libxml/xpath.h>
#include <libxml/xpathInternals.h>

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

    // init libxml2
    xmlInitParser();
    LIBXML_TEST_VERSION

    // open the XML file with no encoding change and no options
    xmlDoc* xmlDoc(xmlReadFile(xmlFilename.c_str(), nullptr, 0));
    if (!xmlDoc) {
        return 1;
    }

    // create xpath evaluation context
    xmlXPathContext* xpathCtx(xmlXPathNewContext(xmlDoc));
    if (!xpathCtx) {
        xmlFreeDoc(xmlDoc);
        return 1;
    }

    // register srcML namespaces
    if (xmlXPathRegisterNs(xpathCtx, BAD_CAST "src", BAD_CAST "http://www.srcML.org/srcML/src")) {
        xmlFreeDoc(xmlDoc);
        xmlXPathFreeContext(xpathCtx);
        return 1;
    }
    if (xmlXPathRegisterNs(xpathCtx, BAD_CAST "cpp", BAD_CAST "http://www.srcML.org/srcML/cpp")) {
        xmlFreeDoc(xmlDoc);
        xmlXPathFreeContext(xpathCtx);
        return 1;
    }

    // evaluate xpath expression
    auto xpath = "//src:*";
    xmlXPathObject* xpathResults(xmlXPathEvalExpression(BAD_CAST xpath, xpathCtx));
    if (!xpathResults) {
        xmlFreeDoc(xmlDoc);
        xmlXPathFreeContext(xpathCtx);
        return 1;
    }

    // collect the count
    auto& nodes = xpathResults->nodesetval;
    int size = (nodes) ? nodes->nodeNr : 0;
    for (int i = 0; i < size; ++i) {
        if(nodes->nodeTab[i]->type == XML_ELEMENT_NODE) {
            ++elementMap[(const char*)(nodes->nodeTab[i]->name)];
        }
    }

    xmlFreeDoc(xmlDoc);
    xmlXPathFreeContext(xpathCtx);

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
