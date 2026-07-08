#include "DEMO.h"

void DemoState::DrawVers2Tab()
{
	if (!FUCK::BeginTabItem("$DEMO_TabVers2"_T)) {
		return;
	}

	FUCK::Header("$DEMO_Multiline"_T);
	FUCK::Dummy(ImVec2(0.0f, FUCK::Scale(4.0f)));
	FUCK::InputTextMultiline("##Multiline", &_multilineBuffer, ImVec2(0, FUCK::Scale(100.0f)));

	FUCK::Separator();
	FUCK::Header("$DEMO_ItemWidth"_T);
	FUCK::Dummy(ImVec2(0.0f, FUCK::Scale(4.0f)));

	FUCK::PushItemWidth(FUCK::Scale(150.0f));
	static float v1 = 0.5f, v2 = 0.5f;
	FUCK::SliderFloat("##v1", &v1, 0.0f, 1.0f);
	FUCK::SameLine();
	FUCK::SeparatorVertical();
	FUCK::SameLine();
	FUCK::SliderFloat("##v2", &v2, 0.0f, 1.0f);
	FUCK::PopItemWidth();

	FUCK::Separator();
	FUCK::Header("$DEMO_Tooltips"_T);
	FUCK::Dummy(ImVec2(0.0f, FUCK::Scale(4.0f)));

	FUCK::Button("$DEMO_HoverMe"_T);
	if (FUCK::IsItemHovered() && FUCK::BeginTooltip()) {
		FUCK::TextColored(ImVec4(1.0f, 0.85f, 0.2f, 1.0f), "$DEMO_TooltipText"_T);
		FUCK::Spacing();
		FUCK::Spinner("##tpspin", 12.0f, 3.0f, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
		FUCK::EndTooltip();
	}

	FUCK::Separator();
	FUCK::Header("$DEMO_Scroll"_T);
	FUCK::Dummy(ImVec2(0.0f, FUCK::Scale(4.0f)));

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

	FUCK::EndTabItem();
}
