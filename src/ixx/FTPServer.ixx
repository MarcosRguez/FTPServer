/**
 * @file FTPServer.ixx
 * @author MarcosRguez (alu0101470368@ull.edu.es)
 * @brief
 * @version 0.1
 * @date 17/06/2025
 *
 *
 */

module;
#include <list>
#include <thread>
export module ftp:server;
import :socket;

export namespace ftp {

/**
 * @brief Clase que representa un servidor FTP
 */
class FTPServer {
 public:
	/**
	 * @brief Construye una instancia del tipo FTPServer
	 * @param port puerto de escuchar conexiones entrantes
	 */
	FTPServer(const int& port = 21);
#if __cpp_lib_jthread < 201911L
	~FTPServer();
#endif
	/**
	 * @brief Ejecuta el servidor
	 */
	void run();
 private:
	int port{};
	Socket socket;
	std::list<
#if __cpp_lib_jthread >= 201911L
		std::jthread
#else
		std::thread
#endif
		>
		clientes;
};

} // namespace ftp