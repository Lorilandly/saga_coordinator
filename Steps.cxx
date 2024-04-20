#include "Steps.h"

// void from_json(const json &j, Action &a)
// {
//     j.at("address").get_to(a.address);
//     j.at("method").get_to(a.method);
//     j.at("data").get_to(a.data);
// }
//
// void from_json(const json &j, Step &s)
// {
//     j.at("name").get_to(s.name);
//     j.at("execute").get_to(s.execute);
//     j.at("compensate").get_to(s.compensate);
// }
//
// void from_json(const json &j, Steps &s)
// {
//     for (const json &elem : j)
//     {
//         s.push_back(elem.get<Step>());
//     }
// }
