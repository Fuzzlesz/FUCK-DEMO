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
		if (_loadedImage)
			FUCK::ReleaseImage(_loadedImage);
		_loadedImage = FUCK::LoadImage(_ImagePath, false);
	}
	FUCK::SameLine();
	if (FUCK::Button("$DEMO_ReleaseImage"_T)) {
		if (_loadedImage) {
			FUCK::ReleaseImage(_loadedImage);
			_loadedImage = nullptr;
		}
	}

	if (_loadedImage) {
		float w = 0.0f, h = 0.0f;
		FUCK::GetImageInfo(_loadedImage, &w, &h);
		FUCK::Text("$DEMO_ImageSize"_T, w, h);
		float availW = FUCK::GetContentRegionAvail().x;
		float scale = (w > availW) ? availW / w : 1.0f;
		FUCK::DrawImage(_loadedImage, ImVec2(w * scale, h * scale));
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
	FUCK::Checkbox("$DEMO_ShowOverlay"_T, &_showOverlay, true, true);

	if (_showOverlay) {
		const char* overlayTypes[] = {
			"$DEMO_Overlay_Grid"_T, "$DEMO_Overlay_Crosshair"_T,
			"$DEMO_Overlay_Spiral"_T, "$DEMO_Overlay_GoldenGrid"_T,
			"$DEMO_Overlay_Triangle"_T
		};
		FUCK::Combo("$DEMO_OverlayType"_T, &_overlayType, overlayTypes, 5);

		FUCK::SliderFloat("$DEMO_Thickness"_T, &_overlayThickness, 0.5f, 10.0f, "%.1f px");
		FUCK::ColorEdit3("$DEMO_Color"_T, _overlayColor, 0);

		if (_overlayType == 0) {  // Grid
			FUCK::SliderInt("$DEMO_GridRows"_T, &_gridRows, 0, 50);
			FUCK::SliderInt("$DEMO_GridCols"_T, &_gridCols, 0, 50);
			FUCK::SliderFloat("$DEMO_Rotation"_T, &_spiralRot, -45.0f, 45.0f, "%.0f deg");
		} else if (_overlayType == 1) {  // Crosshair
			FUCK::SliderInt("$DEMO_GridCols"_T, &_gridCols, 1, 10);
			FUCK::SliderInt("$DEMO_GridRows"_T, &_gridRows, 1, 10);
		} else if (_overlayType == 2) {  // Golden Spiral
			const char* basicAnchors[] = {
				"$DEMO_Anchor_BR"_T, "$DEMO_Anchor_BL"_T,
				"$DEMO_Anchor_TL"_T, "$DEMO_Anchor_TR"_T,
				"$DEMO_Anchor_Center"_T
			};
			FUCK::Combo("$DEMO_SpiralAnchor"_T, &_spiralAnchor, basicAnchors, 5);
			FUCK::Checkbox("$DEMO_ShowSquares"_T, &_showSquares);
			FUCK::SliderFloat("$DEMO_SpiralScale"_T, &_spiralScale, 0.1f, 5.0f);
			FUCK::SliderFloat("$DEMO_Rotation"_T, &_spiralRot, -180.0f, 180.0f);
			FUCK::SliderFloat("$DEMO_SpiralTurns"_T, &_spiralTurns, 1.0f, 20.0f);
		} else if (_overlayType == 3) {  // Golden Grid
			static int subDivs = 0;
			FUCK::SliderInt("$DEMO_Subdivisions"_T, &subDivs, 0, 3);
		} else if (_overlayType == 4) {  // Triangle
			static bool triMirror = false;
			FUCK::Checkbox("$DEMO_Mirror"_T, &triMirror);
		}
	}

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
	if (!_showOverlay)
		return;

	ImU32 col = ImGui::ColorConvertFloat4ToU32(ImVec4(_overlayColor[0], _overlayColor[1], _overlayColor[2], _overlayColor[3]));

	if (_overlayType == 0) {  // Grid
		FUCK::DrawGrid(_overlayThickness, col, (float)_gridRows, (float)_gridCols, _spiralRot);
	} else if (_overlayType == 1) {  // Crosshair
		FUCK::DrawCrosshair(_overlayThickness, col, (float)_gridRows, (float)_gridCols);
	} else if (_overlayType == 2) {       // Golden Spiral
		FUCK::DrawGoldenSpiral(_overlayThickness, col, _spiralAnchor, _spiralTurns, _spiralRot, _spiralScale, _showSquares);
	} else if (_overlayType == 3) {  // Golden Grid
		static int subDivs = 0;
		FUCK::DrawGoldenGrid(_overlayThickness, col, subDivs);
	} else if (_overlayType == 4) {  // Triangle
		static bool triMirror = false;
		FUCK::DrawTriangle(_overlayThickness, col, triMirror);
	}
}
