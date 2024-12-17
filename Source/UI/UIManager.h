#pragma once

struct MemoryEditor;

class UIManager
{
public:
    UIManager();
    ~UIManager();

    void Init(SDL_Window* InWindow, SDL_Renderer* InRenderer);

    void HandleUIEvents(const SDL_Event* InEvent);

    void Update();

    void Clear();
    void Draw();
    void Present(SDL_Renderer* InRenderer);

private:
    void DrawMainMenu();
    void DrawCpuStatusOverlay();
    void DrawLoadROMDialog();

private:
    bool m_bShowMemoryViewer = false;
    bool m_bShowLoadROMDialog = false;
    bool m_bShowCpuStatusOverlay = false;
    bool m_bShowImGuiDemoWindow = false;
    bool m_bEnableSingleStepMode = false;

    bool m_bShowProgramMemoryViewer = false;
    bool m_bShowCharacterMemoryViewer = false;

    MemoryEditor* m_pWorkRAMViewer = nullptr;
    MemoryEditor* m_pProgramMemoryViewer = nullptr;
    MemoryEditor* m_pCharacterMemoryViewer = nullptr;
};