#include <iostream>
#include <type_traits>
#include <string>

template<typename T>
struct Functor_Result {
    T value;
};

template<typename T>
void operator>>(Functor_Result<T> fr, T& out) {
    out = fr.value;
}

template<typename F, typename T>
struct Functor {
    F func;
    T value;

    using result_t = Functor_Result<std::invoke_result_t<F, T>>;

    result_t operator()() {
        return {func(value)};
    }
};

template<typename F, typename Arg>
auto operator<<(F f, Arg&& arg) -> std::enable_if_t<!std::is_same_v<std::remove_reference_t<F>, decltype(std::cout)>, Functor<F, std::decay_t<Arg>>> {
    return {f, std::forward<Arg>(arg)};
}

int print_and_int(std::string str) {
    std::cout << str;
    return str.size();
}

int main() {
    std::string message = "This is a message";
    int printed_chars;
    (print_and_int << message)() >> printed_chars;
    std::cout << printed_chars;
}
