#include <fstream>
#include <iostream>
#include <memory>
#include <regex>
#include <utility>

#include "http_gets.h"

int main( int argc, char** argv ) {
    std::string base_url("https://pages.cs.wisc.edu/~remzi/OSTEP/");
    std::unique_ptr<http_gets> curl = std::unique_ptr<http_gets>(new http_gets(base_url,80));

    std::ifstream infile("test.txt");
    std::string line;
    std::regex r("^(\\d+) (\\S+) (.*)$");
    std::smatch m;
    while(std::getline(infile, line)) {
        while(std::regex_search(line, m, r)) {
            chapter = atoi(m[1].str().c_str());
            std::string filename = m[2].str();
            std::cout << filename << std::endl;
            curl->download_file(chapter, filename);
            line = m.suffix();
        }
    }
    return 0;
}
