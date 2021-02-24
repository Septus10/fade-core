#pragma once

#include <Core/CoreApi.hpp>
#include <Core/Definitions.hpp>
#include <Core/Containers/Array.hpp>
#include <Core/Containers/List.hpp>
#include <Core/Containers/String.hpp>
#include <Core/Containers/UniquePointer.hpp>

#include <ostream>
#include <sstream>

// https://stackoverflow.com/questions/48561227/c-logging-to-console-and-log-file-simultaneously

namespace Fade {

enum class ELogVerbosity : u8
{
	Warning,
	Error,
	Info,
	NUM
};

class CLogVerbosity
{
	SString m_VerbosityString;
	ELogVerbosity m_Verbosity;

public:
	CLogVerbosity(SString a_String, ELogVerbosity a_Verbosity)
		: m_VerbosityString(a_String)
		, m_Verbosity(a_Verbosity)
	{ }

	const SString& GetVerbosityString() const
	{
		return m_VerbosityString;
	}

	ELogVerbosity GetVerbosity() const
	{
		return m_Verbosity;
	}
};

FADE_CORE_API extern const CLogVerbosity Warning;
FADE_CORE_API extern const CLogVerbosity Error;
FADE_CORE_API extern const CLogVerbosity Info;

/**
 * Logger class,
 * Provides us with functionality to log information and save it
 */
class FADE_CORE_API IBaseLogger
{
public:
	virtual ~IBaseLogger() = default;
	virtual void Log(const CLogVerbosity& a_Verbosity, const SString& a_LogCategory, SString& a_Message) = 0;
};

class FADE_CORE_API ILogger : public IBaseLogger
{
public:
	virtual ~ILogger() = default;
	virtual void Log(const CLogVerbosity& a_Verbosity, const SString& a_LogCategory, SString& a_Message) = 0;
	std::ostream* m_Stream;
};

class FADE_CORE_API CFileLogger : public ILogger
{
public:
	CFileLogger(SString a_FileName);
	virtual ~CFileLogger() override;

	virtual void Log(const CLogVerbosity& a_Verbosity, const SString& a_LogCategory, SString& a_Message) override;
};

class FADE_CORE_API CStreamLogger : public ILogger
{
public:
	CStreamLogger(std::ostream* a_Stream);
	~CStreamLogger();

	virtual void Log(const CLogVerbosity& a_Verbosity, const SString& a_LogCategory, SString& a_Message) override;
};

class CCompositeLogger : IBaseLogger
{
	FADE_CORE_API static CCompositeLogger* sm_CompositeLogger;
	TArray<TUniquePtr<ILogger>> m_Loggers;

	// Templated print to stream function
	// This one has only one parameter meaning that it's either the last or only parameter in the variadic template
	// Because of this we append a newline
	template <typename T>
	bool PrintToStream(std::ostream& a_Out, T&& a_Value)
	{
		return!!(a_Out << std::forward<T>(a_Value) << "\n");
	}

	// Templated print to stream function
	// This contains more than 2 templated parameters that we want to print
	template <typename First, typename... Rest>
	bool PrintToStream(std::ostream& a_Out, First&& a_First, Rest&&... a_Rest)
	{
		return !!(a_Out << std::forward<First>(a_First)) && PrintToStream(a_Out, std::forward<Rest>(a_Rest)...);
	}

public:
	/**
	 * Constructor
	 * 
	 * Creates the static pointer to this class so we can get it everywhere.
	 */
	FADE_CORE_API CCompositeLogger();

	/**
	 * Returns a pointer to the 
	 */
	static FADE_CORE_API CCompositeLogger* Get();

	/**
	 * 
	 */
	template <typename... Args>
	void Log(const CLogVerbosity& a_Verbosity, const SString& a_LogCategory, Args&&... a_Arguments)
	{
		SString LogString;
		std::stringstream StringStream;
		PrintToStream(StringStream, std::forward<Args>(a_Arguments)...);
		LogString = StringStream.str();
		// Message is constructed, send it!
		Log(a_Verbosity, a_LogCategory, LogString);
	}

	/**
	 * 
	 */
	void FADE_CORE_API Log(const CLogVerbosity& a_Verbosity, const SString& a_LogCategory, SString& a_Message) override;

	/**
	 * 
	 */
	void FADE_CORE_API RegisterLogger(ILogger* a_Logger);
};

}

#define FADE_LOG(LogVerbosity, LogCategory, ...) Fade::CCompositeLogger::Get()->Log(Fade::LogVerbosity, #LogCategory, __VA_ARGS__)