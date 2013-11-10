#include "Logger.h"

#ifdef _WIN32
#include <stdio.h>
#include "Console.h"
#elif defined(ANDROID)
#include <android/log.h>
#endif
#ifdef DESKTOP
#include <glew.h>
#else
#include <GLES2/gl2.h>
#endif

namespace star {
	Logger * Logger::m_LoggerPtr = nullptr;

	Logger::Logger()
#ifdef _WIN32
		:m_ConsoleHandle(nullptr)
		,m_UseConsole(false)
#endif
	{
	}
	
	Logger::~Logger()
	{
#ifdef _WIN32
		CloseHandle(m_ConsoleHandle);
#endif
	}

	Logger * Logger::GetInstance()
	{
		if(m_LoggerPtr == nullptr)
		{
			m_LoggerPtr = new Logger();
		}
		return m_LoggerPtr;
	}

	void Logger::ResetSingleton()
	{
		//Delete resources.
	}

#ifdef _WIN32
	void Logger::Initialize(bool useConsole)
	{
		m_UseConsole = useConsole;
		if(useConsole)
		{
			WindowsConsole::RedirectIOToConsole();
			m_ConsoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
		}
	}
#else
	void Logger::Initialize()
	{
	}
#endif

	void Logger::Log(LogLevel level, const tstring& pMessage, const tstring& tag) const
	{
#if LOGGER_MIN_LEVEL > 0
		#ifdef DESKTOP
		tstring levelName;
		switch(level)
		{
		case LogLevel::Info :
			levelName = _T("INFO");
			break;
		case LogLevel::Warning:
			levelName = _T("WARNING");
			break;
		case LogLevel::Error:
			levelName = _T("ERROR");
			break;
		case LogLevel::Debug:
			levelName = _T("DEBUG");
			break;
		}

		tstringstream messageBuffer;
		messageBuffer << _T("[") << tag << _T("] ") << _T("[") << levelName <<  _T("] ") << pMessage << std::endl;
		tstring combinedMessage = messageBuffer.str();
		
		if(m_UseConsole)
		{
			switch(level)
			{
			case LogLevel::Info :
				#if LOGGER_MIN_LEVEL < 2
				SetConsoleTextAttribute(m_ConsoleHandle, FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
				#endif
				break;
			case LogLevel::Warning :
				#if LOGGER_MIN_LEVEL < 3
				SetConsoleTextAttribute(m_ConsoleHandle, FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
				#endif
				break;
			case LogLevel::Error :
				#if LOGGER_MIN_LEVEL < 4
				SetConsoleTextAttribute(m_ConsoleHandle, FOREGROUND_INTENSITY | FOREGROUND_RED);
				#endif
				break;
			case LogLevel::Debug :
				#if LOGGER_MIN_LEVEL < 5
				#ifdef DEBUG
				SetConsoleTextAttribute(m_ConsoleHandle, FOREGROUND_INTENSITY | FOREGROUND_GREEN);
				#endif
				#endif
				break;
			}
			tprintf(combinedMessage.c_str());
		}
		else
		{
			OutputDebugString(combinedMessage.c_str());
		}
		#else
		switch(level)
		{
		case LogLevel::Info:
			#if LOGGER_MIN_LEVEL < 2
			__android_log_print(ANDROID_LOG_INFO, tag.c_str(), "%s", pMessage.c_str());
			#endif
			break;
		case LogLevel::Warning:
			#if LOGGER_MIN_LEVEL < 3
			__android_log_print(ANDROID_LOG_WARN, tag.c_str(), "%s", pMessage.c_str());
			#endif
			break;
		case LogLevel::Error:
			#if LOGGER_MIN_LEVEL < 4
			__android_log_print(ANDROID_LOG_ERROR, tag.c_str(), "%s", pMessage.c_str());
			#endif
			break;
		case LogLevel::Debug:
			#if LOGGER_MIN_LEVEL < 5
			#ifdef DEBUG
			__android_log_print(ANDROID_LOG_DEBUG, tag.c_str(), pMessage.c_str());
			#endif
			#endif
			break;
		}
	#endif
#endif
	}

	void Logger::_CheckGlError(const char* file, int line) 
	{
#if LOGGER_MIN_LEVEL > 0
		GLenum err (glGetError());
        while(err!= GL_NO_ERROR) 
		{
            tstring error;
			switch(err) 
			{
                case GL_INVALID_OPERATION:      
					error = _T("INVALID_OPERATION");     
					break;
                case GL_INVALID_ENUM:
					error = _T("INVALID_ENUM");
					break;
                case GL_INVALID_VALUE:
					error = _T("INVALID_VALUE");
					break;
                case GL_OUT_OF_MEMORY:  
					error = _T("OUT_OF_MEMORY"); 
					break;
                case GL_INVALID_FRAMEBUFFER_OPERATION:
					error = _T("INVALID_FRAMEBUFFER_OPERATION");
					break;
				default:
					error =_T("UNKNOWN_ERROR");
					break;
            }
			tstringstream buffer;
            buffer << "GL_" << error << " - " << file << ":" << line << std::endl;
			Logger::GetInstance()->Log(LogLevel::Error,buffer.str(),_T("OPENGL"));
			err = glGetError();
        }
#endif
	}
}
