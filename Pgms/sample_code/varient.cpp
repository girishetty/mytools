#include <variant>
#include <iostream>
#include <type_traits>
#include <iomanip>
#include <vector>
 
 
template<class T> struct always_false : std::false_type {};
 
enum class Types {
    A,
    B,
    C
};

template <Types T>
struct Message {};

using MsgA = Message<Types::A>;
using MsgB = Message<Types::B>;
using MsgC = Message<Types::C>;

using msg_t = std::variant<MsgA, MsgB, MsgC>;
 
template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
template<class... Ts> overloaded(Ts...) -> overloaded<Ts...>;
 
int main() {

    std::vector<msg_t> msgs = {MsgA{}, MsgC{}, MsgC{}, MsgB{}};
    for (auto& msg: msgs) {
        std::visit(overloaded {
            [](MsgA msg) { std::cout << 'A' << std::endl; },
            [](MsgB msg) { std::cout << 'B' << std::endl; },
            [](MsgC msg) { std::cout << 'C' << std::endl; },
        }, msg);
    }
}
