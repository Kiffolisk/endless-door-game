#include "global.h"

const char* doorFindings[4028];
int doorsGoneThrough = 0;
int money = 0;
int abandonedDevices = 0;
int workingDevices = 0;
int health = 100;
int medkits = 0;
bool paused = false;
bool dead = false;
bool isAttacked = false;
bool behind = false;
int behindTimer = 0;

void InitSettings()
{
	srand(time(NULL));
	doorFindings[0] = "You found nothing inside.";
	doorFindings[1] = "You found a coin on the ground. +1 Money.";
	doorFindings[2] = "There's a painting on the wall...";
	doorFindings[3] = "There's a window. You look outside.";
	doorFindings[4] = "You found a broken computer.";
	doorFindings[5] = "You found a cellphone.";
	doorFindings[6] = "You tripped. Lost 1 health."; // originally 5 damage
	// 3 medkit chance because unfair damage stuff
	doorFindings[7] = "You found a medkit.";
	doorFindings[8] = "You found a medkit.";
	doorFindings[9] = "You found a medkit.";
	// 3 different sick chances because unfair chance stuff
	doorFindings[10] = "You feel sick.";

	// cant get normally when opening door, hardcoded results
	doorFindings[4025] = "You feel something behind you.\n\n\nPRESS X!!";
	doorFindings[4026] = "You almost felt sick.";
	doorFindings[4027] = "You're already sick.";
}

void ViewStuffNoDoor()
{
	consoleClear();
	iprintf("Doors gone through: %d\nHealth: %d\nMoney: %d\n\n", doorsGoneThrough, health, money);
	iprintf("Press A to go through door\nPress START to close\nPress SELECT to see stats\n\n");
}

void Tick()
{
	if (behind)
	{
		behindTimer++;
	}
	else if (!behind)
	{
		behindTimer = 0;
	}
	if (behindTimer > 10000)
	{
		dead = true;
		health = 0;
	}
	if (health > 100)
	{
		health = 100;
	}
	if (health < 1 && !dead)
	{
		/* const char* rating = "You sucked.";
		// rating bools
		bool ratingSucked = doorsGoneThrough < 5;
		bool ratingPrettyGood = doorsGoneThrough < 12 && doorsGoneThrough > 4;
		bool ratingGreat = doorsGoneThrough < 32 && doorsGoneThrough > 11;
		bool ratingTheBest = doorsGoneThrough < 42 && doorsGoneThrough > 31;
		// rating strings
		if (ratingSucked)
		{
			// can't get unless you damage yourself???
			rating = "You killed yourself?";
		}
		else if (ratingPrettyGood)
		{
			rating = "You were pretty good!";
		}
		else if (ratingGreat)
		{
			rating = "You were great!";
		}
		else if (ratingTheBest)
		{
			rating = "You were the best!";
		} */
		consoleClear();
		iprintf("You died!\nPress A to restart.");
		dead = true;
	}
}

void GoThroughDoor()
{
	consoleClear();
	doorsGoneThrough++;
	int response = rand() % 12;
	if (response == 1)
	{
		money++;
	}
	else if (response == 4)
	{
		abandonedDevices++;
	}
	else if (response == 5)
	{
		workingDevices++;
	}
	else if (response == 6)
	{
		health -= 1;
	}
	else if (response == 7)
	{
		medkits++;
	}
	else if (response == 8)
	{
		medkits++;
	}
	else if (response == 9)
	{
		medkits++;
	}
	else if (response == 10)
	{
		// chance of not getting sick
		int response2 = rand() % 60;
		if (!isAttacked)
		{
			if (response2 > 32 && response2 < 38)
			{
				isAttacked = true;
			}
			else
			{
				response = 4026;
			}
		}
		else
		{
			response = 4027;
		}
	}
	else if (response == 11)
	{
		// chance of not getting sick
		int response2 = rand() % 295;
		if (response2 > 190 && response2 < 199)
		{
			behind = true;
		}
		else
		{
			response = 0;
		}
	}
	if (isAttacked)
	{
		health -= 5; // was 10 damage
	}
	iprintf("Doors gone through: %d\nHealth: %d\nMoney: %d\n\n", doorsGoneThrough, health, money);
	iprintf("Press A to go through door\nPress START to close\nPress SELECT to see stats\n\n");
	iprintf("%s\n", doorFindings[response]);
}

void ShowInfo()
{
	consoleClear();
	iprintf("Press Y to damage yourself (deals 5 damage)\nPress B to heal up (need a medkit)\n\nDoors gone through: %d\nHealth: %d\nMoney: %d\nBroken devices: %d\nWorking devices: %d\n\nPress SELECT to close", doorsGoneThrough, health, money, abandonedDevices, workingDevices);
}

void Heal()
{
	ViewStuffNoDoor();
	if (medkits > 1)
	{
		medkits = medkits - 1;
		iprintf("Healed up!\nGained 10 health.\n");
		health += 10;
	}
	else
	{
		iprintf("You don't have a medkit!\n");
	}
}

void Restart()
{
	dead = false;
	doorsGoneThrough = 0;
	health = 100;
	money = 0;
	workingDevices = 0;
	abandonedDevices = 0;
	medkits = 0;
	paused = false;
	InitSettings();
	ViewStuffNoDoor();
}

void Damage(int dmg)
{
	ViewStuffNoDoor();
	if (isAttacked)
	{
		iprintf("I feel better now.");
		isAttacked = false;
	}
	else
	{
		iprintf("Ouch!");
	}
	health -= dmg;
}

int main(void) {
	consoleDemoInit();
	InitSettings();
	ViewStuffNoDoor();
	while(1) {
		swiWaitForVBlank();
		scanKeys();
		int pressed = keysDown();
		Tick();
		if(pressed & KEY_A && !paused && !dead)
		{
			GoThroughDoor();
		}
		if(pressed & KEY_A && !paused && dead)
		{
			Restart();
		}
		if(pressed & KEY_B && !paused && !dead)
		{
			Heal();
		}
		if(pressed & KEY_Y && !paused && !dead)
		{
			Damage(5);
		}
		if(pressed & KEY_SELECT && !dead)
		{
			if (!paused)
			{
				ShowInfo();
				paused = true;
			}
			else
			{
				ViewStuffNoDoor();
				paused = false;
			}
		}
		if(pressed & KEY_X && !paused && behind)
		{
			if (behind)
			{

			}
		}
		if(pressed & KEY_START) break;
	}

}
