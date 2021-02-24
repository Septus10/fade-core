#include <Core/Log.hpp>

#include <fstream>
#include <chrono>
#include <ctime>
#include <time.h>

namespace Fade {

const CLogVerbosity Warning("Warning", ELogVerbosity::Warning);
const CLogVerbosity Error("Error", ELogVerbosity::Error);
const CLogVerbosity Info("Info", ELogVerbosity::Info);

CCompositeLogger* CCompositeLogger::sm_CompositeLogger = nullptr;

CFileLogger::CFileLogger(SString a_FileName)
{
	m_Stream = new std::ofstream(a_FileName.c_str(), std::ofstream::out);
}

CFileLogger::~CFileLogger()
{
	std::ofstream* FileStream = dynamic_cast<std::ofstream*>(m_Stream);
	FileStream->close();
	delete m_Stream;
}

void CFileLogger::Log(const CLogVerbosity& a_Verbosity, const SString& a_LogCategory, SString& a_Message)
{
	std::time_t Time = std::time(0);
	std::tm now;
	localtime_s(&now, &Time);

	*m_Stream <<
		"[" << now.tm_mday << "-" << now.tm_mon + 1 << "-" << 1900 + now.tm_year << " "
		<< now.tm_hour << ":" << now.tm_min << ":" << now.tm_sec << "]["
		<< a_Verbosity.GetVerbosityString() << "][" << a_LogCategory << "] " << a_Message;
}

CStreamLogger::CStreamLogger(std::ostream* a_Stream)
{
	m_Stream = a_Stream;
}

CStreamLogger::~CStreamLogger()
{
}

void CStreamLogger::Log(const CLogVerbosity& a_Verbosity, const SString& a_LogCategory, SString& a_Message)
{
	std::time_t Time = std::time(0);
	std::tm now;
	localtime_s(&now, &Time);

	*m_Stream <<
		"[" << now.tm_mday << "-" << now.tm_mon + 1 << "-" << 1900 + now.tm_year << " "
		<< now.tm_hour << ":" << now.tm_min << ":" << now.tm_sec << "]["
		<< a_Verbosity.GetVerbosityString() << "][" << a_LogCategory << "] " << a_Message;
}

CCompositeLogger::CCompositeLogger()
{
	// "Trying to create a CCompositeLogger object, but it already exists on the stack.\nPlease use CCompositeLogger::Get or one of the logging macros"
	assert(sm_CompositeLogger == nullptr);
	sm_CompositeLogger = this;
}

CCompositeLogger* CCompositeLogger::Get()
{
	return sm_CompositeLogger;
}

void CCompositeLogger::Log(const CLogVerbosity& a_Verbosity, const SString& a_LogCategory, SString& a_Message)
{
	for (auto& Logger : m_Loggers)
	{
		Logger->Log(a_Verbosity, a_LogCategory, a_Message);
	}
}

void CCompositeLogger::RegisterLogger(ILogger* a_Logger)
{
	m_Loggers.Add(Fade::MakeUnique<ILogger>(a_Logger));
}

}