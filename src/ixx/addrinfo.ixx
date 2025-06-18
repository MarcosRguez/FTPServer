/**
 * @file addrinfo.ixx
 * @author MarcosRguez (alu0101470368@ull.edu.es)
 * @brief
 * @version 0.1
 * @date 17/06/2025
 *
 *
 */

module;
#include <memory>
#include <string>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
export module ftp:addrinfo;

export namespace ftp {

/**
 * @brief Clase envoltorio para addrinfo con RAII
 */
class AddrInfo {
 public:
	/**
	 * @brief Construye una instancia del tipo Addr Info
	 * @param node
	 * @param service
	 * @param hints
	 */
	AddrInfo(const std::string& node, const std::string& service, const addrinfo& hints);
	/**
	 * @return el addrinfo
	 */
#if __has_cpp_attribute(nodiscard)
	[[nodiscard]]
#endif
#if __cplusplus >= 202302L
	constexpr
#endif
		auto GetInfo() const -> const addrinfo&;
 private:
	std::unique_ptr<addrinfo, decltype(&freeaddrinfo)> res{nullptr, freeaddrinfo};
};

#if __cplusplus >= 202302L
constexpr auto AddrInfo::GetInfo() const -> const addrinfo& {
	return *this->res;
}
#endif

} // namespace ftp