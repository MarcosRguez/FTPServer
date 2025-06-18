/**
 * @file ftp_estado.ixx
 * @author MarcosRguez (alu0101470368@ull.edu.es)
 * @brief
 * @version 0.1
 * @date 17/06/2025
 *
 *
 */

export module ftp:estado;
import :socket;

export namespace ftp {

struct FTPEstado {
	Socket controlSock;
	Socket dataSock;
	bool pasivo{};
	bool parar{false};
};

} // namespace ftp