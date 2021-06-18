#pragma once

#define sLog Log::Get()

#if defined(DEBUG) | defined(_DEBUG)

#ifndef HR
#define HR(x)												\
	{															\
		HRESULT hr = (x);										\
		if(FAILED(hr))											\
		{														\
			sLog->Debug(getErrorStr(hr));						\
		}														\
	}
#define HRFalse(x)												\
	{															\
		HR(x);													\
		HRESULT hr = (x);										\
		if(FAILED(hr))											\
		return false; \
	}
#define HRNull(x)												\
	{															\
		HR(x);												\
		HRESULT hr = (x);										\
		if(FAILED(hr))											\
		return nullptr; \
	}
#endif
#else
#ifndef HR
#define HR(x) (x)
#define HRFalse(x) (x)
#define HRNull(x) (x)
#endif 
#endif

namespace D3D11Framework
{
//------------------------------------------------------------------

	inline const char* getErrorStr(HRESULT hr){
		LPTSTR error_text = NULL;
		FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM |
			FORMAT_MESSAGE_ALLOCATE_BUFFER |
			FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,
			hr,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(LPTSTR)&error_text, 0,
			NULL);

		LPWSTR wideStr = error_text;
		char* buffer = new char[500];

		// First arg is the pointer to destination char, second arg is
		// the pointer to source wchar_t, last arg is the size of char buffer
		wcstombs(buffer, wideStr, 500);

		return buffer;
		}
				
	class Log
	{
	public:
		Log();
		~Log();

		static Log* Get(){return m_instance;}

		void Print(const char *message, ...);
		void Debug(const char *message, ...);
		void Err(const char *message, ...);

	private:
		static Log *m_instance;

		void m_init();
		void m_close();
		void m_print(const char *levtext, const char *text);

		FILE *m_file;	
	};

//------------------------------------------------------------------
}