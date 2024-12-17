#include "game.h"

void showIntroduction(void) {
    printf("\n");
    printf("**************************************************\n");
    printf("*         WELCOME TO Ancient Labyrinth           *\n");
    printf("**************************************************\n");
    printf("\n");
    printf("There are 6 ancient rooms here.\n");
    printf("Your goal is to defeat all the monsters and\n");
    printf("overcome the final boss: The Blue Dragon.\n\n");
    
    printf("GAME COMMANDS:\n");
    printf("  look          - Examine the current room\n");
    printf("  next          - Move to the next room (if cleared)\n");
    printf("  back          - Return to the previous room\n");
    printf("  attack        - Attack a monster in the room\n");
    printf("  take <item>   - Pick up an item from the room\n");
    printf("  drop <item>   - Drop an item from your inventory\n");
    printf("  inventory     - View your inventory\n");
    printf("  help          - Display the list of commands\n");
    printf("  quit          - Exit the game\n\n");

    printf("HINT:\n");
    printf("  - Kill all monsters in a room.\n");
    printf("  - Use items to boost your HP and Mana.\n\n");

    printf("GOOD LUCK....\n");
    printf("**************************************************\n");
    printf("\n");
}



int main(int argc, char *argv[]) {
    // Txt file s 
    const char *roomFile      = "rooms.txt";
    const char *itemFile      = "items.txt";
    const char *creatureFile  = "creatures.txt";

    // Room, item count
    int roomCount     = 0;
    int itemCount     = 0;
    int creatureCount = 0;

    // TXT files reading
    Room     *rooms     = loadRooms(roomFile, &roomCount);
    Item     *items     = loadItems(itemFile, &itemCount);
    Creature *creatures = loadCreatures(creatureFile, &creatureCount);
    
    //TXT loading errors clean the memory and exit the game
    if (!rooms || !items || !creatures) {
        freeRooms(rooms, roomCount);
        freeItems(items, itemCount);
        freeCreatures(creatures, creatureCount);
        return 1;
    }

    // Create the 6 rooms with rooms.txt
    Room *startRoom = createGameWorld(rooms, roomCount, items, itemCount, creatures, creatureCount);
    if (!startRoom) {
        fprintf(stderr, "Failed to create game world.\n");
        freeRooms(rooms, roomCount);
        freeItems(items, itemCount);
        freeCreatures(creatures, creatureCount);
        return 1;
    }

    // Initialize the Player
    Player player;
    initPlayer(&player, "Hero", startRoom);

    showIntroduction();
    // Game Loop
    gameLoop(&player, roomCount);

    // Malloc 
    // Cleaning the all memory
    cleanupPlayer(&player);
    freeRooms(rooms, roomCount);
    freeItems(items, itemCount);
    freeCreatures(creatures, creatureCount);

    return 0;
}
