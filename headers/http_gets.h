#include <string>

class http_gets{
public:
                   http_gets(std::string hostname, unsigned int port=80);
                  ~http_gets();
    int            download_file(std::string chapter, std::string filename);
    std::string    download_html();

private:
    std::string    m_hostname;
    unsigned short m_port;

    std::string    m_html;
};
