#include <iostream>
#include <string>
#include <iomanip>
#include "DataStruct.h"


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

std::istream& operator>>(std::istream& in, DoubleIO&& dest) {
    std::istream::sentry sentry(in);
    if (!sentry) {
        return in;
    }
    return in >> dest.ref >> DelimiterIO{ 'd' };
}

std::istream& operator>>(std::istream& in, UnsignedIO&& dest) {
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

    bool has_hex_prefix = (str.size() >= 2) &&
        (str[0] == '0') &&
        (tolower(str[1]) == 'x');

    try {
        if (!has_hex_prefix) {
            in.setstate(std::ios::failbit);
            return in;
        }
        dest.ref = std::stoull(str, nullptr, 16);
    }
    catch (...) {
        in.setstate(std::ios::failbit);
    }
    return in;
}

std::istream& operator>>(std::istream& in, StringIO&& dest) {
    std::istream::sentry sentry(in);
    if (!sentry) {
        return in;
    }
    return std::getline(in >> DelimiterIO{ '"' }, dest.ref, '"');
}

std::istream& operator>>(std::istream& in, LabelIO&& dest) {
    std::istream::sentry sentry(in);
    if (!sentry) {
        return in;
    }
    std::string data = "";
    if ((in >> StringIO{ data }) && (data != dest.exp)) {
        in.setstate(std::ios::failbit);
    }
    return in;
}

std::istream& operator>>(std::istream& in, DataStruct& dest) {
    DataStruct temp;
    in >> DelimiterIO{ '(' } >> DelimiterIO{ ':' };

    std::string label;
    while (in >> label) {
        if (label == "key1") {
            in >> DoubleIO{ temp.key1 } >> DelimiterIO{ ':' };
        }
        else if (label == "key2") {
            in >> UnsignedIO{ temp.key2 } >> DelimiterIO{ ':' };
        }
        else if (label == "key3") {
            in >> StringIO{ temp.key3 } >> DelimiterIO{ ':' };
        }
        else if (label == ")") {
            break;
        }
        else {
            in.setstate(std::ios::failbit);
            break;
        }
    }

    if (in) {
        dest = temp;
    }
    return in;
}

std::ostream& operator<<(std::ostream& out, const DataStruct& data) {
    std::ostream::sentry sentry(out);
    if (!sentry) {
        return out;
    }
    iofmtguard fmtguard(out);
    out <<
        "(:key1 " << std::fixed << std::setprecision(1) << data.key1 << 'd' <<
        ":key2 0x" << std::hex << std::uppercase << data.key2 << std::dec <<
        ":key3 \"" << data.key3 << "\":)";
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