#pragma once
#include "Logger.hpp"
#include <string>
#include <iostream>
#include <vector>

class Owners
{
public:
	static inline std::vector<std::string> ownersNameBuff;
	static inline std::vector<std::string> ownersIdBuff;

public:
	static inline void AddOwner()
	{
		ownersNameBuff.push_back("");
		ownersIdBuff.push_back("");
		Logger::Log("Added owner entry\n");
	}

	static inline void RemoveOwnerAt(int index)
	{
		ownersNameBuff.erase(ownersNameBuff.begin() + index);
		ownersIdBuff.erase(ownersIdBuff.begin() + index);
		Logger::Log("Removed owner entry\n");
	}
};