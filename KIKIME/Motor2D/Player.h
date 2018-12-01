#pragma once
#include "SDL/include/SDL.h"
#include "j1Module.h"
#include "j1Collision.h"
#include "Animation.h"
#include "Entity.h"

class Player : public Entity
{

public:

	Player() {}
	Player(iPoint pos, Player* e);
	~Player();

	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();
	
	void Move(float dt);
	void MoveFree(float dt);
	bool Jump();
	void PlayerAnimations(float dt);
	

public:

	bool want_right;
	bool want_left;
	bool want_up;
	bool want_down;

	bool jumping;
	bool double_jumping;

	int jump_speed;

	///////////////// PLAYER ///////////////////

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