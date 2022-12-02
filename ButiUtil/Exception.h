#ifndef BUTIEXCEPTION_H
#define BUTIEXCEPTION_H
#include <exception>
#include<string>
#define BUTIEXCEPTION_DEFINED
namespace ButiEngine {
class ButiException :public std::exception {

	std::string m_errorMessage;
	const std::string m_fileName;
	const std::int32_t m_line;
public:
	ButiException(const std::string& arg_message) :m_fileName(""), m_line(-1) {
		m_errorMessage = arg_message;
	}
	ButiException(const std::string arg_fileName,const std::int32_t arg_line, const std::string& arg_message) :m_fileName(arg_fileName), m_line(arg_line) {
		m_errorMessage = arg_message;
	}
	std::int32_t GetLine()const { return m_line; }
	const std::string& GetFileName()const { return m_fileName; }
	const std::string GetErrorMessage()const { return m_errorMessage; }
	const const char* what() const throw() {
		return m_errorMessage.c_str();
	}
};
}

#define THROWBUTIEXCEPTION( Message )\
throw ButiEngine::ButiException(__FILE__, __LINE__, Message )\

#endif // !BUTIEXCEPTION_H
