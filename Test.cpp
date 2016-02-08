#include "EnumReflection.h"

#include <iostream>
using std::cout;
using std::endl;



Z_ENUM_NS( CardSuit,
    Spades,
    Hearts,
    Diamonds,
    Clubs
)

class SomeClass
{
public:
    static const int Constant = 100;
    Z_ENUM( TasteFlags,
        None      = 0,
        Salted    = 1 << 0,
        Sour      = 1 << 1,
        Sweet     = 1 << 2,
        SourSweet = (Sour | Sweet),
        Other     = Constant,
        Last
    )
};

namespace SomeNamespace
{
    Z_ENUM_NS( Ports,
        HTTP  = 80,
        HTTPS = 443,
        SSH   = 22
    )

}

namespace AnotherNamespace
{
    static int GetPort(const std::string& name)
    {
        const auto& port = EnumReflector::For<::SomeNamespace::Ports>().Find(name);
        return port.IsValid() ? port.Value() : 0;
    }
}

int main()
{
    auto& reflector = EnumReflector::For<SomeClass::TasteFlags>();

    cout << "Enum " << reflector.EnumName() << endl;
    for (auto& val : reflector)
    {
        cout << "Value " << val.Name() << " = " << val.Value() << endl;
    }
    cout << endl;
    ///////////////////////

    using AnotherNamespace::GetPort;

    cout << "HTTPS port: " << GetPort("HTTPS") << endl;

    constexpr bool b = std::is_enum<SomeNamespace::Ports>::value;

    return 0;
}
