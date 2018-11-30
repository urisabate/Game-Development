#pragma once

#include "j1Module.h"
#include "p2Defs.h"
#include "Animation.h"
#include "p2Point.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Collision.h"
#include "j1Debug.h"

#define MAX_ENEMIES 100

class Animation;
struct SDL_Texture;

enum entityType
{
	FLOATER,
	ROLLER,
	STATIC,
	PLAYER,
	NO_TYPE
};

class Entity
{
public:

	Entity(){}

	Entity(entityType type) {}

	~Entity(){}

	virtual bool PreUpdate() { return true; }
	virtual bool Update(float dt) { 
		position.x += (int)(speed.x * dt);
		position.y += (int)(speed.y * dt);
		return true; 
	}
	virtual bool PostUpdate() { return true; }

	virtual void Draw(SDL_Texture *sprites) {
		if (App->pause) {
			current_animation->speed = 0;
		} else current_animation->speed = def_anim_speed * App->dt;
		App->render->Blit(sprites, position.x, position.y, &current_animation->GetCurrentFrame(), 1, 0);
		if (collider) collider->SetPos(position.x, position.y);
	}

	virtual bool UpdateLogic(iPoint pos) { return true; }

public:

	////////// GENERAL /////////

	p2SString		name;

	bool			alive;

	int				health;
	int				vision_range;

	SDL_Rect		rect;	

	iPoint			speed;
	iPoint			speed_mult;
	iPoint			position;
	iPoint			initial_pos;

	Collider*		collider;

	entityType		type;

	float			def_anim_speed;

	//Mix_Chunk*	die_FX;


	/////////////// ENEMIES /////////////////

	bool			first_iteration;
	bool			return_origin;

	Animation		follow;

	iPoint			desired_position;


	///////////////// PLAYER ///////////////////
	
	//Animations
	SDL_Rect		anim_rect;
	Animation*		current_animation = nullptr;
	Animation		idle;
	Animation		walk;
	Animation		jump;
	Animation		doublejump;
	Animation		fall;
	Animation		death;
	Animation		god;

	float			gravity;
	float			god_speed;

	bool			god_mode;
	bool			level_finished;



};