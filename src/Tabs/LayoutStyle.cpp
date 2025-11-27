#include "DEMO.h"

void DemoTool::DrawLayoutStyleTab()
{
	if (!FUCK::BeginTabItem("$DEMO_TabLayoutStyle"_T)) {
		return;
	}

	FUCK::Header("$DEMO_Section_Metrics"_T);
	FUCK::Text("$DEMO_ResScale"_T, FUCK::GetResolutionScale());
	FUCK::Text("$DEMO_DeltaTime"_T, FUCK::GetDeltaTime(), 1.0f / FUCK::GetDeltaTime());

	FUCK::Spacing();
	FUCK::SeparatorThick();
	FUCK::Spacing();

	FUCK::Header("$DEMO_Section_Fonts"_T);
	FUCK::PushFont(FUCK::GetFont(FUCK_Font::kLarge));
	FUCK::Text("$DEMO_LargeFont"_T);
	FUCK::PopFont();
	FUCK::Text("$DEMO_RegularFont"_T);

	FUCK::Spacing();
	FUCK::SeparatorText("$DEMO_Sep_Alignment"_T);

	FUCK::BeginGroup();
	FUCK::Button("$DEMO_BtnA"_T);
	FUCK::SameLine();
	FUCK::Button("$DEMO_BtnB"_T);
	FUCK::SameLine();
	FUCK::Button("$DEMO_BtnC"_T);
	FUCK::EndGroup();
	FUCK::SameLine();
	FUCK::Text("$DEMO_GroupedNote"_T);

	FUCK::Spacing();
	FUCK::Separator();

	FUCK::Header("$DEMO_Section_CursorIndent"_T);
	FUCK::Text("$DEMO_IndentLabel"_T);
	FUCK::Indent();
	FUCK::Text("$DEMO_IndentedText"_T);
	FUCK::Unindent();

	FUCK::Spacing();

	ImVec2 pos = FUCK::GetCursorPos();
	FUCK::Text("$DEMO_CursorPos"_T, pos.x, pos.y);

	ImVec2 winSz = FUCK::GetWindowSize();
	FUCK::Text("$DEMO_MetricSize"_T, winSz.x, winSz.y);

	FUCK::Dummy(ImVec2(0, 20));
	FUCK::Text("$DEMO_AfterDummy"_T);

	FUCK::Spacing();
	FUCK::Separator();

	FUCK::Header("$DEMO_Section_ChildWin"_T);
	if (_useChildWindow) {
		FUCK::BeginChild("DemoChild", ImVec2(300, 150), true);
		FUCK::Text("$DEMO_ChildContent"_T);
		FUCK::EndChild();
	} else {
		FUCK::Checkbox("$DEMO_ShowChild"_T, &_useChildWindow, false, false);
	}

	FUCK::Spacing();
	FUCK::Separator();

	FUCK::Header("$DEMO_Section_Disabled"_T);
	static bool disableWidgets = false;
	FUCK::Checkbox("$DEMO_CheckboxDisable"_T, &disableWidgets, false, false);

	FUCK::BeginDisabled(disableWidgets);
	FUCK::Button("$DEMO_DisableButton"_T);
	FUCK::EndDisabled();

	FUCK::EndTabItem();
}
