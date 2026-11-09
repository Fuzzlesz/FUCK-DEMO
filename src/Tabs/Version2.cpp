#include "DEMO.h"

void DemoState::DrawVers2Page()
{
	FUCK::Header("$DEMO_Multiline"_T);
	FUCK::InputTextMultiline("##Multiline", &_multilineBuffer, ImVec2(0, FUCK::Scale(100.0f)));

	FUCK::Separator();
	FUCK::Header("$DEMO_ItemWidth"_T);

	// A width pushed for a run of widgets holds for each of them, so they can share a line. Here a third each, less the gaps between
	static float colour[3] = { 0.8f, 0.5f, 0.2f };
	const float  gap       = FUCK::GetStyleVarVec(ImGuiStyleVar_ItemSpacing).x;
	FUCK::PushItemWidth((FUCK::GetContentRegionAvail().x - gap * 2.0f) / 3.0f);
	FUCK::SliderFloat("##Red", &colour[0], 0.0f, 1.0f, "R %.2f");
	FUCK::SameLine();
	FUCK::SliderFloat("##Green", &colour[1], 0.0f, 1.0f, "G %.2f");
	FUCK::SameLine();
	FUCK::SliderFloat("##Blue", &colour[2], 0.0f, 1.0f, "B %.2f");
	FUCK::PopItemWidth();

	FUCK::Spacing();
	FUCK::TextWrapped("$DEMO_VerticalSeparatorText"_T);
	FUCK::Spacing();

	// Mini Toolbar Demo
	FUCK::Button("Cut");
	FUCK::SameLine();
	FUCK::SeparatorVertical();
	FUCK::SameLine();
	FUCK::Button("Copy");
	FUCK::SameLine();
	FUCK::SeparatorVertical();
	FUCK::SameLine();
	FUCK::Button("Paste");

	FUCK::Separator();
	FUCK::Header("$DEMO_Tooltips"_T);

	FUCK::Button("$DEMO_HoverMeTooltip"_T);
	if (FUCK::IsItemHovered() && FUCK::BeginTooltip()) {
		FUCK::TextColoredWrapped(ImVec4(1.0f, 0.85f, 0.2f, 1.0f), "$DEMO_TooltipText"_T);
		FUCK::Spacing();
		FUCK::Spinner("##tpspin", 12.0f, 3.0f, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
		FUCK::EndTooltip();
	}

	FUCK::Separator();
	FUCK::Header("$DEMO_Scroll"_T);

	static bool scrollNext = false;
	if (FUCK::Button("$DEMO_ScrollTo25"_T)) {
		scrollNext = true;
	}

	FUCK::Spacing();
	FUCK::BeginChild("ScrollDemo", ImVec2(0, FUCK::Scale(180.0f)), true);
	for (int i = 0; i <= 50; i++) {
		if (scrollNext && i == 25) {
			FUCK::SetScrollHereY(0.5f);  // 0.5f centers the item in the window
			scrollNext = false;
		}

		FUCK::Text("$DEMO_ItemX"_T, i, i == 25 ? " <--" : "");
	}
	FUCK::EndChild();
}
