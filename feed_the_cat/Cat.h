#include <string>
#include <set>
#include <regex>

class Cat
{
    private:
        static const std::set<std::string> accepted_food;
        static std::regex msg_re;
    public:
        static std::string process_message(char* buff);
};