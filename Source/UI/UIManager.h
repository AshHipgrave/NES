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
    // Toggles
    bool m_bShowLoadROMDialog = false;
    bool m_bShowCpuStatusOverlay = false;
    bool m_bEnableSingleStepMode = false;
    bool m_bShowImGuiDemoWindow = false;

    // Memory Viewers
    bool m_bWorkRAMMemoryViewer = false;
    bool m_bShowProgramMemoryViewer = false;
    bool m_bShowCharacterMemoryViewer = false;

    // PPU Memory Viewers
    bool m_bShowOAMDataMemoryViewer = false;
    bool m_bShowNametableMemoryViewer = false;

    MemoryEditor* m_pWorkRAMViewer = nullptr;
    MemoryEditor* m_pProgramMemoryViewer = nullptr;
    MemoryEditor* m_pCharacterMemoryViewer = nullptr;
    MemoryEditor* m_pNametableMemoryViewer = nullptr;
    MemoryEditor* m_pOAMDataMemoryViewer = nullptr;
};