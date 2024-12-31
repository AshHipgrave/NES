#include "pch.h"
#include "Emulator.h"
#include "System/Bus.h"
#include "Logging/Log.h"
#include "UI/UIManager.h"

Emulator::Emulator()
    : m_pDataBus(nullptr)
    , m_pUIManager(nullptr)
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

    m_pDataBus = nullptr;
}

void Emulator::Run()
{
    if (Init() == false)
    {
        EMULATOR_LOG_FATAL("Failed to initialise emulator. See output for more information.");
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

            if (m_pDataBus->HasCartridgeLoaded())
            {
                m_pDataBus->Tick();
            }

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
    m_pDataBus = std::make_unique<Bus>();
    m_pDataBus->InitDevices();

    m_pUIManager = std::make_unique<UIManager>();

    if (InitSDL())
    {
        m_pUIManager->Init(m_pWindow.get(), m_pRenderer.get());

        return true;
    }

    return false;
}

bool Emulator::InitSDL()
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        EMULATOR_LOG_FATAL("Failed to initialise SDL2: {}", SDL_GetError());
    }

    m_pWindow.reset(
        SDL_CreateWindow("NES Emulator", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1920, 1080, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI));

    if (m_pWindow == nullptr)
    {
        EMULATOR_LOG_FATAL("Failed to create an SDL2 window: {}", SDL_GetError());
    }

    m_pRenderer.reset(
        SDL_CreateRenderer(m_pWindow.get(), -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC));

    if (m_pRenderer == nullptr)
    {
        EMULATOR_LOG_FATAL("Failed to initialise SDL2 renderer: {}", SDL_GetError());
    }

    SDL_SetRenderDrawColor(m_pRenderer.get(), 100, 149, 237, 255);

    return true;
}

void Emulator::HandleEvents()
{
    SDL_Event sdlEvent;

    while (SDL_PollEvent(&sdlEvent))
    {
        m_pUIManager->HandleUIEvents(&sdlEvent);

        switch (sdlEvent.type)
        {
            case SDL_QUIT:
            case SDL_WINDOWEVENT_CLOSE:
            {
                Stop();
                break;
            }
            case SDL_KEYDOWN:
            {
                if (m_pDataBus->GetIsSingleStepEnabled() && sdlEvent.key.keysym.sym == SDLK_F5)
                {
                    m_pDataBus->SetCanPerformSingleStep();
                }
                break;
            }
        }
    }
}

void Emulator::Clear()
{
    SDL_RenderClear(m_pRenderer.get());

    m_pUIManager->Clear();
}

void Emulator::Draw()
{
    m_pUIManager->Draw();
}

void Emulator::Present()
{
    m_pUIManager->Present(m_pRenderer.get());
}
