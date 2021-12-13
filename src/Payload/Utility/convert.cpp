#include "convert.h"

std::string convert::CHAR_ARRAY_TO_HEXSTR(char* data, int len)
{
	constexpr char hexmap[] = { '0', '1', '2', '3', '4', '5', '6', '7',
						'8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };

	std::string s(len * 2, ' ');
	for (int i = 0; i < len; ++i) {
		s[2 * i] = hexmap[(data[i] & 0xF0) >> 4];
		s[2 * i + 1] = hexmap[data[i] & 0x0F];
	}
	return s;
}

// Converts a '2DF0' to '00002DF0' to ensure we send the exact amount of packages.
std::string convert::COMPLETE_HEX_STRUCT(std::string incompleteHexStruct)
{
	std::string completeHexStructure = "00000000"; // 4 bytes hardcoded currently

	int precedingZeros = completeHexStructure.length() - incompleteHexStruct.length();
	for (int i = 0; i < 8; i++)
	{
		if (i >= precedingZeros)
		{
			completeHexStructure[i] = incompleteHexStruct[i - precedingZeros];
		}
	}

	return completeHexStructure;
}

std::string convert::ULINT_TO_HEXSTR(unsigned long uint)
{
	std::stringstream sstream;
	sstream << std::hex << uint;
	std::string tempStr = sstream.str();

	// map the string into the correct format, exactly 8 characters. 
	std::string ret = COMPLETE_HEX_STRUCT(tempStr);

	return ret;
}

std::string convert::INT_TO_HEXSTRING(int i)
{
	std::stringstream ss;
	ss << std::hex << i; // int decimal_value
	std::string res(ss.str());

	return res;
}

std::string convert::PCWSTR_TO_STRING(PCWSTR string) {

	std::setlocale(LC_ALL, "");
	const std::wstring ws(string);
	const std::locale locale("");
	typedef std::codecvt<wchar_t, char, std::mbstate_t> converter_type;
	const converter_type& converter = std::use_facet<converter_type>(locale);
	std::vector<char> to(ws.length() * converter.max_length());
	std::mbstate_t state;
	const wchar_t* from_next;
	char* to_next;
	const converter_type::result result = converter.out(state, ws.data(), ws.data() + ws.length(), from_next, &to[0], &to[0] + to.size(), to_next);

	if (result == converter_type::ok or result == converter_type::noconv) {
		const std::string str(&to[0], to_next);
		return str;
	}

	throw "PCWSTR_TO_STRING convertion error";
}

std::string convert::PBYTE_TO_HEXSTR(PBYTE data, int len)
{
	std::stringstream ss;
	ss << std::hex;

	for (int i(0); i < len; ++i)
		ss << std::setw(2) << std::setfill('0') << (int)data[i];

	return ss.str();
}

std::string convert::WSTRING_TO_STRING(std::wstring internal)
{
	auto& f = std::use_facet<std::codecvt<wchar_t, char, std::mbstate_t>>(std::locale());

	std::mbstate_t mb{}; // initial shift state
	std::string external(internal.size() * f.max_length(), '\0');
	const wchar_t* from_next;
	char* to_next;
	f.out(mb, &internal[0], &internal[internal.size()], from_next,
		&external[0], &external[external.size()], to_next);
	// error checking skipped for brevity
	external.resize(to_next - &external[0]);

	return external;
}

char* convert::PWCHAR_TO_PCHAR(wchar_t* string)
{
	size_t len = wcslen(string) + 1;
	char* c_string = new char[len];
	size_t numCharsRead;
	wcstombs_s(&numCharsRead, c_string, len, string, _TRUNCATE);
	return c_string;
}