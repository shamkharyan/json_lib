#include "value.hpp"
#include <iostream>

int main()
{
    json::value json = json::object(
        {
            {"string", "hi"},
            {"number", 2.5},
            {"boolean", true},
            {"null_object", nullptr},
            {"object", json::object(
                {
                    {"name", "Kyle"},
                    {"age", 24}
                }
            )},
            {"array", json::array({1,2,3,4})}
        }
    );

    json["esh"] = json::object({
        {"good esh", "42"},
        {"ezopos", false}
    });
    json["esh"] = nullptr;

    std::cout << json::to_string(json) << '\n';
}