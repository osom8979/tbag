/**
 * @file   process_test.cpp
 * @brief  Program entry-point.
 * @author zer0
 * @date   2016-05-17
 */

#include <iostream>
#include <cstring>
#include <string>

template <typename OutputStream>
void runOutput(OutputStream & ostream, char const * argument = nullptr)
{
    std::string output_string;

    if (argument == nullptr) {
        std::cin >> output_string;
    } else {
        output_string.assign(argument);
    }

    ostream << output_string;
}

int main(int argc, char ** argv)
{
    if (argc < 2) {
        std::cout << "Usage: " << argv[0] << " {out|err} {argument}\n";
        return 1;
    }

    if (strcmp(argv[1], "out") == 0) {
        if (argc >= 3) {
            runOutput(std::cout, argv[2]);
        } else {
            runOutput(std::cout);
        }
    } else if (strcmp(argv[1], "err") == 0) {
        if (argc >= 3) {
            runOutput(std::cerr, argv[2]);
        } else {
            runOutput(std::cerr);
        }
    } else {
        std::cout << "Unknown command.\n";
        return 1;
    }

    return 0;
}

