/**
 * @file socket.cxx
 * @author MarcosRguez (alu0101470368@ull.edu.es)
 * @brief
 * @version 0.1
 * @date 21/04/2025
 *
 *
 */

module;
#include <cerrno>
#include <cstring>
#include <stdexcept>
#include <type_traits>
#include <iostream>
#include <thread>
#include <utility>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
module ftp;

namespace ftp {

Socket::Socket(Socket&& socket) :
		sockfd{std::exchange(socket.sockfd, -1)},
		conectado{std::exchange(socket.conectado, false)} {}

Socket::Socket(
	const int& domain,
	const int& type,
	const int& protocol) :
		sockfd{socket(domain, type, protocol)} {
	if (this->sockfd == -1) {
		throw std::runtime_error{std::strerror(errno)};
	}
}

Socket::~Socket() {
	if (this->sockfd != -1) {
		if (this->conectado) {
			this->Shutdown(SHUT_RDWR);
		}
		if (close(this->sockfd) != 0) {
#if __cpp_lib_print >= 202207L
			std::println(std::cerr, "{}", std::strerror(errno));
#else
			std::cerr << std::strerror(errno) << '\n';
#endif
		}
	}
}

void Socket::Shutdown(const int& flags) {
	if (shutdown(this->sockfd, flags) == -1) {
		throw std::runtime_error{std::strerror(errno)};
	}
	this->conectado = false;
}

void Socket::Bind(const sockaddr& addr) {
	if (bind(this->sockfd, &addr, sizeof(addr)) == -1) {
		throw std::runtime_error{std::strerror(errno)};
	}
	this->conectado = true;
}

void Socket::Listen(const int& backlog) {
	if (listen(this->sockfd, backlog) == -1) {
		throw std::runtime_error{std::strerror(errno)};
	}
	this->conectado = true;
}

auto Socket::Accept(const int& flags) -> Socket {
	sockaddr addr{};
	socklen_t addrlen{};
	const int fd{accept4(this->sockfd, &addr, &addrlen, flags)};
	if (fd == -1) {
		throw std::runtime_error{std::strerror(errno)};
	}
	return Socket{fd, true};
}

void Socket::Connect(const sockaddr& addr) {
	if (connect(this->sockfd, &addr, sizeof(addr)) == -1) {
		throw std::runtime_error{std::strerror(errno)};
	}
	this->conectado = true;
}

auto Socket::GetPort() const -> in_port_t {
	const auto info{this->GetSockname()};
	switch (info.sa_family) {
		case (AF_INET): return reinterpret_cast<const sockaddr_in*>(&info)->sin_port;
		case (AF_INET6): return reinterpret_cast<const sockaddr_in6*>(&info)->sin6_port;
		default: throw std::runtime_error{"Unknown address family."};
	}
}

auto Socket::GetSockname() const -> sockaddr {
	sockaddr addr{};
	socklen_t len{sizeof(addr)};
	if (getsockname(this->sockfd, &addr, &len) == -1) {
		throw std::runtime_error{std::strerror(errno)};
	}
	return addr;
}

#if __cpp_lib_string_view >= 201606L
void Socket::Send(const std::string_view& búfer, const int& flags) {
	#if __cpp_lib_span >= 202002L
	this->Send(std::span{búfer.begin(), búfer.end()}, flags);
	#else
	if (send(this->sockfd, búfer.data(), búfer.size() * sizeof(std::remove_const_t<std::remove_reference_t<decltype(búfer)>>::value_type), flags) == -1) {
		throw std::runtime_error{std::strerror(errno)};
	}
	#endif
}
#else
void Socket::Send(const std::string& búfer, const int& flags) {
	if (!this->conectado) {
		throw std::runtime_error{"Socket no conectado."};
	}
	if (send(this->sockfd, búfer.data(), búfer.size() * sizeof(char), flags) == -1) {
		throw std::runtime_error{std::strerror(errno)};
	}
}
#endif

#if __cplusplus < 202002L
auto Socket::GetSockfdYDesasociar() -> int {
	return std::exchange(this->sockfd, -1);
}
#endif

template <>
auto Socket::GetAddr<std::string>() const -> std::string {
	std::string resultado;
	const auto info{this->GetSockname()};
	if (info.sa_family == AF_INET) {
		const auto& info_in{*reinterpret_cast<const sockaddr_in*>(&info)};
		resultado.resize(INET_ADDRSTRLEN);
		if (
			inet_ntop(
				AF_INET, &info_in.sin_addr,
#if __cplusplus >= 201703L
				resultado.data(),
#else
				const_cast<char*>(resultado.data()),
#endif
				resultado.size()) == nullptr) {
			throw std::runtime_error{std::strerror(errno)};
		}
	} else if (info.sa_family == AF_INET6) {
		const auto& info_in{*reinterpret_cast<const sockaddr_in6*>(&info)};
		resultado.resize(INET6_ADDRSTRLEN);
		if (
			inet_ntop(
				AF_INET6, &info_in.sin6_addr,
#if __cplusplus >= 201703L
				resultado.data(),
#else
				const_cast<char*>(resultado.data()),
#endif
				resultado.size()) == nullptr) {
			throw std::runtime_error{std::strerror(errno)};
		}
	} else {
		throw std::runtime_error{"Unknown address family."};
	}
	resultado.resize(std::strlen(resultado.data()));
	return resultado;
}

template <>
auto Socket::GetAddr<in_addr>() const -> in_addr {
	const auto info{this->GetSockname()};
	if (info.sa_family != AF_INET) {
		throw std::runtime_error{"Unknown address family."};
	}
	return reinterpret_cast<const sockaddr_in*>(&info)->sin_addr;
}

template <>
auto Socket::GetAddr<in6_addr>() const -> in6_addr {
	const auto info{this->GetSockname()};
	if (info.sa_family != AF_INET6) {
		throw std::runtime_error{"Unknown address family."};
	}
	return reinterpret_cast<const sockaddr_in6*>(&info)->sin6_addr;
}

template <>
auto Socket::Recv(const int& flags) -> std::string {
	std::string resultado;
	static const auto max{100};
	char búfer[max];
	if (flags & MSG_DONTWAIT) {
		ssize_t n{};
		while (true) {
			n = recv(this->sockfd, búfer, max, flags);
			if (n == -1) {
				if (errno == EAGAIN || errno == EWOULDBLOCK) {
					std::this_thread::yield();
					continue;
				}
				throw std::runtime_error{std::strerror(errno)};
			} else if (n == 0) {
				break;
			}
			resultado.append(búfer, n);
		}
	} else {
		const auto n{recv(this->sockfd, búfer, max, flags)};
		resultado.resize(n);
		resultado.assign(búfer, n);
		if (n == -1) {
			throw std::runtime_error{std::strerror(errno)};
		}
	}
	return resultado;
}

}