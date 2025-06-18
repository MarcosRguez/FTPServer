/**
 * @file comando.ixx
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
export module ftp:comando;

export namespace ftp {

class File;
struct FTPEstado;

/**
 * @brief Clase que representa un comando FTP
 */
class Comando {
 public:
	virtual ~Comando() = default;
	/**
	 * @brief Ejecuta el comando
	 * @param estado estado de la conexión
	 * @param args argumentos del comando
	 */
	virtual void operator()(FTPEstado& estado, const std::vector<std::string>& args);
};

} // namespace ftp