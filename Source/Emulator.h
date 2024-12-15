#pragma once

class Cpu;
class PPU;
class Bus;
class Memory;
class UIManager;

struct SDL_Window;
struct SDL_Renderer;

class Emulator
{
public:
    Emulator();
    ~Emulator();

    void Run();
    void Stop();

private:
    bool Init();
    bool InitSDL();

    void HandleEvents();

    void Clear();
    void Draw();
    void Present();

private:
    bool m_bIsRunning = false;

    std::unique_ptr<Bus> m_pDataBus;
    std::unique_ptr<UIManager> m_pUIManager;

    std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)> m_pWindow;
    std::unique_ptr<SDL_Renderer, decltype(&SDL_DestroyRenderer)> m_pRenderer;
};