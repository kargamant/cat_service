#include "Cat.h"
#include <iostream>

const std::set<std::string> Cat::accepted_food = {"martini", "beer", "tequila", "jin", "whiskey", "cigarette"};
std::regex Cat::msg_re{"@([a-zA-Z0-9]+) - ([a-zA-Z0-9]+)~"};
std::regex Cat::seg_re{".+~([0-9]|[1-9][0-9]+)"};
std::regex Cat::end_seg_re{".+~"};


CatResponse Cat::process_message(const std::string& msg)
{
    std::smatch match;

    bool is_matched = std::regex_match(msg, match, seg_re);
    if(std::regex_match(msg, match, seg_re))
    {
        return {"The Cat is amused by #" + match.str(1), "", CatState::Amused};
    }

    std::regex_match(msg, match, msg_re);

    if (accepted_food.find(match.str(2)) != accepted_food.end())
        return {"Eaten by the Cat", match.str(1), CatState::Eaten};

    else if(!match.str(2).empty() && accepted_food.find(match.str(2)) == accepted_food.end())
        return {"Ignored by the Cat", match.str(1), CatState::Ignore};
        
    else if(std::regex_match(msg, match, end_seg_re))
        return {"Last Segment", "", CatState::EndOfSeg};
    
    return {"Cat got error :(", "", CatState::Error};
}
