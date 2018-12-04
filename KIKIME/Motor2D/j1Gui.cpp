#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Fonts.h"
#include "j1Input.h"
#include "j1Gui.h"
#include "j1Window.h"

j1Gui::j1Gui() : j1Module()
{
	name.create("gui");
}

// Destructor
j1Gui::~j1Gui()
{}

// Called before render is available
bool j1Gui::Awake(pugi::xml_node& conf)
{
	LOG("Loading GUI atlas");
	bool ret = true;

	atlas_file_name = conf.child("atlas").attribute("file").as_string("");

	return ret;
}

// Called before the first frame
bool j1Gui::Start()
{
	atlas = App->tex->Load(atlas_file_name.GetString());

	SDL_RenderGetViewport(App->render->renderer, &App->render->viewport);
  
	//HARDCODED
	//CreateElement(IMAGE, iPoint(App->render->viewport.w / 2, App->render->viewport.h / 11 + 25));

	CreateElement(BUTTON, iPoint(App->render->viewport.w / 12, App->render->viewport.h / 12), nullptr, SETTINGS);
	//CreateElement(BUTTON, iPoint(8 * App->render->viewport.w / 12, 8 * App->render->viewport.h / 12), nullptr, SETTINGS);
	//CreateElement(BUTTON, iPoint(5 * App->render->viewport.w / 12, 6 * App->render->viewport.h / 12), nullptr, SETTINGS);

	CreateElement(LABEL, iPoint(20, 20), "0", GAME_TIMER);
	CreateElement(LABEL, iPoint(9 * App->render->viewport.w / 10, 20), "SCORE : 999", SCORE);

	return true;
}

// Update all guis
bool j1Gui::PreUpdate()
{
	p2List_item<UIElement*>* item = elements.start;

	while (item != nullptr)
	{
		item->data->PreUpdate();

		//if (item->data->type == BUTTON)
		HandleInput(item->data);

		item = item->next;
	}

	return true;
}

// Called after all Updates
bool j1Gui::PostUpdate()
{
	p2List_item<UIElement*>* item = elements.start;

	while (item != NULL)
	{
		item->data->PostUpdate();
		item->data->Draw(atlas);

		item = item->next;
	}
	return true;
}

// Called before quitting
bool j1Gui::CleanUp()
{
	LOG("Freeing GUI");

	p2List_item<UIElement*>* item = elements.start;

	while (item != nullptr)
	{
		delete &item->data;

		item = item->next;
	}
	
	return true;
}

UIElement* j1Gui::CreateElement(UIType type, iPoint pos, p2SString string, ActionType action)
{
	UIElement* elem = nullptr;

	switch (type)
	{
	case IMAGE:

		elem = new Image(pos, SDL_Rect({ 485,829,328,103 }), type);
		break;
	case LABEL:

		elem = new Label(action, pos, type, string);
		break;
	case BUTTON:

		elem = new Button(action, pos, SDL_Rect({641,166,228,68}), type);
		break;
	default:

		break;
	}

	elements.add(elem);

	return elem;
}

void j1Gui::HandleInput(UIElement* element)
{
	//if(element->type == LABEL)
		

	//if (element->state == CLICK_UP)
	//	HandleAction(element);

	iPoint mouse;
	App->input->GetMousePosition(mouse.x, mouse.y);

	bool is_inside = (mouse.x - App->render->camera.x >= element->position.x && mouse.x - App->render->camera.x <= element->position.x + element->rect[element->state].w &&
				  mouse.y - App->render->camera.y >= element->position.y && mouse.y - App->render->camera.y <= element->position.y + element->rect[element->state].h);

	//LOG("mouse pos : %d - %d", mouse.x, mouse.y);
	//LOG("button rect: %d - %d - %d - %d", element->position.x, element->position.y, element->rect[element->state].w, element->rect[element->state].h);
	
	if (element->state != CLICK_DOWN &&	is_inside && App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) != KEY_DOWN)
	{
		element->state = HOVER;
		LOG("hover");
	}
	else if((element->state == HOVER || element->state == CLICK_DOWN) && is_inside && (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_REPEAT || App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN))
	{
		element->state = CLICK_DOWN;
		LOG("click");
	}
	else if (is_inside && App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_UP)
	{
		element->state = CLICK_UP;
		LOG("click up");
	}
	else
	{
		element->state = IDLE;
		LOG("idle");
	}

	element->HandleAction();
}

// const getter for atlas
const SDL_Texture* j1Gui::GetAtlas() const
{
	return atlas;
}

// class Gui ---------------------------------------------------

