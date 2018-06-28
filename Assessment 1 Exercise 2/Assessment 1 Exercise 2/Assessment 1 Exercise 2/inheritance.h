#pragma once
#include <string>
#include <iostream>
#include <vector>

using namespace std;

//Weapon Enum
enum WeaponType {
	SWORD,
	SPEAR,
	GUN
};

//Base Class
class Character
{
public:
	//Base data
	string id;
	int xPos;
	int yPos;
	int health;
	int damageDealt;
	//Base Attacking Function
	virtual void Attack(Character* target) {
		bool dead;
		dead = target->ReceiveDamage(damageDealt);
		cout << "Attacked for " << damageDealt << " damage." << endl;
		if (dead)
		{
			health += damageDealt;
			cout << "Healed for " << damageDealt << " health." << endl;
		}
	}
	//Base Damage Receiving function
	virtual bool ReceiveDamage(int damageReceived)
	{
		health -= damageReceived;
		if (health <= 0)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	Character() {
		//Base Constructor
		health = 50;
		damageDealt = 10;
	}
	virtual ~Character() {
		//Deconstructor
	}
};
class Player : public Character
{
private:
	//Unique data to Player
	int armour = 5; //Damage Reduction
	int lives = 3; //Health Resets on death
public:
	//Updated Attack function
	virtual void Attack(Character* target)
	{
		bool dead;
		//checking if target is in range
		if (((target->xPos - 1 == xPos) && (target->yPos == yPos)) || ((target->xPos + 1 == xPos) && (target->yPos == yPos)) ||
			((target->xPos == xPos) && (target->yPos + 1 == yPos)) || ((target->xPos == xPos) && (target->yPos - 1 == yPos)))
		{
			//dealing damage and checking for dead target
			dead = target->ReceiveDamage(damageDealt);
			cout << "I swung my sword at '" << target->id << "' for " << damageDealt << " damage." << endl;
			if (dead) {
				//leveling up on target death
				cout << "I leveled up! Healed for 10 health." << endl;
				health += 10;
				armour++;
				damageDealt += 2;
			}
		}
		else {
			cout << "I attempted to attack '" << target->id << "' but was out of range." << endl;
		}
	}
	//Updated Damage Receiving function
	virtual bool ReceiveDamage(int damageReceived)
	{
		//damage reduction based on armour
		health -= (damageReceived - armour);
		cout << "I Blocked " << armour << " damage using armour." << endl;
		//checking for death
		if (health <= 0)
		{
			//losing a life on death and checking for remaining lives
			lives--;
			if (lives == 0) {
				//no reset with no lives
				cout << "I have died." << endl;
				return true;
			}
			else {
				//reset health based on level with lives remaining
				health = 100 + armour * 5 - 25;
				cout << "I have been revived at " << health << " health." << endl;
				return true;
			}
		}
		else
		{
			//failed check for death
			return false;
		}
	}
	Player(string newId, int startingX, int startingY)
	{
		//Player Constructor
		id = newId;
		xPos = startingX;
		yPos = startingY;
		health = 100;
		damageDealt = 8;
	}
};
class Grunt : public Character
{
private:
	//Unique data to Grunt
	int rank; //How strong the Grunt is
	WeaponType weapon; //How the Grunt deals damage
public:
	virtual void Attack(Character* target)
	{
		bool dead;
		//attackChance is randomly deciding if the attack hits at all based on rank
		int attackChance;
		attackChance = rand() % rank;
		if (attackChance >= 2)
		{
			//range calculation for sword weapon
			if (((target->xPos - 1 == xPos) && (target->yPos == yPos)) || ((target->xPos + 1 == xPos) && (target->yPos == yPos)) ||
				((target->xPos == xPos) && (target->yPos + 1 == yPos)) || ((target->xPos == xPos) && (target->yPos - 1 == yPos)) && weapon == SWORD)
			{
				//checks for dead target, if true level up
				dead = target->ReceiveDamage(damageDealt + rank);
				cout << "'" << id << "' swung its sword at '" << target->id << "' for " << damageDealt + rank << " damage." << endl;
				if (dead) {
					rank++;
					cout << "'" << id << "' has leveled up!" << endl;
				}
			}
			//range calculation for spear weapon
			else if (((target->xPos - 2 == xPos) && (target->yPos == yPos)) || ((target->xPos + 2 == xPos) && (target->yPos == yPos)) ||
				((target->xPos == xPos) && (target->yPos + 2 == yPos)) || ((target->xPos == xPos) && (target->yPos - 2 == yPos)) ||
				((target->xPos - 1 == xPos) && (target->yPos == yPos)) || ((target->xPos + 1 == xPos) && (target->yPos == yPos)) ||
				((target->xPos == xPos) && (target->yPos + 1 == yPos)) || ((target->xPos == xPos) && (target->yPos - 1 == yPos)) ||
				((target->xPos - 1 == xPos) && (target->yPos - 1 == yPos)) || ((target->xPos + 1 == xPos) && (target->yPos - 1 == yPos)) ||
				((target->xPos - 1 == xPos) && (target->yPos + 1 == yPos)) || ((target->xPos + 1 == xPos) && (target->yPos + 1 == yPos)) && weapon == SPEAR)
			{
				dead = target->ReceiveDamage(damageDealt + rank - 1);
				cout << "'" << id << "' stabbed its spear at '" << target->id << "' for " << damageDealt + rank - 1 << " damage." << endl;
				if (dead) {
					rank++;
					cout << "'" << id << "' has leveled up!" << endl;
				}
			}
			//check for gun if others fail as gun has infinite range
			else if (weapon == GUN)
			{
				dead = target->ReceiveDamage(damageDealt + rank - 2);
				cout << "'" << id << "' shot its gun at '" << target->id << "' for " << damageDealt + rank - 2 << " damage." << endl;
				if (dead) {
					rank++;
					cout << "'" << id << "' has leveled up!" << endl;
				}
			}
			else
			{
				cout << "'" << id << "' attempted to attack '" << target->id << "' but was out of range." << endl;
			}
		}
		else {
			cout << "'" << id << "' waves its weapon in the air stupidly." << endl;
		}
	}
	virtual bool ReceiveDamage(int damageReceived)
	{
		//takes damage as normal
		health -= damageReceived;
		if (health <= 0)
		{
			//if rank is high enough, Grunt revives upon death, without triggering the death flag but loses some rank
			if (rank > 12)
			{
				rank -= 10;
				health = 50;
				cout << "'" << id << "' has survived but lost some levels." << endl;
				return false;
			}
			else
			{
				cout << "'" << id << "' has  died." << endl;
				return true;
			}
		}
		else {
			return false;
		}
	}
	Grunt(string newId, int startingX, int startingY, int startingRank, WeaponType newWeapon)
	{
		//Grunt Constructor
		id = newId;
		xPos = startingX;
		yPos = startingY;
		rank = startingRank;
		weapon = newWeapon;
		health = 50;
		damageDealt = 7;
	}
};
class Fletcher : public Character
{
private:
	//Unique data for Fletcher
	int arrows = 10; //How much ammo Fletcher has
	bool fletching = false; //If in the process of reloading
public:
	virtual void Attack(Character* target) {
		//Checks to see if either out of ammo or reloading
		if (arrows > 0 && !fletching)
		{
			//if check passes then deals damage as normal
			bool dead;
			dead = target->ReceiveDamage(damageDealt);
			cout << "'" << id << "' shot an arrow at '" << target->id << "' for " << damageDealt << " damage." << endl;
			arrows--;
			if (dead)
			{
				health += damageDealt;
				cout << "'" << id << "' has healed for " << damageDealt << " health." << endl;
			}
			//if ammo runs out starts reloaing
			if (arrows == 0)
			{
				fletching = true;
				cout << "'" << id << "' has run out of arrows and is now crafting some more." << endl;
			}
		}
		//check for out of ammo and not reloading as to keep from causing errors
		else if (arrows == 0 && !fletching)
		{
			fletching = true;
			cout << "'" << id << "' has realised they have no arrows and is now crafting some more." << endl;
		}
		//reloading process
		else
		{
			arrows++;
			cout << "'" << id << "' has made an arrow for a total of " << arrows << "arrows." << endl; //I realise this will at some point say 1 arrows.
																									   //At this point I don't really care that much.
																									   //once ammo cap has been reached reloading stops
			if (arrows = 10)
			{
				cout << "'" << id << "' has now made 10 arrows and is ready for combat." << endl;
				fletching = false;
			}
		}
	}
	virtual bool ReceiveDamage(int damageReceived)
	{
		//Receives damage as normal, if damage is taken and survives stop reloading
		health -= damageReceived;
		if (health <= 0)
		{
			cout << "'" << id << "' has died." << endl;
			return true;
		}
		else
		{
			if (fletching == true)
			{
				cout << "'" << id << "' has in a panic stopped making arrows." << endl;
			}
			fletching = false;
			return false;
		}
	}
	Fletcher(string newId, int startingX, int startingY)
	{
		//Flecther Constructor
		id = newId;
		xPos = startingX;
		yPos = startingY;
		health = 80;
		damageDealt = 12;
	}
};

class Test {
public:
	Character * player = new Player("Arthur", 0, 0);
	Character* grunt = new Grunt("Blorg", 0, -1, 5, SPEAR);
	Character* fletcher = new Fletcher("Speedy", 2, -1);

	vector <Character*> characters;

	void AttackCycle() {
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				if (i != j)
				{
					characters[i]->Attack(characters[j]);
				}
			}
		}
	}
	Test() {
		characters.push_back(player);
		characters.push_back(grunt);
		characters.push_back(fletcher);
	}
};