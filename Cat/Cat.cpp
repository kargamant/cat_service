#include "Cat.h"
#include <iostream>
#include <random>

const std::set<std::string> Cat::accepted_food = {"martini", "beer", "tequila", "jin", "whiskey", "cigarette"};
std::regex Cat::msg_re{"@([a-zA-Z0-9]+) - ([a-zA-Z0-9]+)~"};
std::regex Cat::seg_re{".+~([0-9]|[1-9][0-9]+)"};
std::regex Cat::end_seg_re{".+~"};
std::regex Cat::user_name_re{"@([a-zA-Z0-9]+)~"};


CatResponse::CatResponse(const std::string& message, const std::string& client_name, CatState state) : message(message), client_name(client_name), state(state)
{
}

CatResponse Cat::process_message(const std::string& msg)
{
    std::smatch match;

    if(std::regex_match(msg, match, user_name_re))
    {
        return {"Cat is looking at you", match.str(1), CatState::Looking};
    }

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

CatResponse Cat::pet(const std::string& user, double chance)
{
    int attempt = rand() % 100;
    if (attempt < chance*100)
        return {"Bitten by the Cat", user, CatState::Bite};
    else if (attempt > 90)
        return {"Cat is bored(and drunk). Fell Asleep.", user, CatState::Sleep};
    return {"Tolerated by the Cat", user, CatState::Tolerate};
}

std::vector<CatResponse> Cat::process_stream(std::string stream)
{
    std::vector<CatResponse> result;
    
    for(std::smatch users; std::regex_search(stream, users, user_name_re);)
    {
        result.emplace_back("Cat is looking at you", users.str(1), CatState::Looking);
        stream = users.suffix();
    }

    if(result.empty())
    {
        result.emplace_back("Cat got error :(", "", CatState::Error);
    }

    return result;
}

