#include "DEMO.h"

void DemoState::DrawRenderingTab()
{
	if (!FUCK::BeginTabItem("$DEMO_TabRendering"_T)) {
		return;
	}

	FUCK::Header("$DEMO_Section_ImageLoading"_T);
	FUCK::SetNextItemWidth(-1);
	FUCK::InputText("$DEMO_ImagePath"_T, _ImagePath, sizeof(_ImagePath));

	if (FUCK::Button("$DEMO_LoadImage"_T)) {
		_loadedImage = FUCK::Image(_ImagePath, false);
	}
	FUCK::SameLine();
	if (FUCK::Button("$DEMO_ReleaseImage"_T)) {
		_loadedImage.Reset();
	}

	if (_loadedImage.IsLoaded()) {
		float w = _loadedImage.GetWidth();
		float h = _loadedImage.GetHeight();

		FUCK::Text("$DEMO_ImageSize"_T, w, h);
		float availW = FUCK::GetContentRegionAvail().x;
		float scale = (w > availW) ? availW / w : 1.0f;

		FUCK::DrawImage(_loadedImage.GetID(), ImVec2(w * scale, h * scale));
	}

	FUCK::Separator();

	FUCK::Header("$DEMO_Section_Spinner"_T);
	FUCK::Checkbox("$DEMO_ShowSpinner"_T, &_showSpinner, false, false);
	if (_showSpinner) {
		FUCK::Spinner("$DEMO_SpinnerID"_T, 20.0f, 3.0f, ImVec4(0, 1, 0, 1));
	}

	FUCK::Separator();

	// --- OVERLAY SECTION ---
	FUCK::Header("$DEMO_Section_Overlays"_T);
	bool changed = false;
	changed |= FUCK::Checkbox("$DEMO_ShowOverlay"_T, &_cfg.showOverlay, true, true);

	if (_cfg.showOverlay) {
		const char* overlayTypes[] = {
			"$DEMO_Overlay_Grid"_T, "$DEMO_Overlay_Crosshair"_T,
			"$DEMO_Overlay_Spiral"_T, "$DEMO_Overlay_GoldenGrid"_T,
			"$DEMO_Overlay_Triangle"_T
		};
		changed |= FUCK::Combo("$DEMO_OverlayType"_T, &_cfg.overlayType, overlayTypes, 5);

		FUCK::SliderFloat("$DEMO_Thickness"_T, &_cfg.overlayThickness, 0.5f, 10.0f, "%.1f px");
		if (FUCK::IsItemDeactivatedAfterEdit())
			changed = true;

		FUCK::ColorEdit3("$DEMO_Color"_T, _cfg.overlayColor.data(), 0);
		if (FUCK::IsItemDeactivatedAfterEdit())
			changed = true;

		if (_cfg.overlayType == 0) {  // Grid
			FUCK::SliderInt("$DEMO_GridRows"_T, &_cfg.gridRows, 0, 50);
			if (FUCK::IsItemDeactivatedAfterEdit())
				changed = true;
			FUCK::SliderInt("$DEMO_GridCols"_T, &_cfg.gridCols, 0, 50);
			if (FUCK::IsItemDeactivatedAfterEdit())
				changed = true;
			FUCK::SliderFloat("$DEMO_Rotation"_T, &_cfg.spiralRot, -45.0f, 45.0f, "%.0f deg");
			if (FUCK::IsItemDeactivatedAfterEdit())
				changed = true;
		} else if (_cfg.overlayType == 1) {  // Crosshair
			FUCK::SliderInt("$DEMO_GridCols"_T, &_cfg.gridCols, 1, 10);
			if (FUCK::IsItemDeactivatedAfterEdit())
				changed = true;
			FUCK::SliderInt("$DEMO_GridRows"_T, &_cfg.gridRows, 1, 10);
			if (FUCK::IsItemDeactivatedAfterEdit())
				changed = true;
		} else if (_cfg.overlayType == 2) {  // Golden Spiral
			const char* basicAnchors[] = {
				"$DEMO_Anchor_BR"_T, "$DEMO_Anchor_BL"_T,
				"$DEMO_Anchor_TL"_T, "$DEMO_Anchor_TR"_T,
				"$DEMO_Anchor_Center"_T
			};
			changed |= FUCK::Combo("$DEMO_SpiralAnchor"_T, &_cfg.spiralAnchor, basicAnchors, 5);
			changed |= FUCK::Checkbox("$DEMO_ShowSquares"_T, &_cfg.showSquares);

			FUCK::SliderFloat("$DEMO_SpiralScale"_T, &_cfg.spiralScale, 0.1f, 5.0f);
			if (FUCK::IsItemDeactivatedAfterEdit())
				changed = true;
			FUCK::SliderFloat("$DEMO_Rotation"_T, &_cfg.spiralRot, -180.0f, 180.0f);
			if (FUCK::IsItemDeactivatedAfterEdit())
				changed = true;
			FUCK::SliderFloat("$DEMO_SpiralTurns"_T, &_cfg.spiralTurns, 1.0f, 20.0f);
			if (FUCK::IsItemDeactivatedAfterEdit())
				changed = true;
		} else if (_cfg.overlayType == 3) {  // Golden Grid
			static int subDivs = 0;
			FUCK::SliderInt("$DEMO_Subdivisions"_T, &subDivs, 0, 3);
		} else if (_cfg.overlayType == 4) {  // Triangle
			changed |= FUCK::Checkbox("$DEMO_Mirror"_T, &_cfg.triMirror);
		}
	}
	if (changed)
		DemoState::GetSingleton()->SaveSettings();

	FUCK::Separator();

	FUCK::Header("$DEMO_Section_RenderCtrl"_T);
	if (FUCK::Button("$DEMO_SuspendBtn"_T)) {
		_renderSuspended = true;
		FUCK::SuspendRendering(true);
		std::thread([this]() {
			std::this_thread::sleep_for(std::chrono::seconds(3));
			FUCK::SuspendRendering(false);
			this->_renderSuspended = false;
		}).detach();
	}
	FUCK::SetTooltip("$DEMO_SuspendTip"_T);

	if (_renderSuspended) {
		FUCK::SameLine();
		FUCK::Text("$DEMO_SuspendMsg"_T);
	}

	FUCK::Separator();

	FUCK::Header("$DEMO_Section_Primitives"_T);
	ImVec2 p = FUCK::GetCursorScreenPos();
	FUCK::DrawRect(ImVec2(p.x, p.y), ImVec2(p.x + 100, p.y + 50), ImVec4(1, 0, 0, 1), 0.0f, 2.0f);
	FUCK::Dummy(ImVec2(0, 60));

	FUCK::TextDisabled("$DEMO_BgPrimDesc"_T);
	p = FUCK::GetCursorScreenPos();
	FUCK::DrawBackgroundLine(ImVec2(p.x, p.y), ImVec2(p.x + 300, p.y), 0x88FF0000, 4.0f);
	FUCK::Dummy(ImVec2(0, 10));

	FUCK::EndTabItem();
}

void DemoState::DrawOverlays()
{
	if (!_cfg.showOverlay)
		return;

	ImU32 col = ImGui::ColorConvertFloat4ToU32(ImVec4(_cfg.overlayColor[0], _cfg.overlayColor[1], _cfg.overlayColor[2], _cfg.overlayColor[3]));

	if (_cfg.overlayType == 0) {  // Grid
		FUCK::DrawGrid(_cfg.overlayThickness, col, static_cast<float>(_cfg.gridRows), static_cast<float>(_cfg.gridCols), _cfg.spiralRot);
	} else if (_cfg.overlayType == 1) {  // Crosshair
		FUCK::DrawCrosshair(_cfg.overlayThickness, col, static_cast<float>(_cfg.gridRows), static_cast<float>(_cfg.gridCols));
	} else if (_cfg.overlayType == 2) {  // Golden Spiral
		FUCK::DrawGoldenSpiral(_cfg.overlayThickness, col, _cfg.spiralAnchor, _cfg.spiralTurns, _cfg.spiralRot, _cfg.spiralScale, _cfg.showSquares);
	} else if (_cfg.overlayType == 3) {  // Golden Grid
		static int subDivs = 0;
		FUCK::DrawGoldenGrid(_cfg.overlayThickness, col, subDivs);
	} else if (_cfg.overlayType == 4) {  // Triangle
		FUCK::DrawTriangle(_cfg.overlayThickness, col, _cfg.triMirror);
	}
}
