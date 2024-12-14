#include "pch.h"
#include "Emulator.h"
#include "CPU/6502.h"
#include "System/Bus.h"
#include "System/Memory.h"
#include "ImGui/imgui_impl_sdl2.h"
#include "ImGui/imgui_impl_sdlrenderer2.h"
#include "ImGui/ImGuiFileDialog.h"

Emulator::Emulator()
    : m_pCpu(nullptr)
    , m_pMemory(nullptr)
    , m_pDataBus(nullptr)
    , m_pWindow(nullptr, SDL_DestroyWindow)
    , m_pRenderer(nullptr, SDL_DestroyRenderer)
{
}

Emulator::~Emulator()
{
    if (m_pRenderer)
    {
        m_pRenderer.release();
        m_pRenderer = nullptr;
    }

    if (m_pWindow)
    {
        m_pWindow.release();
        m_pWindow = nullptr;
    }

    ::SDL_Quit();

    m_pCpu = nullptr;
    m_pDataBus = nullptr;
    m_pMemory = nullptr;
}

void Emulator::Run()
{
    if (Init() == false)
    {
        std::cout << "Error: Failed to initialise emualtor. See output for more information." << std::endl;
        return;
    }

    m_bIsRunning = true;

    while (m_bIsRunning)
    {
        HandleEvents();

        // Re-check because 'HandleEvents' can set this to false.
        if (m_bIsRunning)
        {
            if (SDL_GetWindowFlags(m_pWindow.get()) & SDL_WINDOW_MINIMIZED)
            {
                SDL_Delay(50);
                continue;
            }

            // TODO: PPU

            // TODO: Only run for the number of cycles that can execute during a single frame
            m_pCpu->Tick();

            Clear();
            Draw();
            Present();
        }
    }
}

void Emulator::Stop()
{
    m_bIsRunning = false;
}

bool Emulator::Init()
{
    m_pMemory = std::make_shared<Memory>();
    m_pDataBus = std::make_shared<Bus>(m_pMemory);

    m_pCpu = std::make_unique<Cpu>(m_pDataBus);

    if (InitSDL())
    {
        InitImGui();

        return true;
    }

    return false;
}

bool Emulator::InitSDL()
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        std::cout << "Error: Failed to initialise SDL2: " << SDL_GetError() << std::endl;
        return false;
    }

    m_pWindow.reset(
        SDL_CreateWindow("NES Emulator", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1920, 1080, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI));

    if (m_pWindow == nullptr)
    {
        std::cout << "Error: Failed to create an SDL2 window: " << SDL_GetError() << std::endl;
        return false;
    }

    m_pRenderer.reset(
        SDL_CreateRenderer(m_pWindow.get(), -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC));

    if (m_pRenderer == nullptr)
    {
        std::cout << "Error: Failed to initialise SDL2 renderer: " << SDL_GetError() << std::endl;
        return false;
    }

    return true;
}

void Emulator::InitImGui()
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

    ImGui::StyleColorsDark();

    ImGui_ImplSDL2_InitForSDLRenderer(m_pWindow.get(), m_pRenderer.get());
    ImGui_ImplSDLRenderer2_Init(m_pRenderer.get());
}

void Emulator::HandleEvents()
{
    SDL_Event sdlEvent;

    while (SDL_PollEvent(&sdlEvent))
    {
        ImGui_ImplSDL2_ProcessEvent(&sdlEvent);

        switch (sdlEvent.type)
        {
            case SDL_QUIT:
            case SDL_WINDOWEVENT_CLOSE:
            {
                Stop();
                break;
            }
        }
    }
}

void Emulator::Clear()
{
    SDL_RenderClear(m_pRenderer.get());

    ImGui_ImplSDLRenderer2_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();
}

void Emulator::Draw()
{
    static bool bShowDemoWindow = true;
    ImGui::ShowDemoWindow(&bShowDemoWindow);

    if (ImGui::Button("Open File Dialog"))
    {
        IGFD::FileDialogConfig config;
        config.path = ".";
        ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey", "Choose File", ".cpp,.h,.hpp", config);
    }
    // display
    if (ImGuiFileDialog::Instance()->Display("ChooseFileDlgKey"))
    {
        if (ImGuiFileDialog::Instance()->IsOk())
        { // action if OK
            std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
            std::string filePath = ImGuiFileDialog::Instance()->GetCurrentPath();
            // action
        }

        // close
        ImGuiFileDialog::Instance()->Close();
    }
}

void Emulator::Present()
{
    ImGuiIO& io = ImGui::GetIO();

    ImGui::Render();
    SDL_SetRenderDrawColor(m_pRenderer.get(), 100, 149, 237, 255);
    SDL_RenderSetScale(m_pRenderer.get(), io.DisplayFramebufferScale.x, io.DisplayFramebufferScale.y);
    SDL_RenderClear(m_pRenderer.get());
    ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), m_pRenderer.get());
    SDL_RenderPresent(m_pRenderer.get());
}
