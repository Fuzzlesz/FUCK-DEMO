#pragma once

enum class FileType
{
	kSettings
};

class Settings
{
public:
	using INIFunc = std::function<void(CSimpleIniA&)>;

	static Settings* GetSingleton()
	{
		return &instance;
	}

	// Removed unused boolean parameter
	void Load(INIFunc a_func) const;
	void Save(INIFunc a_func) const;

private:
	static void LoadINI(const wchar_t* a_path, INIFunc a_func, bool a_generate = false);
	static void LoadINI(const wchar_t* a_defaultPath, const wchar_t* a_userPath, INIFunc a_func);

	const wchar_t* defaultSettingsPath{ L"Data/SKSE/Plugins/FUCK_Demo.ini" };
	const wchar_t* userSettingsPath{ L"Data/SKSE/Plugins/FUCK_Demo_Custom.ini" };

	static Settings instance;
};

inline constinit Settings Settings::instance;
