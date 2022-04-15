#ifndef BUTIEXCEPTION_H
#define BUTIEXCEPTION_H
#include <exception>
#include<string>
#define BUTIEXCEPTION_DEFINED
namespace ButiEngine {
class ButiException :public std::exception {

	std::wstring wstr_errorMessage;
public:
	ButiException(const std::wstring& message1, const std::wstring& message2, const std::wstring& message3) {
		wstr_errorMessage = message1;
		wstr_errorMessage += L"\n" + message2 + L"\n" + message3;
	}
	ButiException(const std::wstring& message) {
		wstr_errorMessage = message;
	}
	const std::wstring& what_w() const throw() {
		return wstr_errorMessage;
	}
};
}
#endif // !BUTIEXCEPTION_H
