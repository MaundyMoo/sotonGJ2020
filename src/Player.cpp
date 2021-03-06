#include <iostream>
#include "include/constants.h"
#include "include/olcPixelGameEngine.h"
#include "Entity.cpp"

class Player : public Entity
{
private:
	int max_vel_x = 5;	
	float y_vel = 0.0f;
	
	bool grounded = false;
	// 1 or -1
	int direction = 1;
	bool moving = false;
	float grav_vel = 0.0f;
	float run_vel = 0.0f; 
	int checkX, checkY, yindex, xindex;
	bool throughWall = false;
	float firstWall;
public:

	using Entity::Entity;

	int getSpeed()
	{
		return this->max_vel_x;
	}	

	void setGrounded(bool grounded) 
	{
		this->grounded = grounded;	
	}
	void setMoving(bool moving) 
	{
		this->moving = moving;
	}
	void setDirection(int direction)
	{
		this->direction = direction;
	}

	void setXVel(float vel)
	{
		run_vel = vel;
	}
	void setYVel(float vel) 
	{
		y_vel = vel;
	}

	bool getGrounded()
	{
		return grounded;
	}
	int getDirection() 
	{
		return direction;
	}		
	void update(int (*inputmap)[SCREEN_HEIGHT/TILESIZE][SCREEN_WIDTH/TILESIZE]) override
	{
		// Some acceleration
		if(moving) {
			run_vel += 0.05f ;
			if(run_vel > 1.0f) run_vel = 1.0f;
		} else {
			run_vel -= 0.05f;
			if(run_vel <= 0.0f) run_vel = 0.0f;
		}
		checkX = x + (getSpeed() * run_vel * direction);
		checkY = y + y_vel;
		int checkAmount = 20;
		for(int i=1;i<checkAmount;i++){
			yindex = (y + (i * (checkY-y)/checkAmount))/TILESIZE;
			xindex = (x + (i * (checkX-x)/checkAmount))/TILESIZE;
			if((*inputmap)[yindex][xindex]==1){
				if(!throughWall)
					firstWall = i;
				throughWall=true;
			}
		}
		// gravity accelerates
		if(!grounded) 
		{
			if(!throughWall){
				setY(getY() + y_vel);

			}
			else{
				setY((int)(getY()+(checkY-getY())*firstWall/checkAmount));
				throughWall = false;
			}
			y_vel += GRAVITY * grav_vel; 	
			grav_vel += 0.01f;
			if(grav_vel > 1.0f) grav_vel = 1.0f;
		} else {
			grav_vel = 0.0f;
		}
		if(!throughWall){
			setX(getX() + (getSpeed() * run_vel * direction));
		}
		else{
			setX((int)(getX()+(checkX-getX())*firstWall/checkAmount));
			throughWall = false;
		}
	}	
	
	void jump() 
	{
		if(grounded) 
		{
			y_vel = -12.0f;
			setGrounded(false);
		}
	}
	
private:
	void readSpriteData(){
	}
};
