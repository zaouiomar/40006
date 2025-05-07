#include <algorithm>
#include <sstream>
#include <string>
#include <cassert>
#include <iterator>
#include <vector>
#include <utility>
#include <iomanip>
#include "DataStruct.h"

namespace nspace {
    std::istream& operator>>(std::istream& in, DelimiterIO&& dest) {
        std::istream::sentry sentry(in);
        if (!sentry)
        {
            return in;
        }
        char c = '0';
        in >> c;
        if (in && (c != dest.exp))
        {
            in.setstate(std::ios::failbit);
        }
        return in;
    }

    std::istream& operator>>(std::istream& in, StringIO&& dest) {
        std::istream::sentry sentry(in);
        if (!sentry)
        {
            return in;
        }
        return std::getline(in >> DelimiterIO{ '"' }, dest.ref, '"');
    }

    std::istream& operator>>(std::istream& in, UllLitIO&& lit) {
        std::istream::sentry sentry(in);
        if (!sentry)
        {
            return in;
        }
        unsigned long long value = 0;
        in >> value;
        if (!in) {
            return in;
        }
        char c[4];
        c[3] = '\0';
        in.read(c, 3);
        if (strcmp(c, "ull") == 0 && in.peek() == ':') {
            lit.num = value;
        }
        else {
            in.setstate(std::ios::failbit);
        }
        return in;
    }

    std::istream& operator>>(std::istream& in, UllBinIO&& bin) {
        std::istream::sentry sentry(in);
        if (!sentry)
        {
            return in;
        }
        std::string str = "";
        char c = '0';
        while (in >> c) {
            if (c == ':') {
                in.unget();
                break;
            }
            str = str + c;
        }
        if (str.size() < 3) {
            in.setstate(std::ios::failbit);
        }
        if (in && str[0] == '0' && (str[1] == 'b' || str[1] == 'B') && (str[2] == '1' || str[2] == '0')) {
            for (size_t i = 2; i < str.size(); i++) {
                if (str[i] != '0' && str[i] != '1') {
                    in.setstate(std::ios::failbit);
                }
            }
            str = str.substr(2);
            bin.num = std::stoull(str, nullptr, 2);
        }
        else {
            in.setstate(std::ios::failbit);
        }
        return in;
    }
    std::istream& operator>>(std::istream& in, DataStruct& dest) {
        std::istream::sentry sentry(in);
        if (!sentry) {
            return in;
        }
        DataStruct input;

        in >> DelimiterIO{ '(' } >> DelimiterIO{ ':' };
        bool f1 = false;
        bool f2 = false;
        bool f3 = false;
        std::string inp;
        while (in.peek() != ')' && in) {
            in >> inp;
            if (inp == "key1") {
                in >> UllLitIO{ input.key1 };
                in >> DelimiterIO{ ':' };
                f1 = true;
            }
            else if (inp == "key2") {
                in >> UllBinIO{ input.key2 };
                in >> DelimiterIO{ ':' };
                f2 = true;
            }
            else if (inp == "key3") {
                in >> StringIO{ input.key3 };
                in >> DelimiterIO{ ':' };
                f3 = true;
            }
            else {
                in.setstate(std::ios::failbit);
            }
        }
        in >> DelimiterIO{ ')' };
        if (f1 && f2 && f3 && in)
            dest = input;
        else
            in.setstate(std::ios::failbit);
        return in;
    }
    std::ostream& operator<<(std::ostream& out, const DataStruct& dest) {
        std::ostream::sentry sentry(out);
        if (!sentry)
        {
            return out;
        }
        std::string k2 = "0";
        unsigned long long num = dest.key2;
        while (num) {
            k2 += (num % 2 ? "1" : "0");
            num = num / 2;
        }
        iofmtguard guard(out);
        out << "(:key1 " << dest.key1 << "ull:key2 0b" << k2 << ":key3 \"" << dest.key3 << "\":)";
        return out;
    }

    bool compare(const DataStruct& a, const DataStruct& b) {
        if (a.key1 != b.key1) {
            return a.key1 < b.key1;
        }
        if (a.key2 != b.key2) {
            return a.key2 < b.key2;
        }
        return a.key3 < b.key3;
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
}
