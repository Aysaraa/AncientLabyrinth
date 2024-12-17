#ifndef GAME_H
#define GAME_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_INVENTORY 5
#define MAX_NAME_LEN  64
#define MAX_DESC_LEN  256



typedef struct {
    char *name;
    char *description;
    int  hpBoost;      // Enhancement for HP
    int  manaBoost;    // Enhancement for Mana
} Item;

typedef struct {
    char *name;
    char *description;
    int  health;
    int  attackPower;
    int  xpValue;
    bool isAlive;
} Creature;

typedef struct Room {
    char *name;
    char *description;
    int   level;

    // Items will save on pointer like array
    Item  *item;

    // Every room has got 2 creatures
    Creature **creatures;
    int creatureCount; // This variable took the creature count.

    bool cleared;      // This variable ensure the next level
    struct Room *next;
    struct Room *prev;
} Room;

typedef struct {
    char *name;
    int  level;
    int  xp;
    int  maxHP;
    int  currentHP;
    int  maxMana;
    int  currentMana;
    bool isAlive;

    Room *currentRoom;
    Item *inventory[MAX_INVENTORY];
} Player;

// FUNCTIONS

// TXT file reading
Room*      loadRooms   (const char *filename, int *roomCount);
Item*      loadItems   (const char *filename, int *itemCount);
Creature*  loadCreatures(const char *filename, int *creatureCount);

// Creating the game world
Room* createGameWorld(Room *rooms, int roomCount, 
                      Item *items, int itemCount, 
                      Creature *creatures, int creatureCount);

// Memory functions
void freeRooms     (Room *rooms, int roomCount);
void freeItems     (Item *items, int itemCount);
void freeCreatures (Creature *creatures, int creatureCount);

// Player's functions
void initPlayer(Player *player, const char *name, Room *startRoom);
void cleanupPlayer(Player *player);

// Game loop
void gameLoop(Player *player, int roomCount);
void handleCommand(Player *player, const char *command, int roomCount);

// Command function
void cmdLook       (Player *player);
void cmdNext       (Player *player, int roomCount);
void cmdBack       (Player *player);
void cmdInventory  (Player *player);
void cmdTakeItem   (Player *player, const char *itemName);
void cmdDropItem   (Player *player, const char *itemName);
void cmdAttack     (Player *player);

// Help command function
void showHelp(void);

// Control the levels
void checkLevelUp(Player *player);

// Dead player will go to first room 
void resetPlayerOnDeath(Player *player);

// Other functions
char* safeStringCopy(const char *src);
void  clearInputBuffer(void);

#endif
