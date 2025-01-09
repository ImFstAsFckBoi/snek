#pragma once

#include "format.hh"
#include "utility.hh"

#include <pytypedefs.h>
#include <methodobject.h>

#include <iomanip>
#include <sstream>

namespace snek {

template<typename T, typename A, typename... Args>
std::string _GenSignature(fnptr_t<T, A, Args...>) {
    using _fntpr = fnptr<T, Args...>;
    std::stringstream ss;
    ss << "(" << GetFmtStrT<A>();
    
    ((ss << ", " << GetFmtStrT<Args>()), ...);

    ss << ") -> " << GetFmtStrT<typename _fntpr::return_type>();
    return ss.str();
}

template<typename T>
std::string _GenSignature(fnptr_t<T>) {
    using _fntpr = fnptr<T>;
    std::stringstream ss;
    ss << "() -> " << GetFmtStrT<typename _fntpr::return_type>();
    return ss.str();
}

template<auto Fn, std::enable_if_t<!std::is_same_v<decltype(Fn), PyCFunction>, int> = 0>
std::string GenFnInfo(const char *name) {
    using _fntpr = get_fnptr_t<decltype(Fn)>;
    using _callMeth = _GetCallMethod<_fntpr::n_args>;
    
    std::stringstream ss;

    ss << "Generated PyCFunction <" << name << ">" << std::endl
              << "\t Base signature: " << _GenSignature(Fn) << std::endl
              << "\t Call method:    " << _callMeth::name << "(" << std::setbase(16) << std::showbase << _callMeth::value << ")" << std::endl;

    return ss.str();
}


}