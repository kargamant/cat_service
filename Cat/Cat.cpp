#include "Cat.h"
#include <iostream>

const std::set<std::string> Cat::accepted_food = {"martini", "beer", "tequila", "jin", "whiskey", "cigarette"};
std::regex Cat::msg_re{"@[a-zA-Z0-9]+ - ([a-zA-Z0-9]+)~"};
std::regex Cat::seg_re{".+~([0-9]|[1-9][0-9]+)"};


std::pair<std::string, CatState> Cat::process_message(char* buff)
{
    std::string msg{buff};
    std::smatch match;

    if(std::regex_match(msg, match, seg_re))
    {
        return {"The Cat is amused by #" + match.str(1), CatState::Amused};
    }

    std::regex_match(msg, match, msg_re);

    if (accepted_food.find(match.str(1)) != accepted_food.end())
        return {"Eaten by the Cat", CatState::Eaten};
    else
        return {"Ignored by the Cat", CatState::Ignore};
}
