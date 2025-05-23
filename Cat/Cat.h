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

struct CatResponse
{
    std::string message;
    std::string client_name;
    CatState state;
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

        static CatResponse process_message(const std::string& msg);
};