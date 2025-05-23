#include <string>
#include <set>
#include <regex>
#include <unordered_map>

enum class CatState
{
    Eaten,
    Ignore,
    Amused,
    EndOfSeg,
    Error
};

class Cat
{
    private:
        static const std::set<std::string> accepted_food;
        static std::regex msg_re;
        static std::regex seg_re;
        static std::regex end_seg_re;

    public:
        Cat() {}

        static std::pair<std::string, CatState> process_message(const std::string& msg);
};