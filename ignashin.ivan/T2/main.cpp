#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <complex>
#include <iterator>


struct DataStruct {
    double key1;
    unsigned long long key2;
    std::string key3;
};


std::istream& operator>>(std::istream& in, DataStruct& data);
std::ostream& operator<<(std::ostream& out, const DataStruct& data);
bool compareDataStructs(const DataStruct& a, const DataStruct& b);


int main() {
    std::vector<DataStruct> dataVector;

    std::copy(
        std::istream_iterator<DataStruct>(std::cin),
        std::istream_iterator<DataStruct>(),
        std::back_inserter(dataVector)
    );

    std::sort(dataVector.begin(), dataVector.end(), compareDataStructs);

    std::copy(
        dataVector.begin(),
        dataVector.end(),
        std::ostream_iterator<DataStruct>(std::cout, "\n")
    );

    return 0;
}


std::istream& operator>>(std::istream& in, DataStruct& data) {
    DataStruct temp;
    bool key1_found = false, key2_found = false, key3_found = false;
    std::string line;


    char c;
    while (in.get(c) && isspace(c)) {}

    if (!std::getline(in, line, ')')) {
        in.setstate(std::ios::failbit);
        return in;
    }

    std::istringstream iss(line);
    std::string token;

    while (iss >> token) {
        if (token == ":key1") {
            char c;
            double number;
            if ((iss >> number >> c) && (c == 'd' || c == 'D')) {
                temp.key1 = number;
                key1_found = true;
            }
            else {
                in.setstate(std::ios::failbit);
                return in;
            }
        }
        else if (token == ":key2") {
            unsigned long long val;
            if (iss >> std::hex >> val) {
                temp.key2 = val;
                key2_found = true;
            }
        }
        else if (token == ":key3") {
            char quote;
            std::string str;
            if (iss >> quote && quote == '"') {
                std::getline(iss, str, '"');
                temp.key3 = str;
                key3_found = true;
            }
        }
    }

    if (key1_found && key2_found && key3_found) {
        data = temp;
        if (in.fail()) {
            in.clear();
        }
    }
    else {
        in.setstate(std::ios::failbit);
    }

    return in;
}


std::ostream& operator<<(std::ostream& out, const DataStruct& data) {
    out <<
        "(:key1 " << std::fixed << std::setprecision(2) << data.key1 << 'd' <<
        ":key2 0x" << std::hex << std::uppercase << data.key2 << std::dec <<
        ":key3 \"" << data.key3 << "\":)";
    return out;
}


bool compareDataStructs(const DataStruct& a, const DataStruct& b) {
    if (a.key1 != b.key1) {
        return a.key1 < b.key1;
    }

    if (a.key2 != b.key2) {
        return a.key2 < b.key2;
    }

    return a.key3.length() < b.key3.length();
}
