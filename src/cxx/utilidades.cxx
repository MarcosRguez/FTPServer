/**
 * @file utilidades.cxx
 * @author MarcosRguez (alu0101470368@ull.edu.es)
 * @brief
 * @version 0.1
 * @date 17/06/2025
 *
 *
 */

module;
#include <string>
#include <vector>
module ftp;

namespace ftp {

auto split(const std::string& str, const char& sep) -> std::vector<std::string> {
	std::vector<std::string> resultado;
#if __cpp_size_t_suffix >= 202011L
	auto pos{0uz};
	auto index{0uz};
#else
	std::size_t pos{0};
	std::size_t index{0};
#endif
	do {
		index = str.find(sep, pos);
		resultado.push_back(str.substr(pos, index - pos));
		pos = index + 1;
	} while (index != std::string::npos);
	return resultado;
}

auto replace(std::string& str, const char& viejo, const char& nuevo) -> std::string& {
#if __cpp_size_t_suffix >= 202011L
	auto pos{0uz};
	auto index{0uz};
#else
	std::size_t pos{0};
	std::size_t index{0};
#endif
	while (index != std::string::npos) {
		index = str.find(viejo, pos);
		str[index] = nuevo;
		pos = index;
	}
	return str;
}

} // namespace ftp