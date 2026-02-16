#include "DEMO.h"

void DemoState::DrawAdvancedWidgetsTab()
{
	if (!FUCK::BeginTabItem("$DEMO_TabAdvancedWidgets"_T)) {
		return;
	}

	FUCK::Header("$DEMO_Section_Combos"_T);

	static int simpleCombo = 0;
	const char* simpleItems[] = {
		"$DEMO_Combo_OptA"_T, "$DEMO_Combo_OptB"_T,
		"$DEMO_Combo_OptC"_T, "$DEMO_Combo_OptD"_T
	};
	FUCK::SetNextItemWidth(-1);
	FUCK::Combo("$DEMO_ComboLabel"_T, &simpleCombo, simpleItems, 4);

	static int filterComboItem = 0;
	const char* filterItems[] = {
		"$DEMO_Fruit_Apple"_T, "$DEMO_Fruit_Banana"_T,
		"$DEMO_Fruit_Cherry"_T, "$DEMO_Fruit_Date"_T,
		"$DEMO_Fruit_Elderberry"_T, "$DEMO_Fruit_Fig"_T,
		"$DEMO_Fruit_Grape"_T, "$DEMO_Fruit_Honeydew"_T
	};
	FUCK::SetNextItemWidth(-1);
	FUCK::ComboWithFilter("$DEMO_FilteredCombo"_T, &filterComboItem, filterItems, 8, 5);

	FUCK::Spacing();

	FUCK::Header("$DEMO_Section_FormCombo"_T);

	static std::uint8_t formType = 0;
	static int formTypeCombo = 0;
	const char* formTypes[] = {
		"$DEMO_FormType_All0"_T, "$DEMO_FormType_Wep41"_T,
		"$DEMO_FormType_Arm26"_T, "$DEMO_FormType_Pot46"_T,
		"$DEMO_FormType_Spl21"_T, "$DEMO_FormType_NPC43"_T
	};
	static const std::uint8_t formTypeValues[] = { 0, 41, 26, 46, 21, 43 };

	FUCK::SetNextItemWidth(-1);
	if (FUCK::Combo("$DEMO_FormFilterLabel"_T, &formTypeCombo, formTypes, 6)) {
		formType = formTypeValues[formTypeCombo];
		_selectedFormID = 0;
	}

	FUCK::SetNextItemWidth(-1);
	FUCK::ComboForm("$DEMO_FormCombo_Select"_T, &_selectedFormID, formType);

	if (_selectedFormID != 0) {
		FUCK::Text("$DEMO_FormCombo_Selected"_T, _selectedFormID);
	}

	FUCK::Spacing();
	FUCK::Separator();

	FUCK::Header("$DEMO_Section_Selectable"_T);
	FUCK::TextWrapped("$DEMO_SelectableDesc"_T);
	FUCK::Spacing();

	static int selectedItem = -1;
	for (int i = 0; i < 5; i++) {
		FUCK::PushID(i);
		char label[64];
		snprintf(label, 64, "$DEMO_SelectableItem"_T, i);
		if (FUCK::Selectable(label, selectedItem == i, 0, ImVec2(0, 0))) {
			selectedItem = i;
		}
		FUCK::PopID();
	}

	FUCK::Spacing();
	FUCK::Separator();

	FUCK::Header("$DEMO_Section_IDStack"_T);
	FUCK::TextWrapped("$DEMO_IDStackDesc"_T);
	FUCK::Spacing();

	static bool checkList[3] = { false, false, false };

	for (int i = 0; i < 3; i++) {
		FUCK::PushID(i);
		FUCK::Checkbox("$DEMO_RepeatedLabel"_T, &checkList[i], false, false);
		FUCK::PopID();
	}

	FUCK::Spacing();
	FUCK::Separator();

	FUCK::Header("$DEMO_Section_Stepper"_T);
	FUCK::EnumStepper("$DEMO_StepperDifficulty"_T, &_stepperValue, _stepperOptions);

	FUCK::Spacing();

	FUCK::Header("$DEMO_Section_CollapsingHeaders"_T);
	if (FUCK::CollapsingHeader("$DEMO_CollapsingHeader"_T)) {
		FUCK::Indent();
		FUCK::Text("$DEMO_CollapsingContent1"_T);

		FUCK::Spacing();
		// Demonstrate nested nodes within headers
		if (FUCK::TreeNode("$DEMO_NestedNode"_T)) {
			FUCK::Text("$DEMO_NestedContent"_T);
			FUCK::Button("$DEMO_NestedBtn"_T);
			FUCK::TreePop();
		}

		FUCK::Unindent();
	}

	FUCK::EndTabItem();
}
