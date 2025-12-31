#ifndef MAINWINDOW_H
#define MAINWINDOW_H
// ------ Project wide settings ---------------------
#include "Config.h"

// ------ STL ---------------------------------------
#include <vector>

// ------ Homebrew ----------------------------------
#include "BaseWindow.h"

class MainWindow : public BaseWindow<MainWindow>
{
    float                       m_DpiScale{ 1.0f };
    ID2D1Factory*               m_pFactory = nullptr;
    ID2D1HwndRenderTarget*      m_pRenderTarget = nullptr;
    ID2D1SolidColorBrush*       m_pBrush = nullptr;
    std::vector<D2D1_ELLIPSE>   m_Ellipses{};
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
    void    CalculateLayout();

    // ---- Input -----------------------------------
    void    OnLButtonDown(const int x, const int y);
    void    OnRButtonDown(const int x, const int y);

public:
    // ---- Special Member Functions ----------------
    MainWindow() = default;
    ~MainWindow() override;

    // ---- Window Properties & Logic ---------------
    PCWSTR  ClassName() const override;
    LRESULT HandleMessage(UINT message, WPARAM wParam, LPARAM lParam) override;
};

#endif
