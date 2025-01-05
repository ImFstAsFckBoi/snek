#pragma once

#include <string>
#include <type_traits>
#include <iterator>
#include <sstream>

namespace snek {

/*
    Join strings
*/

template<typename T>
void _join(std::stringstream &ss, std::string const &, T arg) {
    ss << arg;
}

template<typename  T, typename... Ts>
void _join(std::stringstream &ss, std::string const &delim, T arg, Ts... args) {
    ss << arg << delim;
    _join(ss, delim, args...);
}


template<typename... Ts>
std::string join(std::string const &delim, Ts... args) {
    std::stringstream ss;
    _join(ss, delim, args...);
    return ss.str();
}

/*
    Iterable type trait
*/

template<typename T, typename = void>
struct is_iterable {
    static constexpr bool value = false;
};

template<typename T>
struct is_iterable<T, std::void_t<decltype(std::begin(std::declval<T>()), std::end(std::declval<T>()))>> {
    static constexpr bool value = true;
};

/*
    Function pointer type traits
*/

template<int N, typename T, typename... Ts>
struct helper_nth_of_pack {
   using type = typename helper_nth_of_pack<N-1, Ts...>::type;
};

template<int N, typename T>
struct helper_nth_of_pack<N, T> {
   static_assert(false, "N > n_args");
};

template<int N>
struct helper_nth_of_pack<N, void> {
   static_assert(false, "N > n_args");
};


template<typename T, typename... Ts>
struct helper_nth_of_pack<0, T, Ts...> {
   using type = T;
};


template<typename Ret, typename... Args>
struct fnptr {
    using type = Ret (*)(Args...);
    using return_type = Ret;

    static constexpr int n_args = sizeof...(Args); 

    template<int N>
    using nth_arg_type = typename helper_nth_of_pack<N, Args...>::type;
};

template<typename Ret>
struct fnptr<Ret> {
    using type = Ret (*)();
    using return_type = Ret;

    static constexpr int n_args = 0; 
};

template<typename Ret, typename... Args>
using fnptr_t = Ret (*)(Args...);

template <typename T>
struct get_fnptr {
    static_assert(false, "Type is not a function!");
};

template <typename Ret, typename... Args>
struct get_fnptr<Ret(*&)(Args...)> {
    using type = fnptr<Ret, Args...>;
};

template <typename Ret, typename... Args>
struct get_fnptr<Ret(*)(Args...)> {
    using type = fnptr<Ret, Args...>;
};

template <typename T>
using get_fnptr_t = typename get_fnptr<T>::type;


template <typename T>
struct has_void_return {
    static constexpr bool value = std::is_void_v<typename get_fnptr_t<T>::return_type>;
};

template <typename T>
constexpr bool has_void_return_v = has_void_return<T>::value;

}