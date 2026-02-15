#include "DEMO.h"
#include "FUCK_API.h"

// The Global Instance
DemoTool* g_DemoTool = nullptr;

// ==========================================
// SKSE Setup & Logging
// ==========================================

void InitializeLog()
{
	auto path = SKSE::log::log_directory();
	if (!path) {
		return;
	}

	*path /= std::format("{}.log", Version::PROJECT);
	auto sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(path->string(), true);
	auto log = std::make_shared<spdlog::logger>("global log", std::move(sink));

	log->set_level(spdlog::level::info);
	log->flush_on(spdlog::level::info);

	spdlog::set_default_logger(std::move(log));
	spdlog::set_pattern("%v");

	SKSE::log::info("{} v{}", Version::PROJECT, Version::NAME);
}

#ifdef SKYRIM_AE
extern "C" DLLEXPORT constinit auto SKSEPlugin_Version = []() {
	SKSE::PluginVersionData v;
	v.PluginVersion(Version::MAJOR);
	v.PluginName("FUCK-DEMO");
	v.AuthorName("Fuzzles");
	v.UsesAddressLibrary();
	v.UsesUpdatedStructs();
	v.CompatibleVersions({ SKSE::RUNTIME_SSE_LATEST });
	return v;
}();
#else
extern "C" DLLEXPORT bool SKSEAPI SKSEPlugin_Query(const SKSE::QueryInterface* a_skse, SKSE::PluginInfo* a_info)
{
	a_info->infoVersion = SKSE::PluginInfo::kVersion;
	a_info->name = "FUCK-DEMO";
	a_info->version = Version::MAJOR;

	if (a_skse->IsEditor()) {
		return false;
	}
	return true;
}
#endif

extern "C" DLLEXPORT bool SKSEAPI SKSEPlugin_Load(const SKSE::LoadInterface* a_skse)
{
	InitializeLog();
	SKSE::Init(a_skse, false);

	const auto messaging = SKSE::GetMessagingInterface();
	messaging->RegisterListener([](SKSE::MessagingInterface::Message* msg) {
		if (msg->type == SKSE::MessagingInterface::kDataLoaded) {
			
			// Connection logic
			if (FUCK::Connect()) {
				SKSE::log::info("Connected to FUCK API");
				g_DemoTool = new DemoTool();
				FUCK::RegisterTool(g_DemoTool);
				FUCK::LoadTranslation("FUCK-DEMO");
				SKSE::log::info("FUCK-DEMO Registered.");
			}
		}
	});

	return true;
}
