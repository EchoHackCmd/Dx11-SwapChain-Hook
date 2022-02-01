#include "Renderer/Renderer.h"

typedef HRESULT(__fastcall* D3D11PresentHook) (IDXGISwapChain* pChain, UINT syncInterval, UINT flags);
D3D11PresentHook oPresent = nullptr;

IDXGISwapChain* pSwapChain = nullptr;
ID3D11Device* pDevice = nullptr;
ID3D11DeviceContext* pContext = nullptr;
ID3D11RenderTargetView* renderTargetView = nullptr;

Renderer* renderer = nullptr;

HRESULT __fastcall PresentCallback(IDXGISwapChain* pChain, UINT syncInterval, UINT flags) {
    if(renderer == nullptr) {
        renderer = new Renderer();

        pSwapChain = pChain;

        if (SUCCEEDED(pSwapChain->GetDevice(__uuidof(pDevice), reinterpret_cast<void**>(&pDevice)))) {
            pSwapChain->GetDevice(__uuidof(pDevice), reinterpret_cast<void**>(&pDevice));
            pDevice->GetImmediateContext(&pContext);
        };
    };

    renderer->init(pChain, pDevice, pContext);
    renderer->beginFrame();

    std::vector<std::wstring> words = { L"Hello there!", L"Dx11 SwapChain", L"By EchoHackCmd" };

    auto I = 0;
    auto textSize = 52.f;

    for(auto text : words) {
        auto yOff = (I * (textSize * 2)) + textSize;
        auto len = renderer->getTextWidth(text, textSize);
        auto height = renderer->getTextHeight(text, textSize);

        renderer->fillRectangle(Vec4<float>(8.f, yOff, 8.f + len, yOff + height), Color(21, 21, 21, 0.6f));
        renderer->drawString(text, textSize, Vec2<float>(9.f, yOff), Color(255, 255, 255, 1));

        Utils::logToDebug(std::string("Text: " + std::string(text.begin(), text.end()) + ", Height: " + std::to_string(height)));

        I++;
    };

    renderer->endFrame();
    renderer->releaseTextures();

    return oPresent(pChain, syncInterval, flags);
};

auto init() -> void {
    Utils::logToDebug("Initialized DLL...");

    WNDCLASSEX wc{};
    wc.cbSize = sizeof(wc);
    wc.lpfnWndProc = DefWindowProc;
    wc.lpszClassName = TEXT("TemporaryWindow");
    RegisterClassEx(&wc);

    HWND hWnd = CreateWindow(wc.lpszClassName, TEXT(""), WS_DISABLED, 0, 0, 0, 0, nullptr, nullptr, nullptr, nullptr);

    D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;
    DXGI_SWAP_CHAIN_DESC sd;
    {
        memset((&sd), 0, (sizeof(sd)));
        sd.BufferCount = 1;
        sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
        sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
        sd.OutputWindow = hWnd;
        sd.SampleDesc.Count = 1;
        sd.SampleDesc.Quality = 0;
        sd.Windowed = TRUE;
        sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    }

    HRESULT hrD3D11CreateDeviceAndSwapChain = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr,
        0, &featureLevel, 1, D3D11_SDK_VERSION, &sd,
        &pSwapChain, &pDevice, nullptr, &pContext);

    if (FAILED(hrD3D11CreateDeviceAndSwapChain)) {
        DestroyWindow(sd.OutputWindow);
        UnregisterClass(wc.lpszClassName, GetModuleHandle(nullptr));
        return;
    };

    Utils::logToDebug("Created SwapChain dummie device...");

    uintptr_t** pSwapChainVTable = *reinterpret_cast<uintptr_t***>(pSwapChain);

    if(MH_Initialize() == MH_OK) {
        Utils::logToDebug("Initialized MinHook...");
        if(MH_CreateHook((void*)pSwapChainVTable[8], &PresentCallback, reinterpret_cast<LPVOID*>(&oPresent)) == MH_OK) {
            MH_EnableHook((void*)pSwapChainVTable[8]);
            Utils::logToDebug("Successfully created hook...");
        } else {
            Utils::logToDebug("Failed to create hook...");
        };
    };

    //pDevice->Release();
    //pContext->Release();
    //pSwapChain->Release();

    //DestroyWindow(sd.OutputWindow);
    //UnregisterClass(wc.lpszClassName, GetModuleHandle(nullptr));
};

BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD fdwReason, LPVOID lpRes) {
    switch(fdwReason) {
        case DLL_PROCESS_ATTACH:
            CreateThread(0, 0, (LPTHREAD_START_ROUTINE)init, hInstance, 0, 0);
        break;
    };
    return TRUE;
};