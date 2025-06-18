/**
 * @file FTPServer.cpp
 * @author MarcosRguez (alu0101470368@ull.edu.es)
 * @brief
 * @version 0.1
 * @date 21/04/2025
 *
 *
 */

module;
#ifdef __has_include
	#include <cerrno>
	#include <cstring>
	#include <cstdarg>
	#include <cstdio>
	#if __has_include(<iostream>)
		#include <iostream>
	#else
		#error "error"
	#endif
	#include <stdexcept>
	#if __has_include(<format>)
		#include <format>
	#endif
	#include <thread>
	#include <functional>
	#include <sys/socket.h>
	#include <arpa/inet.h>
#endif
module ftp;
import :common;

namespace ftp {

FTPServer::FTPServer(const int& port) :
		port{port},
		socket{AF_INET, SOCK_STREAM, 0},
		clientes{} {
#if __cpp_designated_initializers >= 201707L
	const sockaddr_in sin{
		.sin_family{AF_INET},
		.sin_port{htons(static_cast<uint16_t>(port))},
		.sin_addr{.s_addr{INADDR_ANY}},
		.sin_zero{}};
#else
	const sockaddr_in sin{AF_INET, htons(static_cast<uint16_t>(port)), in_addr{INADDR_ANY}, 0};
#endif
	this->socket.Setsockopt(SOL_SOCKET, SO_REUSEADDR, 1);
	this->socket.Bind(*reinterpret_cast<const sockaddr*>(&sin));
	this->socket.Listen(5);
}

#if __cpp_lib_jthread < 201911L
FTPServer::~FTPServer() {
	for (auto& cliente : this->clientes) {
		cliente.join();
	}
}
#endif

void FTPServer::run() {
#if __cpp_lib_print >= 202207L
	std::println(std::cout, "Esperando clientes");
#else
	std::cout << "Esperando clientes" << '\n';
#endif
	while (true) {
		this->clientes.push_back(
#if __cpp_lib_jthread >= 201911L
			std::jthread{WaitForRequests, this->socket.Accept()}
#else
			std::thread{WaitForRequests, this->socket.Accept()}
#endif
		);
#if __cpp_lib_print >= 202207L
		std::println(std::cout, "Cliente ha entrado");
#else
		std::cout << "Cliente ha entrado" << '\n';
#endif
	}
}

}