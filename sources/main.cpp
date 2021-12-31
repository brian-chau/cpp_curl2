#include <fstream>
#include <iostream>
#include <map>
#include <memory>
#include <regex>
#include <utility>

#include "http_gets.h"

int main( int argc, char** argv ) {
    std::string base_url("https://pages.cs.wisc.edu/~remzi/OSTEP/");
    std::unique_ptr<http_gets> curl = std::unique_ptr<http_gets>(new http_gets(base_url,80));

    std::ifstream infile("test.txt");
    std::string line;
    std::regex chapterless_regex("yellow><a href=(\\S+.pdf)");
    std::regex chapter_regex("<small>(\\d+)</small> (<i>)?<a href=(\\S+.pdf)");
    std::smatch m;

    std::string html = curl->download_html();
    std::string html2(html);

    int chapter(0);
    std::map<std::string, std::string> chapter_map;
    while(std::regex_search(html, m, chapterless_regex)) {
        chapter_map["0" + std::to_string(chapter++)] = m[1].str();
        html = m.suffix();
    }

    while(std::regex_search(html2, m, chapter_regex)) {
        chapter_map[m[1].str()] = m[3].str();
        html2 = m.suffix();
    }

    for (auto const& [chapter, filename] : chapter_map) {
        std::cout << filename << std::endl;
        curl->download_file(chapter, filename);
    }
    return 0;
}