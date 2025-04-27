#include "json_lib.hpp"
#include <iostream>

int main()
{
    std::cout << "** Test 1 **\n";
    json::value json = json::from_string("42");
    std::cout << json.to_string() << '\n';

    std::cout << "\n** Test 2 **\n";
    json = json::from_string(R"("hello world")");
    std::cout << json.to_string() << '\n';

    std::cout << "\n** Test 3 **\n";
    json = json::from_string(R"(null)");
    std::cout << json.to_string() << '\n';

    std::cout << "\n** Test 4 **\n";
    json = json::from_string(R"(42.346)");
    std::cout << json.to_string() << '\n';

    std::cout << "\n** Test 5 **\n";
    json = json::from_string(R"(42e-1)");
    std::cout << json.to_string() << '\n';

    std::cout << "\n** Test 6 **\n";
    json = json::from_string(R"({})");
    std::cout << json.to_string() << '\n';

    std::cout << "\n** Test 7 **\n";
    json = json::from_string(R"([])");
    std::cout << json.to_string() << '\n';

    std::cout << "\n** Test 8 **\n";
    json = json::from_string(R"(
        {
        "metadata": {
            "version": "1.0",
            "timestamp": "2023-05-15T12:00:00Z"
        },
        "data": {
            "users": [
            {
                "id": 101,
                "name": "Alice",
                "roles": ["admin", "editor"],
                "contact": {
                "email": "alice@example.com",
                "phone": null
                }
            },
            {
                "id": 102,
                "name": "Bob",
                "roles": ["viewer"],
                "contact": {
                "email": "bob@example.com",
                "phone": "+1234567890"
                }
            }
            ]
        }
        }
    )");
    std::cout << json.to_string() << '\n';

}