/**
 * @file ftp.ixx
 * @author MarcosRguez (alu0101470368@ull.edu.es)
 * @brief
 * @version 0.1
 * @date 17/06/2025
 *
 *
 */

export module ftp;
export import :addrinfo;
export import :comando;
export import :common;
export import :estado;
export import :server;
export import :mapas;
export import :socket;
export import :utilidades;
export import :comando.cdup;
export import :comando.cwd;
export import :comando.eprt;
export import :comando.epsv;
export import :comando.feat;
export import :comando.list;
export import :comando.mdtm;
export import :comando.mkd;
export import :comando.pass;
export import :comando.pasv;
export import :comando.port;
export import :comando.pwd;
export import :comando.quit;
export import :comando.retr;
export import :comando.size;
export import :comando.stor;
export import :comando.syst;
export import :comando.type;
export import :comando.user;