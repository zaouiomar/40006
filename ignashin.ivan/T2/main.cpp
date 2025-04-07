#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <complex>
#include <iterator>

namespace nspace
{
    struct DataStruct
    {
        double key1;
        unsigned long long key2;
        std::string key3;
    };

    struct DelimiterIO
    {
        char exp;
    };

    struct DoubleIO
    {
        double& ref;
    };

    struct UnsignedIO
    {
        unsigned long long& ref;
    };

    struct StringIO
    {
        std::string& ref;
    };

    struct LabelIO
    {
        std::string exp;
    };

    class iofmtguard
    {
    public:
        iofmtguard(std::basic_ios< char >& s);
        ~iofmtguard();
    private:
        std::basic_ios< char >& s_;
        std::streamsize width_;
        char fill_;
        std::streamsize precision_;
        std::basic_ios< char >::fmtflags fmt_;
    };

    std::istream& operator>>(std::istream& in, DelimiterIO&& dest);
    std::istream& operator>>(std::istream& in, DoubleIO&& dest);
    std::istream& operator>>(std::istream& in, StringIO&& dest);
    std::istream& operator>>(std::istream& in, LabelIO&& dest);
    std::istream& operator>>(std::istream& in, DataStruct& dest);
    std::ostream& operator<<(std::ostream& out, const DataStruct& dest);
}

namespace nspace {
    bool compareDataStructs(const DataStruct& a, const DataStruct& b) {
        if (a.key1 != b.key1) {
            return a.key1 < b.key1;
        }

        if (a.key2 != b.key2) {
            return a.key2 < b.key2;
        }

        return a.key3.length() < b.key3.length();
    }
}

int main()
{
    using nspace::DataStruct;

    std::vector<DataStruct> data;
    std::string line;


    while (std::getline(std::cin, line)) {
        std::istringstream iss(line);
        std::copy(
            std::istream_iterator<DataStruct>(iss),
            std::istream_iterator<DataStruct>(),
            std::back_inserter(data)
        );


        if (iss.fail() && !iss.eof()) {
            iss.clear();
        }
    }

    std::sort(data.begin(), data.end(), nspace::compareDataStructs);

    std::copy(
        data.begin(),
        data.end(),
        std::ostream_iterator<DataStruct>(std::cout, "\n")
    );

    return 0;
}

namespace nspace {
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

    std::istream& operator>>(std::istream& in, UnsignedIO&& dest) {
        std::istream::sentry sentry(in);
        if (!sentry)
        {
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
        try
        {
            dest.ref = std::stoull(str, nullptr, 16);
        }
        catch (...)
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
        return std::getline(in >> DelimiterIO{ '"' }, dest.ref, '"');
    }

    std::istream& operator>>(std::istream& in, LabelIO&& dest)
    {
        std::istream::sentry sentry(in);
        if (!sentry)
        {
            return in;
        }
        std::string data = "";
        if ((in >> StringIO{ data }) && (data != dest.exp))
        {
            in.setstate(std::ios::failbit);
        }
        return in;
    }

    std::istream& operator>>(std::istream& in, DataStruct& dest)
    {
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

    std::ostream& operator<<(std::ostream& out, const DataStruct& data)
    {
        std::ostream::sentry sentry(out);
        if (!sentry)
        {
            return out;
        }
        iofmtguard fmtguard(out);
        out <<
            "(:key1 " << std::fixed << std::setprecision(2) << data.key1 << 'd' <<
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
    {
    }

    iofmtguard::~iofmtguard()
    {
        s_.width(width_);
        s_.fill(fill_);
        s_.precision(precision_);
        s_.flags(fmt_);
    }
}