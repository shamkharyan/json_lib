#include "json_lib.hpp"
#include <iostream>
#include <cassert>

void test_valid()
{
    std::vector<std::string> valid_cases = {
        R"({"key": "value"})",
        R"([1, 2, 3, 4, 5])",
        R"({"user": {"id": 1, "name": "John"}})",
        R"([{"id":1}, {"id":2}, {"id":3}])",
        R"({"a": null, "b": true, "c": false})",
        R"({})",
        R"([])",
        R"({"int": 123, "float": -123.456, "exp": 1.2e10})",
        R"({"a":[{"b":[{"c":null}]}]})",
        R"({"quote": "He said \"hello\"."})"
    };

    for (const auto& json_str : valid_cases)
    {
        try
        {
            json::from_string(json_str); // should NOT throw
        }
        catch (const std::exception& e)
        {
            std::cerr << "Valid JSON test failed: " << json_str << "\n";
            std::cerr << "Error: " << e.what() << "\n";
            assert(false);
        }
    }

    std::cout << "All valid JSON tests passed!\n";
}

void test_invalid()
{
    std::vector<std::string> invalid_cases = {
        R"({"key" "value"})",          // Missing colon
        R"({"a":1,})",                  // Trailing comma in object
        R"([1,2,3,])",                  // Trailing comma in array
        R"({"a":1)",                    // Unclosed object
        R"([1,2,3)",                    // Unclosed array
        R"({"a":1]})",                  // Wrong bracket closing
        R"({"a": "value})",              // Unclosed string
        R"({"bool": True})",             // Wrong literal (True vs true)
        R"({,"a":1})",                   // Comma before first item
    };

    for (const auto& json_str : invalid_cases)
    {
        try
        {
            json::from_string(json_str); // should throw
            std::cerr << "Invalid JSON test passed but should have failed: " << json_str << "\n";
            assert(false);
        }
        catch (const json::json_parse_error&)
        {
            // Expected error
        }
        catch (const std::exception& e) {
            std::cerr << "Unexpected exception type: " << e.what() << "\n";
            assert(false);
        }
    }

    std::cout << "All invalid JSON tests passed!\n";
}


int main()
{
    test_valid();
    test_invalid();
    std::cout << "All tests completed successfully!\n";
    return 0;
}