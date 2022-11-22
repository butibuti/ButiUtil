#ifndef BUTIEXCEPTION_H
#define BUTIEXCEPTION_H
#include <exception>
#include<string>
#define BUTIEXCEPTION_DEFINED
namespace ButiEngine {
class ButiException :public std::exception {

	std::wstring m_wstr_errorMessage;
	const std::string m_fileName;
	const std::int32_t m_line;
public:
	ButiException(const std::wstring& message1, const std::wstring& message2, const std::wstring& message3):m_fileName(""), m_line(-1) {
		m_wstr_errorMessage = message1;
		m_wstr_errorMessage += L"\n" + message2 + L"\n" + message3;
	}
	ButiException(const std::wstring& message) :m_fileName(""), m_line(-1) {
		m_wstr_errorMessage = message;
	}
	ButiException(const std::string arg_fileName,const std::int32_t arg_line, const std::wstring& message) :m_fileName(arg_fileName), m_line(arg_line) {
		m_wstr_errorMessage = message;
	}
	std::int32_t GetLine()const { return m_line; }
	const std::string& GetFileName()const { return m_fileName; }
	const std::wstring GetErrorMessage()const { return m_wstr_errorMessage; }
	const std::wstring& what_w() const throw() {
		return m_wstr_errorMessage;
	}
};
}

#define THROWBUTIEXCEPTION( Message )\
throw ButiEngine::ButiException(__FILE__, __LINE__, Message )\

#endif // !BUTIEXCEPTION_H
