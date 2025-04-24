#include <iostream>
#include <string>
#include "DataStruct.h"

namespace nspace {
    std::istream& operator>>(std::istream& in, DelimeterIO&& dest) {
        std::istream::sentry sentry(in);
        if (!sentry) {
            return in;
        }

        char c;
        if ((in >> c) && (c != dest.exp)) {
            in.setstate(std::ios::failbit);
        }
        return in;
    }

    std::istream& operator>>(std::istream& in, StringIO&& dest) {
        std::istream::sentry sentry(in);
        if (!sentry) {
            return in;
        }

        return std::getline(in >> DelimeterIO{ '"' }, dest.ref, '"');
    }

    std::istream& operator>>(std::istream& in, SuffixIO&&) {
        std::istream::sentry sentry(in);
        if (!sentry) {
            return in;
        }

        char a, b, c;
        in.get(a).get(b).get(c);

        if ((a == 'u' && b == 'l' && c == 'l') ||
            (a == 'U' && b == 'L' && c == 'L')) {
        }
        else {
            in.setstate(std::ios::failbit);
        }

        return in;
    }

    std::istream& operator>>(std::istream& in, UllLitIO&& dest) {
        std::istream::sentry sentry(in);
        if (!sentry) return in;

        unsigned long long value;
        in >> value;

        if (!in) {
            return in;
        }

        in >> SuffixIO{};

        if (in) {
            dest.ref = value;
        }

        return in;
    }

    std::istream& operator>>(std::istream& in, UllOctIO&& dest) {
        std::istream::sentry sentry(in);
        if (!sentry) {
            return in;
        }

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
            size_t pos;
            dest.ref = std::stoull(str, &pos, 8);

            if (pos != str.length()) {
                in.setstate(std::ios::failbit);
            }
        }
        catch (...) {
            in.setstate(std::ios::failbit);
        }

        return in;
    }

    std::istream& operator>>(std::istream& in, DataStruct& dest) {
        std::istream::sentry sentry(in);
        if (!sentry) {
            return in;
        }

        DataStruct final;

        in >> DelimeterIO{ '(' } >> DelimeterIO{ ':' };

        std::string str;

        bool key1 = false;
        bool key2 = false;
        bool key3 = false;

        while (in >> str) {
            if (str == "key1") {
                in >> UllLitIO{ final.key1 } >> DelimeterIO{ ':' };
                key1 = true;
            }
            else if (str == "key2") {
                in >> DelimeterIO{ '0' } >> UllOctIO{ final.key2 } >> DelimeterIO{ ':' };
                key2 = true;
            }
            else if (str == "key3") {
                in >> StringIO{ final.key3 } >> DelimeterIO{ ':' };
                key3 = true;
            }
            else if (str == ")") {
                if (!(key1 && key2 && key3)) {
                    in.setstate(std::ios::failbit);
                }
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
