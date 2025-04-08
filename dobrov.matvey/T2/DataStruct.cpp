#include <iostream>
#include <string>
#include "DataStruct.h"

namespace nspace {
    std::istream& operator>>(std::istream& in, DelimeterIO&& dest) {
        std::istream::sentry sentry(in);
        if (!sentry) return in;

        char c;
        in >> c;
        if (in && (c != dest.exp)) {
            in.setstate(std::ios::failbit);
        }
        return in;
    }

    std::istream& operator>>(std::istream& in, StringIO&& dest) {
        std::istream::sentry sentry(in);
        if (!sentry) return in;

        return std::getline(in >> DelimeterIO{ '"' }, dest.ref, '"');
    }

    std::istream& operator>>(std::istream& in, LabelIO&& dest) {
        std::istream::sentry sentry(in);
        if (!sentry) return in;

        std::string data;
        if ((in >> data) && (data != dest.exp)) {
            in.setstate(std::ios::failbit);
        }
        return in;
    }

    std::istream& operator>>(std::istream& in, UllLitIO&& dest) {
        std::istream::sentry sentry(in);
        if (!sentry) return in;

        std::string str;
        char c;

        while (in.get(c)) {
            if (c == ':' || isspace(c)) {
                in.unget();
                break;
            }
            str.push_back(c);
        }

        size_t pos = str.find("ull");
        if (pos == std::string::npos) {
            pos = str.find("ULL");
            if (pos == std::string::npos) {
                in.setstate(std::ios::failbit);
                return in;
            }
        }

        try {
            dest.ref = std::stoull(str.substr(0, pos));
        }
        catch (...) {
            in.setstate(std::ios::failbit);
        }

        return in;
    }

    std::istream& operator>>(std::istream& in, UllOctIO&& dest) {
        std::istream::sentry sentry(in);
        if (!sentry) return in;

        std::string str;
        char c;

        while (in.get(c)) {
            if (c == ':' || isspace(c)) {
                in.unget();
                break;
            }
            str.push_back(c);
        }

        try {
            dest.ref = std::stoull(str, nullptr, 8);
        }
        catch (...) {
            in.setstate(std::ios::failbit);
        }

        return in;
    }

    std::istream& operator>>(std::istream& in, DataStruct& dest) {
        std::istream::sentry sentry(in);
        if (!sentry) return in;

        DataStruct final;

        in >> DelimeterIO{ '(' } >> DelimeterIO{ ':' };

        std::string str;
        while (in >> str) {
            if (str == "key1") {
                in >> UllLitIO{ final.key1 } >> DelimeterIO{ ':' };
            }
            else if (str == "key2") {
                in >> UllOctIO{ final.key2 } >> DelimeterIO{ ':' };
            }
            else if (str == "key3") {
                in >> StringIO{ final.key3 } >> DelimeterIO{ ':' };
            }
            else if (str == ")") {
                break;
            }
            else {
                in.setstate(std::ios::failbit);
                break;
            }
        }

        if (in) {
            dest = final;
        }

        return in;
    }

    std::ostream& operator<<(std::ostream& out, const DataStruct& dest) {
        std::ostream::sentry sentry(out);

        if (!sentry) {
            return out;
        }

        iofmtguard fmtguard(out);
        out << "(:key1 " << dest.key1 << "ull:key2 0" << std::oct << dest.key2 <<
            std::dec << ":key3 \"" << dest.key3 << "\":)";
        return out;
    }

    iofmtguard::iofmtguard(std::basic_ios<char>& s) :
        s_(s),
        width_(s.width()),
        fill_(s.fill()),
        precision_(s.precision()),
        fmt_(s.flags())
    {}

    iofmtguard::~iofmtguard() {
        s_.width(width_);
        s_.fill(fill_);
        s_.precision(precision_);
        s_.flags(fmt_);
    }

    bool compareDataStruct(const DataStruct& a, const DataStruct& b) {
        if (a.key1 != b.key1) {
            return a.key1 < b.key1;
        }
        if (a.key2 != b.key2) {
            return a.key2 < b.key2;
        }
        return a.key3.length() < b.key3.length();
    }
}