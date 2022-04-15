#ifndef TYPEDETAIL_H
#define TYPEDETAIL_H
#include<type_traits>
namespace ButiTypeDetail {
namespace Impls {
struct has_addition_impl {
	template <typename T>	static auto check(T&& x)->decltype(x + x, std::true_type{});

	template <typename T>	static auto check(...)->std::false_type;
};
struct has_subtract_impl {
	template <typename T>	static auto check(T&& x)->decltype(x - x, std::true_type{});

	template <typename T>	static auto check(...)->std::false_type;
};
struct has_multiply_impl {
	template <typename T>	static auto check(T&& x)->decltype(x* x, std::true_type{});

	template <typename T>	static auto check(...)->std::false_type;
};
struct has_division_impl {
	template <typename T>	static auto check(T&& x)->decltype(x / x, std::true_type{});

	template <typename T>	static auto check(...)->std::false_type;
};

// ÉÅÉìÉoÅ[ä÷åW
template <typename ClassType, typename ValueType>auto member_variable_type_impl(ValueType ClassType::* p)->std::pair<ClassType, ValueType>;
template <typename ClassType, typename RetType, typename... Args>auto member_function_type_impl(RetType(ClassType::* p)(Args...))->std::tuple<ClassType, RetType, std::tuple<Args ...>>;
template <typename ClassType, typename RetType, typename... Args>auto member_function_type_impl(RetType(ClassType::* p)(Args...) const)->std::tuple<ClassType, RetType, std::tuple<Args ...>>;
template <typename RetType, typename... Args>auto function_type_impl(RetType(* p)(Args...))->std::tuple<RetType, std::tuple<Args ...>>;

}

template <typename T>class has_addition : public decltype(Impls::has_addition_impl::check<T>(std::declval<T>())) {};
template <typename T>class has_subtract : public decltype(Impls::has_subtract_impl::check<T>(std::declval<T>())) {};
template <typename T>class has_multiply : public decltype(Impls::has_multiply_impl::check<T>(std::declval<T>())) {};
template <typename T>class has_division : public decltype(Impls::has_division_impl::check<T>(std::declval<T>())) {};

template <typename T>constexpr bool has_addition_v = has_addition<T>();
template <typename T>constexpr bool has_subtract_v = has_subtract<T>();
template <typename T>constexpr bool has_multiply_v = has_multiply<T>();
template <typename T>constexpr bool has_division_v = has_division<T>();


template <auto Ptr>using member_variable_class_type = typename decltype(Impls::member_variable_type_impl(Ptr))::first_type;
template <auto Ptr>using member_variable_type = typename decltype(Impls::member_variable_type_impl(Ptr))::second_type;


template <auto Ptr>using member_function_class_type = std::tuple_element_t<0, decltype(Impls::member_function_type_impl(Ptr))>;
template <auto Ptr>using member_function_return_type = std::tuple_element_t<1, decltype(Impls::member_function_type_impl(Ptr))>;
template <auto Ptr>using member_function_argment_types = std::tuple_element_t<2, decltype(Impls::member_function_type_impl(Ptr))>;
template <auto Ptr, size_t Index=0>using member_function_argment_type = std::tuple_element_t<Index, member_function_argment_types<Ptr>>;

template <auto Ptr>using function_return_type = std::tuple_element_t<0, decltype(Impls::function_type_impl(Ptr))>;
template <auto Ptr>using function_argment_types = std::tuple_element_t<1, decltype(Impls::function_type_impl(Ptr))>;
template <auto Ptr, size_t Index = 0>using function_argment_type = std::tuple_element_t<Index, function_argment_types<Ptr>>;

template <auto Ptr> constexpr std::uint64_t function_argment_size = std::tuple_size_v< function_argment_types<Ptr>>;
}


#endif // !TYPEDETAIL_H
