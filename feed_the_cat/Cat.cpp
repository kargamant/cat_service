#include "Cat.h"
#include <iostream>

const std::set<std::string> Cat::accepted_food = {"martini", "beer", "tequila", "jin", "whiskey", "cigarette"};
std::regex Cat::msg_re{"@[a-zA-Z0-9]+ - ([a-zA-Z0-9]+)~"};


std::string Cat::process_message(char* buff)
{
    std::string msg{buff};
    std::smatch match;
    std::regex_match(msg, match, msg_re);

    if (accepted_food.find(match.str(1)) != accepted_food.end())
        return "Eaten by the Cat";
    else
        return "Ignored by the Cat";
}
