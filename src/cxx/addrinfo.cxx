/**
 * @file addrinfo.cpp
 * @author MarcosRguez (alu0101470368@ull.edu.es)
 * @brief
 * @version 0.1
 * @date 21/04/2025
 *
 *
 */

module;
#include <stdexcept>
#include <cstring>
#if __has_include(<format>)
	#include <format>
#elif __has_include(<sstream>)
	#include <sstream>
#endif
#include <netdb.h>
module ftp;

namespace ftp {

AddrInfo::AddrInfo(const std::string& node, const std::string& service, const addrinfo& hints) {
	addrinfo* cosa{};
#if __cplusplus >= 201703L
	if (const auto ret{getaddrinfo(node.c_str(), service.c_str(), &hints, &cosa)}; ret != 0)
#else
	const auto ret{getaddrinfo(node.c_str(), service.c_str(), &hints, &cosa)};
	if (ret != 0)
#endif
	{
		if (ret == EAI_SYSTEM) {
#if __cpp_lib_format >= 201907L
			throw std::runtime_error{std::format("{}: {}", gai_strerror(ret), std::strerror(errno))};
#else
			std::stringstream ss;
			ss << gai_strerror(ret) << ": " << std::strerror(errno);
			throw std::runtime_error{ss.str()};
#endif
		} else {
			throw std::runtime_error{gai_strerror(ret)};
		}
	}
	this->res.reset(cosa);
}

#if __cplusplus < 202302L
auto AddrInfo::GetInfo() const -> const addrinfo& {
	return *this->res;
}
#endif

}