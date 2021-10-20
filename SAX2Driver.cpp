/*
    SAX2Driver.cpp

    Driver program for XML parsing examples.
*/

#include <iostream>
#include <string>
#include <string.h>

#include <libxml/parser.h>
#include <libxml/parserInternals.h>

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

    // create an input buffer from the filename
    xmlParserInputBuffer* input_buffer(xmlParserInputBufferCreateFilename(xmlFilename.c_str(), XML_CHAR_ENCODING_UTF8));
    if (input_buffer == 0)
        return 1;

    // create a parser context
    xmlParserCtxtPtr ctxt = xmlNewParserCtxt();
    if (ctxt == 0)
        return 1;

    // parser context options for large files
    xmlCtxtUseOptions(ctxt, XML_PARSE_COMPACT | XML_PARSE_HUGE | XML_PARSE_NODICT);

    // create the parser input from the context and the input buffer
    xmlParserInputPtr input = xmlNewIOInputStream(ctxt, input_buffer, XML_CHAR_ENCODING_UTF8);
    if (input == 0) {
        xmlFreeParserCtxt(ctxt);
        return 1;
    }

    // push the new parser input on the input stack
    inputPush(ctxt, input);

    // setup the SAX2 handlers
    xmlSAXHandler sax;
    memset(&sax, 0, sizeof(sax));
    sax.initialized    = XML_SAX2_MAGIC;
    sax.startDocument = 0;
    sax.endDocument = 0;
    sax.startElementNs = [](void* /* ctx */,
                            const xmlChar* localname,
                            const xmlChar* /* prefix */,
                            const xmlChar* /* URI */,
                            int /* nb_namespaces */,
                            const xmlChar** /* namespaces */,
                            int /* nb_attributes */,
                            int /* nb_defaulted */,
                            const xmlChar** /* attributes */) {

        // increment element count
        ++elementMap[(const char*)(localname)];
    };
    sax.endElementNs = [](void* /* ctx */,
                          const xmlChar* /* localname */,
                          const xmlChar* /* prefix */,
                          const xmlChar* /* URI */)-> void {
    };
    sax.characters = 0;
    sax.cdataBlock = 0;
    sax.comment = 0;
    sax.ignorableWhitespace = 0;

    // tie the SAX2 handler into the context
    ctxt->sax = &sax;

    // parse the entire document using the callbacks from the SAX2 handler
    xmlParseDocument(ctxt);

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
