#include <iostream>
#include <vector>
#include <iterator>
#include <string>
#include <sstream>
#include <limits>
#include <algorithm>
#include "DataStruct.h"

int main() {
    using nspace::DataStruct;
    using nspace::compareDataStruct;

    std::vector<DataStruct> data;
    std::string str;

    while (std::cin.good())
    {
        std::copy(
            std::istream_iterator<DataStruct>(std::cin),
            std::istream_iterator<DataStruct>(),
            std::back_inserter(data)
        );
        if (std::cin.fail() && !std::cin.eof())
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
        }
    }

    std::sort(data.begin(), data.end(), compareDataStruct);

    std::copy(
        data.begin(),
        data.end(),
        std::ostream_iterator<DataStruct>(std::cout, "\n")
    );
}
