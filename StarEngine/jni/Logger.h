#pragma once

#include "defines.h"

namespace star 
{
	enum LogLevel : byte
	{
		Info, 
		Warning, 
		Error, 
		Debug
	};

    class Logger {
    public:
		~Logger();
		static Logger * GetInstance();
		static void ResetSingleton();

#ifdef _WIN32
		void Initialize(bool useConsole);
#else
		void Initialize();
#endif
		void Log(LogLevel level, const tstring& pMessage, const tstring& tag = ANDROID_LOG_TAG) const;
		void _CheckGlError(const char* file, int line);
		#define CheckGlError() _CheckGlError(__FILE__,__LINE__);
	private:
		Logger(void);
		static Logger * m_LoggerPtr;

		#ifdef _WIN32
		bool m_UseConsole;
		HANDLE m_ConsoleHandle;
		#endif

		Logger(const Logger& t);
		Logger(Logger&& t);
		Logger& operator=(const Logger& t);
    };
}
