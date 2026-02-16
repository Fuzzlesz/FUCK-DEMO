#include "DEMO.h"

void DemoState::DrawGameControlTab()
{
	if (!FUCK::BeginTabItem("$DEMO_TabGameControl"_T)) {
		return;
	}

	FUCK::Header("$DEMO_Section_OverlayWindow"_T);
	if (FUCK::Button("$DEMO_ToggleOverlay"_T)) {
		_overlay._isOpen = !_overlay._isOpen;
	}
	FUCK::TextWrapped("$DEMO_OverlayDesc"_T);

	FUCK::Spacing();
	FUCK::Separator();
	FUCK::Spacing();

	FUCK::Header("$DEMO_Section_Utils"_T);

	FUCK::LeftLabel("$DEMO_RawPath"_T);
	FUCK::SetNextItemWidth(-1);
	FUCK::InputText("##RawPathInput", _pathBuffer, sizeof(_pathBuffer));

	if (FUCK::Button("$DEMO_Sanitize"_T)) {
		FUCK::SanitizePath(_sanitizedBuffer, _pathBuffer, sizeof(_sanitizedBuffer));
	}

	if (_sanitizedBuffer[0] != '\0') {
		FUCK::TextColored(ImVec4(0, 1, 0, 1), "$DEMO_Sanitized"_T, _sanitizedBuffer);
	}

	FUCK::Spacing();
	FUCK::Separator();

	FUCK::Header("$DEMO_Section_Translations"_T);
	FUCK::Text("$DEMO_TabGeneral"_T);

	FUCK::EndTabItem();
}
