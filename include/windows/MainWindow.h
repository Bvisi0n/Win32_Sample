#ifndef MAINWINDOW_H
#define MAINWINDOW_H

// ------ Project wide settings ---------------------
#include "Config.h"

// ------ Win32 and more ----------------------------
#include <d2d1.h> // I haven't found an alternative for D2D1_COLOR_F
#include <wrl/client.h>

// ------ STL ---------------------------------------
#include <map>
#include <memory>
#include <vector>

// ------ Homebrew ----------------------------------
#include "controls/MenuBar.h"
#include "controls/UIConstants.h"
#include "windows/BaseWindow.h"

// ------ Forward Declarations ----------------------
struct HFONT__;
    typedef struct HFONT__* HFONT;

class Control;
class Button;
class Label;
class TextBox;

class MainWindow : public BaseWindow<MainWindow>
{
public:
    // ---- Special Member Functions ----------------
    MainWindow();
    ~MainWindow();

    // ---- Window Properties & Logic ---------------
    PCWSTR  ClassName() const override;
    LRESULT HandleMessage(UINT message, WPARAM wParam, LPARAM lParam) override;

    // ---- Member Access ---------------------------
    Button*         GetButton(UI::ControlID id);
    Label*          GetLabel(UI::ControlID id);
    TextBox*        GetTextBox(UI::ControlID id);
    UI::ControlID   GetCursorType() const { return m_CursorType; };
    void            SetCursorType(UI::ControlID cursorId);
    
private:
    // Per.2: Don’t optimize prematurely (readable variable order vs perfectly aligned)
    // ---- Scaling & Internal State ----------------
    float   m_DpiScale;
    HFONT   m_UIFontHandle;

    // ---- Direct2D Resources ----------------------
    Microsoft::WRL::ComPtr<ID2D1Factory>            m_pFactory;
    Microsoft::WRL::ComPtr<ID2D1HwndRenderTarget>   m_pRenderTarget;
    Microsoft::WRL::ComPtr<ID2D1SolidColorBrush>    m_pBrush;

    // ---- UI & Data ------------------------------
    std::map<UI::ControlID, std::unique_ptr<Control>>   m_Controls;
    std::vector<D2D1_ELLIPSE>   m_Ellipses;
    float                       m_EllipseSize;
    D2D1_COLOR_F                m_BackgroundColor;
    MenuBar                     m_MenuBar;
    UI::ControlID               m_CursorType;

    // ---- Initialization & Layout -----------------
    void InitializeUI();
    void SyncUIOrder();
    void UpdateUIFont();
    void UpdateControlLayouts();

    // ---- Rendering -------------------------------
                    void    OnPaint();
    [[nodiscard]]   HRESULT CreateGraphicsResources();

    // ---- Coordinate Systems ----------------------
    void    UpdateDpiScale();
    float   PixelsToDips(int pixelValue) const;

    // ---- Input Handling --------------------------
    [[nodiscard]]   bool HandleMenuBarCommands(const WORD id);
                    void OnLButtonDown(const int x, const int y);
                    void OnRButtonDown(const int x, const int y);
};

#endif
