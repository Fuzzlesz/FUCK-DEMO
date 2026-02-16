#include "DEMO.h"
#include "System/IconsFontAwesome6.h"

void DemoState::DrawIconsTab()
{
	if (!FUCK::BeginTabItem("$DEMO_TabIcons"_T)) {
		return;
	}

	FUCK::Header("$DEMO_IconGalleryTitle"_T);

	FUCK::Columns(4, "IconGrid", false);

	// Row 1
	FUCK::Text(ICON_FA_HEART " %s", "$DEMO_Icon_Heart"_T);
	FUCK::NextColumn();
	FUCK::Text(ICON_FA_STAR " %s", "$DEMO_Icon_Star"_T);
	FUCK::NextColumn();
	FUCK::Text(ICON_FA_USER " %s", "$DEMO_Icon_User"_T);
	FUCK::NextColumn();
	FUCK::Text(ICON_FA_GEAR " %s", "$DEMO_Icon_Gear"_T);
	FUCK::NextColumn();

	// Row 2
	FUCK::Text(ICON_FA_TRASH " %s", "$DEMO_Icon_Trash"_T);
	FUCK::NextColumn();
	FUCK::Text(ICON_FA_PEN " %s", "$DEMO_Icon_Pen"_T);
	FUCK::NextColumn();
	FUCK::Text(ICON_FA_CHECK " %s", "$DEMO_Icon_Check"_T);
	FUCK::NextColumn();
	FUCK::Text(ICON_FA_XMARK " %s", "$DEMO_Icon_XMark"_T);
	FUCK::NextColumn();

	// Row 3
	FUCK::Text(ICON_FA_CIRCLE_PLUS " %s", "$DEMO_Icon_Plus"_T);
	FUCK::NextColumn();
	FUCK::Text(ICON_FA_CIRCLE_MINUS " %s", "$DEMO_Icon_Minus"_T);
	FUCK::NextColumn();
	FUCK::Text(ICON_FA_MAGNIFYING_GLASS " %s", "$DEMO_Icon_Search"_T);
	FUCK::NextColumn();
	FUCK::Text(ICON_FA_HOUSE " %s", "$DEMO_Icon_Home"_T);
	FUCK::NextColumn();

	// Row 4
	FUCK::Text(ICON_FA_FLOPPY_DISK " %s", "$DEMO_Icon_Save"_T);
	FUCK::NextColumn();
	FUCK::Text(ICON_FA_FOLDER " %s", "$DEMO_Icon_Folder"_T);
	FUCK::NextColumn();
	FUCK::Text(ICON_FA_FILE " %s", "$DEMO_Icon_File"_T);
	FUCK::NextColumn();
	FUCK::Text(ICON_FA_DOWNLOAD " %s", "$DEMO_Icon_Download"_T);
	FUCK::NextColumn();

	// Row 5
	FUCK::Text(ICON_FA_UPLOAD " %s", "$DEMO_Icon_Upload"_T);
	FUCK::NextColumn();
	FUCK::Text(ICON_FA_PLAY " %s", "$DEMO_Icon_Play"_T);
	FUCK::NextColumn();
	FUCK::Text(ICON_FA_PAUSE " %s", "$DEMO_Icon_Pause"_T);
	FUCK::NextColumn();
	FUCK::Text(ICON_FA_STOP " %s", "$DEMO_Icon_Stop"_T);
	FUCK::NextColumn();

	FUCK::Columns(1);

	FUCK::EndTabItem();
}
