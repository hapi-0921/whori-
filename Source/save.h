#pragma once
#include <targetManager.h>
class Save
{
public:

	void SaveGame();
	void LoadGame();
	//void ResetSave();
	void Reset();

	static Save& Instance()
	{
		static Save save;
		return save;
	}
	int ranking[4] = { 0,0,0,0 };
};
