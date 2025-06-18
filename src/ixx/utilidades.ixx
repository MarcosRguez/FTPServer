/**
 * @file utilidades.ixx
 * @author MarcosRguez (alu0101470368@ull.edu.es)
 * @brief
 * @version 0.1
 * @date 17/06/2025
 *
 *
 */

module;
#include <filesystem>
#include <array>
#include <vector>
#include <compare>
#include <deque>
#include <forward_list>
#include <list>
#include <fstream>
#ifdef __has_include
	#if __has_include(<inplace_vector>)
		#include <inplace_vector>
	#elif __has_include(<experimental/inplace_vector>)
		#include <experimental/inplace_vector>
	#endif
#endif
export module ftp:utilidades;

export namespace ftp {

template <typename T, template <typename...> typename D>
struct is_specialization : std::false_type {};

template <template <typename...> typename T, typename... D>
struct is_specialization<T<D...>, T> : std::true_type {};

template <typename T, template <typename...> typename D>
inline constexpr bool is_specialization_v{is_specialization<T, D>::value};

template <typename T>
#if __has_cpp_attribute(nodiscard)
[[nodiscard]]
#endif
auto CargarFichero(
	const
#if __cpp_lib_filesystem >= 201703L
	std::filesystem::path
#else
	std::string
#endif
		& ruta) -> T {
#if __cpp_lib_logical_traits >= 201510L
	static_assert(std::disjunction_v<std::is_same<T, std::string>, is_specialization<T, std::vector>>);
#endif
	T resultado;
	std::fstream file{ruta};
	if (!file.is_open()) { throw std::runtime_error{"Fichero no encontrado"}; }
	auto fsize{file.tellg()};
	file.seekg(0, std::fstream::end);
	fsize = file.tellg() - fsize;
	resultado.resize(fsize);
	file.seekg(std::fstream::beg);
	file.read(reinterpret_cast<char*>(
#if __cplusplus >= 201703L
							resultado.data()
#else
							const_cast<char*>(resultado.data())
#endif
								),
						fsize);
	return resultado;
}

template <typename T, std::size_t N>
constexpr auto contains(const std::array<T, N>& contenedor, const T& valor) noexcept -> bool {
	return
#if __cpp_lib_ranges >= 201911L
		std::ranges::find(contenedor, valor)
#else
		std::find(contenedor.cbegin(), contenedor.cend(), valor)
#endif
		!= contenedor.cend();
}

template <typename T>
constexpr auto contains(const std::vector<T>& contenedor, const T& valor) noexcept -> bool {
	return
#if __cpp_lib_ranges >= 201911L
		std::ranges::find(contenedor, valor)
#else
		std::find(contenedor.cbegin(), contenedor.cend(), valor)
#endif
		!= contenedor.cend();
}

template <typename T>
constexpr auto contains(const std::deque<T>& contenedor, const T& valor) noexcept -> bool {
	return
#if __cpp_lib_ranges >= 201911L
		std::ranges::find(contenedor, valor)
#else
		std::find(contenedor.cbegin(), contenedor.cend(), valor)
#endif
		!= contenedor.cend();
}

template <typename T>
constexpr auto contains(const std::forward_list<T>& contenedor, const T& valor) noexcept -> bool {
	return
#if __cpp_lib_ranges >= 201911L
		std::ranges::find(contenedor, valor)
#else
		std::find(contenedor.cbegin(), contenedor.cend(), valor)
#endif
		!= contenedor.cend();
}

template <typename T>
constexpr auto contains(const std::list<T>& contenedor, const T& valor) noexcept -> bool {
	return
#if __cpp_lib_ranges >= 201911L
		std::ranges::find(contenedor, valor)
#else
		std::find(contenedor.cbegin(), contenedor.cend(), valor)
#endif
		!= contenedor.cend();
}

template <typename T, std::size_t N>
constexpr auto size_bytes(const std::array<T, N>& contenedor) noexcept -> std::size_t {
	return contenedor.size() * sizeof(T);
}

template <typename T>
constexpr auto size_bytes(const std::vector<T>& contenedor) noexcept -> std::size_t {
	return contenedor.size() * sizeof(T);
}

#if __cpp_lib_inplace_vector >= 202406L
template <typename T>
constexpr auto size_bytes(const std::inplace_vector<T>& contenedor) noexcept -> std::size_t {
	return contenedor.size() * sizeof(T);
}
#endif

template <typename T>
constexpr auto size_bytes(const std::deque<T>& contenedor) noexcept -> std::size_t {
	return contenedor.size() * sizeof(T);
}

template <typename T>
constexpr auto size_bytes(const std::forward_list<T>& contenedor) noexcept -> std::size_t {
	return contenedor.size() * sizeof(T);
}

template <typename T>
constexpr auto size_bytes(const std::list<T>& contenedor) noexcept -> std::size_t {
	return contenedor.size() * sizeof(T);
}

#if __cplusplus >= 202002L
template <typename L, typename R>
constexpr auto operator<=>(const std::reference_wrapper<L>& lhs, const std::reference_wrapper<R>& rhs) {
	return lhs.get() <=> rhs.get();
}
#endif

template <typename T>
constexpr auto unmove(T&& t) noexcept -> T& {
	return static_cast<T&>(t);
}

template <typename... Args>
constexpr auto format(std::string fmt, Args&&... args) -> std::string {
#if __cpp_size_t_suffix >= 202011L
	auto index{0uz};
	auto pos{0uz};
#else
	std::size_t index{0};
	std::size_t pos{0};
#endif
	([&] {
		index = fmt.find("{}", pos);
		fmt.replace(index, 2, args);
		pos = index + 1;
		// if (index != std::string::npos) { return fmt; }
	}(),
	 ...);
	return fmt;
}

auto split(const std::string&, const char& sep = ' ') -> std::vector<std::string>;
auto replace(std::string&, const char&, const char&) -> std::string&;

} // namespace ftp