#ifndef RENDERER_RENDERER
#define RENDERER_RENDERER

#include "../Utils/Utils.h"

#include <dxgi.h>
#include <d3d11.h>
#include <d2d1_1.h>
#include <dwrite_1.h>

#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dwrite.lib")

class Renderer {
private:
    ID3D11DeviceContext* pContext = nullptr;
    ID2D1RenderTarget* d2dRenderTarget = nullptr;

    ID2D1Factory* factory = nullptr;
    IDXGISurface* dxgiBackbuffer = nullptr;
    IDWriteFactory1* writeFactory = nullptr;

    IDWriteTextFormat* textFormat = nullptr;
    ID2D1SolidColorBrush* brush = nullptr;
private:
    bool initialized = false;
public:
    auto init(IDXGISwapChain*, ID3D11Device*, ID3D11DeviceContext*) -> void;
    auto releaseTextures() -> void;
    auto beginFrame() -> void;
    auto endFrame() -> void;
private:
    auto charToWStr(char const &c) -> std::wstring;
public:
    auto getTextWidth(std::wstring, float) -> float;
    auto getTextHeight(std::wstring, float) -> float;
public:
    auto drawString(std::wstring, float, Vec2<float>, Color) -> void;
    auto fillRectangle(Vec4<float>, Color color) -> void;
};

#endif /* RENDERER_RENDERER */