#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Window.h"
#include "j1Render.h"
#include "j1Map.h"
#include "j1Collision.h"
#include "j1Debug.h"



j1Debug::j1Debug()
{
}


j1Debug::~j1Debug()
{
}

bool j1Debug::Awake(pugi::xml_node & config)
{
	show_colliders = false;

	return true;
}

bool j1Debug::PostUpdate()
{
	bool ret = true;

	if (App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

bool j1Debug::Update(float dt)
{

	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN) 
	{
		App->RestartGame();
	}

	if (App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
	{
		App->RestartLevel();
	}

	if (App->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
	{
		App->SaveGame();
	}

	if (App->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)
	{
		App->LoadGame();
	}

	if (App->input->GetKey(SDL_SCANCODE_F9) == KEY_DOWN)
	{
		show_colliders = !show_colliders;

	}

	if (App->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN)
	{
		//FUCKING GOD MODE CUSI

	}

	App->win->SetTitle(App->map->DebugToString().GetString());
	return true;
}
