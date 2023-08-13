#include "Drawing.h"
#include "GuiUtils.hpp"
#include "Owners.hpp"
#include "InOut.hpp"
#include "Logger.hpp"
#include <algorithm>

LPCSTR Drawing::lpWindowName = "SOTF DS Configurator";
ImVec2 Drawing::vWindowSize = { 540, 820 };
ImGuiWindowFlags Drawing::WindowFlags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_MenuBar;
bool Drawing::bDraw = true;

void Drawing::Active()
{
	bDraw = true;
}

bool Drawing::isActive()
{
	return bDraw == true;
}

void Drawing::Draw()
{
	if (isActive())
	{
		style = &ImGui::GetStyle();
		style->WindowTitleAlign = ImVec2(0.5f, 0.5f);
		style->ChildRounding = 5.0f;
		style->FrameRounding = 3.0f;
		style->Colors[ImGuiCol_TitleBg] = ImColor(100, 195, 85);
		style->Colors[ImGuiCol_TitleBgActive] = ImColor(100, 195, 85);
		style->Colors[ImGuiCol_TitleBgCollapsed] = ImColor(100, 195, 85);
		style->Colors[ImGuiCol_ChildBg] = ImColor(24, 24, 24);
		style->Colors[ImGuiCol_Text] = ImColor(0, 0, 0);
		style->Colors[ImGuiCol_WindowBg] = ImColor(56, 56, 56);
		style->Colors[ImGuiCol_FrameBg] = ImColor(34, 34, 34);
		style->Colors[ImGuiCol_Button] = ImColor(Colors::red);
		style->Colors[ImGuiCol_ButtonHovered] = ImColor(Colors::red - 30);
		style->WindowBorderSize = 1.0f;
		style->Colors[ImGuiCol_Border] = ImColor(24, 24, 24);
		style->FrameBorderSize = 0.1f;

		if (InOut::AskOverwrite)
		{			
			ImGui::SetNextWindowSize(ImVec2(530, 250));
			ImGui::SetCursorPos(ImVec2(5, 220));
			ImGui::Begin("Warning", NULL, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoResize);
			style->Colors[ImGuiCol_Text] = ImColor(255, 255, 255);
			ImGui::Text("It seems a previous dedicated server config already exist, to proceed it\nneeds to be overwritten. Relaunch the program after pressing OK");
			if (ImGui::Button("OK"))
			{
				InOut::OverwriteOgFiles();
				ExitProcess(0);
			}
			ImGui::SameLine();
			if (ImGui::Button("CANCEL"))
			{
				InOut::RestoreFirstRun();
				ExitProcess(0);
			}
			style->Colors[ImGuiCol_Text] = ImColor(0, 0, 0);
			ImGui::End();			
			return;
		}

		ImGui::SetNextWindowSize(vWindowSize, ImGuiCond_Once);
		ImGui::SetNextWindowBgAlpha(1.0f);
		ImGui::Begin(lpWindowName, &bDraw, WindowFlags);
		{
			style->Colors[ImGuiCol_Border] = ImColor(50, 50, 50);
			style->Colors[ImGuiCol_Text] = ImColor(255, 255, 255, 210);

			ImGui::BeginMenuBar();
			ImGui::Text("About");
			if (ImGui::IsItemHovered(ImGuiHoveredFlags_None))
			{
				ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
				if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
					showAbout = !showAbout;
			}

			ImGui::Text("Log");
			if (ImGui::IsItemHovered(ImGuiHoveredFlags_None))
			{
				ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
				if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
					showLog = !showLog;
			}
			ImGui::EndMenuBar();

			ImGui::Text("Server owners"); ImGui::SameLine();
			style->FrameBorderSize = 0.0f;
			PushButtonCol(Colors::green, Colors::greenHov);
			if (ImGui::SmallButton("+"))
				Owners::AddOwner();

			PushButtonCol(Colors::red, Colors::redHov);
			ImGui::BeginChild("owners", ImVec2(ImGui::GetContentRegionAvail().x, 167), true);
			{
				for (int i{ 0 }; i < Owners::ownersNameBuff.size(); ++i)
				{
					ImGui::PushID(i);

					if (ImGui::SmallButton("X"))
					{
						Owners::RemoveOwnerAt(i);
						ImGui::PopID();
						break;
					}

					style->FrameBorderSize = 0.1f;
					ImGui::InputText("Name", &Owners::ownersNameBuff.at(i), ImGuiInputTextFlags_CharsNoBlank);
					ImGui::InputText("Steam ID (17 digits)", &Owners::ownersIdBuff.at(i), ImGuiInputTextFlags_CharsDecimal | ImGuiInputTextFlags_CharsNoBlank);
					style->FrameBorderSize = 0.0f;

					ImGui::PopID();

					ImGui::Dummy(ImVec2(0, 5));
				}
			}		
			ImGui::EndChild();

			ImGui::Dummy(ImVec2(0, 5));

			ImGui::InputText("Server name", &ServerConfig::ServerName);
			ImGui::SliderInt("Max Players", &ServerConfig::MaxPlayers, 2, 8);
			(showPassword) ? ImGui::InputText("Password", &ServerConfig::Password) : ImGui::InputText("Password", &ServerConfig::Password, ImGuiInputTextFlags_Password);
			ImGui::SameLine();
			if (ImGui::SmallButton("Show"))
			{
				showPassword = !showPassword;
			}
			ImGui::Checkbox("Lan only", &ServerConfig::LanOnly); InfoBox("Allows or restricts the server visibility to LAN only.");
			if (ImGui::BeginCombo("GameMode", ServerConfig::GameModes[ServerConfig::GameMode_Selected].c_str()))
			{
				unsigned int index = 0;
				for (auto mode : ServerConfig::GameModes)
				{
					if (ImGui::Selectable(mode.c_str()))
						ServerConfig::GameMode_Selected = index;

					index++;
				}
				ImGui::EndCombo();
			}

			if (ImGui::BeginCombo("SaveMode", ServerConfig::SaveModes[ServerConfig::SaveMode_Selected].c_str()))
			{
				unsigned int index = 0;
				for (auto mode : ServerConfig::SaveModes)
				{
					if (ImGui::Selectable(mode.c_str()))
						ServerConfig::SaveMode_Selected = index;

					index++;
				}
				ImGui::EndCombo();
			}
			InfoBox("CONTINUE: will create a new save on SaveSlot if it does not exist, or load it if it exist.\nNEW: will create a new game, with a new game id, and overwrite any game previously saved on the SaveSlot. If the server stops and restarts, the previous save will be overwritten for as long as the mode is set to NEW.");

			if (ImGui::InputInt("Save slot", &ServerConfig::SaveSlot, 1, 0))
				ServerConfig::SaveSlot = std::clamp(ServerConfig::SaveSlot, 1, 1000);
			InfoBox("When creating a new save, this number will be the id of the save.");

			ImGui::Checkbox("Tree Regrowth", &ServerConfig::TreeRegrowth);
			ImGui::Checkbox("Structure Damage", &ServerConfig::StructureDamage);

			ImGui::Dummy(ImVec2(0, 5));

			style->FrameBorderSize = 0.1f;
			ImGui::Text("Advanced (do not touch if you don't know)");
			ImGui::BeginChild("advanced", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y - 30), true);
			{
				ImGui::InputText("IpAddress", &ServerConfig::IpAddress); InfoBox("Listening interface for the game server, usually 0.0.0.0 if listening on all interfaces.");
				ImGui::InputInt("GamePort", &ServerConfig::GamePort, 0, 0); InfoBox("UDP port used for gameplay netcode.");
				ImGui::InputInt("QueryPort", &ServerConfig::QueryPort, 0, 0); InfoBox("UDP port used by Steam to list the server and enable the discovery services.");
				ImGui::InputInt("BlobSyncPort", &ServerConfig::BlobSyncPort, 0, 0); InfoBox("BlobSyncPort UDP port used by the BlobSync system to initialize game systems and exchange data.");
				ImGui::InputInt("SaveInterval", &ServerConfig::SaveInterval, 0, 0); InfoBox("How often the game server automatically saves the game to SaveSlot, in seconds.");
				ImGui::InputFloat("IdleDayCycleSpeed", &ServerConfig::IdleDayCycleSpeed, 0, 0, "%.1f"); InfoBox("A multiplier to how quickly the time passes compared to normal gameplay when the server is considered idle (no player connected).");
				ImGui::InputInt("IdleTargetFramerate", &ServerConfig::IdleTargetFramerate, 0, 0); InfoBox("Target framerate of the server when it\'s considered idle (no player connected).");
				ImGui::InputInt("ActiveTargetFramerate", &ServerConfig::ActiveTargetFramerate, 0, 0); InfoBox("Target framerate of the server when it\'s NOT considered idle (one or more player connected).");
				ImGui::Checkbox("LogFilesEnabled", &ServerConfig::LogFilesEnabled); InfoBox("Defines if the logs will be written to files. The logs will be output in <user data folder>/logs.");
				ImGui::Checkbox("TimestampLogFilenames", &ServerConfig::TimestampLogFilenames); InfoBox("true: every time the server runs will dump log output to a new file\nfalse: the filename will be sotf_log.txt and previous log will be overwritten if it already exists.");
				ImGui::Checkbox("TimestampLogEntries", &ServerConfig::TimestampLogEntries); InfoBox("Enables each log entry written to file to be timestamped.");
				ImGui::Checkbox("SkipNetworkAccessibilityTest", &ServerConfig::SkipNetworkAccessibilityTest); InfoBox("Opt-out of network accessibility self tests: retrieval of the public IP and listing on Steam Master Server, as well as port accessibility check.");
			}
			ImGui::EndChild();

			style->FrameBorderSize = 0.0f;
			PushButtonCol(Colors::green, Colors::greenHov);
			style->Colors[ImGuiCol_Text] = ImColor(0, 0, 0);
			if (ImGui::Button("Save & Apply"))
			{
				InOut::SaveAndApply();
			}
			ImGui::SameLine();
			PushButtonCol(Colors::lime, Colors::limeHov);
			if (ImGui::Button("Save & Run server"))
			{
				InOut::SaveAndRun();
			}
			style->Colors[ImGuiCol_Text] = ImColor(255, 255, 255);
			PushButtonCol(Colors::red, Colors::red - 30);
			style->FrameBorderSize = 0.1f;

			if (showAbout)
			{
				ImGui::SetCursorPos(ImVec2(5, 232));
				ImGui::BeginChild("about", ImVec2(530, 80), true, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
				TextCenteredXY("Made by ImAxel0");
				ImGui::SetCursorPosY(58);
				ImGui::Text("Descriptions from:"); ImGui::SameLine();
				ImGui::TextColored(ImVec4(0.09, 0.61, 1.00, 1), "link");
				if (ImGui::IsItemHovered(ImGuiHoveredFlags_None))
				{
					ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
					if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
						ShellExecute(NULL, L"open", L"https://steamcommunity.com/sharedfiles/filedetails/?id=2992700419&searchtext=server", NULL, NULL, SW_SHOWDEFAULT);
				}
				ImGui::EndChild();
			}
		}
		ImGui::End();

		if (showLog)
		{
			ImGui::SetNextWindowSize(ImVec2(vWindowSize.x, 450));
			ImGui::Begin("Log", &showLog, ImGuiWindowFlags_NoResize);
			ImGui::InputTextMultiline("##", &Logger::GetContent(), ImGui::GetContentRegionAvail(), ImGuiInputTextFlags_ReadOnly);
			ImGui::End();
		}
	}

	#ifdef _WINDLL
	if (GetAsyncKeyState(VK_INSERT) & 1)
		bDraw = !bDraw;
	#endif
}
