#ifndef FACTORY_H
#define FACTORY_H

#include <memory>
#include <string>
#include <map>
#include <functional>
#ifdef __GNUG__
#include <cstdlib>
#include <cxxabi.h>
#endif

#include <iostream>

namespace {
std::string demangle(const char* name) noexcept {
#ifdef __GNUG__
    int status = -4;
    try {
        std::unique_ptr<char, void(*)(void*)> res {
            abi::__cxa_demangle(name, nullptr, nullptr, &status),
                    std::free
        };
        return (status == 0) ? res.get() : name;
    } catch (const std::exception &) {
        return name;
    }
#else
    return name;
#endif
}
} // namespace

template <class Base, class... Args>
class Factory
{
public:
    Factory(const Factory &other) = delete;
    Factory(Factory &&other) = delete;
    Factory &operator=(const Factory &other) = delete;
    Factory &operator=(Factory &&other) = delete;
    using BasePtr = std::unique_ptr<Base>;
public:
    template <class... Arguments>
    static BasePtr create(std::string_view s, Arguments &&...args) noexcept {
        static_assert(std::is_same_v<std::tuple<Arguments...>, std::tuple<Args...>>, "Cannot create object from provided arguments");
        auto createMethods = sCreateMethods();
        auto it = createMethods.find(std::string(s));
        return it != createMethods.end()
                ? createMethods.at(std::string(s))(std::forward<Arguments>(args)...)
                : nullptr;
    }
    template <class T>
    static bool registerT() noexcept {
        auto &createMethods = sCreateMethods();
        const auto name = demangle(typeid(T).name());
        if (auto it = createMethods.find(name); it != createMethods.end()) {
            return false;
        }
        const auto creator = [](Args... args) -> BasePtr {
            return std::make_unique<T>(std::forward<Args>(args)...);
        };
        createMethods.emplace(name, creator);
        std::cout << "REGISTRED! " << name << std::endl;
        return true;
    }
private:
    using FuncType = BasePtr (*)(Args...);
    Factory() = default;
private:
    static auto &sCreateMethods() {
        static std::map<std::string, FuncType> s;
        return s;
    }
};

//template<class FactoryT>
//class FactoryRegistrator {
//private:
//    template<class T>
//    inline static bool sIsRegistred = FactoryT::template registerT<T>();
//    FactoryRegistrator() = default;
//    friend FactoryT;
//};

#endif // FACTORY_H
