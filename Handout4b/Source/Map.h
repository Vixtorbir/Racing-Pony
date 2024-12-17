#pragma once


#include "Module.h"
#include "Globals.h"
#include "ModulePhysics.h"
#include "PhysicEntity.h"

#include <vector>
#include <utility>


class Map : public Module
{
public:
	Map(Application* app, bool start_enabled = true);
	~Map();
	bool Start();
	update_status Update();
	void CreateBorders();
	bool CleanUp();	


private:

	Texture2D mapTexture;
	std::vector<PhysBody*> mapBorders;

	std::vector<std::pair<int, int>> mapPoints;
};
