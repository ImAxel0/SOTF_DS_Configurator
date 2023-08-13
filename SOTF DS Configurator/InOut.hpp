#pragma once
#include "Owners.hpp"
#include "Logger.hpp"
#include "ServerConfig.hpp"
#include "ini.h"
#include <string>
#include <filesystem>
#include <shlobj_core.h>
#include <fstream>
#include <iostream>

using namespace mINI;

class InOut
{
public:
	static inline std::string SonsOfTheForestDSFolder;
	static inline bool IsFirstRun;
	static inline bool AskOverwrite;

private:
	static inline std::string GetCurrentDirectory()
	{
		char buffer[MAX_PATH];
		GetModuleFileNameA(NULL, buffer, MAX_PATH);
		std::string::size_type pos = std::string(buffer).find_last_of("\\/");

		return std::string(buffer).substr(0, pos);
	}

public:
	static inline void GetSonsOfTheForestDSFolder()
	{
		std::filesystem::path path;
		PWSTR path_tmp;
		auto get_folder_path_ret = SHGetKnownFolderPath(FOLDERID_LocalAppDataLow, 0, nullptr, &path_tmp);
		if (get_folder_path_ret != S_OK) {
			CoTaskMemFree(path_tmp);
		}
		path = path_tmp;
		CoTaskMemFree(path_tmp);

		path += "\\Endnight\\SonsOfTheForestDS";

		if (!std::filesystem::is_directory(path))
		{
			MessageBox(nullptr, L"Sons Of The Forest Dedicated Server isn't installed or hasn't been run at least once, install it from Steam and run once before opening this program.", L"Warning", MB_ICONINFORMATION | MB_OK);
			ExitProcess(0);
		}

		SonsOfTheForestDSFolder = path.string();
	}

	static inline void OverwriteOgFiles()
	{
		std::string backup = SonsOfTheForestDSFolder;
		SonsOfTheForestDSFolder.append("\\ownerswhitelist.txt");

		if (std::filesystem::exists(SonsOfTheForestDSFolder))
		{
			if (remove(SonsOfTheForestDSFolder.c_str()) != 0)
				MessageBox(nullptr, L"Couldn't overwrite ownerswhitelist.txt file, try to manually delete it in C:\\Users\\YourUserName\\AppData\\LocalLow\\Endnight\\SonsOfTheForestDS", L"Error", MB_ICONWARNING | MB_OK);
		}

		SonsOfTheForestDSFolder = backup;
		SonsOfTheForestDSFolder.append("\\dedicatedserver.cfg");

		if (std::filesystem::exists(SonsOfTheForestDSFolder))
		{
			if (remove(SonsOfTheForestDSFolder.c_str()) != 0)
				MessageBox(nullptr, L"Couldn't overwrite dedicatedserver.cfg file, try to manually delete it in C:\\Users\\YourUserName\\AppData\\LocalLow\\Endnight\\SonsOfTheForestDS", L"Error", MB_ICONWARNING | MB_OK);

		}
		SonsOfTheForestDSFolder = backup;
	}

	static inline void ReadIniConfig()
	{
		std::string backup = SonsOfTheForestDSFolder;
		SonsOfTheForestDSFolder.append("\\lastSavedConfig (don't touch me).ini");

		INIFile config(SonsOfTheForestDSFolder);
		INIStructure ini;

		if (config.read(ini))
		{
			ServerConfig::ServerName = ini["ServerName"].get("value");
			ServerConfig::MaxPlayers = std::stoi(ini["MaxPlayers"].get("value"));
			ServerConfig::Password = ini["Password"].get("value");
			ServerConfig::LanOnly = std::stoi(ini["LanOnly"].get("value"));
			ServerConfig::GameMode_Selected = std::stoi(ini["GameMode"].get("value"));
			ServerConfig::SaveMode_Selected = std::stoi(ini["SaveMode"].get("value"));
			ServerConfig::SaveSlot = std::stoi(ini["SaveSlot"].get("value"));
			ServerConfig::IpAddress = ini["IpAddress"].get("value");
			ServerConfig::GamePort = std::stoi(ini["GamePort"].get("value"));
			ServerConfig::QueryPort = std::stoi(ini["QueryPort"].get("value"));
			ServerConfig::BlobSyncPort = std::stoi(ini["BlobSyncPort"].get("value"));
			ServerConfig::SaveInterval = std::stoi(ini["SaveInterval"].get("value"));
			ServerConfig::IdleDayCycleSpeed = std::stod(ini["IdleDayCycleSpeed"].get("value"));
			ServerConfig::IdleTargetFramerate = std::stoi(ini["IdleTargetFramerate"].get("value"));
			ServerConfig::ActiveTargetFramerate = std::stoi(ini["ActiveTargetFramerate"].get("value"));
			ServerConfig::LogFilesEnabled = std::stoi(ini["LogFilesEnabled"].get("value"));
			ServerConfig::TimestampLogFilenames = std::stoi(ini["TimestampLogFilenames"].get("value"));
			ServerConfig::TimestampLogEntries = std::stoi(ini["TimestampLogEntries"].get("value"));
			ServerConfig::SkipNetworkAccessibilityTest = std::stoi(ini["SkipNetworkAccessibilityTest"].get("value"));
			ServerConfig::TreeRegrowth = std::stoi(ini["TreeRegrowth"].get("value"));
			ServerConfig::StructureDamage = std::stoi(ini["StructureDamage"].get("value"));
		}
		else
		{
			ini["ServerName"]["value"] = ServerConfig::ServerName.c_str();
			ini["MaxPlayers"]["value"] = std::to_string(ServerConfig::MaxPlayers);
			ini["Password"]["value"] = ServerConfig::Password.c_str();
			ini["LanOnly"]["value"] = std::to_string(ServerConfig::LanOnly);
			ini["GameMode"]["value"] = std::to_string(ServerConfig::GameMode_Selected);
			ini["SaveMode"]["value"] = std::to_string(ServerConfig::SaveMode_Selected);
			ini["SaveSlot"]["value"] = std::to_string(ServerConfig::SaveSlot);
			ini["IpAddress"]["value"] = ServerConfig::IpAddress.c_str();
			ini["GamePort"]["value"] = std::to_string(ServerConfig::GamePort);
			ini["QueryPort"]["value"] = std::to_string(ServerConfig::QueryPort);
			ini["BlobSyncPort"]["value"] = std::to_string(ServerConfig::BlobSyncPort);
			ini["SaveInterval"]["value"] = std::to_string(ServerConfig::SaveInterval);
			ini["IdleDayCycleSpeed"]["value"] = std::to_string(ServerConfig::IdleDayCycleSpeed);
			ini["IdleTargetFramerate"]["value"] = std::to_string(ServerConfig::IdleTargetFramerate);
			ini["ActiveTargetFramerate"]["value"] = std::to_string(ServerConfig::ActiveTargetFramerate);
			ini["LogFilesEnabled"]["value"] = std::to_string(ServerConfig::LogFilesEnabled);
			ini["TimestampLogFilenames"]["value"] = std::to_string(ServerConfig::TimestampLogFilenames);
			ini["TimestampLogEntries"]["value"] = std::to_string(ServerConfig::TimestampLogEntries);
			ini["SkipNetworkAccessibilityTest"]["value"] = std::to_string(ServerConfig::SkipNetworkAccessibilityTest);
			ini["TreeRegrowth"]["value"] = std::to_string(ServerConfig::TreeRegrowth);
			ini["StructureDamage"]["value"] = std::to_string(ServerConfig::StructureDamage);
			config.write(ini, true);
		}

		SonsOfTheForestDSFolder = backup;
	}

	static inline void UpdateIni()
	{
		std::string backup = SonsOfTheForestDSFolder;
		SonsOfTheForestDSFolder.append("\\lastSavedConfig (don't touch me).ini");

		INIFile config(SonsOfTheForestDSFolder);
		INIStructure ini;

		ini["ServerName"]["value"] = ServerConfig::ServerName.c_str();
		ini["MaxPlayers"]["value"] = std::to_string(ServerConfig::MaxPlayers);
		ini["Password"]["value"] = ServerConfig::Password.c_str();
		ini["LanOnly"]["value"] = std::to_string(ServerConfig::LanOnly);
		ini["GameMode"]["value"] = std::to_string(ServerConfig::GameMode_Selected);
		ini["SaveMode"]["value"] = std::to_string(ServerConfig::SaveMode_Selected);
		ini["SaveSlot"]["value"] = std::to_string(ServerConfig::SaveSlot);
		ini["IpAddress"]["value"] = ServerConfig::IpAddress.c_str();
		ini["GamePort"]["value"] = std::to_string(ServerConfig::GamePort);
		ini["QueryPort"]["value"] = std::to_string(ServerConfig::QueryPort);
		ini["BlobSyncPort"]["value"] = std::to_string(ServerConfig::BlobSyncPort);
		ini["SaveInterval"]["value"] = std::to_string(ServerConfig::SaveInterval);
		ini["IdleDayCycleSpeed"]["value"] = std::to_string(ServerConfig::IdleDayCycleSpeed);
		ini["IdleTargetFramerate"]["value"] = std::to_string(ServerConfig::IdleTargetFramerate);
		ini["ActiveTargetFramerate"]["value"] = std::to_string(ServerConfig::ActiveTargetFramerate);
		ini["LogFilesEnabled"]["value"] = std::to_string(ServerConfig::LogFilesEnabled);
		ini["TimestampLogFilenames"]["value"] = std::to_string(ServerConfig::TimestampLogFilenames);
		ini["TimestampLogEntries"]["value"] = std::to_string(ServerConfig::TimestampLogEntries);
		ini["SkipNetworkAccessibilityTest"]["value"] = std::to_string(ServerConfig::SkipNetworkAccessibilityTest);
		ini["TreeRegrowth"]["value"] = std::to_string(ServerConfig::TreeRegrowth);
		ini["StructureDamage"]["value"] = std::to_string(ServerConfig::StructureDamage);
		config.write(ini, true);

		SonsOfTheForestDSFolder = backup;
	}

	static inline bool CheckFirstRun()
	{
		std::string exePath = GetCurrentDirectory();
		exePath.append("\\SOTF_DSfirstrun.bin (don't delete me)");

		if (!std::filesystem::exists(exePath))
		{
			std::ofstream outfile(exePath);
			outfile.close();
			return true;
		}
		return false;
	}

	static inline void RestoreFirstRun()
	{
		std::string exePath = GetCurrentDirectory();
		exePath.append("\\SOTF_DSfirstrun.bin (don't delete me)");

		if (std::filesystem::exists(exePath))
		{
			if (remove(exePath.c_str()) != 0)
				MessageBox(nullptr, L"Couldn't remove SOTF_DSfirstrun.bin (don't delete me) file, manually delete it", L"Error", MB_ICONWARNING | MB_OK);
		}
	}
	
	static inline void ReadOwners()
	{
		std::string backup = SonsOfTheForestDSFolder;
		SonsOfTheForestDSFolder.append("\\ownerswhitelist.txt");		

		if (!std::filesystem::exists(SonsOfTheForestDSFolder))
		{
			std::ofstream outfile(SonsOfTheForestDSFolder);
			outfile.close();
		}
		else if (IsFirstRun) // if it's first run and ownerswhitelist.txt already exist
		{
			AskOverwrite = true;
		}

		std::fstream Owners;
		Owners.open(SonsOfTheForestDSFolder);
		if (Owners.is_open())
		{
			std::string line;
			while (std::getline(Owners, line))
			{
				if (line.empty())
					continue;

				if (line.at(0) == '#')
				{
					line.erase(0, 2);
					Owners::ownersNameBuff.push_back(line);
				}
				else
				{
					Owners::ownersIdBuff.push_back(line);
				}
			}
			Owners.close();
		}
		else
		{
			MessageBox(nullptr, L"Couldn\' t create/read ownerswhitelist.txt file", L"Error", MB_ICONERROR | MB_OK);
			ExitProcess(1);
		}
		
		SonsOfTheForestDSFolder = backup;
	}
	
	static inline void CheckConfigPresent()
	{
		std::string backup = SonsOfTheForestDSFolder;
		SonsOfTheForestDSFolder.append("\\dedicatedserver.cfg");

		if (IsFirstRun)
		{
			if (std::filesystem::exists(SonsOfTheForestDSFolder))
				AskOverwrite = true;
		}

		SonsOfTheForestDSFolder = backup;
	}

	static inline void SaveAndApply()
	{
		std::string backup = SonsOfTheForestDSFolder;

		std::ofstream ownerswhitelist(SonsOfTheForestDSFolder.append("\\ownerswhitelist.txt"));
		if (ownerswhitelist.is_open())
		{
			for (int i{ 0 }; i < Owners::ownersNameBuff.size(); ++i)
			{
				ownerswhitelist << "# " << (Owners::ownersNameBuff.at(i)) << '\n';
				ownerswhitelist << Owners::ownersIdBuff.at(i) << "\n\n";
			}
			ownerswhitelist.close();
		}
		else
		{
			Logger::Log("Couldn\'t open/create ownerswhitelist.txt file");
			MessageBox(nullptr, L"Couldn\'t open/create ownerswhitelist.txt file", L"Error", MB_ICONERROR | MB_OK);
		}

		SonsOfTheForestDSFolder = backup;

		std::ofstream dedicatedserver(SonsOfTheForestDSFolder.append("\\dedicatedserver.cfg"));
		if (dedicatedserver.is_open())
		{
			dedicatedserver << "{\n";

				dedicatedserver << "\"IpAddress\": " << "\"" << ServerConfig::IpAddress << "\",\n";
				dedicatedserver << "\"GamePort\": " << ServerConfig::GamePort << ",\n";
				dedicatedserver << "\"QueryPort\": " << ServerConfig::QueryPort << ",\n";
				dedicatedserver << "\"BlobSyncPort\": " << ServerConfig::BlobSyncPort << ",\n";
				dedicatedserver << "\"ServerName\": " << "\"" << ServerConfig::ServerName << "\",\n";
				dedicatedserver << "\"MaxPlayers\": " << ServerConfig::MaxPlayers << ",\n";
				dedicatedserver << "\"Password\": " << "\"" << ServerConfig::Password << "\",\n";
				dedicatedserver << std::boolalpha << "\"LanOnly\": " << ServerConfig::LanOnly << ",\n";
				dedicatedserver << "\"SaveSlot\": " << ServerConfig::SaveSlot << ",\n";
				dedicatedserver << "\"SaveMode\": " << "\"" << ServerConfig::SaveModes[ServerConfig::SaveMode_Selected] << "\",\n";
				dedicatedserver << "\"GameMode\": " << "\"" << ServerConfig::GameModes[ServerConfig::GameMode_Selected] << "\",\n";
				dedicatedserver << "\"SaveInterval\": " << ServerConfig::SaveInterval << ",\n";
				dedicatedserver << "\"IdleDayCycleSpeed\": " << ServerConfig::IdleDayCycleSpeed << ",\n";
				dedicatedserver << "\"IdleTargetFramerate\": " << ServerConfig::IdleTargetFramerate << ",\n";
				dedicatedserver << "\"ActiveTargetFramerate\": " << ServerConfig::ActiveTargetFramerate << ",\n";
				dedicatedserver << std::boolalpha << "\"LogFilesEnabled\": " << ServerConfig::LogFilesEnabled << ",\n";
				dedicatedserver << std::boolalpha << "\"TimestampLogFilenames\": " << ServerConfig::TimestampLogFilenames << ",\n";
				dedicatedserver << std::boolalpha << "\"TimestampLogEntries\": " << ServerConfig::TimestampLogEntries << ",\n";
				dedicatedserver << std::boolalpha << "\"SkipNetworkAccessibilityTest\": " << ServerConfig::SkipNetworkAccessibilityTest << ",\n";
				dedicatedserver << std::boolalpha << "\"GameSettings\": " << "\n{\n" << "\"Gameplay.TreeRegrowth\": " << ServerConfig::TreeRegrowth << ",\n" << "\"Structure.Damage\": " << ServerConfig::StructureDamage << "\n},\n";
				dedicatedserver << std::boolalpha << "\"CustomGameModeSettings\": " << "{},\n";

			dedicatedserver << "}";

			dedicatedserver.close();

			SonsOfTheForestDSFolder = backup;
			UpdateIni();
		}
		else
		{
			Logger::Log("Couldn\'t open/create dedicatedserver.cfg file");
			MessageBox(nullptr, L"Couldn\'t open/create dedicatedserver.cfg file", L"Error", MB_ICONERROR | MB_OK);
		}

		SonsOfTheForestDSFolder = backup;
	}

	static inline void SaveAndRun()
	{
		SaveAndApply();
		try {
			ShellExecute(NULL, L"open", L"C:\\Program Files (x86)\\Steam\\steamapps\\common\\Sons Of The Forest Dedicated Server\\SonsOfTheForestDS.exe", NULL, NULL, SW_SHOWDEFAULT);
		}
		catch (...) {
			MessageBox(nullptr, L"Couldn\'t launch SonsOfTheForestDS.exe, please launch it manually after saving the dedicated server config", L"Error", MB_ICONERROR | MB_OK);
		}
	}
};