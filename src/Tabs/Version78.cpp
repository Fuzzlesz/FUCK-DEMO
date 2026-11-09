#include "DEMO.h"
#include "API/IconsGameIcons.h"

void DemoState::DrawVers78Page()
{
	// --- Combo With Key Badges ---
	FUCK::Header("$DEMO_KeyBadgeCombo"_T);
	FUCK::TextWrapped("$DEMO_KeyBadgeComboDesc"_T);
	FUCK::Spacing();

	// One key per entry, as GetIconForKey() takes them, and 0 for an entry with none. Here the 1, 2, and 3 keys as DirectInput codes
	static const char*         presets[] = { "$DEMO_Preset_Portrait"_T, "$DEMO_Preset_FullBody"_T, "$DEMO_Preset_OverShoulder"_T, "$DEMO_Preset_Custom"_T };
	static const std::uint32_t keys[]    = { 0x02, 0x03, 0x04, 0 };
	static int                 current   = 0;

	// The row under the cursor or gamepad focus while the list is open, which a plugin would bind a key pressed over it to
	int hovered = -1;
	FUCK::ComboWithKeyBadges("$DEMO_PresetCombo"_T, &current, presets, IM_ARRAYSIZE(presets), keys, &hovered);
	if (hovered >= 0)
		FUCK::TextDisabled("$DEMO_PresetHovered"_T, presets[hovered]);

	FUCK::Separator();

	// --- Font Faces ---
	FUCK::Header("$DEMO_FontFaces"_T);
	FUCK::TextWrapped("$DEMO_FontFacesDesc"_T);
	FUCK::Spacing();

	// Each is the regular font where the typeface has no face of its own, as with the built-in one
	const std::pair<FUCK::Font, const char*> faces[] = {
		{ FUCK::Font::kLight, "$DEMO_FontLight"_T },
		{ FUCK::Font::kMedium, "$DEMO_FontMedium"_T },
		{ FUCK::Font::kBold, "$DEMO_FontBold"_T }
	};
	for (const auto& [font, label] : faces) {
		FUCK::PushFont(FUCK::GetFont(font));
		FUCK::Text("%s", label);
		FUCK::PopFont();
	}

	FUCK::Separator();

	// --- Game Icons ---
	FUCK::Header("$DEMO_GameIconsTitle"_T);
	FUCK::TextWrapped("$DEMO_GameIconsDesc"_T);
	FUCK::Spacing();

	// The game-icons.net set shares its codes with Font Awesome, so its macros are drawn with its own font, which has the same text
	FUCK::PushFont(FUCK::GetFont(FUCK::Font::kGameIcons));

	static constexpr const char* gameIcons[] = {
		ICON_GI_ANCIENT_SWORD, ICON_GI_BOW_ARROW, ICON_GI_AXE_SWORD, ICON_GI_ATTACHED_SHIELD, ICON_GI_BLACK_KNIGHT_HELM, ICON_GI_CHAIN_MAIL,
		ICON_GI_RUNE_SWORD, ICON_GI_SACRIFICIAL_DAGGER, ICON_GI_THOR_HAMMER, ICON_GI_QUIVER, ICON_GI_ORB_WAND, ICON_GI_ROUND_POTION,
		ICON_GI_HEALTH_POTION, ICON_GI_CAULDRON_DARKZAITZEV, ICON_GI_RUNE_STONE, ICON_GI_WHITE_BOOK, ICON_GI_BOOK_PILE, ICON_GI_GOLD_BAR,
		ICON_GI_OPEN_TREASURE_CHEST, ICON_GI_CAMPING_TENT, ICON_GI_CASTLE_RUINS, ICON_GI_CAVE_ENTRANCE, ICON_GI_THRONE_KING, ICON_GI_TELESCOPE
	};
	FUCK::Columns(12, "GameIconGrid", false);
	for (const char* icon : gameIcons) {
		FUCK::Text("%s", icon);
		FUCK::NextColumn();
	}
	FUCK::Columns(1);
	FUCK::PopFont();

	// The same font pushed at twice the size, which suits the more detailed icons. They're drawn afresh at that size, so stay sharp
	static constexpr const char* largeIcons[] = {
		ICON_GI_MEDUSA_HEAD, ICON_GI_ANUBIS, ICON_GI_WITCH_FACE, ICON_GI_HOODED_ASSASSIN, ICON_GI_MOUNTED_KNIGHT, ICON_GI_PEGASUS,
		ICON_GI_GOBLIN, ICON_GI_SPIDER_FACE, ICON_GI_ALL_SEEING_EYE, ICON_GI_CALAVERA, ICON_GI_ORRERY, ICON_GI_TAROT_13_DEATH
	};
	FUCK::PushFontScaled(FUCK::GetFont(FUCK::Font::kGameIcons), 2.0f);
	FUCK::Columns(6, "LargeGameIconGrid", false);
	for (const char* icon : largeIcons) {
		FUCK::Text("%s", icon);
		FUCK::NextColumn();
	}
	FUCK::Columns(1);
	FUCK::PopFont();

	// Buttons and other widgets take them the same way while the font is pushed
	FUCK::Spacing();
	FUCK::PushFont(FUCK::GetFont(FUCK::Font::kGameIcons));
	FUCK::Button(FUCK::IconText(ICON_GI_OPEN_TREASURE_CHEST, "$DEMO_GameIconButton"_T));
	FUCK::PopFont();
}
