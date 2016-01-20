#pragma once

// remove this after adding Boost.Fusion
#include <tuple>
#include <type_traits>

template<std::size_t I = 0, typename FuncT, typename... Tp>
inline typename std::enable_if<I == sizeof...(Tp), void>::type
for_each(std::tuple<Tp...> &, FuncT)
{ }

template<std::size_t I = 0, typename FuncT, typename... Tp>
inline typename std::enable_if<I < sizeof...(Tp), void>::type
    for_each(std::tuple<Tp...>& t, FuncT f)
{
    f(std::integral_constant<std::size_t, I>(), std::get<I>(t));
    for_each<I + 1, FuncT, Tp...>(t, f);
}
