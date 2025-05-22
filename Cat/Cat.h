#include <string>
#include <set>
#include <regex>
#include <unordered_map>

class Cat
{
    private:
        static const std::set<std::string> accepted_food;
        static std::regex msg_re;

        std::unordered_map<std::string, std::string> ip_dgram_map;
    public:
        Cat() {}

        static std::string process_message(char* buff);
};