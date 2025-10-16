#include "DataStruct.h"
#include <iomanip>
#include <string>

namespace nspace
{
    std::istream& operator>>(std::istream& in, DelimiterIO&& dest)
    {
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

    std::istream& operator>>(std::istream& in, DoubleIO&& dest)
    {
        std::istream::sentry sentry(in);
        if (!sentry)
        {
            return in;
        }
        return in >> dest.ref >> DelimiterIO{ 'd' };
    }

    std::istream& operator>>(std::istream& in, RationalIO&& dest)
    {
        std::istream::sentry sentry(in);
        if (!sentry)
        {
            return in;
        }

        long long n = 0;
        unsigned long long d = 0;

        in >> DelimiterIO{ '(' } >> DelimiterIO{ ':' } >> LabelIO{ "N" };
        in >> n;

        in >> DelimiterIO{ ':' } >> LabelIO{ "D" };
        in >> d;

        in >> DelimiterIO{ ':' } >> DelimiterIO{ ')' };

        if (in && d != 0)
        {
            dest.ref = { n, d };

        }
        else
        {
            in.setstate(std::ios::failbit);
        }
        return in;
    }

    std::istream& operator>>(std::istream& in, StringIO&& dest)
    {
        std::istream::sentry sentry(in);
        if (!sentry)
        {
            return in;
        }
        in >> DelimiterIO{ '"' };

        return std::getline(in, dest.ref, '"');
    }

    std::istream& operator>>(std::istream& in, LabelIO&& dest)
    {
        std::istream::sentry sentry(in);

        if (!sentry)
        {
            return in;
        }

        std::string data = "";

        if ((in >> data) && (data != dest.exp))
        {
            in.setstate(std::ios::failbit);
        }

        return in;
    }

    std::istream& operator>>(std::istream& in, DataStruct& dest)
    {
        std::istream::sentry sentry(in);
        if (!sentry)
        {
            return in;
        }

        DataStruct temp;
        bool hasKey1 = false, hasKey2 = false, hasKey3 = false;

        in >> DelimiterIO{ '(' } >> DelimiterIO{ ':' };

        std::string field;
        while (in >> field && field != ")")
        {
            if (field == "key1") {
                in >> DoubleIO{ temp.key1 } >> DelimiterIO{ ':' };
                hasKey1 = true;
            }
            else if (field == "key2")
            {
                in >> RationalIO{ temp.key2 } >> DelimiterIO{ ':' };
                hasKey2 = true;
            }
            else if (field == "key3")
            {
                in >> StringIO{ temp.key3 } >> DelimiterIO{ ':' };
                hasKey3 = true;
            }
            else
            {
                in.setstate(std::ios::failbit);
                break;
            }
        }

        if (in && hasKey1 && hasKey2 && hasKey3) {
            dest = temp;
        }
        else {
            in.setstate(std::ios::failbit);
        }

        return in;
    }

    std::ostream& operator<<(std::ostream& out, const DataStruct& src)
    {
        std::ostream::sentry sentry(out);
        if (!sentry)
        {
            return out;
        }
        iofmtguard fmtguard(out);
        out << "(:key1 " << std::fixed << std::setprecision(1) << src.key1 << "d:key2 (:N "
            << src.key2.first << ":D " << src.key2.second << ":):key3 \""
            << src.key3 << "\":)";
        return out;
    }

    iofmtguard::iofmtguard(std::basic_ios<char>& s) :
        s_(s),
        width_(s.width()),
        fill_(s.fill()),
        precision_(s.precision()),
        fmt_(s.flags())
    {}

    iofmtguard::~iofmtguard()
    {
        s_.width(width_);
        s_.fill(fill_);
        s_.precision(precision_);
        s_.flags(fmt_);
    }

    bool compareDataStruct(const DataStruct& a, const DataStruct& b)
    {
        if (a.key1 != b.key1)
        {
            return a.key1 < b.key1;
        }
        double aVal = static_cast<double>(a.key2.first) / a.key2.second;
        double bVal = static_cast<double>(b.key2.first) / b.key2.second;
        if (aVal != bVal)
        {
            return aVal < bVal;
        }
        return a.key3.length() < b.key3.length();
    }
}
