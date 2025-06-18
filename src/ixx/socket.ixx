/**
 * @file socket.ixx
 * @author MarcosRguez (alu0101470368@ull.edu.es)
 * @brief
 * @version 0.1
 * @date 17/06/2025
 *
 *
 */

module;
#include <tuple>
#include <vector>
#include <span>
#include <string>
#include <string_view>
#include <sys/socket.h>
#include <netinet/in.h>
#ifdef __has_include
	#if __has_include(<cstring>)
		#include <cstring>
	#else
		#error "error"
	#endif
	#if __has_include(<utility>)
		#include <utility>
	#else
		#error "error"
	#endif
	#if __has_include(<stdexcept>)
		#include <stdexcept>
	#else
		#error "error"
	#endif
#endif
#include <arpa/inet.h>
export module ftp:socket;

export namespace ftp {

/**
 * @brief Socket
 */
class Socket {
 public:
	Socket() = default;
	Socket(const Socket&) = delete;
	Socket(Socket&&);
	constexpr Socket(const int& sockfd, const bool& conectado = false);
	Socket(
		const int&,
		const int&,
		const int&);
	~Socket();
	constexpr auto operator=(const Socket&) -> Socket& = delete;
	constexpr auto operator=(Socket&&) -> Socket&;
#if __has_cpp_attribute(nodiscard)
	[[nodiscard]]
#endif
	constexpr auto GetSockfd() const -> const int&;
#if __cplusplus >= 202002L
	constexpr
#endif
		auto
		GetSockfdYDesasociar() -> int;
	void Bind(const sockaddr&);
	void Setsockopt(const int& level, const int& option_name, const auto& value);
	void Shutdown(const int&);
	void Listen(const int&);
#if __has_cpp_attribute(nodiscard)
	[[nodiscard]]
#endif
	auto Accept(const int& flags = 0) -> Socket;
	void Connect(const sockaddr&);
#if __has_cpp_attribute(nodiscard)
	[[nodiscard]]
#endif
	auto GetSockname() const -> sockaddr;
	template <typename T>
#if __has_cpp_attribute(nodiscard)
	[[nodiscard]]
#endif
	auto GetAddr() const -> T = delete;
#if __has_cpp_attribute(nodiscard)
	[[nodiscard]]
#endif
	auto GetPort() const -> in_port_t;
#if __cpp_lib_span >= 202002L
	template <typename T>
	void Send(const std::span<T>&, const int& flags = 0);
#endif
#if __cpp_lib_string_view >= 201606L
	void Send(const std::string_view&, const int& flags = 0);
#else
	void Send(const std::string&, const int& flags = 0);
#endif
	template <typename T>
#if __has_cpp_attribute(nodiscard)
	[[nodiscard]]
#endif
	auto Recv(const int& flags = 0) -> T = delete;
 private:
	int sockfd{-1};
	bool conectado{false};
};

template <>
#if __has_cpp_attribute(nodiscard)
[[nodiscard]]
#endif
auto Socket::Recv(const int& flags) -> std::string;
template <>
#if __has_cpp_attribute(nodiscard)
[[nodiscard]]
#endif
auto Socket::GetAddr<std::string>() const -> std::string;
template <>
#if __has_cpp_attribute(nodiscard)
[[nodiscard]]
#endif
auto Socket::GetAddr<in_addr>() const -> in_addr;
template <>
#if __has_cpp_attribute(nodiscard)
[[nodiscard]]
#endif
auto Socket::GetAddr<in6_addr>() const -> in6_addr;

constexpr Socket::Socket(const int& sockfd, const bool& conectado) :
		sockfd{sockfd},
		conectado{conectado} {}

#if __cpp_lib_span >= 202002L
template <typename T>
void Socket::Send(const std::span<T>& búfer, const int& flags) {
	if (send(this->sockfd, búfer.data(), búfer.size_bytes(), flags) == -1) {
		throw std::runtime_error{std::strerror(errno)};
	}
}
#endif

constexpr auto Socket::operator=(Socket&& socket) -> Socket& {
	if (this == &socket) { return *this; }
	this->sockfd = std::exchange(socket.sockfd, -1);
	this->conectado = std::exchange(socket.conectado, false);
	return *this;
}

constexpr auto Socket::GetSockfd() const -> const int& {
	return this->sockfd;
}

#if __cplusplus >= 202002L
constexpr auto Socket::GetSockfdYDesasociar() -> int {
	return std::exchange(this->sockfd, -1);
}
#endif

void Socket::Setsockopt(const int& level, const int& option_name, const auto& value) {
	if (setsockopt(this->sockfd, level, option_name, &value, sizeof(value)) == -1) {
		throw std::runtime_error{std::strerror(errno)};
	}
}

} // namespace ftp