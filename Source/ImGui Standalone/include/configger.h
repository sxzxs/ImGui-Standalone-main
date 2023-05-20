#ifndef __CONFIGGER_H__
#define  __CONFIGGER_H__
#include "json.hpp"
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <codecvt>
#include <spd_logger.h>
using namespace nlohmann;
using namespace std;

static std::string wstr_utf8(const std::wstring& str)
{
    std::wstring_convert<std::codecvt_utf8<wchar_t>> myconv;
    return myconv.to_bytes(str);
}
static std::wstring utf8_wstr(const std::string& str)
{
    std::wstring_convert<std::codecvt_utf8<wchar_t>> myconv;
    return myconv.from_bytes(str);
}

class Configger
{
public:
	static Configger* instance()
	{
		static Configger configger;
		return &configger;
	}
	void set_config_path(std::string path)
	{
		m_config_path = path;
	}
	bool load()
	{
		fstream myFile;
		myFile.open(utf8_wstr(m_config_path));
		myFile >> m_cfg;
		myFile.close();
		return true;
	}
	bool save()
	{
		bool rtn = true;
		std::ofstream o(utf8_wstr(m_config_path));
		o << m_cfg << std::endl;
		o.close();
		return rtn;
	}
	json &config()
	{
		return m_cfg;
	}
private:
	Configger(){}
	~Configger(){}
	json m_cfg;
	std::string m_config_path;
};


#endif // !__CONFIGGER_H__
