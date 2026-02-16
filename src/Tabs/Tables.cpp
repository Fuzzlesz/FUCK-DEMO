#include "DEMO.h"

void DemoState::DrawTablesTab()
{
	if (!FUCK::BeginTabItem("$DEMO_TabTables"_T)) {
		return;
	}

	FUCK::Header("$DEMO_Section_FormInsp"_T);
	FUCK::TextWrapped("$DEMO_FormInspDesc"_T);
	FUCK::Spacing();

	static std::uint8_t formType = 0;
	static int formTypeCombo = 0;
	const char* formTypes[] = {
		"$DEMO_FT_All"_T,          // 0
		"$DEMO_FT_Weapon"_T,       // 41
		"$DEMO_FT_Ammo"_T,         // 42
		"$DEMO_FT_Armor"_T,        // 26
		"$DEMO_FT_Potion"_T,       // 46
		"$DEMO_FT_Ingredient"_T,   // 30
		"$DEMO_FT_Spell"_T,        // 22
		"$DEMO_FT_Enchantment"_T,  // 21
		"$DEMO_FT_Scroll"_T,       // 23
		"$DEMO_FT_SoulGem"_T,      // 52
		"$DEMO_FT_NPC"_T,          // 43
		"$DEMO_FT_Key"_T           // 45
	};

	static const std::uint8_t formTypeValues[] = {
		0, 41, 42, 26, 46, 30, 22, 21, 23, 52, 43, 45
	};

	FUCK::SetNextItemWidth(-1);
	if (FUCK::Combo("$DEMO_FilterType"_T, &formTypeCombo, formTypes, IM_ARRAYSIZE(formTypes))) {
		formType = formTypeValues[formTypeCombo];
		_comboSelectedFormID = 0;
	}

	FUCK::SetNextItemWidth(-1);
	FUCK::ComboForm("$DEMO_SelectForm"_T, &_comboSelectedFormID, formType);

	if (_comboSelectedFormID != 0) {
		FUCK::Spacing();
		if (FUCK::Button("$DEMO_AddFormBtn"_T)) {
			auto* form = RE::TESForm::LookupByID(_comboSelectedFormID);
			if (form) {
				FormRow newRow;
				newRow.id = form->GetFormID();
				newRow.name = form->GetName();
				newRow.editorID = clib_util::editorID::get_editorID(form);

				switch (form->GetFormType()) {
				case RE::FormType::Weapon:
					newRow.typeStr = "$DEMO_Type_Weapon"_T;
					break;
				case RE::FormType::Ammo:
					newRow.typeStr = "$DEMO_Type_Ammo"_T;
					break;
				case RE::FormType::Armor:
					newRow.typeStr = "$DEMO_Type_Armor"_T;
					break;
				case RE::FormType::NPC:
					newRow.typeStr = "$DEMO_Type_NPC"_T;
					break;
				case RE::FormType::AlchemyItem:
					newRow.typeStr = "$DEMO_Type_Potion"_T;
					break;
				case RE::FormType::Ingredient:
					newRow.typeStr = "$DEMO_Type_Ingredient"_T;
					break;
				case RE::FormType::Spell:
					newRow.typeStr = "$DEMO_Type_Spell"_T;
					break;
				case RE::FormType::Enchantment:
					newRow.typeStr = "$DEMO_Type_Enchantment"_T;
					break;
				case RE::FormType::Scroll:
					newRow.typeStr = "$DEMO_Type_Scroll"_T;
					break;
				case RE::FormType::SoulGem:
					newRow.typeStr = "$DEMO_Type_SoulGem"_T;
					break;
				case RE::FormType::KeyMaster:
					newRow.typeStr = "$DEMO_Type_Key"_T;
					break;
				default:
					char buf[32];
					snprintf(buf, 32, "$DEMO_Type_Generic"_T, (int)form->GetFormType());
					newRow.typeStr = buf;
					break;
				}

				newRow.weight = 0.0f;
				if (auto* weightForm = form->As<RE::TESWeightForm>())
					newRow.weight = weightForm->weight;

				newRow.value = 0;
				if (auto* valForm = form->As<RE::TESValueForm>())
					newRow.value = valForm->value;

				bool exists = false;
				for (const auto& r : _formRows) {
					if (r.id == newRow.id)
						exists = true;
				}
				if (!exists)
					_formRows.push_back(newRow);
			}
		}
	}

	FUCK::Spacing();
	FUCK::SeparatorThick();
	FUCK::Spacing();

	TableFlags tableFlags = TableFlags::kResizable | TableFlags::kRowBg | TableFlags::kBorders |
	                        TableFlags::kSortable | TableFlags::kReorderable |
	                        TableFlags::kSizingStretchProp;

	if (FUCK::BeginTable("FormInspectorTable", 5, tableFlags)) {
		FUCK::TableSetupColumn("$DEMO_Col_ID"_T, TableColumnFlags::kWidthStretch, 0.8f);
		FUCK::TableSetupColumn("$DEMO_Col_Name"_T, TableColumnFlags::kWidthStretch, 3.0f);
		FUCK::TableSetupColumn("$DEMO_Col_Type"_T, TableColumnFlags::kWidthStretch, 1.2f);
		FUCK::TableSetupColumn("$DEMO_Col_Weight"_T, TableColumnFlags::kWidthStretch, 0.6f);
		FUCK::TableSetupColumn("$DEMO_Col_Value"_T, TableColumnFlags::kWidthStretch, 0.6f);

		FUCK::TableHeadersRow();

		if (ImGuiTableSortSpecs* sorts_specs = FUCK::GetTableSortSpecs()) {
			if (sorts_specs->SpecsDirty) {
				std::vector<FormRow>* rows = &_formRows;  // capture pointers for lambda
				std::sort(rows->begin(), rows->end(), [sorts_specs](const FormRow& a, const FormRow& b) {
					for (int n = 0; n < sorts_specs->SpecsCount; n++) {
						const ImGuiTableColumnSortSpecs* spec = &sorts_specs->Specs[n];
						int delta = 0;
						switch (spec->ColumnIndex) {
						case 0:
							delta = (a.id < b.id) ? -1 : (a.id > b.id ? 1 : 0);
							break;
						case 1:
							delta = a.name.compare(b.name);
							break;
						case 2:
							delta = a.typeStr.compare(b.typeStr);
							break;
						case 3:
							delta = (a.weight < b.weight) ? -1 : (a.weight > b.weight ? 1 : 0);
							break;
						case 4:
							delta = (a.value < b.value) ? -1 : (a.value > b.value ? 1 : 0);
							break;
						}
						if (delta > 0)
							return (spec->SortDirection == ImGuiSortDirection_Ascending);
						if (delta < 0)
							return (spec->SortDirection == ImGuiSortDirection_Descending);
					}
					return (a.id < b.id);
				});
				sorts_specs->SpecsDirty = false;
			}
		}

		for (const auto& row : _formRows) {
			FUCK::TableNextRow();
			FUCK::TableNextColumn();
			FUCK::Text("%08X", row.id);
			FUCK::TableNextColumn();
			FUCK::Text("%s", row.name.empty() ? row.editorID.c_str() : row.name.c_str());
			if (!row.editorID.empty() && !row.name.empty() && FUCK::IsItemHovered(0))
				FUCK::SetTooltip(row.editorID.c_str());
			FUCK::TableNextColumn();
			FUCK::Text("%s", row.typeStr.c_str());
			FUCK::TableNextColumn();
			FUCK::Text("%.1f", row.weight);
			FUCK::TableNextColumn();
			FUCK::Text("%d", row.value);
		}
		FUCK::EndTable();
	}

	if (_formRows.empty()) {
		FUCK::Spacing();
		FUCK::CenteredText("$DEMO_TableEmpty"_T, true);
	} else {
		FUCK::Spacing();
		if (FUCK::Button("$DEMO_ClearTable"_T)) {
			_formRows.clear();
		}
	}

	FUCK::EndTabItem();
}
