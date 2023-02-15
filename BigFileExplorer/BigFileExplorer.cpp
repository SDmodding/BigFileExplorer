#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string>
#include <vector>
#include <dwmapi.h>
#pragma comment(lib, "dwmapi")
#include <d3d11.h>
#pragma comment(lib, "d3d11")
#include <map>

#include "3rdParty/ImGui/imgui.h"
#include "3rdParty/ImGui/imgui_impl_win32.h"
#include "3rdParty/ImGui/imgui_impl_dx11.h"

#define STB_IMAGE_IMPLEMENTATION
#include "3rdParty/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "3rdParty/stb_image_write.h"

ID3D11Device* g_D3DDevice = nullptr;
ID3D11DeviceContext* g_D3DDeviceContext = nullptr;
IDXGISwapChain* g_SwapChain = nullptr;
ID3D11RenderTargetView* g_MainRenderTargetView = nullptr;

namespace Wrappers
{
    void StyleColors()
    {
        ImVec4* colors = ImGui::GetStyle().Colors;
        colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
        colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
        colors[ImGuiCol_WindowBg] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
        colors[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        colors[ImGuiCol_PopupBg] = ImVec4(0.19f, 0.19f, 0.19f, 0.92f);
        colors[ImGuiCol_Border] = ImVec4(0.19f, 0.19f, 0.19f, 0.29f);
        colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.24f);
        colors[ImGuiCol_FrameBg] = ImVec4(0.05f, 0.05f, 0.05f, 0.54f);
        colors[ImGuiCol_FrameBgHovered] = ImVec4(0.19f, 0.19f, 0.19f, 0.54f);
        colors[ImGuiCol_FrameBgActive] = ImVec4(0.20f, 0.22f, 0.23f, 1.00f);
        colors[ImGuiCol_TitleBg] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
        colors[ImGuiCol_TitleBgActive] = ImVec4(0.06f, 0.06f, 0.06f, 1.00f);
        colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
        colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
        colors[ImGuiCol_ScrollbarBg] = ImVec4(0.05f, 0.05f, 0.05f, 0.54f);
        colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.34f, 0.34f, 0.34f, 0.54f);
        colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.40f, 0.40f, 0.40f, 0.54f);
        colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.56f, 0.56f, 0.56f, 0.54f);
        colors[ImGuiCol_CheckMark] = ImVec4(0.33f, 0.67f, 0.86f, 1.00f);
        colors[ImGuiCol_SliderGrab] = ImVec4(0.34f, 0.34f, 0.34f, 0.54f);
        colors[ImGuiCol_SliderGrabActive] = ImVec4(0.56f, 0.56f, 0.56f, 0.54f);
        colors[ImGuiCol_Button] = ImVec4(0.05f, 0.05f, 0.05f, 0.54f);
        colors[ImGuiCol_ButtonHovered] = ImVec4(0.19f, 0.19f, 0.19f, 0.54f);
        colors[ImGuiCol_ButtonActive] = ImVec4(0.20f, 0.22f, 0.23f, 1.00f);
        colors[ImGuiCol_Header] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
        colors[ImGuiCol_HeaderHovered] = ImVec4(0.00f, 0.00f, 0.00f, 0.36f);
        colors[ImGuiCol_HeaderActive] = ImVec4(0.20f, 0.22f, 0.23f, 0.33f);
        colors[ImGuiCol_Separator] = ImVec4(0.28f, 0.28f, 0.28f, 0.29f);
        colors[ImGuiCol_SeparatorHovered] = ImVec4(0.44f, 0.44f, 0.44f, 0.29f);
        colors[ImGuiCol_SeparatorActive] = ImVec4(0.40f, 0.44f, 0.47f, 1.00f);
        colors[ImGuiCol_ResizeGrip] = ImVec4(0.28f, 0.28f, 0.28f, 0.29f);
        colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.44f, 0.44f, 0.44f, 0.29f);
        colors[ImGuiCol_ResizeGripActive] = ImVec4(0.40f, 0.44f, 0.47f, 1.00f);
        colors[ImGuiCol_Tab] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
        colors[ImGuiCol_TabHovered] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
        colors[ImGuiCol_TabActive] = ImVec4(0.20f, 0.20f, 0.20f, 0.36f);
        colors[ImGuiCol_TabUnfocused] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
        colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
        colors[ImGuiCol_PlotLines] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
        colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
        colors[ImGuiCol_PlotHistogram] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
        colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
        colors[ImGuiCol_TableHeaderBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
        colors[ImGuiCol_TableBorderStrong] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
        colors[ImGuiCol_TableBorderLight] = ImVec4(0.28f, 0.28f, 0.28f, 0.29f);
        colors[ImGuiCol_TableRowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
        colors[ImGuiCol_TextSelectedBg] = ImVec4(0.20f, 0.22f, 0.23f, 1.00f);
        colors[ImGuiCol_DragDropTarget] = ImVec4(0.33f, 0.67f, 0.86f, 1.00f);
        colors[ImGuiCol_NavHighlight] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
        colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 0.00f, 0.00f, 0.70f);
        colors[ImGuiCol_NavWindowingDimBg] = ImVec4(1.00f, 0.00f, 0.00f, 0.20f);
        colors[ImGuiCol_ModalWindowDimBg] = ImVec4(1.00f, 0.00f, 0.00f, 0.35f);

        ImGuiStyle& style = ImGui::GetStyle();
        style.WindowPadding = ImVec2(8.00f, 8.00f);
        style.FramePadding = ImVec2(5.00f, 2.00f);
        style.CellPadding = ImVec2(6.00f, 6.00f);
        style.ItemSpacing = ImVec2(6.00f, 6.00f);
        style.ItemInnerSpacing = ImVec2(6.00f, 6.00f);
        style.TouchExtraPadding = ImVec2(0.00f, 0.00f);
        style.IndentSpacing = 25;
        style.ScrollbarSize = 15;
        style.GrabMinSize = 10;
        style.WindowBorderSize = 1;
        style.ChildBorderSize = 1;
        style.PopupBorderSize = 1;
        style.FrameBorderSize = 1;
        style.TabBorderSize = 1;
        style.WindowRounding = 7;
        style.ChildRounding = 4;
        style.FrameRounding = 3;
        style.PopupRounding = 4;
        style.ScrollbarRounding = 9;
        style.GrabRounding = 3;
        style.LogSliderDeadzone = 4;
        style.TabRounding = 4;
    }

    void CreateRenderTarget()
    {
        ID3D11Texture2D* m_BackBuffer;
        g_SwapChain->GetBuffer(0, IID_PPV_ARGS(&m_BackBuffer));
        g_D3DDevice->CreateRenderTargetView(m_BackBuffer, 0, &g_MainRenderTargetView);
        m_BackBuffer->Release();
    }

    bool CreateDeviceD3D(HWND m_Window)
    {
        DXGI_SWAP_CHAIN_DESC m_DXDesc;
        ZeroMemory(&m_DXDesc, sizeof(m_DXDesc));
        m_DXDesc.BufferCount = 2;
        m_DXDesc.BufferDesc.Width = 0;
        m_DXDesc.BufferDesc.Height = 0;
        m_DXDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        m_DXDesc.BufferDesc.RefreshRate.Numerator = 60;
        m_DXDesc.BufferDesc.RefreshRate.Denominator = 1;
        m_DXDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
        m_DXDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        m_DXDesc.OutputWindow = m_Window;
        m_DXDesc.SampleDesc.Count = 1;
        m_DXDesc.SampleDesc.Quality = 0;
        m_DXDesc.Windowed = TRUE;
        m_DXDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

        D3D_FEATURE_LEVEL m_FeatureLevel;
        D3D_FEATURE_LEVEL m_FeatureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };

        HRESULT m_Result = D3D11CreateDeviceAndSwapChain(0, D3D_DRIVER_TYPE_HARDWARE, 0, 0, m_FeatureLevelArray, 2, D3D11_SDK_VERSION, &m_DXDesc, &g_SwapChain, &g_D3DDevice, &m_FeatureLevel, &g_D3DDeviceContext);
        if (m_Result == DXGI_ERROR_UNSUPPORTED)
            m_Result = D3D11CreateDeviceAndSwapChain(0, D3D_DRIVER_TYPE_WARP, 0, 0, m_FeatureLevelArray, 2, D3D11_SDK_VERSION, &m_DXDesc, &g_SwapChain, &g_D3DDevice, &m_FeatureLevel, &g_D3DDeviceContext);

        if (m_Result != S_OK)
            return false;

        CreateRenderTarget();
        return true;
    }

    void CleanupRenderTarget()
    {
        if (g_MainRenderTargetView) { g_MainRenderTargetView->Release(); g_MainRenderTargetView = nullptr; }
    }

    void CleanupDeviceD3D()
    {
        CleanupRenderTarget();
        if (g_SwapChain) { g_SwapChain->Release(); g_SwapChain = nullptr; }
        if (g_D3DDeviceContext) { g_D3DDeviceContext->Release(); g_D3DDeviceContext = nullptr; }
        if (g_D3DDevice) { g_D3DDevice->Release(); g_D3DDevice = nullptr; }
    }

    uint8_t* ConvertTextureToPNG(uint8_t* m_Buffer, int m_Width, int m_Height, int* m_Size)
    {
        return stbi_write_png_to_mem(m_Buffer, 0, m_Width, m_Height, 4, m_Size);
    }

    bool LoadTextureFromMemory(uint8_t* m_Buffer, int m_Size, ID3D11ShaderResourceView** m_ShaderResource, int* m_Width, int* m_Height)
    {
        uint8_t* m_TextureBuffer = stbi_load_from_memory(m_Buffer, m_Size, m_Width, m_Height, 0, 4);
        if (!m_TextureBuffer)
            return false;

        D3D11_TEXTURE2D_DESC m_TextureDesc;
        ZeroMemory(&m_TextureDesc, sizeof(m_TextureDesc));
        m_TextureDesc.Width             = *m_Width;
        m_TextureDesc.Height            = *m_Height;
        m_TextureDesc.MipLevels         = 1;
        m_TextureDesc.ArraySize         = 1;
        m_TextureDesc.Format            = DXGI_FORMAT_R8G8B8A8_UNORM;
        m_TextureDesc.SampleDesc.Count  = 1;
        m_TextureDesc.Usage             = D3D11_USAGE_DEFAULT;
        m_TextureDesc.BindFlags         = D3D11_BIND_SHADER_RESOURCE;
        m_TextureDesc.CPUAccessFlags    = 0;

        D3D11_SUBRESOURCE_DATA m_SubResource;
        m_SubResource.pSysMem = m_TextureBuffer;
        m_SubResource.SysMemPitch = m_TextureDesc.Width * 4;
        m_SubResource.SysMemSlicePitch = 0;

        ID3D11Texture2D* m_Texture = nullptr;
        g_D3DDevice->CreateTexture2D(&m_TextureDesc, &m_SubResource, &m_Texture);

        D3D11_SHADER_RESOURCE_VIEW_DESC m_ShaderResourceDesc;
        ZeroMemory(&m_ShaderResourceDesc, sizeof(m_ShaderResourceDesc));
        m_ShaderResourceDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        m_ShaderResourceDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
        m_ShaderResourceDesc.Texture2D.MipLevels = m_TextureDesc.MipLevels;
        m_ShaderResourceDesc.Texture2D.MostDetailedMip = 0;

        g_D3DDevice->CreateShaderResourceView(m_Texture, &m_ShaderResourceDesc, m_ShaderResource);
        m_Texture->Release();

        stbi_image_free(m_TextureBuffer);
        return true;
    }
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    switch (msg)
    {
        case WM_MOUSEMOVE:
        {
            static bool m_Checked = false;
            static bool m_Dragging = false;
            if (wParam == MK_LBUTTON)
            {
                static RECT m_WindowPos;
                GetWindowRect(hWnd, &m_WindowPos);

                static POINT m_MousePos, m_LastMousePos;
                GetCursorPos(&m_MousePos);

                if (m_Dragging)
                {
                    m_WindowPos.left += m_MousePos.x - m_LastMousePos.x;
                    m_WindowPos.top += m_MousePos.y - m_LastMousePos.y;
                    SetWindowPos(hWnd, HWND_TOPMOST, m_WindowPos.left, m_WindowPos.top, 0, 0, SWP_SHOWWINDOW | SWP_NOSIZE | SWP_NOZORDER);
                }
                else if (!m_Checked)
                {
                    int m_Titlebar = static_cast<int>(15);
                    if (m_MousePos.y >= m_WindowPos.top && m_WindowPos.top + m_Titlebar >= m_MousePos.y)
                        m_Dragging = true;

                    m_Checked = true;
                }

                m_LastMousePos = m_MousePos;
            }
            else
                m_Checked = m_Dragging = false;
        }
        break;
        case WM_SIZE:
        {
            if (g_D3DDevice && wParam != SIZE_MINIMIZED)
            {
                Wrappers::CleanupRenderTarget();
                g_SwapChain->ResizeBuffers(0, (UINT)LOWORD(lParam), (UINT)HIWORD(lParam), DXGI_FORMAT_UNKNOWN, 0);
                Wrappers::CreateRenderTarget();
            }
            return 0;
        }
        break;
        case WM_SYSCOMMAND:
        {
            if ((wParam & 0xFFF0) == SC_KEYMENU)
                return 0;
        }
        break;
        case WM_DESTROY:
        {
            PostQuitMessage(0);
            return 0;
        }
        break;
    }

    return DefWindowProcA(hWnd, msg, wParam, lParam);
}

namespace FileWrapper
{
    bool Read(const char* m_Path, const char* m_Mode, uint8_t** m_Buffer, size_t* m_BufferSize)
    {
        FILE* m_File = fopen(m_Path, m_Mode);
        if (m_File)
        {
            fseek(m_File, 0L, SEEK_END);

            *m_BufferSize = static_cast<size_t>(ftell(m_File));
            *m_Buffer = reinterpret_cast<uint8_t*>(malloc(*m_BufferSize));

            fseek(m_File, 0L, SEEK_SET);
            fread(*m_Buffer, sizeof(uint8_t), *m_BufferSize, m_File);
            fclose(m_File);
            return true;
        }

        return false;
    }

    bool Write(const char* m_Path, const char* m_Mode, uint8_t* m_Buffer, size_t m_BufferSize)
    {
        FILE* m_File = fopen(m_Path, m_Mode);
        if (m_File)
        {
            fwrite(m_Buffer, sizeof(uint8_t), m_BufferSize, m_File);
            fclose(m_File);
            return true;
        }

        return false;
    }
}

#define STRUCT_PAD_INSERT(x, y) x ## y
#define STRUCT_PAD_DEFINE(x, y) STRUCT_PAD_INSERT(x, y)
#define STRUCT_PAD(size) char STRUCT_PAD_DEFINE(padding_, __LINE__)[size]
#include "BigFile.hpp"
#include "BixFile.hpp"

namespace FileTypDetection
{
    static std::vector<std::map<std::string, std::vector<uint8_t>>> m_List =
    {
        {{ "Texture", 
            { 0xD7, 0xCD, 0x73, 0x5E }
        }}
    };

    std::string GetName(size_t m_Index)
    {
        if (m_Index != -1)
            return m_List[m_Index].begin()->first;

        return "Unknown";
    }

    size_t Get(uint8_t* m_Buffer)
    {
        for (size_t i = 0; m_List.size() > i; ++i)
        {
            std::vector<uint8_t>& m_ByteArray = m_List[i].begin()->second;
            if (memcmp(m_Buffer, m_ByteArray.data(), m_ByteArray.size()) == 0)
                return i;
        }

        return -1;
    }
}

namespace Globals
{
    struct File_t
    {
        BigFile_t* Big = nullptr;
        size_t BigSize = 0;

        BixFile_t* Bix = nullptr;
        size_t BixSize = 0;

        std::map<uint32_t, size_t> EntryTypes;

        void Initialize()
        {
            for (uint32_t i = 0; Bix->EntryCount > i; ++i)
            {
                BigFileIndexEntry_t* m_BixEntry = Bix->GetEntry(i);
                if (!m_BixEntry->IsValid())
                    continue;

                BigFileEntry_t* m_BigEntry = Big->GetEntry(m_BixEntry->GetFileOffset());
                if (!m_BigEntry->IsValid())
                    continue;

                uint8_t* m_FileBuffer = nullptr;
                if (!m_BigEntry->Get(&m_FileBuffer))
                    continue;

                EntryTypes[i] = FileTypDetection::Get(m_FileBuffer);

                free(m_FileBuffer);
            }
        }
    };
    std::map<std::string, File_t> m_FileList;

    void TryLoadFile(std::string m_FilePath)
    {
        std::string m_FilePathNoExt = m_FilePath.substr(0, m_FilePath.find_last_of('.'));
        std::string m_FileName      = m_FilePathNoExt; m_FileName.erase(0, m_FileName.find_last_of("/\\") + 1);

        File_t m_File;
        if (!FileWrapper::Read(&(m_FilePathNoExt + ".bix")[0], "rb", reinterpret_cast<uint8_t**>(&m_File.Bix), &m_File.BixSize))
        {
            MessageBoxA(0, "Couldn't open .bix file!", "BigFileExplorer", MB_OK | MB_ICONERROR);
            return;
        }

        if (!m_File.Bix->IsValid())
        {
            MessageBoxA(0, "The bix file is not valid!", "BigFileExplorer", MB_OK | MB_ICONERROR);
            return;
        }

        if (!FileWrapper::Read(&(m_FilePathNoExt + ".big")[0], "rb", reinterpret_cast<uint8_t**>(&m_File.Big), &m_File.BigSize))
        {
            MessageBoxA(0, "Couldn't open .big file!", "BigFileExplorer", MB_OK | MB_ICONERROR);
            return;
        }

        m_File.Initialize();
        m_FileList[m_FileName] = m_File;
    }

    struct TexturePreview_t
    {
        ID3D11ShaderResourceView* Resource = nullptr;
        int Width   = 0;
        int Height  = 0;
        int Size    = 0;

        void Load(uint8_t* m_TextureBuffer, int m_Width, int m_Height)
        {
            if (Resource)
            {
                Resource->Release();
                Resource = nullptr;
            }

            uint8_t* m_PNGBuffer = Wrappers::ConvertTextureToPNG(m_TextureBuffer, m_Width, m_Height, &Size);
            if (m_PNGBuffer)
            {
                Wrappers::LoadTextureFromMemory(m_PNGBuffer, Size, &Resource, &Width, &Height);
                free(m_PNGBuffer);
            }
        }

        void Test()
        {
            Wrappers::CleanupDeviceD3D();
        }
    };
    static TexturePreview_t TexturePreview;
}

#define WINDOW_SIZE_X 1000
#define WINDOW_SIZE_Y 600

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    WNDCLASSEXA m_WindowClass = { sizeof(WNDCLASSEXA), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(0), 0, 0, 0, 0, "BigFileExplorer_sneakyevil", 0 };
    RegisterClassExA(&m_WindowClass);

    HWND m_Window = CreateWindowA(m_WindowClass.lpszClassName, "BigFileExplorer", WS_POPUP, 0, 0, WINDOW_SIZE_X, WINDOW_SIZE_Y, 0, 0, m_WindowClass.hInstance, 0);
    if (!Wrappers::CreateDeviceD3D(m_Window))
    {
        Wrappers::CleanupDeviceD3D();
        UnregisterClassA(m_WindowClass.lpszClassName, m_WindowClass.hInstance);
        return 1;
    }

    MARGINS m_Margins = { -1 };
    DwmExtendFrameIntoClientArea(m_Window, &m_Margins);

    ShowWindow(m_Window, SW_SHOWDEFAULT);
    UpdateWindow(m_Window);

    ImGui::CreateContext();

    ImGuiIO& m_IO = ImGui::GetIO();
    m_IO.IniFilename = nullptr;

    ImGui::StyleColorsDark();
    Wrappers::StyleColors();

    ImGui_ImplWin32_Init(m_Window);
    ImGui_ImplDX11_Init(g_D3DDevice, g_D3DDeviceContext);

    bool m_Render = true;
    while (m_Render)
    {
        MSG m_Msg;
        while (PeekMessageA(&m_Msg, 0, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&m_Msg);
            DispatchMessageA(&m_Msg);
            if (m_Msg.message == WM_QUIT)
                m_Render = false;
        }
        if (!m_Render)
            break;

        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        ImGui::SetNextWindowPos(ImVec2(0.f, 0.f));
        ImGui::SetNextWindowSize(ImVec2(static_cast<float>(WINDOW_SIZE_X), static_cast<float>(WINDOW_SIZE_Y)));
        if (ImGui::Begin("BigFileExplorer", &m_Render, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_MenuBar))
        {
            if (ImGui::BeginMenuBar())
            {
                if (ImGui::BeginMenu("File"))
                {
                    if (ImGui::MenuItem("Import"))
                    {
                        char m_OpenFilePath[MAX_PATH];
                        memset(m_OpenFilePath, 0, sizeof(m_OpenFilePath));

                        OPENFILENAMEA m_OpenFile = { 0 };
                        m_OpenFile.lStructSize      = sizeof(OPENFILENAMEA);
                        m_OpenFile.hwndOwner        = m_Window;
                        m_OpenFile.lpstrFile        = m_OpenFilePath;
                        m_OpenFile.nMaxFile         = sizeof(m_OpenFilePath);
                        m_OpenFile.lpstrFilter      = "Big File (.bix/.big)\0*.BIG;*.BIX\0";
                        m_OpenFile.nFilterIndex     = 1;
                        m_OpenFile.lpstrFileTitle   = 0;
                        m_OpenFile.nMaxFileTitle    = 0;
                        m_OpenFile.lpstrInitialDir  = 0;
                        m_OpenFile.Flags            = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

                        if (GetOpenFileNameA(&m_OpenFile) == TRUE)
                            Globals::TryLoadFile(m_OpenFilePath);
                    }

                    if (ImGui::MenuItem("Close All"))
                    {
                        for (auto& m_File : Globals::m_FileList)
                        {
                            free(m_File.second.Bix);
                            free(m_File.second.Big);
                        }
                        Globals::m_FileList.clear();
                    }
                    ImGui::EndMenu();
                }

                ImGui::EndMenuBar();
            }

            static std::string m_SelectedFile   = "";
            static uint32_t m_SelectedFileID    = -1;
            bool m_NewFileSelected = false;

            if (ImGui::BeginChild("##FileList", ImVec2(static_cast<float>(WINDOW_SIZE_X) * 0.45f, 0.f), true))
            {
                for (auto& m_Pair : Globals::m_FileList)
                {
                    if (ImGui::TreeNode(m_Pair.first.c_str()))
                    {
                        auto& m_File = m_Pair.second;
                        bool m_IsCurrentFileSelected = (m_SelectedFile == m_Pair.first);

                        for (uint32_t i = 0; m_File.Bix->EntryCount > i; ++i)
                        {
                            BigFileIndexEntry_t* m_BixEntry = m_File.Bix->GetEntry(i);
                            if (!m_BixEntry->IsValid())
                                continue;

                            char m_FormatFileName[32];
                            sprintf_s(m_FormatFileName, sizeof(m_FormatFileName), "0x%X", m_BixEntry->Hash);

                            bool m_Dummy = (m_IsCurrentFileSelected && m_SelectedFileID == i);
                            if (ImGui::Selectable(m_FormatFileName, &m_Dummy))
                            {
                                m_SelectedFile      = m_Pair.first;
                                m_SelectedFileID    = i;
                                m_NewFileSelected   = true;
                            }
                            ImGui::SameLine(200);
                            ImGui::Text("(%s)", &FileTypDetection::GetName(m_File.EntryTypes[i])[0]);
                            ImGui::SameLine(300); 
                            ImGui::Text("%u bytes", m_BixEntry->UncompressedSize);
                        }

                        ImGui::TreePop();
                    }
                }

                ImGui::EndChild();
            }

            ImGui::SameLine();
            if (ImGui::BeginChild("##FileInfo", ImVec2(0.f, 0.f), false))
            {
                auto m_FilePair = Globals::m_FileList.find(m_SelectedFile);
                if (m_FilePair != Globals::m_FileList.end() && m_SelectedFileID != -1)
                {
                    auto& m_File = m_FilePair->second;

                    BigFileIndexEntry_t* m_BixEntry = m_File.Bix->GetEntry(m_SelectedFileID);
                    char m_FormatFileName[32];
                    sprintf_s(m_FormatFileName, sizeof(m_FormatFileName), "0x%X", m_BixEntry->Hash);

                    ImGui::Text("Current selected file: %s\\%s", &m_SelectedFile[0], m_FormatFileName);

                    if (FileTypDetection::GetName(m_File.EntryTypes[m_SelectedFileID]) == "Texture")
                    {
                        bool m_ReloadTexture = m_NewFileSelected;

                        static int m_WidthTexture   = 128;
                        static int m_HeightTexture  = 128;

                        ImGui::Text("Texture X: ");
                        for (int i = 0; 5 > i; ++i)
                        {
                            ImGui::SameLine();
                            int m_Value = (128 << i);
                            std::string m_RadioText = std::to_string(m_Value) + "##WidthTexture";
                            if (ImGui::RadioButton(&m_RadioText[0], m_WidthTexture == m_Value))
                            {
                                m_ReloadTexture = true;
                                m_WidthTexture = m_Value;
                            }
                        }

                        ImGui::Text("Texture Y: ");
                        for (int i = 0; 5 > i; ++i)
                        {
                            ImGui::SameLine();
                            int m_Value = (128 << i);
                            std::string m_RadioText = std::to_string(m_Value) + "##HeightTexture";
                            if (ImGui::RadioButton(&m_RadioText[0], m_HeightTexture == m_Value))
                            {
                                m_ReloadTexture = true;
                                m_HeightTexture = m_Value;
                            }
                        }

                        if (m_ReloadTexture)
                        {
                            BigFileEntry_t* m_BigEntry = m_File.Big->GetEntry(m_BixEntry->GetFileOffset());
                            uint8_t* m_TextureBuffer = nullptr;
                            if (m_BigEntry->Get(&m_TextureBuffer))
                            {
                                int m_TotalSize = (*reinterpret_cast<int*>(&m_TextureBuffer[0x4]) / 4);
                                int m_TextureSize[2] = { m_WidthTexture, m_HeightTexture };
                                while ((m_TextureSize[0] * m_TextureSize[1]) >= m_TotalSize)
                                {
                                    m_TextureSize[0] /= 2;
                                    m_TextureSize[1] /= 2;
                                }

                                Globals::TexturePreview.Load(&m_TextureBuffer[0x10], m_TextureSize[0], m_TextureSize[1]);
                                free(m_TextureBuffer);
                            }
                        }

                        static ImVec2 m_TexturePreviewSize(400.f, 400.f);
                        if (Globals::TexturePreview.Resource)
                        {
                            if (ImGui::BeginChild("##TexturePreview", m_TexturePreviewSize, true))
                            {
                                ImVec2 m_Size(static_cast<float>(Globals::TexturePreview.Width), static_cast<float>(Globals::TexturePreview.Height));
                                if (m_Size.x > m_TexturePreviewSize.x)
                                {
                                    float m_Calc = m_Size.y / m_Size.x;
                                    m_Size.x = m_TexturePreviewSize.x;
                                    m_Size.y = floorf(m_Size.x * m_Calc);
                                }

                                if (m_Size.y > m_TexturePreviewSize.y)
                                {
                                    float m_Calc = m_Size.x / m_Size.y;
                                    m_Size.y = m_TexturePreviewSize.y;
                                    m_Size.x = floorf(m_Size.y * m_Calc);
                                }

                                ImGui::Image(Globals::TexturePreview.Resource, m_Size);

                                ImGui::EndChild();
                            }

                            if (ImGui::Button("Save as PNG##Texture"))
                            {
                                char m_SaveFilePath[MAX_PATH];
                                memset(m_SaveFilePath, 0, sizeof(m_SaveFilePath));

                                OPENFILENAMEA m_OpenFile = { 0 };
                                m_OpenFile.lStructSize = sizeof(OPENFILENAMEA);
                                m_OpenFile.hwndOwner = m_Window;
                                m_OpenFile.lpstrFile = m_SaveFilePath;
                                m_OpenFile.nMaxFile = sizeof(m_SaveFilePath);
                                m_OpenFile.lpstrFilter = "PNG File (.png)\0*.PNG\0";
                                m_OpenFile.nFilterIndex = 1;
                                m_OpenFile.lpstrFileTitle = 0;
                                m_OpenFile.nMaxFileTitle = 0;
                                m_OpenFile.lpstrInitialDir = 0;
                                m_OpenFile.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

                                if (GetSaveFileNameA(&m_OpenFile) == TRUE)
                                {
                                    std::string m_SavePNGPath = m_SaveFilePath;
                                    if (m_SavePNGPath.find(".png") == std::string::npos)
                                        m_SavePNGPath += ".png";

                                    BigFileEntry_t* m_BigEntry = m_File.Big->GetEntry(m_BixEntry->GetFileOffset());
                                    uint8_t* m_TextureBuffer = nullptr;
                                    if (m_BigEntry->Get(&m_TextureBuffer))
                                    {
                                        int m_PNGSize = 0;
                                        uint8_t* m_PNGBuffer = Wrappers::ConvertTextureToPNG(&m_TextureBuffer[0x10], Globals::TexturePreview.Width, Globals::TexturePreview.Height, &m_PNGSize);
                                        if (m_PNGBuffer)
                                        {
                                            FileWrapper::Write(&m_SavePNGPath[0], "wb", m_PNGBuffer, static_cast<size_t>(m_PNGSize));
                                            free(m_PNGBuffer);
                                        }

                                        free(m_TextureBuffer);
                                    }
                                }
                            }
                        }
                    }
                }

                ImGui::EndChild();
            }

            ImGui::End();
        }

      
        ImGui::Render(); 
        static float m_Clear[4] = { 0.f, 0.f, 0.f, 0.f };
        g_D3DDeviceContext->ClearRenderTargetView(g_MainRenderTargetView, m_Clear);
        g_D3DDeviceContext->OMSetRenderTargets(1, &g_MainRenderTargetView, 0);
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

        g_SwapChain->Present(1, 0);
    }

    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    Wrappers::CleanupDeviceD3D();
    DestroyWindow(m_Window);
    UnregisterClassA(m_WindowClass.lpszClassName, m_WindowClass.hInstance);

    return 0;
}