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
	
	void CreateBorders(const std::vector<std::pair<int, int>>& points, ColliderType colliderType);
	bool CleanUp();

	void SetMapTexture(Texture2D texture);



private:

	Texture2D mapTexture;

	std::vector<PhysBody*> mapBorders;

	std::vector<std::pair<int, int>> mapPoints;

	std::vector<std::pair<int, int>> insidePointsVector;

	std::vector<PhysBody*> insideBorders;
};
