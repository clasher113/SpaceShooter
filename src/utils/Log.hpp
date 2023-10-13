#pragma once
#ifdef __cplusplus
#include <sstream>
#ifdef _DEBUG
#include <iostream>
#define INFO(X)	{std::ostringstream __outStringStream__; \
				 __outStringStream__ << X << std::endl; \
				 std::cout << __outStringStream__.str();}
#define ERR(X) {std::ostringstream __outStringStream__; \
			    __outStringStream__ << X; \
			    std::cerr << __outStringStream__.str() << std::endl;}
#else
#define INFO(X) // ¯\_(ツ)_/¯
#define ERR(X)  // ¯\_(ツ)_/¯
#endif // _DEBUG
#ifdef _WIN32
#define NOMINMAX // sfml fix
#include <windows.h>
#define FATAL(ERROR, DESCRIPTION)  std::ostringstream error; \
								   std::ostringstream description; \
								   error << ERROR; \
								   description << DESCRIPTION; \
								   MessageBoxA(NULL, (LPCSTR)description.str().c_str(), (LPCSTR)error.str().c_str(), MB_ICONERROR | MB_OK); \
								   exit(1);
#elif __linux__ 
#define FATAL(ERROR, DESCRIPTION) put linux code here
#elif __APPLE__
#define FATAL(ERROR, DESCRIPTION) put apple code here
#endif // _WIN32
#endif // __cplusplus