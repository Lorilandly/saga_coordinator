#include <nlohmann/json.hpp>

struct Action
{
    std::string host;
    std::string path;
    std::string method;
    std::string data;
};

struct Step
{
    std::string name;
    Action execute;
    Action compensate;
};

typedef std::vector<Step> Steps;
