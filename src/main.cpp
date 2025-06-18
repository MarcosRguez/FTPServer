/**
 * @file main.cpp
 * @author MarcosRguez (alu0101470368@ull.edu.es)
 * @brief
 * @version 0.1
 * @date 21/04/2025
 *
 *
 */

#ifdef __has_include
	#if __has_include(<cstdlib>)
		#include <cstdlib>
	#endif
	#if __has_include(<exception>)
		#include <exception>
	#endif
	#if __has_include(<iostream>)
		#include <iostream>
	#endif
	#if __has_include(<locale>)
		#include <locale>
	#endif
	#if __has_include(<ranges>)
		#include <ranges>
	#endif
	#if __has_include(<string_view>)
		#include <string_view>
	#endif
#endif

// #include "hpp/FTPServer.hpp"

#ifndef __linux__
	#error "este programa sólo funciona en linux"
#endif

import ftp;

using namespace std::literals;

/**
 * @brief Punto de entrada
 * @param[in] args Argumentos
 */
#if __cpp_constexpr >= 202207L
constexpr
#endif
	void
	Main(
#if __has_cpp_attribute(maybe_unused)
		[[maybe_unused]]
#endif
#if __cpp_lib_ranges >= 201911L
		const std::ranges::view auto&& args
#else
	#if __cpp_lib_string_view >= 201606L
	const std::vector<std::string_view>&& args
	#else
	const std::vector<std::string>&& args
	#endif
#endif
	) {
	std::locale::global(std::locale{"es_ES.UTF-8"});
	ftp::FTPServer server{2121};
	server.run();
}

/**
 * @brief Punto de entrada
 * @param[in] argc Nº de argumentos
 * @param[in] argv Argumentos
 * @return Código de salida
 */
int main(const int argc, const char* const* const argv) {
	try {
#if __cpp_lib_ranges >= 201911L
		Main(std::views::counted(argv, argc) | std::views::transform([](const char* const c) noexcept { return std::string_view{c}; }));
#else
	#if __cpp_lib_string_view >= 201606L
		Main({argv, argv + argc});
	#else
		Main({argv, argv + argc});
	#endif
#endif
	} catch (const std::exception& error) {
#if __cpp_lib_print >= 202207L
		std::println(std::cerr, "{}", error.what());
#else
		std::cerr << error.what() << std::endl;
#endif
		return EXIT_FAILURE;
	} catch (...) {
#if __cpp_lib_print >= 202207L
		std::println(std::cerr, "{}", "Error desconocido");
#else
		std::cerr << "Error desconocido" << std::endl;
#endif
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}