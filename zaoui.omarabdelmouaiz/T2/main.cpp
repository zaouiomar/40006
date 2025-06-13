#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <sstream>
#include <string>
#include <cmath>
#include <cctype>

struct DataStruct {
    double key1;
    long long key2;
    std::string key3;
};

std::istream& operator>>(std::istream& in, DataStruct& data) {
    DataStruct temp;
    bool isValid = false;
    std::string line;

    if (std::getline(in, line)) {
        std::istringstream iss(line);
        char ch;

        if (iss >> ch && ch == '(') {
            std::string key;
            while (iss >> ch && ch == ':') {
                iss >> key;
                if (key == "key1") {

                    if (iss >> temp.key1) {

                        char next = iss.peek();
                        if (next == 'e' || next == 'E') {
                            isValid = true;
                        }
                    }
                }
                else if (key == "key2") {

                    std::string value;
                    iss >> value;
                    try {
                        size_t pos = 0;
                        temp.key2 = std::stoll(value, &pos);
                        if (pos == value.size()) {
                            isValid = true;
                        }
                    } catch (...) {

                    }
                }
                else if (key == "key3") {
                    if (iss >> ch && ch == '"') {
                        std::string str;
                        while (iss.get(ch) && ch != '"') {
                            str += ch;
                        }
                        temp.key3 = str;
                        isValid = true;
                    }
                }

                while (iss >> ch && ch != ':') {}
                iss.putback(ch);
            }
        }
    }

    if (isValid) {
        data = temp;
    } else {
        in.setstate(std::ios::failbit);
    }

    return in;
}


std::ostream& operator<<(std::ostream& out, const DataStruct& data) {

    std::ostringstream sciStream;
    sciStream << std::scientific << data.key1;
    std::string sciStr = sciStream.str();
    

    if (sciStr.find("e") != std::string::npos) {
        size_t ePos = sciStr.find("e");
        double mantissa = std::stod(sciStr.substr(0, ePos));
        if (mantissa >= 10.0 || mantissa < 1.0) {
            int exponent = std::stoi(sciStr.substr(ePos + 1));
            while (mantissa >= 10.0) {
                mantissa /= 10.0;
                exponent++;
            }
            while (mantissa < 1.0) {
                mantissa *= 10.0;
                exponent--;
            }
            sciStr = std::to_string(mantissa) + "e" + std::to_string(exponent);
        }
    }

    out << "(:key1 " << sciStr << ":key2 " << data.key2
        << ":key3 \"" << data.key3 << "\":)";
    return out;
}


bool compareDataStruct(const DataStruct& a, const DataStruct& b) {
    if (a.key1 != b.key1) return a.key1 < b.key1;
    if (a.key2 != b.key2) return a.key2 < b.key2;
    return a.key3.length() < b.key3.length();
}

int main() {
    std::vector<DataStruct> data;

    std::copy(std::istream_iterator<DataStruct>(std::cin),
              std::istream_iterator<DataStruct>(),
              std::back_inserter(data));

    std::sort(data.begin(), data.end(), compareDataStruct);

    std::copy(data.begin(),
              data.end(),
              std::ostream_iterator<DataStruct>(std::cout, "\n"));

    return 0;
}
