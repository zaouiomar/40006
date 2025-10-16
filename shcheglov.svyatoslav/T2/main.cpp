#include <iostream>
#include <algorithm>
#include <sstream>
#include <limits>
#include <string>
#include <cassert>
#include <iterator>
#include <vector>
#include <utility>
#include <iomanip>
#include "DataStruct.h"

int main() {
    using namespace nspace;
    std::vector<DataStruct> data;
    while (!std::cin.eof())
    {
        if (std::cin.fail())
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
        std::copy(
            std::istream_iterator<DataStruct>(std::cin),
            std::istream_iterator<DataStruct>(),
            std::back_inserter(data)
        );
    }

    std::sort(data.begin(), data.end(), compareDataStruct);

    std::copy(
        data.begin(),
        data.end(),
        std::ostream_iterator<DataStruct>(std::cout, "\n")
    );
    return 0;
}
