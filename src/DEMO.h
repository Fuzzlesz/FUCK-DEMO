#pragma once
#include "FUCK_API.h"
#include "System/Settings.h"

// ==========================================
// Secondary Simple Overlay
// ==========================================
class SimpleOverlay : public IWindow
{
public:
	const char*	Title() const override { return "$DEMO_SecondOverlayTitle"_T; }
	void		Draw() override;
	bool		IsOpen() const override { return _isOpen; }
	void		SetOpen(bool a_open) override { _isOpen = a_open; }

	WindowFlags GetFlags() const override
	{
		WindowFlags f = WindowFlags::kNoDecoration;
		if (_passInput)
			f = f | WindowFlags::kPassInputToGame;
		return f;
	}

	ImVec2 GetDefaultPos() const override;
	ImVec2 GetDefaultSize() const override;
	bool GetRequestedPos(ImVec2& outPos) override;

	void UpdateState(const ImVec2& currentPos, const ImVec2& currentSize) override
	{
		_pos = currentPos;
		_size = currentSize;
	}

	bool _isOpen = false;
	bool _passInput = false;

	ImVec2 _pos{ 800.0f, 400.0f };
	ImVec2 _size{ 400.0f, 300.0f };

	IWindow* _parentWindow = nullptr;
};

// ==========================================
// Demo Overlay Window (Main)
// ==========================================
class DemoOverlay : public IWindow
{
public:
	// --- IWindow Interface Implementation ---
	const char*	Title() const override { return "$DEMO_OverlayTitleHint"_T; }
	void		Draw() override;
	bool		IsOpen() const override { return _isOpen; }
	void		SetOpen(bool a_open) override { _isOpen = a_open; }
	WindowFlags	GetFlags() const override;

	ImVec2 GetDefaultSize() const override;
	ImVec2 GetDefaultPos() const override;
	bool GetRequestedPos(ImVec2& outPos) override;

	void UpdateState(const ImVec2& currentPos, const ImVec2& currentSize) override
	{
		_windowPos = currentPos;
		_windowSize = currentSize;
	}

	bool _isOpen = false;
	bool _hasLoadedPos = false;

	// Base Window Metrics
	ImVec2 _windowPos{ 0.0f, 0.0f };
	ImVec2 _windowSize{ 0.0f, 0.0f };

	ImVec2 _lastSavedPos{ 0.0f, 0.0f };
	ImVec2 _lastSavedSize{ 0.0f, 0.0f };

	const ImVec2 _baseSize{ 430.0f, 855.0f };

	FUCK::ManagedHotkey _toggleHotkey{ 35, 0, 42, -1, -1, -1 };
	SimpleOverlay* _secondWindow = nullptr;

private:
	// Window Configuration Flags
	bool _reqBlur = false;
	bool _reqHideHUD = false;
	bool _reqPauseHard = false;
	bool _reqPauseSoft = false;
	bool _reqCloseOnEsc = false;
	bool _reqCloseOnMenu = false;
	bool _reqPassInput = false;
	bool _reqBlockVanity = false;
	bool _reqNoBackground = false;
	bool _reqNoDecoration = false;
	bool _reqExtendBorder = false;

	friend class DemoState;
};

// ==========================================
// Demo State Manager & Tools
// ==========================================
class DemoState : public REX::Singleton<DemoState>
{
public:
	DemoState();

	void OnOpen();
	void OnClose();
	bool OnAsyncInput(const void* inputEvent);

	// Helper to save/load settings
	void LoadSettings();
	void SaveSettings();

	DemoOverlay* GetOverlay() { return &_overlay; }

	// --- Sub-Tools ---
	// 1. Basic Tool (Root Level)
	class ToolGeneral : public ITool
	{
	public:
		const char*	Name() const override { return "$DEMO_Tool_General"_T; }
		const char*	Group() const override { return nullptr; }  // Top level
		void		Draw() override;
		void		RenderOverlay() override { DemoState::GetSingleton()->DrawOverlays(); }
		void		OnOpen() override { DemoState::GetSingleton()->OnOpen(); }
		void		OnClose() override { DemoState::GetSingleton()->OnClose(); }
		bool		OnAsyncInput(const void* e) override { return DemoState::GetSingleton()->OnAsyncInput(e); }
	};

	// 2. Visuals Tool (Grouped)
	class ToolVisuals : public ITool
	{
	public:
		const char*	Name() const override { return "$DEMO_Tool_Visuals"_T; }
		const char*	Group() const override { return "$DEMO_Group_Advanced"_T; }  // Grouped
		void		Draw() override;
		void		RenderOverlay() override { DemoState::GetSingleton()->DrawOverlays(); }
	};

	// 3. System Tool (Grouped)
	class ToolSystem : public ITool
	{
	public:
		const char*	Name() const override { return "$DEMO_Tool_System"_T; }
		const char*	Group() const override { return "$DEMO_Group_Advanced"_T; }  // Grouped
		void		Draw() override;
		void		RenderOverlay() override { DemoState::GetSingleton()->DrawOverlays(); }
	};

private:
	// Draw Implementations
	void DrawBasicWidgetsTab();
	void DrawAdvancedWidgetsTab();
	void DrawLayoutStyleTab();
	void DrawRenderingTab();
	void DrawOverlays();
	void DrawInputIOTab();
	void DrawTablesTab();
	void DrawGameControlTab();
	void DrawIconsTab();

	// --- Shared State ---

	// Standard Mutex Group
	bool _chkNear = false;
	bool _chkMutexA = true;
	bool _chkMutexB = false;

	// Independent Group
	bool _chkFarA = false;
	bool _chkFarB = true;

	bool _toggleState = false;
	float _sliderVal = 50.0f;
	int _intVal = 5;
	float _dragFloat = 0.5f;
	int _dragInt = 10;
	char _inputBuffer[256] = { 0 };

	// Feature Demos
	bool _useChildWindow = false;
	void* _loadedImage = nullptr;
	bool _showSpinner = false;
	bool _renderSuspended = false;
	char _ImagePath[256] = "Data/Interface/test.png";
	int _iconBtnClicks = 0;

	// Overlay Demo
	bool _showOverlay = false;
	int _overlayType = 0;  // 0=Grid, 1=Columns, 2=Spiral, 3=X, 4=Diag
	float _overlayThickness = 2.0f;
	float _overlayColor[4] = { 1.0f, 1.0f, 1.0f, 0.5f };
	int _gridRows = 10;
	int _gridCols = 10;
	int _spiralAnchor = 0;  // 0=BR, 1=BL, 2=TL, 3=TR
	float _spiralRot = 0.0f;
	float _spiralScale = 1.0f;
	float _spiralTurns = 6.0f;
	bool _showSquares = false;

	// Input Capture Demo
	int _keyTestIndex = 1;
	bool _inputCaptured = false;
	std::uint32_t _lastPressedKey = 0;

	// Game State
	bool _vanityBlocked = false;
	bool _hardPaused = false;
	bool _softPaused = false;
	bool _cursorForced = false;

	// Form Inspector
	struct FormRow
	{
		std::uint32_t id;
		std::string name;
		std::string editorID;
		std::string typeStr;
		float weight;
		std::int32_t value;
	};
	std::vector<FormRow> _formRows;
	std::uint32_t _comboSelectedFormID = 0;

	// Utilities
	char _pathBuffer[256] = "Data/My Bad\\Path//File.dds";
	char _sanitizedBuffer[256] = { 0 };

	std::uint32_t _selectedFormID = 0;
	int _stepperValue = 0;
	std::vector<std::string> _stepperOptions;

	DemoOverlay _overlay;
	SimpleOverlay _secondOverlay;

	// Tool Instances
	ToolGeneral _toolGeneral;
	ToolVisuals _toolVisuals;
	ToolSystem _toolSystem;
};
