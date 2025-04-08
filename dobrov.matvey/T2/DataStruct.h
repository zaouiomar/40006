#ifndef DATASTRUCT_H
#define DATASTRUCT_H
#include <iostream>

namespace nspace {
    struct DataStruct {
        unsigned long long key1;
        unsigned long long key2;
        std::string key3;
    };

    struct DelimeterIO {
        char exp;
    };

    struct StringIO {
        std::string& ref;
    };

    struct LabelIO {
        std::string exp;
    };

    struct UllLitIO {
        unsigned long long& ref;
    };

    struct UllOctIO {
        unsigned long long& ref;
    };

    std::istream& operator>>(std::istream& in, DelimeterIO&& dest);
    std::istream& operator>>(std::istream& in, StringIO&& dest);
    std::istream& operator>>(std::istream& in, LabelIO&& dest);
    std::istream& operator>>(std::istream& in, UllLitIO&& dest);
    std::istream& operator>>(std::istream& in, UllOctIO&& dest);
    std::istream& operator>>(std::istream& in, DataStruct& dest);
    std::ostream& operator<<(std::ostream& out, const DataStruct& dest);

    bool compareDataStruct(const DataStruct& a, const DataStruct& b);

    class iofmtguard {
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
}

#endif