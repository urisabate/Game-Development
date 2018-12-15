#include "j1App.h"
#include "j1Collision.h"
#include "j1Input.h"
#include "p2Point.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Map.h"
#include "p2Log.h"
#include "Player.h"
#include "Brofiler/Brofiler.h"
#include "j1Pathfinding.h"
#include "Player.h"

Player::Player(iPoint pos, Player * e) : Entity(pos, e)
{
	lifes = e->lifes;
	
	speed.x = e->speed.x;
	speed.y = 0;
	jump_speed	= e->speed.y;
	smash_speed = e->smash_speed;
	max_speed	= e->max_speed;
	
	god_speed = e->god_speed;
	gravity	  = e->gravity;
	god_mode  = e->god_mode;
	
	idle  = e->idle;
	fall  = e->fall;
	walk  = e->walk;
	death = e->death;
	jump  = e->jump;
	god	  = e->god;
	smash = e->smash;
	doublejump = e->doublejump;

	def_anim_speed = e->def_anim_speed;

	current_animation = &idle;

	//Initializing
	alive			= true;
	want_right		= false;
	want_left		= false;
	want_up			= false;
	want_down		= false;
	jumping			= false;
	double_jumping	= false;
	smashing		= false;
	level_finished	= false;
	going_down		= false;
	moving_left		= false;
	moving_right	= false;
	dx = 0;
	dy = 0;
	delta_x = 0;
	delta_y = 0;
	delta_movement = 7.0f;

}

Player::~Player()
{

}

bool Player::PreUpdate()
{
	BROFILER_CATEGORY("Player->PreUpdate", Profiler::Color::BlueViolet);
	

	if (alive) {
		
		//Jump
		if ((App->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN) || (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) && !god_mode)
		{
			Jump();
		}
		if ((App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT))
		{
			want_up = true;
		}
		//Smash 
		if ((App->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN) && !god_mode)
		{
			Smash();
		}
		//want_down only in god mode
		if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
		{
			want_down = true;
		}
		//Left
		if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
		{
			want_left = true;
			moving_left = true;
		}
		//Right
		if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
		{
			want_right = true;
			moving_right = true;
		}
		
	}
		return true;
}

bool Player::Update(float dt)
{
	BROFILER_CATEGORY("Player->Update", Profiler::Color::BlueViolet);

	/*float normalize = 90 / (1000 / dt);

	if (normalize < 1) { normalize = 1; }*/

	if (!god_mode && alive) {
		Move(dt);
	}
	else {
		MoveFree(dt);
	}
	PlayerAnimations(dt);
	
	return true;
}

void Player::PlayerAnimations(float dt)
{
	if (alive) {
	//	current_animation = &death;
		if (!god_mode) 
		{
			current_animation = &idle;
			if (moving_left || moving_right)
				current_animation = &walk;

			if (jumping)
				current_animation = &jump;

			if (double_jumping)
				current_animation = &doublejump;

			if (going_down)
				current_animation = &fall;

			if (smashing)
				current_animation = &smash;
		}
		else
		{
			current_animation = &god;
		}
	}

	current_animation->speed = def_anim_speed * dt;
}

void Player::Draw(SDL_Texture * sprites)
{
	if (App->pause) {
		current_animation->speed = 0;
	}
	else current_animation->speed = def_anim_speed * App->dt;

	if (collider) 
		collider->SetPos(position.x, position.y);

	if (double_jumping && !going_down && alive)
		App->render->Blit(sprites, position.x, position.y, &current_animation->GetCurrentFrame(), 1, 0, SDL_FLIP_VERTICAL);

	else if(moving_left)
		App->render->Blit(sprites, position.x, position.y, &current_animation->GetCurrentFrame(), 1, 0, SDL_FLIP_HORIZONTAL);

	else
		App->render->Blit(sprites, position.x, position.y, &current_animation->GetCurrentFrame(), 1, 0);
}

void Player::ResetPlayer()
{
	collider->SetPos(position.x, position.y);

	alive			= true;
	want_right		= false;
	want_left		= false;
	want_up			= false;
	want_down		= false;
	jumping			= false;
	double_jumping  = false;
	smashing		= false;
	level_finished  = false;
	going_down		= false;
	moving_left		= false;
	moving_right	= false;
	dx = 0;
	dy = 0;
	delta_x = 0;
	delta_y = 0;
}


bool Player::PostUpdate()
{
	BROFILER_CATEGORY("Player->PostUpdate", Profiler::Color::BlueViolet);
	moving_left = false;
	moving_right = false;

	return true;
}

void Player::Jump()
{
	if (!jumping && !going_down) {
		speed.y = -jump_speed;
		jumping = true;
		going_down = false;
		//App->audio->PlayFx(fx_name);
	}
	else if (!double_jumping) {
		speed.y = -jump_speed;
		double_jumping = true;
		going_down = false;
		//App->audio->PlayFx(fx_name);
	}
}

void Player::Smash()
{
	if (!smashing) {
		speed.y = smash_speed;
		smashing = true;
		//App->audio->PlayFx(fx_name);
	}
}

void Player::Move(float dt)
{
	dx = 0;
	dy = 0;
	can_move = true;

	if (want_right)
	{
		iPoint pos = App->render->ScreenToWorld(position.x + collider->rect.w + App->render->camera.x + (int)(speed.x * dt), position.y + App->render->camera.y);
		pos = App->map->WorldToMap(pos.x, pos.y);
		iPoint pos2 = App->render->ScreenToWorld(position.x + collider->rect.w + App->render->camera.x + (int)(speed.x * dt), position.y + App->render->camera.y + collider->rect.h / 2);
		pos2 = App->map->WorldToMap(pos2.x, pos2.y);
		iPoint pos3 = App->render->ScreenToWorld(position.x + collider->rect.w + App->render->camera.x + (int)(speed.x * dt), position.y + App->render->camera.y + collider->rect.h);
		pos3 = App->map->WorldToMap(pos3.x, pos3.y);

		if (App->pathfinding->IsWalkable(pos) && App->pathfinding->IsWalkable(pos2) && App->pathfinding->IsWalkable(pos3)) {
			dx = (float)(speed.x * dt);
			//position.x += (int)(speed.x * dt);
		}
		else {
			can_move = false;
		}
		want_right = false;
	}
	else if (want_left)
	{
		iPoint pos = App->render->ScreenToWorld(position.x + App->render->camera.x - (int)(speed.x * dt), position.y + App->render->camera.y);
		pos = App->map->WorldToMap(pos.x, pos.y);
		iPoint pos2 = App->render->ScreenToWorld(position.x + App->render->camera.x - (int)(speed.x * dt), position.y + App->render->camera.y + collider->rect.h / 2);
		pos2 = App->map->WorldToMap(pos2.x, pos2.y);
		iPoint pos3 = App->render->ScreenToWorld(position.x + App->render->camera.x - (int)(speed.x * dt), position.y + App->render->camera.y + collider->rect.h);
		pos3 = App->map->WorldToMap(pos3.x, pos3.y);

		if (App->pathfinding->IsWalkable(pos) && App->pathfinding->IsWalkable(pos2) && App->pathfinding->IsWalkable(pos3)) {
			dx = -(float)(speed.x * dt);
			//position.x -= (int)(speed.x * dt);
		}
		else {
			can_move = false;
		}
		want_left = false;
	}

	//if (want_up)
	if (speed.y < 0) // jumping or double jumping
	{
		iPoint pos = App->render->ScreenToWorld(position.x + App->render->camera.x, position.y + App->render->camera.y + (int)(speed.y * dt));
		pos = App->map->WorldToMap(pos.x, pos.y);
		iPoint pos2 = App->render->ScreenToWorld(position.x + App->render->camera.x + collider->rect.w / 2, position.y + App->render->camera.y + (int)(speed.y * dt));
		pos2 = App->map->WorldToMap(pos2.x, pos2.y);
		iPoint pos3 = App->render->ScreenToWorld(position.x + App->render->camera.x + collider->rect.w, position.y + App->render->camera.y + (int)(speed.y * dt));
		pos3 = App->map->WorldToMap(pos3.x, pos3.y);

		if (App->pathfinding->IsWalkable(pos) && App->pathfinding->IsWalkable(pos2) && App->pathfinding->IsWalkable(pos3)) {
			dy = (float)(speed.y * dt);
			//position.y += (int)(speed.y * dt);
		}
		else { 
			can_move = false;
			speed.y = (int)(gravity * dt); 
		}
		want_up = false;
	}
	//Always move down if gravity is true, and player can, obviously
	else if (speed.y > 0)
	{
		iPoint pos = App->render->ScreenToWorld(position.x + App->render->camera.x, position.y + App->render->camera.y + collider->rect.h + (int)(speed.y * dt));
		pos = App->map->WorldToMap(pos.x, pos.y);
		iPoint pos2 = App->render->ScreenToWorld(position.x + App->render->camera.x + collider->rect.w/2, position.y + App->render->camera.y + collider->rect.h + (int)(speed.y * dt));
		pos2 = App->map->WorldToMap(pos2.x, pos2.y);
		iPoint pos3 = App->render->ScreenToWorld(position.x + App->render->camera.x + collider->rect.w, position.y + App->render->camera.y + collider->rect.h + (int)(speed.y * dt));
		pos3 = App->map->WorldToMap(pos3.x, pos3.y);

		if (App->pathfinding->IsWalkable(pos) && App->pathfinding->IsWalkable(pos2) && App->pathfinding->IsWalkable(pos3)) {
			dy = (float)(speed.y * dt);
			//position.y += (int)(speed.y * dt);
			//Animation
			going_down = true;
		}
		else {
			//position.y  = pos.y - position.y + collider->rect.h -5;
			//dy = pos.y - position.y + collider->rect.h -1;
			can_move = false;
			dy = 0;
			delta_y = 0;
			speed.y			= max_speed;
			going_down		= false;

			//Animations
			jumping			= false;
			double_jumping	= false;
			smashing		= false;
		}
	}


	//Gravity acting
	if (speed.y < max_speed) {
		speed.y += (int)(gravity * dt);
		if (speed.y > max_speed) speed.y = max_speed;
	}




	float max_y_speed = (2 * jump_speed);
	// Check if a frame drop occured
	if (abs(dx) < App->map->data.tile_width) {
		// Accumulate speed
		if (abs(delta_x) < delta_movement) {
			delta_x += dx;
		}
		else {
			// Actual movement
			if (delta_x > 0) {
				position.x += delta_movement;
				delta_x = dx - (delta_movement - delta_x);
			}
			else if (delta_x < 0) {
				position.x -= delta_movement;
				delta_x = -(abs(dx) - (delta_movement - abs(delta_x)));
			}
		}
	}

	// Check if a frame drop occured
	if (abs(dy) < App->map->data.tile_height) {
		//delta_y += dy;
		// Accumulate speed
		if (abs(delta_y) < delta_movement) {
			delta_y += dy;
		}
		else {
			// Actual movement
			if (delta_y > 0) {
				position.y += delta_movement;
				delta_y = dy - (delta_movement - delta_y);
			}
			else if (delta_y < 0) {
				/*if (abs(delta_y) >= de) {
					int a = 1;
				}*/
				position.y -= delta_movement;
				delta_y = -(abs(dy) - (delta_movement - abs(delta_y)));
			}
		}
	}
	
	//if (can_move && abs(dx) < App->map->data.tile_width)  position.x += dx;
	//if (can_move && abs(dy) < App->map->data.tile_height) position.y += dy;
}

void Player::MoveFree(float dt)
{
	if (want_right)
	{
		position.x += (int)(god_speed * dt);
		want_right = false;
	}
	else if (want_left)
	{
		position.x -= (int)(god_speed * dt);
		want_left = false;
	}
	if (want_up)
	{
		position.y -= (int)(god_speed * dt);
		want_up = false;
	}
	else if (want_down)
	{
		position.y += (int)(god_speed * dt);
		want_down = false;
	}
}