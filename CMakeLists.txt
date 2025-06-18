cmake_minimum_required(VERSION 3.20)
file(READ "${CMAKE_CURRENT_SOURCE_DIR}/VERSION" VERSION)
project(
	FTPServer
	VERSION ${VERSION}
	LANGUAGES CXX)
message(STATUS "Configurando '${PROJECT_NAME}' versión: ${VERSION}")
# Colorear
set(CMAKE_COLOR_DIAGNOSTICS ON)
set(CMAKE_COLOR_MAKEFILE ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
if(NOT (CMAKE_BUILD_TYPE STREQUAL "Debug"))
	set(BUILD_SHARED_LIBS OFF)
else()
	set(BUILD_SHARED_LIBS ON)
endif()

message(CHECK_START "Buscando ccache...")
find_program(CCACHE_PROGRAM ccache)
if(CCACHE_PROGRAM)
	message(CHECK_PASS "ccache disponible")
	set(C_COMPILER_LAUNCHER ${CCACHE_PROGRAM})
	set(CXX_COMPILER_LAUNCHER ${CCACHE_PROGRAM})
else()
	message(CHECK_FAIL "ccache no encontrado")
endif()

include(CheckIPOSupported)

message(CHECK_START "check_ipo_supported...")
check_ipo_supported(RESULT ipo_supported)
if(ipo_supported)
	message(CHECK_PASS "IPO soportado")
else()
	message(CHECK_FAIL "IPO no soportado")
endif()

file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/include/")
file(GLOB comandos CONFIGURE_DEPENDS "src/ixx/comandos/*.ixx")
# file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/include/comandos.hxx" "#pragma once\n")
# foreach(comando ${comandos})
# 	file(APPEND "${CMAKE_CURRENT_BINARY_DIR}/include/comandos.hxx" "#include \"${comando}\"\n")
# endforeach()

file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/include/mapa.hxx" "#pragma once\n")
file(APPEND "${CMAKE_CURRENT_BINARY_DIR}/include/mapa.hxx" "#define mapa \\\n")
foreach(comando ${comandos})
	get_filename_component(nombre "${comando}" NAME_WE)
	string(TOUPPER "${nombre}" nombre_mayus)
	file(APPEND "${CMAKE_CURRENT_BINARY_DIR}/include/mapa.hxx" "resultado.emplace(\"${nombre_mayus}\", std::make_unique<${nombre_mayus}>());\\\n")
endforeach()
file(APPEND "${CMAKE_CURRENT_BINARY_DIR}/include/mapa.hxx" "\n")

add_subdirectory("src")