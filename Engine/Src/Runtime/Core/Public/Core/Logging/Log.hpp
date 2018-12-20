#pragma once

#include <Core/definitions.hpp>

namespace Fade {

enum class ELogVerbosity : u8 
{
	Info,
	Warning,
	Error
};

/**
 * Logger class,
 * Provides us with functionality to log information and save it
 * 
 * This class should:
 * - Save the logs in a fixed size fixed buffer array
 * - If available print to console
 * - If chosen print to text file
 */
class CLogger
{
	/**
	* The buffer is where the log is saved, it has a fixed size.
	* this size is decided by the max number of lines times the size of a line
	*
	* There is two options I can think of right now for 'efficiency'
	* Either the line size is not used and I just place newlines to simulate a new line
	* This approach is memory 'efficient' but makes it harder/less efficient to get rid of lines
	* or
	* I use the line size, which allows me to get rid of older messages by 
	* overwriting them with new ones. The initial index of the array will then 
	* shift to the next line and we then just loop. 
	* This approach wastes memory but makes getting rid of older messages far easier
	* and more efficient.
	* 
	*/
	char* m_Buffer;

public:
	/**
	 * The constructor creates the buffer and opens the log file for writing
	 */
	explicit CLogger(u32 a_NumLines, u32 a_LineWidth);

	// Variadic templates!
	template <typename... Args>
	void LogMessage(ELogVerbosity a_Verbosity, cstr a_LogCategory, cstr a_Message, Args... a_Arguments)
	{
		
	}

private:
	template <typename LastArg>
	cstr* UnpackParams(LastArg a_Argument)
	{
	}

	template <typename Arg, typename... OtherArgs>
	cstr* UnpackParams(Arg a_FirstArg, OtherArgs... a_Arguments)
	{
	}
};

extern CLogger* g_Logger;

}