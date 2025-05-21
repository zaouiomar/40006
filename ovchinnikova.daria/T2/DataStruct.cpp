#include <iostream>
#include <string>
#include <iomanip>
#include <sstream>
#include "DataStruct.h"

namespace nspace {
    std::istream& operator>>(std::istream& in, DelimiterIO&& dest) {
        std::istream::sentry sentry(in);
        if (!sentry) {
            return in;
        }
        char c = '0';
        in >> c;
        if (in && (c != dest.exp)) {
            in.setstate(std::ios::failbit);
        }
        return in;
    }

    std::istream& operator>>(std::istream& in, DblLitIO&& dest) {
        std::istream::sentry sentry(in);
        if (!sentry) {
            return in;
        }

        if (!(in >> dest.ref)) {
            return in;
        }

        char c;
        in >> c;

        if (!(c == 'd' || c == 'D')) {
            in.setstate(std::ios::failbit);
            return in;
        }
        return in;
    }

    std::istream& operator>>(std::istream& in, UllLitIO&& dest) {
        std::istream::sentry sentry(in);
        if (!sentry) {
            return in;
        }

        unsigned long long value;
        in >> value;
        if (!in) {
            return in;
        }

        char s1, s2, s3;
        in.get(s1).get(s2).get(s3);

        if (!((s1 == 'u' && s2 == 'l' && s3 == 'l') || (s1 == 'U' && s2 == 'L' && s3 == 'L'))) {
            in.setstate(std::ios::failbit);
            return in;
        }

        if (in) {
            dest.ref = value;
        }
        return in;
    }

    std::istream& operator>>(std::istream& in, StringIO&& dest) {
        std::istream::sentry sentry(in);
        if (!sentry) {
            return in;
        }
        in >> DelimiterIO{ '"' };
        return std::getline(in, dest.ref, '"');
    }

    std::istream& operator>>(std::istream& in, DataStruct& dest) {
        std::istream::sentry sentry(in);
        if (!sentry) {
            return in;
        }
        DataStruct input;
        in >> DelimiterIO{ '(' } >> DelimiterIO{ ':' };
        std::string item;
        while (in >> item) {
            if (item == "key1") {
                in >> DblLitIO{ input.key1 } >> DelimiterIO{ ':' };
            }
            else if (item == "key2") {
                in >> UllLitIO{ input.key2 } >> DelimiterIO{ ':' };
            }
            else if (item == "key3") {
                in >> StringIO{ input.key3 } >> DelimiterIO{ ':' };
            }
            else if (item == ")") {
                break;
            }
            else {
                in.setstate(std::ios::failbit);
                break;
            }
        }
        if (in) {
            dest = input;
        }
        return in;
    }

    std::ostream& operator<<(std::ostream& out, const DataStruct& src) {
        std::ostream::sentry sentry(out);
        if (!sentry) {
            return out;
        }
        iofmtguard fmtguard(out);
        out << "(:key1 " << std::fixed << std::setprecision(1) << src.key1 << 'd';
        out << ":key2 " << src.key2 << "ull";
        out << ":key3 \"" << src.key3 << "\":)";
        return out;
    }

    iofmtguard::iofmtguard(std::basic_ios< char >& s) :
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

    bool compareData(const DataStruct& src1, const DataStruct& src2) {
        if (src1.key1 != src2.key1) {
            return src1.key1 < src2.key1;
        }
        if (src1.key2 != src2.key2) {
            return src1.key2 < src2.key2;
        }
        return src1.key3.length() < src2.key3.length();
    }
}

