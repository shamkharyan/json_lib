#include "value.hpp"
#include <iostream>

int main()
{
    json::value json = json::object(
        {
            {"Hello! 😊", "hi"},
            {"true", nullptr},
            {"number", 2.5},
            {"boolean", true},
            {"null_object", nullptr},
            {"object", json::object(
                {
                    {"name", "Kyle"},
                    {"age", 24}
                }
            )},
            {"array", json::array({1,2.01,3,4})}
        }
    );

    json["new val"] = json::object({
        {"val", "42"},
        {"second val", false}
    });

    json = json::array({1,2,3,4});
    std::cout << json.to_string() << '\n';

    json.to_file("fafay.json");
}