#include "j1Collision.h"
#include "j1App.h"
#include "j1Input.h"
#include "p2Point.h"
#include "j1Player.h"
#include "j1Render.h"
#include "p2Log.h"


j1Player::j1Player()
{
	name.create("player");
}


j1Player::~j1Player()
{

}

bool j1Player::Awake(pugi::xml_node & config)
{
	pugi::xml_node player_node = config;

	//LVL 1 INITIAL POSITION
	data.init_pos1.x = player_node.child("lvl1").attribute("x").as_float();
	data.init_pos1.y = player_node.child("lvl1").attribute("y").as_float();

	//LVL 2 INITIAL POSITION
	data.init_pos2.x = player_node.child("lvl2").attribute("x").as_float();
	data.init_pos2.y = player_node.child("lvl2").attribute("y").as_float();

	//PLAYER RECT DIMENSIONS
	data.player_rect.w = player_node.child("rect").attribute("width").as_uint();
	data.player_rect.h = player_node.child("rect").attribute("height").as_uint();
	
	//PL. COLLIDER
	player_collider = App->collision->AddCollider(data.player_rect,COLLIDER_PLAYER, this);

	//SCROLL AND JUMPSPEED (CONST)
	data.speed.x = player_node.child("speed").attribute("scrollspeed").as_float();
	data.speed.y = player_node.child("speed").attribute("jumpspeed").as_float();
	
	LOG("%d  %d", data.player_rect.h, data.player_rect.w);
	LOG("%d  %d", data.speed.x, data.speed.y);

	return true;
}

bool j1Player::Start()
{
	//PLACING PLAYER AT INITIAL POS
	data.position.x = data.init_pos1.x;
	data.position.y = data.init_pos1.y;

	//PLACING PLAYER RECT
	data.player_rect.x = data.position.x;
	data.player_rect.y = data.position.y;

	max_speed_y = data.speed.y;

	return true;
}

bool j1Player::Update(float dt)
{
	Move();
		
	player_collider->SetPos(data.position.x, data.position.y);
	Draw();
	return true;
}

void j1Player::Draw()
{
	App->render->DrawQuad(data.player_rect, 255, 0, 0);

}

////////////////////////////////////////////////////////////////////////////////////////////
void j1Player::Move()
{
	move_left = false;
	move_right = false;
	is_jumping = false;

	on_top = false;
	on_floor = false;
	
	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) 
	{
		move_right = true;
		move_left = false;

		if (move_right)
		{
			data.position.x += data.speed.x;
			move_right = false;
		}
	}

	
	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	{
		move_right = false;
		move_left = true;

		if (move_left)
		{
			data.position.x -= data.speed.x;
			move_left = false;
		}
	}

	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
	{
		if (!on_floor)
		{
			data.position.y += (data.speed.y + 2);
			data.speed.y += 0.2;
		}
	}

	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN)
	{
		is_jumping = true;	
		data.position.y -= data.speed.y;
		data.speed.y -= 0.2;

		if (data.speed.y <= 0)
		{
			data.position.y += data.speed.y;
			data.speed.y += 0.2;
		}
		if(abs(data.speed.y) >= max_speed_y)
		{
			data.position.y += data.speed.y;
		}
		
	}
	
	
	data.player_rect.x = data.position.x;
	data.player_rect.y = data.position.y;

//	/if (is_jumping && !move_left && !move_right)
//	/{
//	/	data.position.y -= aux_speed_y;
//	/	aux_speed_y -= 0.2;
//	/	if (aux_speed_y == 0)
//	/	{
//	/		on_top = true;
//	/		data.position.y += aux_speed_y;
//	/		aux_speed_y += 0.2;
//	/		if (aux_speed_y == data.speed.y)
//	/		{
//	/			data.position.y += aux_speed_y;
//	/		}
//  /
//	/	}
//  /
//	/}
//	/if (is_jumping && move_left && !move_right)
//	/{
//	/	data.position.x -= data.speed.x;
//	/	data.position.y -= aux_speed_y;
//	/	aux_speed_y -= 0.2;
//	/	if (aux_speed_y == 0)
//	/	{
//	/		on_top = true;
//	/		data.position.y += aux_speed_y;
//	/		aux_speed_y += 0.1;
//	/		if (aux_speed_y == data.speed.y)
//	/		{
//	/			data.position.y += aux_speed_y;
//	/		}
//  /
//	/	}
//	/}
//	/if (is_jumping && !move_left && move_right)
//	/{
//	/	data.position.x -= data.speed.x;
//	/	data.position.y -= aux_speed_y;
//	/	aux_speed_y -= 0.1;
//	/	if (aux_speed_y == 0)
//	/	{
//	/		on_top = true;
//	/		data.position.y += aux_speed_y;
//	/		aux_speed_y += 0.1;
//	/		if (aux_speed_y == data.speed.y)
//	/		{
//	/			data.position.y += aux_speed_y;
//	/		}
//  /
//	/	}
//	/}
//	/if (is_jumping && move_left && move_right)
//	/{
//	/	data.position.y -= aux_speed_y;
//	/	aux_speed_y -= 0.1;
//	/	if (aux_speed_y == 0)
//	/	{
//	/		on_top = true;
//	/		data.position.y += aux_speed_y;
//	/		aux_speed_y += 0.1;
//	/	}
//	/	if (aux_speed_y == data.speed.y)
//	/	{
//	/		data.position.y += aux_speed_y;
//	/	}
//  /
//  /
//	///if (!is_jumping && !on_floor)
//	///{
//	///	on_top = true;
//	///	data.position.y += aux_speed_y;
//	///	aux_speed_y += 0.1;
//	///	if (aux_speed_y == data.speed.y)
//	///	{
//	///		data.position.y += aux_speed_y;
//	///	}
//	///}
//  //
}

///////////////////////////////////////////////////////////////////////////////
bool j1Player::Load(pugi::xml_node & node)
{
	LOG("Loading PLAYER");

	data.position.x = node.child("player").child("position").attribute("x").as_int();
	data.position.y = node.child("player").child("position").attribute("y").as_int();

	return true;
}


bool j1Player::Save(pugi::xml_node & node)
{
	LOG("Saving PLAYER");

	pugi::xml_node pl_node = node.append_child("position");

	pl_node.append_attribute("x") = data.position.x;
	pl_node.append_attribute("y") = data.position.y;

	//pl_node.append_attribute("x") = data.init_pos1.x;
	//pl_node.append_attribute("y") = data.init_pos1.y;

	LOG("playerX: %d - %d \n playerY: %d - %d", data.init_pos1.x, pl_node.attribute("x").as_int(), data.init_pos1.y, pl_node.attribute("y").as_int());

	return true;
}

void j1Player::OnCollision(Collider * c1, Collider * c2)
{

}

void j1Player::OnCollisionLine(Collider * c, int x1, int y1, int x2, int y2)
{

}



