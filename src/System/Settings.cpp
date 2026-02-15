#include "Settings.h"

void Settings::LoadINI(const wchar_t* a_path, const INIFunc a_func, bool a_generate)
{
	CSimpleIniA ini;
	ini.SetUnicode();

	std::filesystem::path p(a_path);
	auto pathStr = p.string();

	if (a_generate) {
		std::filesystem::create_directories(p.parent_path());
	}

	if (ini.LoadFile(pathStr.c_str()) >= 0 || a_generate) {
		a_func(ini);

		if (a_generate) {
			(void)ini.SaveFile(pathStr.c_str());
		}
	}
}

void Settings::LoadINI(const wchar_t* a_defaultPath, const wchar_t* a_userPath, INIFunc a_func)
{
	LoadINI(a_defaultPath, a_func);
	LoadINI(a_userPath, a_func);
}

void Settings::Load(INIFunc a_func) const
{
	if (a_func) {
		LoadINI(defaultSettingsPath, userSettingsPath, a_func);
	}
}

void Settings::Save(INIFunc a_func) const
{
	LoadINI(userSettingsPath, a_func, true);
}
