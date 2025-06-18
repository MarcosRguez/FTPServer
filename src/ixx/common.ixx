/**
 * @file common.ixx
 * @author MarcosRguez (alu0101470368@ull.edu.es)
 * @brief
 * @version 0.1
 * @date 17/06/2025
 *
 *
 */

module;
#ifdef __has_include
	#if __has_include(<string>)
		#include <string>
	#elif __has_include(<experimental/string>)
		#include <experimental/string>
	#else
		#error "error"
	#endif
	#if __has_include(<string_view>)
		#include <string_view>
	#elif __has_include(<experimental/string_view>)
		#include <experimental/string_view>
	#else
		#error "error"
	#endif
	#if __has_include(<memory>)
		#include <memory>
	#elif __has_include(<experimental/memory>)
		#include <experimental/memory>
	#else
		#error "error"
	#endif
	#if __has_include(<filesystem>)
		#include <filesystem>
	#elif __has_include(<experimental/filesystem>)
		#include <experimental/filesystem>
	#else
		#error "error"
	#endif
	#if __has_include(<map>)
		#include <map>
	#elif __has_include(<experimental/map>)
		#include <experimental/map>
	#else
		#error "error"
	#endif
	#if __has_include(<type_traits>)
		#include <type_traits>
	#elif __has_include(<experimental/type_traits>)
		#include <experimental/type_traits>
	#else
		#error "error"
	#endif
#endif
export module ftp:common;
import :socket;
import :comando;

export namespace ftp {

class ClientConnection; // ¿Esto tiene sentido con módulos?

#if __has_cpp_attribute(nodiscard)
[[nodiscard]]
#endif
auto ConectarSocket(const std::string&, const int&) -> Socket;
#if __cpp_lib_filesystem >= 201703L
	#if __has_cpp_attribute(nodiscard)
[[nodiscard]]
	#endif
auto convert_file_time_to_mdtm_format(const std::filesystem::file_time_type&) -> std::string;
#endif
#if __has_cpp_attribute(nodiscard)
[[nodiscard]]
#endif
auto CrearMapaComandos() -> std::map<
#if __cpp_lib_string_view >= 201606L
	std::string_view,
#else
	std::string,
#endif
	std::unique_ptr<Comando>>;
/**
 * @brief Obtiene una cadena de formato de respuesta asociada al código
 * @param code Código de respuesta FTP
 * @return std::string_view o std::string dependiendo de la versión de la biblioteca estándar
 */
#if __has_cpp_attribute(nodiscard)
[[nodiscard]]
#endif
constexpr auto GetReplyFMT(const int& code) ->
#if __cpp_lib_string_view >= 201606L
	std::string_view
#else
	std::string
#endif
	;
/**
 * @brief
 * @return Obtiene una cadena de formato de respuesta asociada al código
 */
#if __has_cpp_attribute(nodiscard)
[[nodiscard]]
#endif
auto GetReply(const int& code) -> std::string;
#if __has_cpp_attribute(nodiscard)
[[nodiscard]]
#endif
#if __cpp_lib_ranges >= 201911L
constexpr auto to_string_from_range(auto&& rango) -> std::string {
	return std::string{std::ranges::begin(rango), std::ranges::end(rango)};
}
#endif
void WaitForRequests(Socket&&);

} // namespace ftp