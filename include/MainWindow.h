#ifndef MAINWINDOW_H
#define MAINWINDOW_H
// ------ Project wide settings ---------------------
#include "Config.h"

// ------ STL ---------------------------------------
#include <vector>

// ------ Homebrew ----------------------------------
#include "BaseWindow.h"
#include "MenuBar.h"
#include "CursorModule.h"
#include "DatePickerModule.h"
#include "PopUpModule.h"

// MVP
    // TODO 1a: Add a button that opens a file selector, display info on the file in a label field

//Advanced
    // TODO 1b: Use <filesystem>, <fstream> & <regex> to save and load the ellipses to a file in both binary and text. Should be coupled with file selecter module

class MainWindow : public BaseWindow<MainWindow>
{
public:
    // ---- Special Member Functions ----------------
    MainWindow() = default;
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
    PopUpModule                 m_PopUpModule{};

    std::vector<D2D1_ELLIPSE>   m_Ellipses{}; // TODO 4a: Wrap D2D1_ELLIPSE in a struct so it can manage it's own position, size, color, etc...
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
    [[nodiscard]] bool    HandlePopUpModuleCommands(const WORD id, const WORD code);
                  void    OnLButtonDown(const int x, const int y);
                  void    OnRButtonDown(const int x, const int y);
};

#endif
