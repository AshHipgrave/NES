#include "pch.h"
#include "UI/UIManager.h"
#include "CPU/6502.h"
#include "System/Bus.h"
#include "System/Memory.h"
#include "System/Cartridge.h"
#include "Core/Utils.h"
#include "Core/Core.h"

UIManager::UIManager()
{
#if EMULATOR_DEBUG_BUILD
    m_bShowMemoryViewer = true;
    m_bShowCpuStatusOverlay = true;
    m_bShowProgramMemoryViewer = true;
    m_bShowCharacterMemoryViewer = true;

    m_bEnableSingleStepMode = true;
#endif
}

UIManager::~UIManager()
{
    if (m_pMemoryViewer)
    {
        delete m_pMemoryViewer;
        m_pMemoryViewer = nullptr;
    }
}

void UIManager::Init(SDL_Window* InWindow, SDL_Renderer* InRenderer)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

    ImGui::StyleColorsDark();

    ImGui_ImplSDL2_InitForSDLRenderer(InWindow, InRenderer);
    ImGui_ImplSDLRenderer2_Init(InRenderer);

    SDL_RenderSetScale(InRenderer, io.DisplayFramebufferScale.x, io.DisplayFramebufferScale.y);

    m_pMemoryViewer = new MemoryEditor();
    m_pMemoryViewer->ReadOnly = true;

    m_pProgramMemoryViewer = new MemoryEditor();
    m_pProgramMemoryViewer->ReadOnly = true;

    m_pCharacterMemoryViewer = new MemoryEditor();
    m_pCharacterMemoryViewer->ReadOnly = true;
}

void UIManager::HandleUIEvents(const SDL_Event* InEvent)
{
    ImGui_ImplSDL2_ProcessEvent(InEvent);
}

void UIManager::Update()
{

}

void UIManager::Clear()
{
    ImGui_ImplSDLRenderer2_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();
}

void UIManager::Draw()
{
    DrawMainMenu();

    if (m_bShowCpuStatusOverlay)
        DrawCpuStatusOverlay();

    if (m_bShowLoadROMDialog)
        DrawLoadROMDialog();

    Bus* bus = Bus::Get();
    bus->SetSingleStepModeEnabled(m_bEnableSingleStepMode); // TODO: Hack - The UI's Draw function shouldn't change the state of another class.

    if (m_bShowMemoryViewer)
        m_pMemoryViewer->DrawWindow("Memory Viewer", bus->GetMemory()->m_MemoryBuffer, 2048);

    if (m_bShowProgramMemoryViewer && bus->HasCartridgeLoaded())
        m_pProgramMemoryViewer->DrawWindow("Program RAM", bus->GetCartridge()->m_ProgramROM.data(), bus->GetCartridge()->m_ProgramROM.size());

    if (m_bShowCharacterMemoryViewer && bus->HasCartridgeLoaded())
        m_pProgramMemoryViewer->DrawWindow("Character RAM", bus->GetCartridge()->m_CharacterROM.data(), bus->GetCartridge()->m_CharacterROM.size());

    if (m_bShowImGuiDemoWindow)
        ImGui::ShowDemoWindow(&m_bShowImGuiDemoWindow);
}

void UIManager::Present(SDL_Renderer* InRenderer)
{
    ImGui::Render();
    ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), InRenderer);
    SDL_RenderPresent(InRenderer);
}

void UIManager::DrawMainMenu()
{
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("Load ROM"))
            {
                IGFD::FileDialogConfig config;
                config.path = ".";

                if (std::filesystem::is_directory("./ROMs"))
                    config.path = "./ROMs";

                ImGuiFileDialog::Instance()->OpenDialog("LoadRomKey", "Load ROM File", ".nes,.rom", config);
                
                m_bShowLoadROMDialog = true;
            }

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Debug"))
        {
            ImGui::MenuItem("Enable Single-Step Mode", NULL, &m_bEnableSingleStepMode);
            ImGui::MenuItem("Enable CPU Status Overlay", NULL, &m_bShowCpuStatusOverlay);
            ImGui::MenuItem("Enable Memory Viewer", NULL, &m_bShowMemoryViewer);
            ImGui::MenuItem("Enable Program RAM Viewer", NULL, &m_bShowProgramMemoryViewer);
            ImGui::MenuItem("Enable Character RAM Viewer", NULL, &m_bShowCharacterMemoryViewer);

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Help"))
        {
            ImGui::MenuItem("Show ImGui Demo", NULL, &m_bShowImGuiDemoWindow);

            ImGui::EndMenu();
        }
    }

    ImGui::EndMainMenuBar();
}

void UIManager::DrawCpuStatusOverlay()
{
    ImGuiIO& io = ImGui::GetIO();

    ImVec2 windowPosition = ImVec2(10.0f, io.DisplaySize.y - 10.0f);
    ImVec2 windowPostionPivot = ImVec2(0.0f, 1.0f);

    ImGui::SetNextWindowPos(windowPosition, ImGuiCond_Always, windowPostionPivot);
    ImGui::SetNextWindowBgAlpha(0.75f);

    ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;

    if (ImGui::Begin("Register View", &m_bShowCpuStatusOverlay, windowFlags))
    {
        const char* format_byte_space = "%02X ";

        const Cpu* cpu = Bus::Get()->GetCPU();
        const CpuRegisters registers = cpu->GetRegisters();

        const ImU16 pc = registers.ProgramCounter;
        ImGui::Text("Program Counter: ");
        ImGui::SameLine();
        ImGui::Text(format_byte_space, pc);

        const ImU8 sp = registers.StackPointer;
        ImGui::Text("Stack Pointer:     ");
        ImGui::SameLine();
        ImGui::Text(format_byte_space, sp);

        const ImU8 accumulator = registers.Accumulator;
        ImGui::Text("Accumulator:       ");
        ImGui::SameLine();
        ImGui::Text(format_byte_space, accumulator);

        const ImU8 xReg = registers.X;
        ImGui::Text("X Register:        ");
        ImGui::SameLine();
        ImGui::Text(format_byte_space, xReg);

        const ImU8 yReg = registers.Y;
        ImGui::Text("Y Register:        ");
        ImGui::SameLine();
        ImGui::Text(format_byte_space, yReg);

        ImGui::Separator();

        for (int i = 0; i < 8; i++)
        {
            const ECpuFlag flag = static_cast<ECpuFlag>(i);

            const ImU8 flagBit = registers.IsFlagSet(flag) ? 1 : 0;
            const std::string flagStr = Utils::ConvertFlagToString(flag);

            ImGui::Text("%s .................. %d", flagStr.c_str(), flagBit);
        }
    }

    ImGui::End();
}

void UIManager::DrawLoadROMDialog()
{
    ImGuiIO& io = ImGui::GetIO();
    ImVec2 windowSize = ImVec2(io.DisplaySize.x / 2.0f, io.DisplaySize.y / 2.0f);

    if (ImGuiFileDialog::Instance()->Display("LoadRomKey", ImGuiWindowFlags_NoCollapse, windowSize))
    {
        if (ImGuiFileDialog::Instance()->IsOk())
        {
            const std::string romFile = ImGuiFileDialog::Instance()->GetFilePathName();

            Bus::Get()->LoadCartridge(romFile);
        }

        ImGuiFileDialog::Instance()->Close();
        m_bShowLoadROMDialog = false;
    }
}
