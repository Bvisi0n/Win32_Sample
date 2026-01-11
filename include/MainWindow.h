#ifndef MAINWINDOW_H
#define MAINWINDOW_H
// ------ Project wide settings ---------------------
#include "Config.h"

// ------ Win32 and more ----------------------------
#include <windows.h>	// The core
#include <commctrl.h>
#include <wrl/client.h>	// Microsoft::WRL::ComPtr (smartpointer)
#include <d2d1.h>	    // Direct2D

// ------ STL ---------------------------------------
#include <map>
#include <vector>

// ------ Homebrew ----------------------------------
#include "BaseWindow.h"
#include "MenuBar.h"
#include "modules/CursorModule.h"
#include "modules/DatePickerModule.h"
#include "modules/FileSelectModule.h"
#include "modules/PopUpModule.h"
#include "UIConstants.h"
#include "Control.h"

class MainWindow : public BaseWindow<MainWindow>
{
public:
    // ---- Special Member Functions ----------------
    MainWindow()  = default;
    ~MainWindow() = default;

    // ---- Window Properties & Logic ---------------
    PCWSTR  ClassName() const override;
    LRESULT HandleMessage(UINT message, WPARAM wParam, LPARAM lParam) override;
    
private:
    float                                           m_DpiScale{ 1.0f };
    Microsoft::WRL::ComPtr<ID2D1Factory>            m_pFactory;
    Microsoft::WRL::ComPtr<ID2D1HwndRenderTarget>   m_pRenderTarget;
    Microsoft::WRL::ComPtr<ID2D1SolidColorBrush>    m_pBrush;

    MenuBar                     m_MenuBar{};
    D2D1_COLOR_F                m_BackgroundColor{ D2D1::ColorF(D2D1::ColorF::AliceBlue) };

    CursorModule                m_CursorModule{};
    DatePickerModule            m_DatePickerModule{};
    FileSelectModule            m_FileSelectModule{};
    PopUpModule                 m_PopUpModule{};

    std::map<UI::ControlID, std::unique_ptr<Control>> m_Controls;

    std::vector<D2D1_ELLIPSE>   m_Ellipses{};
    float                       m_EllipseSize{ 10.f };

    // ---- Rendering -------------------------------
                  void    OnPaint();
    [[nodiscard]] HRESULT CreateGraphicsResources();

    // ---- Coordinate Systems ----------------------
                  void    UpdateDpiScale();
                  float   PixelsToDips(int pixelValue) const;

    // ---- Layout ----------------------------------
                  void    OnSize();
                  void    UpdateModuleLayouts();

    // ---- Input -----------------------------------
    [[nodiscard]] bool    HandleMenuBarCommands(const WORD id);
    [[nodiscard]] bool    HandleCursorModuleCommands(const WORD id);
    [[nodiscard]] bool    HandleFileSelectModuleCommands(const WORD id);
    [[nodiscard]] bool    HandlePopUpModuleCommands(const WORD id, const WORD code);
                  void    OnLButtonDown(const int x, const int y);
                  void    OnRButtonDown(const int x, const int y);
};

#endif
