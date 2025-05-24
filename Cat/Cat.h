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
    Looking,
    Bite,
    Tolerate,
    Sleep,
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
        static std::regex user_name_re;

    public:
        Cat() {}

        static CatResponse process_message(const std::string& msg);
        static CatResponse pet(const std::string& user, double chance);
};