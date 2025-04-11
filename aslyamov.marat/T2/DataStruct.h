#ifndef DATASTRUCT_H_
#define DATASTRUCT_H_
#include <iostream>

namespace nspace {
    struct DataStruct {
        unsigned long long key1;
        std::string key2;
        std::string key3;
    };

    struct DelimiterIO {
        char exp;
    };

    struct UllLitIO {
        unsigned long long& num;
    };

    struct UllBinIO {
        std::string& num;
    };

    struct StringIO {
        std::string& ref;
    };

    class iofmtguard
    {
    public:
        iofmtguard(std::basic_ios<char>& s);
        ~iofmtguard();
    private:
        std::basic_ios<char>& s_;
        std::streamsize width_;
        char fill_;
        std::streamsize precision_;
        std::basic_ios<char>::fmtflags fmt_;
    };

    std::istream& operator>>(std::istream& in, DelimiterIO&& dest);
    std::istream& operator>>(std::istream& in, StringIO&& dest);
    std::istream& operator>>(std::istream& in, UllLitIO&& lit);
    std::istream& operator>>(std::istream& in, UllBinIO&& bin);
    std::istream& operator>>(std::istream& in, DataStruct& dest);
    std::ostream& operator<<(std::ostream& out, const DataStruct& dest);

    bool compare(const DataStruct& a, const DataStruct& b);
}
#endif
