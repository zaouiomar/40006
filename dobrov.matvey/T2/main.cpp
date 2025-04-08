#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include "DataStruct.h"

int main() {
    using nspace::DataStruct;
    using nspace::compareDataStruct;

    std::vector<DataStruct> data;
    std::string str;

    while (std::getline(std::cin, str)) {
        std::istringstream iss(str);
        std::copy(
            std::istream_iterator<DataStruct>(iss),
            std::istream_iterator<DataStruct>(),
            std::back_inserter(data)
        );

        if (iss.fail() && !iss.eof()) {
            iss.clear();
        }
    }

    std::sort(data.begin(), data.end(), compareDataStruct);

    std::copy(
        data.begin(),
        data.end(),
        std::ostream_iterator<DataStruct>(std::cout, "\n")
    );
}
