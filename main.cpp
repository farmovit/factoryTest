#include <iostream>
#include <Factory.h>

struct Interface {
    virtual ~Interface() = default;
    virtual void foo() const = 0;
};

class InterfaceFactory: public Factory<Interface> {};

template<class T>
class BaseInterface : public Interface {
protected:
    inline static bool sIsRegistred = InterfaceFactory::registerT<T>();
    BaseInterface() = default;
private:
    friend T;
};

struct Impl: BaseInterface<Impl> {
public:
    Impl() = default;
    void foo() const override {
        (void)sIsRegistred;
        std::cout << "Hello from Impl\n";
    }
};

struct ImplImpl: Impl {
public:
    ImplImpl() = default;
    void foo() const override {
        (void)sIsRegistred;
        std::cout << "Hello from ImplImpl\n";
    }
};

int main()
{
    auto impl = InterfaceFactory::create("Impl");
//    impl->foo();
    auto impl2 = InterfaceFactory::create("ImplImpl");
//    auto impl2 = Interface::make("Impl2");
//    impl2->foo();
    return 0;
}
