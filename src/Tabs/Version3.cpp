#include "DEMO.h"

void DemoState::DrawVers3Tab()
{
	if (!FUCK::BeginTabItem("$DEMO_TabVers3"_T)) {
		return;
	}

	FUCK::Header("$DEMO_HotkeyAPI"_T);
	FUCK::Dummy(ImVec2(0.0f, FUCK::Scale(4.0f)));
	if (FUCK::Checkbox("$DEMO_DisableHotkey"_T, &_disableHotkey, false, false)) {
		// Communicate the state to the Host framework
		FUCK::SetHotkeyEnabled(!_disableHotkey);
	}
	FUCK::SetTooltip("$DEMO_DisableHotkeyTT"_T);

	FUCK::Separator();
	FUCK::Header("$DEMO_TreeNodeEx"_T);
	FUCK::Dummy(ImVec2(0.0f, FUCK::Scale(4.0f)));

	static bool optSelected    = false;
	static bool optLeaf        = false;
	static bool optDefaultOpen = true;
	static bool optBullet      = false;

	FUCK::Checkbox("ImGuiTreeNodeFlags_Selected", &optSelected, false, false);
	FUCK::SameLine();
	FUCK::Checkbox("ImGuiTreeNodeFlags_Leaf", &optLeaf, false, false);

	FUCK::Checkbox("ImGuiTreeNodeFlags_DefaultOpen", &optDefaultOpen, false, false);
	FUCK::SameLine();

	FUCK::BeginDisabled(!optLeaf);
	FUCK::Checkbox("ImGuiTreeNodeFlags_Bullet", &optBullet, false, false);
	FUCK::EndDisabled();

	// Automatically uncheck the bullet flag if leaf is disabled
	if (!optLeaf) {
		optBullet = false;
	}

	FUCK::Spacing();

	int flags = 0;
	if (optSelected)
		flags |= ImGuiTreeNodeFlags_Selected;
	if (optLeaf)
		flags |= ImGuiTreeNodeFlags_Leaf;
	if (optDefaultOpen)
		flags |= ImGuiTreeNodeFlags_DefaultOpen;
	if (optBullet)
		flags |= ImGuiTreeNodeFlags_Bullet;

	if (FUCK::TreeNode("$DEMO_DynamicNode"_T, flags)) {
		FUCK::Text("$DEMO_NodeOpen"_T);
		// Important: If Leaf or NoTreePushOnOpen is set, you don't call Pop
		if (!optLeaf) {
			FUCK::TreePop();
		}
	}

	FUCK::Separator();
	FUCK::Header("$DEMO_Popups"_T);
	FUCK::Dummy(ImVec2(0.0f, FUCK::Scale(4.0f)));

	if (FUCK::Button("$DEMO_OpenNormal"_T)) {
		FUCK::OpenPopup("NormalPopup");
	}
	FUCK::SameLine();
	if (FUCK::Button("$DEMO_OpenModal"_T)) {
		FUCK::OpenPopup("ModalPopup");
	}

	if (FUCK::BeginPopup("NormalPopup")) {
		FUCK::Text("$DEMO_NormalPopup"_T);
		if (FUCK::IsWindowAppearing()) {
			FUCK::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "$DEMO_JustAppeared"_T);
		}
		if (FUCK::Button("$DEMO_Close"_T)) {
			FUCK::CloseCurrentPopup();
		}
		FUCK::EndPopup();
	}

	// Modal popups steal input focus until closed
	if (FUCK::BeginPopupModal("ModalPopup")) {
		FUCK::Text("$DEMO_ModalPopup"_T);
		FUCK::Spacing();
		if (FUCK::Button("$DEMO_Close"_T)) {
			FUCK::CloseCurrentPopup();
		}
		FUCK::EndPopup();
	}

	FUCK::Separator();
	FUCK::Header("$DEMO_TextWrap"_T);
	FUCK::Dummy(ImVec2(0.0f, FUCK::Scale(4.0f)));

	FUCK::PushTextWrapPos(FUCK::Scale(300.0f));
	FUCK::TextColored(ImVec4(1.0f, 0.5f, 0.8f, 1.0f), "$DEMO_TextWrapDesc"_T);
	FUCK::PopTextWrapPos();

	FUCK::Separator();
	FUCK::Header("$DEMO_NavCursor"_T);
	FUCK::Dummy(ImVec2(0.0f, FUCK::Scale(4.0f)));

	if (FUCK::Button("$DEMO_FocusInput"_T)) {
		FUCK::SetKeyboardFocusHere(0);
	}
	FUCK::SameLine();
	static char buf[32] = "";
	FUCK::InputText("##focusTest", buf, 32);

	// Spacer to prevent the input field from overlapping with the Nav toggle button
	FUCK::Dummy(ImVec2(0.0f, FUCK::Scale(4.0f)));

	static bool navVisible = false;
	if (FUCK::Button("$DEMO_ToggleNav"_T)) {
		navVisible = !navVisible;
		FUCK::SetNavCursorVisible(navVisible);
	}

	FUCK::Separator();
	FUCK::Header("$DEMO_Shapes"_T);
	FUCK::Dummy(ImVec2(0.0f, FUCK::Scale(4.0f)));

	ImVec2 p  = FUCK::GetCursorScreenPos();
	float  sz = FUCK::Scale(40.0f);
	float  sp = FUCK::Scale(15.0f);

	// Local window drawings (respect scrolling & clipping)
	FUCK::DrawCircle(ImVec2(p.x + sz / 2, p.y + sz / 2), sz / 2, ImVec4(1.0f, 0.2f, 0.2f, 1.0f), 0, 2.0f);
	FUCK::DrawCircleFilled(ImVec2(p.x + sz * 1.5f + sp, p.y + sz / 2), sz / 2, ImVec4(1.0f, 0.2f, 0.2f, 1.0f), 0);

	FUCK::DrawTriangle(ImVec2(p.x + sz * 2 + sp * 2, p.y + sz), ImVec2(p.x + sz * 2.5f + sp * 2, p.y), ImVec2(p.x + sz * 3 + sp * 2, p.y + sz), ImVec4(0.2f, 1.0f, 0.2f, 1.0f), 2.0f);
	FUCK::DrawTriangleFilled(ImVec2(p.x + sz * 3 + sp * 3, p.y + sz), ImVec2(p.x + sz * 3.5f + sp * 3, p.y), ImVec2(p.x + sz * 4 + sp * 3, p.y + sz), ImVec4(0.2f, 1.0f, 0.2f, 1.0f));

	FUCK::DrawQuad(ImVec2(p.x + sz * 4 + sp * 4, p.y), ImVec2(p.x + sz * 5 + sp * 4, p.y + sp), ImVec2(p.x + sz * 4.8f + sp * 4, p.y + sz), ImVec2(p.x + sz * 4.2f + sp * 4, p.y + sz - sp), ImVec4(0.2f, 0.5f, 1.0f, 1.0f), 2.0f);
	FUCK::DrawQuadFilled(ImVec2(p.x + sz * 5 + sp * 5, p.y), ImVec2(p.x + sz * 6 + sp * 5, p.y + sp), ImVec2(p.x + sz * 5.8f + sp * 5, p.y + sz), ImVec2(p.x + sz * 5.2f + sp * 5, p.y + sz - sp), ImVec4(0.2f, 0.5f, 1.0f, 1.0f));

	// Create a dummy bounding box over the shapes we just drew
	FUCK::Dummy(ImVec2(sz * 6 + sp * 5, sz));

	// Screen drawing test
	if (FUCK::IsItemHovered()) {
		FUCK::SetTooltip("$DEMO_HoverShapes"_T);

		ImVec2 mp = FUCK::GetMousePos();
		// Draw over EVERYTHING directly to the background layer tracking the mouse
		FUCK::DrawScreenCircle(mp, FUCK::Scale(20.0f), IM_COL32(255, 0, 255, 255), 0, 3.0f);
	}

	FUCK::EndTabItem();
}
