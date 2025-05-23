#include <string>
#include <set>
#include <regex>
#include <unordered_map>

enum class CatState
{
    Eaten,
    Ignore,
    Amused
};

class Cat
{
    private:
        static const std::set<std::string> accepted_food;
        static std::regex msg_re;
        static std::regex seg_re;

    public:
        Cat() {}

        static std::pair<std::string, CatState> process_message(char* buff);
};