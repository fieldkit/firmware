#ifndef _FAUX_CPP_STD_FILL_H_INCLUDED
#define _FAUX_CPP_STD_FILL_H_INCLUDED

#include <tuple>
#include <utility>

template <size_t... Ints>
struct index_sequence {
    using type = index_sequence;
    using value_type = size_t;
    static constexpr std::size_t size() noexcept {
        return sizeof...(Ints);
    }
};

template <class Sequence1, class Sequence2>
struct _merge_and_renumber;

template <size_t... I1, size_t... I2>
struct _merge_and_renumber<index_sequence<I1...>, index_sequence<I2...>> : index_sequence<I1..., (sizeof...(I1) + I2)...>
{ };

template <size_t N>
struct make_index_sequence : _merge_and_renumber<typename make_index_sequence<N / 2>::type,
                                                 typename make_index_sequence<N - N / 2>::type>
{ };

template<> struct make_index_sequence<0> : index_sequence<> { };
template<> struct make_index_sequence<1> : index_sequence<0> { };

namespace detail {

template <class T, class Tuple, std::size_t... I>
constexpr T make_from_tuple_impl(Tuple&& t, index_sequence<I...>) {
    return T(std::get<I>(std::forward<Tuple>(t))...);
}

template <class T, typename W, class Tuple, std::size_t... I>
constexpr T *make_from_tuple_at_impl(W&& where, Tuple&& t, index_sequence<I...>) {
    return new (where) T(std::get<I>(std::forward<Tuple>(t))...);
}

template <class T, class Tuple, std::size_t... I>
constexpr T *make_from_tuple_heap_impl(Tuple&& t, index_sequence<I...>) {
    return new T(std::get<I>(std::forward<Tuple>(t))...);
}

} // namespace detail

template <class T, class Tuple>
constexpr T make_from_tuple(Tuple&& t) {
    return detail::make_from_tuple_impl<T>(std::forward<Tuple>(t), make_index_sequence<std::tuple_size<typename std::decay<Tuple>::type>::value>{});
}

template <class T, typename W, class Tuple>
constexpr T *make_from_tuple_at(W&& where, Tuple&& t) {
    return detail::make_from_tuple_at_impl<T, W>(where, std::forward<Tuple>(t), make_index_sequence<std::tuple_size<typename std::decay<Tuple>::type>::value>{});
}

template <class T, class Tuple>
constexpr T *make_from_tuple_heap(Tuple&& t) {
    return detail::make_from_tuple_heap_impl<T>(std::forward<Tuple>(t), make_index_sequence<std::tuple_size<typename std::decay<Tuple>::type>::value>{});
}
/*
template <typename... T>
constexpr auto make_array(T&&... values) ->
    std::array<
        typename std::decay<
            typename std::common_type<T...>::type>::type,
        sizeof...(T)> {
    return std::array<
        typename std::decay<
            typename std::common_type<T...>::type>::type,
        sizeof...(T)>{std::forward<T>(values)...};
}
*/
template <typename... T>
using common_type_t = typename std::common_type<T...>::type;

template <typename T>
using remove_cv_t = typename std::remove_cv<T>::type;

template <bool, typename T, typename... U>
struct lazy_conditional_c;

template <typename T>
struct lazy_conditional_c<true, T>
{
	using type = typename T::type;
};

template <typename T, typename U>
struct lazy_conditional_c<true, T, U>
{
	using type = typename T::type;
};

template <typename T, typename U>
struct lazy_conditional_c<false, T, U>
{
	using type = typename U::type;
};

template <typename V, typename T, typename... U>
using If = lazy_conditional_c<V::value, T, U...>;

template <typename V, typename T, typename... U>
using If_t = typename If<V, T, U...>::type;

template <typename T>
struct identity_of
{
	using type = T;
};

template <template <typename> class F, typename... T>
struct no_type : std::true_type {};

template <template <typename> class F, typename T1, typename... T2>
struct no_type<F, T1, T2...> :
	std::integral_constant
	<
	    bool,
	    not F<T1>::value and no_type<F, T2...>::value
	>
{};

template <template <typename> class F, template <typename> class G>
struct composed
{
	template <typename T>
	using call = F<typename G<T>::type>;
};

template <typename T>
struct _is_reference_wrapper : std::false_type {};

template <typename T>
struct _is_reference_wrapper<std::reference_wrapper<T>> : std::true_type {};

template <typename T>
using is_reference_wrapper =
	composed<_is_reference_wrapper, std::remove_cv>::call<T>;

template <typename V = void, typename... T>
constexpr auto make_array(T&&... t)
	-> std::array
	<
	    If_t
	    <
		std::is_void<V>,
		If
		<
		    no_type
		    <
			composed
			<
			    is_reference_wrapper,
			    std::remove_reference
			>
			::call,
			T...
		    >,
		    std::common_type<T...>
		>,
		identity_of<V>
	    >,
	    sizeof...(T)
	>
{
	return {{ std::forward<T>(t)... }};
}

template <size_t... I>
struct _indices {};

template <size_t N, size_t... I>
struct _build_indices : _build_indices<N - 1, N - 1, I...> {};
 
template <size_t... I>
struct _build_indices<0, I...> : _indices<I...> {};

template <typename T, size_t N, size_t... I>
constexpr auto _to_array(T (&arr)[N], _indices<I...>)
	-> std::array<remove_cv_t<T>, N>
{
	return {{ arr[I]... }};
}

template <typename T, size_t N>
constexpr auto to_array(T (&arr)[N])
	-> std::array<remove_cv_t<T>, N>
{
	return _to_array(arr, _build_indices<N>());
}

#endif
