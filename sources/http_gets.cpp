#define _XOPEN_SOURCE 700
#include <arpa/inet.h>
#include <assert.h>
#include <curl/curl.h>
#include <iomanip>
#include <iostream>
#include <memory>
#include <netdb.h> /* getprotobyname */
#include <netinet/in.h>
#include <regex>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sys/socket.h>
#include <unistd.h>

#include "http_gets.h"

http_gets::http_gets(std::string hostname, unsigned int port) {
    m_hostname = hostname;
    m_port     = port;
}

http_gets::~http_gets() {

}

static size_t get_pdf(void *contents, size_t size, size_t nmemb, FILE *stream) {
    size_t written = fwrite(contents, size, nmemb, stream);
    return written;
}

int http_gets::download_file(std::string chapter, std::string filename) {
    CURL *curl;
    FILE *fp;
    curl = curl_easy_init();
    std::string new_filename = chapter + "_" + filename;
    if(curl) {
        fp = fopen(new_filename.c_str(), "wb");
        std::string remote_file = m_hostname + filename;
        curl_easy_setopt(curl, CURLOPT_URL,           remote_file.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, get_pdf);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA,     fp);
        curl_easy_perform(curl);
        curl_easy_cleanup(curl);
        fclose(fp);
    }
    return EXIT_SUCCESS;
}

static size_t get_html(void *contents, size_t size, size_t nmemb, void *userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

std::string http_gets::download_html() {
    CURL *curl;
    curl = curl_easy_init();
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL,           m_hostname.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, get_html);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA,     &m_html);
        curl_easy_perform(curl);
        curl_easy_cleanup(curl);
    }
    return m_html;
}
