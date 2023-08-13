#include <string>

class ServerConfig
{
	enum SaveMode
	{
		Continue,
		New
	};

	enum GameMode
	{
		Normal,
		Hard,
		Hardsurvival,
		Peaceful
	};

public:
	static inline std::string IpAddress = "0.0.0.0";
	static inline int GamePort = 8766;
	static inline int QueryPort = 27016;
	static inline int BlobSyncPort = 9700;
	static inline std::string ServerName = "Sons Of The Forest Server (dedicated)";
	static inline int MaxPlayers = 2;
	static inline std::string Password = "";
	static inline bool LanOnly = false;
	static inline int SaveSlot = 1;
	static inline std::string SaveModes[2] = { "continue", "new" };
	static inline int SaveMode_Selected = SaveMode::Continue;
	static inline std::string GameModes[4] = { "normal", "hard", "hardsurvival", "peaceful" };
	static inline int GameMode_Selected = GameMode::Normal;
	static inline int SaveInterval = 600;
	static inline float IdleDayCycleSpeed = 0.0f;
	static inline int IdleTargetFramerate = 5;
	static inline int ActiveTargetFramerate = 60;
	static inline bool LogFilesEnabled = false;
	static inline bool TimestampLogFilenames = true;
	static inline bool TimestampLogEntries = true;
	static inline bool SkipNetworkAccessibilityTest = false;
	static inline bool TreeRegrowth = true;
	static inline bool StructureDamage = true;
};
