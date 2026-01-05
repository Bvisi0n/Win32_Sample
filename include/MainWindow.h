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
#include "PopUpModule.h"

// MVP
    // TODO: Add a date picker, doesn't have to do anything
    // TODO: Add a button that opens a file selector, display info on the file in a label field

//Advanced
    // TODO: Use <filesystem> and <fstream> to save and load the ellipses to a file
    // TODO: Look to implement <expected> and/or <optional> and their monadic functions
    // TODO: Hunt for ways to use the STL, Algorithms, Containers, Iterators.

class MainWindow : public BaseWindow<MainWindow>
{
public:
    // ---- Special Member Functions ----------------
    MainWindow() = default;
    ~MainWindow() override;

    // ---- Window Properties & Logic ---------------
    PCWSTR  ClassName() const override;
    LRESULT HandleMessage(UINT message, WPARAM wParam, LPARAM lParam) override;
    
private:
    float                       m_DpiScale{ 1.0f };
    ID2D1Factory*               m_pFactory = nullptr;
    ID2D1HwndRenderTarget*      m_pRenderTarget = nullptr;
    ID2D1SolidColorBrush*       m_pBrush = nullptr;

    MenuBar                     m_MenuBar{};
    D2D1_COLOR_F                m_BackgroundColor{ D2D1::ColorF(D2D1::ColorF::AliceBlue) };

    CursorModule                m_CursorModule{};
    PopUpModule                 m_PopUpModule{};

    std::vector<D2D1_ELLIPSE>   m_Ellipses{}; // TODO: Wrap D2D1_ELLIPSE in a struct so it can manage it's own position, size, color, etc...
    float                       m_EllipseSize{ 10.f };

    // ---- Rendering -------------------------------
    void    OnPaint();
    HRESULT CreateGraphicsResources();
    void    DiscardGraphicsResources();

    // ---- Coordinate Systems ----------------------
    void    UpdateDpiScale();
    float   PixelsToDips(int pixelValue) const;

    // ---- Layout ----------------------------------
    void    OnSize();

    // ---- Input -----------------------------------
    [[nodiscard]]   bool HandleMenuBarCommands      (const WORD id);
    [[nodiscard]]   bool HandleCursorModuleCommands (const WORD id);
    [[nodiscard]]   bool HandlePopUpModuleCommands  (const WORD id, const WORD code);
                    void OnLButtonDown(const int x, const int y);
                    void OnRButtonDown(const int x, const int y);
};

#endif
