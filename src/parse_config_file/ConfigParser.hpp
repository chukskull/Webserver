#include <iostream>
#include <string>
#include <vector>
#include "everything.hpp"

class ConfigParser {
public:
    ConfigParser(const std::string& input, char bgin, char end) : input_(input), pos_(0), count_(0) {

        run.push_back(bgin);
        run.push_back(end);
        run.push_back('[');
        run.push_back(']');
    }
    // _server_config  
    std::vector<std::pair<std::string, std::string> > parse() {
        std::vector<std::pair<std::string, std::string> > configs;
        while (pos_ < input_.size()) {
            // puts("cuts my lip");
            _string key = parseKey();
            // std::cerr << key << std::endl;
            // std::cerr << "his mum is fat " << pos_ << std::endl;
            _string value = parseValue();
            // std::cerr << value << std::endl;
            // std::cerr << "his mum is fat 2" << std::endl;
            configs.push_back(std::make_pair(key, value));
        }
        return configs;
    }

private:
    char peek() {
        return input_[pos_];
    }

    void consume() {
        ++this->pos_;
    }

    bool isAlpha(char c) {
        return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
    }

    bool isDigit(char c) {
        return c >= '0' && c <= '9';
    }

    bool isWhitespace(char c) {
        return c == ' ' || c == '\t' || c == '\n' || c == '\r';
    }

    _string parseKey() {
        _string key;
        while (isWhitespace(peek())) {
            consume();
        }
        while (isAlpha(peek()) || isDigit(peek()) || peek() == '_') {
            key += peek();
            std::cerr << "sure " <<peek() << std::endl;
            consume();
        }
        while (isWhitespace(peek())) {
            consume();
        }
        if (peek() != run[0]) {
            throw std::runtime_error("expected start");
        }
        count_++;
        consume();
        return key;
    }

    _string parseValue() {
        _string value;
        while (isWhitespace(peek())) {
            consume();
        }
        while (peek() != run[1]) {
            value += peek();
            consume();
        }
        if (peek() != run[1]) {
            throw std::runtime_error("expected end");
        }
        consume();
        return value;
    }
    int count_;
    _string input_;
    int pos_;
    std::vector<char> run;
};

int main() {
    _string input =
        "server {"
   " location / ["
        "autoindex true;"
        "index index.html;"
        "root ./data;"
        "method GET;"
        "method POST"
        "method DELE"
    "]"
    "listen      8080;"
    "host        69.0.0.69;"
    "body_size   1024;"
    "name        localhost;"
    "location   /test ["
    " autoindex true;"
    " root ./data;"
    " index test.html;"
    " method  DELETE;"
    ""
    "location    /error ["
   " autoindex   true;"
   " root        ./data;"
   " index       404.html;"
   " method      GET;"
   " method      POST;"
   " method      DELETE;"
   ""
   "location    /redirect ["
   " redirect    https://www.youtube.com/watch?v=dQw4w9WgXcQ;"
   ""
"}";
        // "    server_name  example.com;\n"
        // "    location / {\n"
        // "        root   /usr/share/nginx/html;\n"
        // "        index  index.html index.htm;\n"
        // "    }\n"
        // "}";
        //
        for(int i = 0; i < input.size(); i++)
        {
            std::cerr << input[i] << std::endl;
        }
    ConfigParser parser(input, '{', '}');
    std::vector<std::pair<std::string, std::string> > configs = parser.parse();
    for (std::vector<std::pair<std::string, std::string> >::iterator it = configs.begin(); it != configs.end(); ++it) {
        std::cout << it->first << ": " << it->second << std::endl;
    }
    return 0;
}