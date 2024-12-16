#pragma once


#include "Module.h"
#include "Globals.h"
#include "ModulePhysics.h"
#include "PhysicEntity.h"

#include <vector>
#include <utility>


//Crea una clase mapa que que carge la textura del mapa y la dibuje, ademas de crear vector<PhysBody*> para los bordes del mapa (utiliza la función createChain de ModulePhysics)

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
