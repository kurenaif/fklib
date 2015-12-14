#pragma once

#include <string>
#include <sstream>
#include <Windows.h>

namespace fk
{
	typedef std::basic_string<TCHAR> tstring;
	typedef std::basic_stringstream<TCHAR> tstringstream;

	class Tstr
	{
	private:
	tstring s;
		
	public:
		Tstr() {}
		~Tstr() {}

		template< class T >
		Tstr(const T &src)
		{
			tstringstream ss;
			ss << src;
			s = ss.str();
		}

		Tstr &operator << (const Tstr &src)
		{
			s += src.s;
			return *this;
		}

		template< class T >
		Tstr &operator << (const T &src)
		{
			tstringstream ss;
			ss << src;
			s += ss.str();
			return *this;
		}

		operator const TCHAR* () {return s.c_str();}

		operator tstring() {return s;}

		Tstr &operator += (const Tstr &src)
		{
			s += src.s;
			return *this;
		}

		Tstr operator + (const Tstr &src) const { return Tstr(s+src.s);}

		const tstring tstr() {return s;}
	};

	class Str
	{
	private:
	std::string s;
		
	public:
		Str() {}
		~Str() {}

		template< class T >
		Str(const T &src)
		{
			std::stringstream ss;
			ss << src;
			s = ss.str();
		}

		Str &operator << (const Str &src)
		{
			s += src.s;
			return *this;
		}

		template< class T >
		Str &operator << (const T &src)
		{
			std::stringstream ss;
			ss << src;
			s += ss.str();
			return *this;
		}

		operator const char* () {return s.c_str();}

		operator std::string() {return s;}

		Str &operator += (const Str &src)
		{
			s += src.s;
			return *this;
		}

		Str operator + (const Str &src) const { return Str(s+src.s);}

		const std::string str() {return s;}
	};
};
