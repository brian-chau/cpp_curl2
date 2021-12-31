#include <fstream>
#include <iostream>
#include <map>
#include <memory>
#include <regex>
#include <utility>

#include "http_gets.h"

std::map<std::string, std::string> parse_html(std::string html, std::regex regex_pattern, std::map<std::string, std::string> chapter_map) {
    int chapter(0);
    while(std::regex_search(html, m, regex_pattern)) {
        chapter_map["0" + std::to_string(chapter++)] = m[1].str();
        html = m.suffix();
    }
}

int main( int argc, char** argv ) {
    std::string base_url("https://pages.cs.wisc.edu/~remzi/OSTEP/");
    std::unique_ptr<http_gets> curl = std::unique_ptr<http_gets>(new http_gets(base_url,80));

    std::ifstream infile("test.txt");
    std::string line;
    std::regex chapterless_regex("yellow><a href=(\\S+.pdf)");
    std::regex chapter_regex("<small>(\\d+)</small> (<i>)?<a href=(\\S+.pdf)");
    std::smatch m;

    std::string html = curl->download_html();
    std::map<std::string, std::string> chapter_map;
    parse_html(html, chapterless_regex, chapter_map);
    parse_html(html, chapter_regex,     chapter_map);

    for (auto const& [chapter, filename] : chapter_map) {
        std::cout << filename << std::endl;
        curl->download_file(chapter, filename);
    }
    return 0;
}
