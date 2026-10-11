#include "DEMO.h"
#include "API/IconsFontAwesome6.h"

void DemoState::DrawVers6Page()
{
	// --- Colour Pickers & Palettes ---
	FUCK::Header("$DEMO_ColorPalette"_T);
	FUCK::TextWrapped("$DEMO_ColorPickersDesc"_T);
	FUCK::TextWrapped("$DEMO_ColorPaletteDesc"_T);
	FUCK::TextWrapped("$DEMO_DragDropNote"_T);
	FUCK::Spacing();

	// Recents fit as many as the width allows, the fixed grid keeps its shape
	static FUCK::ColorPalette recents(20, FUCK::ColorPalette::Mode::kRecent);

	static FUCK::ColorPalette fixedGrid(20, FUCK::ColorPalette::Mode::kFixed);
	fixedGrid.columns = 10;
	fixedGrid.rows    = 2;

	static float pickedColor[4]    = { 1.0f, 0.0f, 0.0f, 1.0f };
	static float pickedColorRGB[3] = { 0.0f, 1.0f, 0.0f };
	static float inlineColor[3]    = { 0.3f, 0.6f, 0.9f };

	// The picker on the left, the edits and swatches beside it
	if (FUCK::BeginTable("ColourLayout", 2, FUCK::TableFlags::kSizingStretchSame)) {
		FUCK::TableNextColumn();

		// The picker sizes itself from the item width, then draws its colour preview to the right of that,
		// three frames wide. Leave room for it, or the column cuts it off
		const float previewWidth = FUCK::GetFrameHeight() * 3.0f + FUCK::GetStyleVarVec(ImGuiStyleVar_ItemInnerSpacing).x;
		FUCK::SetNextItemWidth(FUCK::GetContentRegionAvail().x - previewWidth);
		FUCK::ColorPicker3("##InlinePicker", inlineColor, FUCK::ColorEditFlags::kPickerHueWheel);

		FUCK::TableNextColumn();

		// Labels above, so the edits get the whole column.
		// Add to the recents once an edit is finished, not on every change while dragging
		FUCK::Text("%s", "$DEMO_PickerRGBA"_T);
		FUCK::ColorEdit4("##RGBA", pickedColor, FUCK::ColorEditFlags::kNone);
		if (FUCK::IsItemDeactivatedAfterEdit())
			recents.Push(pickedColor);

		FUCK::Spacing();
		FUCK::Text("%s", "$DEMO_PickerRGB"_T);
		FUCK::ColorEdit3("##RGB", pickedColorRGB, FUCK::ColorEditFlags::kNone);
		if (FUCK::IsItemDeactivatedAfterEdit())
			recents.Push(pickedColorRGB);

		FUCK::Spacing();
		FUCK::TextDisabled("%s", "$DEMO_RecentColours"_T);
		recents.Draw("##Recents", pickedColor, FUCK::Scale(36.0f));

		FUCK::Spacing();
		FUCK::TextDisabled("%s", "$DEMO_FixedPalette"_T);
		fixedGrid.Draw("##Fixed", pickedColor, FUCK::Scale(30.0f));

		FUCK::EndTable();
	}

	FUCK::Separator();

	// --- HUD Widget ---
	FUCK::Header("$DEMO_Section_HUDWidget"_T);
	FUCK::TextWrapped("$DEMO_HUDWidgetDesc"_T);
	FUCK::Spacing();

	if (FUCK::Button(_hudWidget._isOpen ? "$DEMO_CloseHUDWidget"_T : "$DEMO_OpenHUDWidget"_T)) {
		_hudWidget._wantOpen = !_hudWidget._isOpen;
		_hudWidget.SetOpen(!_hudWidget._isOpen);
	}

	// Session only, so the widget starts plain each time
	if (_hudWidget._isOpen) {
		FUCK::Spacing();
		FUCK::Checkbox("$DEMO_KeepHudOpen"_T, &_cfg.hudKeepOpen, false, false);
		FUCK::SetTooltip("$DEMO_KeepHudOpenTip"_T);
		FUCK::Checkbox("$DEMO_Flag_Scalable"_T, &_cfg.hudScalable, false, false);
		FUCK::SetTooltip("$DEMO_FlagTip_Scalable"_T);
		FUCK::Checkbox("$DEMO_Flag_AnchorRight"_T, &_cfg.hudAnchorRight, false, false);
		FUCK::SetTooltip("$DEMO_FlagTip_AnchorRight"_T);
		FUCK::Checkbox("$DEMO_Flag_Rotatable"_T, &_cfg.hudRotatable, false, false);
		FUCK::SetTooltip("$DEMO_FlagTip_Rotatable"_T);
	}

	FUCK::Separator();

	// --- Sticky Headers ---
	FUCK::Header("$DEMO_StickyHeaders"_T);
	FUCK::TextWrapped("$DEMO_StickyHeadersDesc"_T);
	FUCK::Spacing();

	const char* fruits[] = {
		"$DEMO_Fruit_Apple"_T,      "$DEMO_Fruit_Banana"_T, "$DEMO_Fruit_Cherry"_T, "$DEMO_Fruit_Date"_T,
		"$DEMO_Fruit_Elderberry"_T, "$DEMO_Fruit_Fig"_T,    "$DEMO_Fruit_Grape"_T,  "$DEMO_Fruit_Honeydew"_T
	};
	const char* icons[] = {
		"$DEMO_Icon_Heart"_T,  "$DEMO_Icon_Star"_T,   "$DEMO_Icon_User"_T,   "$DEMO_Icon_Gear"_T,
		"$DEMO_Icon_Trash"_T,  "$DEMO_Icon_Pen"_T,    "$DEMO_Icon_Check"_T,  "$DEMO_Icon_XMark"_T,
		"$DEMO_Icon_Plus"_T,   "$DEMO_Icon_Minus"_T,  "$DEMO_Icon_Search"_T, "$DEMO_Icon_Home"_T,
		"$DEMO_Icon_Save"_T,   "$DEMO_Icon_Folder"_T, "$DEMO_Icon_File"_T,   "$DEMO_Icon_Download"_T,
		"$DEMO_Icon_Upload"_T, "$DEMO_Icon_Play"_T,   "$DEMO_Icon_Pause"_T,  "$DEMO_Icon_Stop"_T
	};
	static const char* const iconGlyphs[] = {
		ICON_FA_HEART,       ICON_FA_STAR,         ICON_FA_USER,             ICON_FA_GEAR,
		ICON_FA_TRASH,       ICON_FA_PEN,          ICON_FA_CHECK,            ICON_FA_XMARK,
		ICON_FA_CIRCLE_PLUS, ICON_FA_CIRCLE_MINUS, ICON_FA_MAGNIFYING_GLASS, ICON_FA_HOUSE,
		ICON_FA_FLOPPY_DISK, ICON_FA_FOLDER,       ICON_FA_FILE,             ICON_FA_DOWNLOAD,
		ICON_FA_UPLOAD,      ICON_FA_PLAY,         ICON_FA_PAUSE,            ICON_FA_STOP
	};
	const char* itemTypes[] = {
		"$DEMO_FT_Weapon"_T,     "$DEMO_FT_Ammo"_T,  "$DEMO_FT_Armor"_T,       "$DEMO_FT_Potion"_T,
		"$DEMO_FT_Ingredient"_T, "$DEMO_FT_Spell"_T, "$DEMO_FT_Enchantment"_T, "$DEMO_FT_Scroll"_T,
		"$DEMO_FT_SoulGem"_T,    "$DEMO_FT_NPC"_T,   "$DEMO_FT_Key"_T
	};
	const char* difficulties[] = {
		"$DEMO_Stepper_Novice"_T, "$DEMO_Stepper_Apprentice"_T, "$DEMO_Stepper_Adept"_T,
		"$DEMO_Stepper_Expert"_T, "$DEMO_Stepper_Master"_T
	};
	const char* overlays[] = {
		"$DEMO_Overlay_Grid"_T,       "$DEMO_Overlay_Crosshair"_T, "$DEMO_Overlay_Spiral"_T,
		"$DEMO_Overlay_GoldenGrid"_T, "$DEMO_Overlay_Triangle"_T
	};
	const char* anchors[] = {
		"$DEMO_Anchor_BR"_T, "$DEMO_Anchor_BL"_T, "$DEMO_Anchor_TL"_T,
		"$DEMO_Anchor_TR"_T, "$DEMO_Anchor_Center"_T
	};

	struct StickySection
	{
		const char*        icon;
		const char*        title;
		const char* const* rows;
		int                count;
		const char* const* rowIcons = nullptr;
	};
	const StickySection sections[] = {
		{ ICON_FA_APPLE_WHOLE, "$DEMO_Sticky_Fruits"_T,     fruits,       IM_ARRAYSIZE(fruits) },
		{ ICON_FA_ICONS,       "$DEMO_TabIcons"_T,          icons,        IM_ARRAYSIZE(icons), iconGlyphs },
		{ ICON_FA_TAGS,        "$DEMO_Col_Type"_T,          itemTypes,    IM_ARRAYSIZE(itemTypes) },
		{ ICON_FA_GAUGE,       "$DEMO_StepperDifficulty"_T, difficulties, IM_ARRAYSIZE(difficulties) },
		{ ICON_FA_BORDER_ALL,  "$DEMO_Section_Overlays"_T,  overlays,     IM_ARRAYSIZE(overlays) },
		{ ICON_FA_ANCHOR,      "$DEMO_SpiralAnchor"_T,      anchors,      IM_ARRAYSIZE(anchors) }
	};
	constexpr int sectionCount = IM_ARRAYSIZE(sections);

	// Optional: keeping the open states in your own bools lets you expand or collapse them all at once
	static bool stickyOpen[sectionCount] = { true, true, true, true, true, true };
	static int  stickySelected           = -1;

	// IconText joins an icon to translated text, for any widget that takes a label
	if (FUCK::Button(FUCK::IconText(ICON_FA_ANGLES_DOWN, "$DEMO_ExpandAll"_T)))
		std::fill(std::begin(stickyOpen), std::end(stickyOpen), true);
	FUCK::SameLine();
	if (FUCK::Button(FUCK::IconText(ICON_FA_ANGLES_UP, "$DEMO_CollapseAll"_T)))
		std::fill(std::begin(stickyOpen), std::end(stickyOpen), false);
	FUCK::Spacing();

	// Rows are indented and their IDs kept apart by the list, so any widgets can go straight in
	FUCK::BeginStickyList("StickyList", ImVec2(0.0f, FUCK::Scale(220.0f)));
	for (int s = 0; s < sectionCount; s++) {
		const auto& sec = sections[s];
		if (FUCK::StickyHeader(FUCK::IconText(sec.icon, sec.title), &stickyOpen[s], sec.count)) {
			for (int i = 0; i < sec.count; i++) {
				const int   id   = s * 100 + i;
				const char* text = sec.rowIcons ? FUCK::IconText(sec.rowIcons[i], sec.rows[i]) : sec.rows[i];
				if (FUCK::Selectable(text, stickySelected == id))
					stickySelected = id;
			}
		}
	}
	FUCK::EndStickyList();
}
