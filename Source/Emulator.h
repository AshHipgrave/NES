#pragma once

class Cpu;
class Memory;

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
    void InitImGui();

    void HandleEvents();

    void Clear();
    void Draw();
    void Present();

private:
    bool m_bIsRunning = false;

    std::unique_ptr<Cpu> m_pCpu;
    std::shared_ptr<Memory> m_pMemory;

    std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)> m_pWindow;
    std::unique_ptr<SDL_Renderer, decltype(&SDL_DestroyRenderer)> m_pRenderer;
};