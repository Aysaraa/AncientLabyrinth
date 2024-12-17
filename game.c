#include "game.h"


char* safeStringCopy(const char *src) {
    if (!src) return NULL;
    char *dest = malloc(strlen(src) + 1);
    if (dest) {
        strcpy(dest, src);
    }
    return dest;
}

void clearInputBuffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

//This part managed the file operation IO
//Rooms saved on txt file 
//Creatures, items saved on txt file
//Text Files will open and read
Room* loadRooms(const char *filename, int *roomCount) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        fprintf(stderr, "Error opening '%s'\n", filename);
        *roomCount = 0;
        return NULL;
    }

    if (fscanf(fp, "%d\n", roomCount) != 1) {
        fprintf(stderr, "Failed to read roomCount.\n");
        fclose(fp);
        *roomCount = 0;
        return NULL;
    }

    Room *rooms = malloc(sizeof(Room) * (*roomCount));
    if (!rooms) {
        fprintf(stderr, "Memory alloc error for rooms.\n");
        fclose(fp);
        *roomCount = 0;
        return NULL;
    }

    for (int i = 0; i < *roomCount; i++) {
        rooms[i].name        = NULL;
        rooms[i].description = NULL;
        rooms[i].level       = 0;
        rooms[i].item        = NULL;
        rooms[i].creatures   = NULL;
        rooms[i].creatureCount = 0;
        rooms[i].cleared     = false;
        rooms[i].next        = NULL;
        rooms[i].prev        = NULL;

        char bufName [MAX_NAME_LEN];
        char bufDesc [MAX_DESC_LEN];
        int  level;

        if (!fgets(bufName, MAX_NAME_LEN, fp)) {
            fprintf(stderr, "Failed reading room name.\n");
            free(rooms);
            fclose(fp);
            *roomCount = 0;
            return NULL;
        }
        bufName[strcspn(bufName, "\n")] = '\0';

        if (!fgets(bufDesc, MAX_DESC_LEN, fp)) {
            fprintf(stderr, "Failed reading room description.\n");
            free(rooms);
            fclose(fp);
            *roomCount = 0;
            return NULL;
        }
        bufDesc[strcspn(bufDesc, "\n")] = '\0';

        if (fscanf(fp, "%d\n", &level) != 1) {
            fprintf(stderr, "Failed reading room level.\n");
            free(rooms);
            fclose(fp);
            *roomCount = 0;
            return NULL;
        }

        rooms[i].name        = safeStringCopy(bufName);
        rooms[i].description = safeStringCopy(bufDesc);
        rooms[i].level       = level;
    }

    fclose(fp);
    return rooms;
}

Item* loadItems(const char *filename, int *itemCount) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        fprintf(stderr, "Error opening '%s'\n", filename);
        *itemCount = 0;
        return NULL;
    }

    if (fscanf(fp, "%d\n", itemCount) != 1) {
        fprintf(stderr, "Failed to read itemCount.\n");
        fclose(fp);
        *itemCount = 0;
        return NULL;
    }

    Item *items = malloc(sizeof(Item) * (*itemCount));
    if (!items) {
        fprintf(stderr, "Memory alloc error for items.\n");
        fclose(fp);
        *itemCount = 0;
        return NULL;
    }

    for (int i = 0; i < *itemCount; i++) {
        items[i].name        = NULL;
        items[i].description = NULL;
        items[i].hpBoost     = 0;
        items[i].manaBoost   = 0;

        char bufName [MAX_NAME_LEN];
        char bufDesc [MAX_DESC_LEN];
        int  hpBoost, manaBoost;

        if (!fgets(bufName, MAX_NAME_LEN, fp)) {
            fprintf(stderr, "Failed reading item name.\n");
            free(items);
            fclose(fp);
            *itemCount = 0;
            return NULL;
        }
        bufName[strcspn(bufName, "\n")] = '\0';

        if (!fgets(bufDesc, MAX_DESC_LEN, fp)) {
            fprintf(stderr, "Failed reading item description.\n");
            free(items);
            fclose(fp);
            *itemCount = 0;
            return NULL;
        }
        bufDesc[strcspn(bufDesc, "\n")] = '\0';

        if (fscanf(fp, "%d %d\n", &hpBoost, &manaBoost) != 2) {
            fprintf(stderr, "Failed reading item boosts.\n");
            free(items);
            fclose(fp);
            *itemCount = 0;
            return NULL;
        }

        items[i].name        = safeStringCopy(bufName);
        items[i].description = safeStringCopy(bufDesc);
        items[i].hpBoost     = hpBoost;
        items[i].manaBoost   = manaBoost;
    }

    fclose(fp);
    return items;
}

Creature* loadCreatures(const char *filename, int *creatureCount) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        fprintf(stderr, "Error opening '%s'\n", filename);
        *creatureCount = 0;
        return NULL;
    }

    if (fscanf(fp, "%d\n", creatureCount) != 1) {
        fprintf(stderr, "Failed to read creatureCount.\n");
        fclose(fp);
        *creatureCount = 0;
        return NULL;
    }

    Creature *creatures = malloc(sizeof(Creature) * (*creatureCount));
    if (!creatures) {
        fprintf(stderr, "Memory alloc error for creatures.\n");
        fclose(fp);
        *creatureCount = 0;
        return NULL;
    }

    for (int i = 0; i < *creatureCount; i++) {
        creatures[i].name        = NULL;
        creatures[i].description = NULL;
        creatures[i].health      = 0;
        creatures[i].attackPower = 0;
        creatures[i].xpValue     = 0;
        creatures[i].isAlive     = true;

        char bufName [MAX_NAME_LEN];
        char bufDesc [MAX_DESC_LEN];
        int  hp, atk, xpval;

        if (!fgets(bufName, MAX_NAME_LEN, fp)) {
            fprintf(stderr, "Failed reading creature name.\n");
            free(creatures);
            fclose(fp);
            *creatureCount = 0;
            return NULL;
        }
        bufName[strcspn(bufName, "\n")] = '\0';

        if (!fgets(bufDesc, MAX_DESC_LEN, fp)) {
            fprintf(stderr, "Failed reading creature description.\n");
            free(creatures);
            fclose(fp);
            *creatureCount = 0;
            return NULL;
        }
        bufDesc[strcspn(bufDesc, "\n")] = '\0';

        if (fscanf(fp, "%d %d %d\n", &hp, &atk, &xpval) != 3) {
            fprintf(stderr, "Failed reading creature stats.\n");
            free(creatures);
            fclose(fp);
            *creatureCount = 0;
            return NULL;
        }

        creatures[i].name        = safeStringCopy(bufName);
        creatures[i].description = safeStringCopy(bufDesc);
        creatures[i].health      = hp;
        creatures[i].attackPower = atk;
        creatures[i].xpValue     = xpval;
        creatures[i].isAlive     = true;
    }

    fclose(fp);
    return creatures;
}

// This part will create a game world 
// 6 odada, her odada 2 yaratık olsun => toplam 12 yaratık
// her odada 1 item => 6 item
Room* createGameWorld(Room *rooms, int roomCount, 
                      Item *items, int itemCount, 
                      Creature *creatures, int creatureCount)
{
    if (!rooms || roomCount <= 0) {
        fprintf(stderr, "Room array invalid.\n");
        return NULL;
    }

    // Odaları lineer bağla (next, prev)
    for (int i = 0; i < roomCount - 1; i++) {
        rooms[i].next = &rooms[i+1];
        rooms[i+1].prev = &rooms[i];
    }

    //Every room has got 2 creatures.
    //6 rooms each rooms has got 2 creatures
    // 6x2 = 12 creatures totaly.
    for (int i = 0; i < roomCount; i++) {
        int cIndex1 = 2*i;     // first rooms 
        int cIndex2 = 2*i + 1; // second creatures index
        if (cIndex2 < creatureCount) {
            rooms[i].creatureCount = 2;
            rooms[i].creatures = malloc(sizeof(Creature*) * 2);
            rooms[i].creatures[0] = &creatures[cIndex1];
            rooms[i].creatures[1] = &creatures[cIndex2];
        } else {
            // If the creature will not load 
            rooms[i].creatureCount = 0;
            rooms[i].creatures = NULL;
        }
    }

    // Every room has got 1 item.
    for (int i = 0; i < roomCount && i < itemCount; i++) {
        rooms[i].item = &items[i];
    }

    // Başlangıç oda pointer'ı döndür
    return &rooms[0];
}


void freeRooms(Room *rooms, int roomCount) {
    if (!rooms) return;
    for (int i = 0; i < roomCount; i++) {
        free(rooms[i].name);
        free(rooms[i].description);
        if (rooms[i].creatures) {
            free(rooms[i].creatures); // sadece pointer dizisini serbest bırakıyoruz
        }
    }
    free(rooms);
}

// MEMORY SECTION
void freeItems(Item *items, int itemCount) {
    if (!items) return;
    for (int i = 0; i < itemCount; i++) {
        free(items[i].name);
        free(items[i].description);
    }
    free(items);
}

void freeCreatures(Creature *creatures, int creatureCount) {
    if (!creatures) return;
    for (int i = 0; i < creatureCount; i++) {
        free(creatures[i].name);
        free(creatures[i].description);
    }
    free(creatures);
}

// Player Section
void initPlayer(Player *player, const char *name, Room *startRoom) {
    player->name        = safeStringCopy(name);
    player->level       = 1;
    player->xp          = 0;
    player->maxHP       = 100;
    player->currentHP   = 100;
    player->maxMana     = 50;
    player->currentMana = 50;
    player->isAlive     = true;
    player->currentRoom = startRoom;
    for (int i = 0; i < MAX_INVENTORY; i++) {
        player->inventory[i] = NULL;
    }
}

void cleanupPlayer(Player *player) {
    if (!player) return;
    free(player->name);
    // inventory'de sadece referanslar var, asıl item'lar global listede free ediliyor.
}

// ------------------------------------------------------------
// Oyun Döngüsü
// ------------------------------------------------------------
void gameLoop(Player *player, int roomCount) {
    printf("Welcome to the 6-Room Labyrinth!\n");
    printf("Type 'help' to see available commands.\n\n");

    char command[128];
    while (true) {
        if (!player->isAlive) {
            resetPlayerOnDeath(player);
        }

        // Boss odası bitiş kontrolü
        // Oyun son oda (index = roomCount-1) cleared ise bitirelim
        // (Elbette odalar lineer; isterseniz ek mantık ekleyebilirsiniz.)
        Room *lastRoom = &player->currentRoom[roomCount-1]; // bu pek sağlıklı bir hesap değil, 
        // ama pointer aritmetiğine güvenmektense, 
        // en sondaki odaya: rooms[roomCount -1] atama yapabilirdik vs.
        // Basit tutuyoruz: eğer player->currentRoom == o son oda VE cleared 
        // (veya boss yaratık öldüyse) => oyun kazanıldı

        // Daha direkt bir yol: 
        // if (player->currentRoom->next == NULL && player->currentRoom->cleared)
        //     => son oda, temizlenmiş => bitiş

        if (player->currentRoom->next == NULL && player->currentRoom->cleared) {
            printf("\n*** Congratulations! You cleared the final room and defeated the Blue Dragon! ***\n");
            break;
        }

        printf("\n[%s (Level %d)] > ", player->currentRoom->name, player->currentRoom->level);
        if (!fgets(command, sizeof(command), stdin)) {
            printf("Input error or EOF. Exiting...\n");
            break;
        }
        command[strcspn(command, "\n")] = '\0';

        if (strncmp(command, "quit", 4) == 0) {
            printf("Exiting game...\n");
            break;
        }

        handleCommand(player, command, roomCount);

        if (player->currentHP <= 0) {
            player->isAlive = false;
        }
    }
}

// ------------------------------------------------------------
// Komut İşleme
// ------------------------------------------------------------
void handleCommand(Player *player, const char *command, int roomCount) {
    if (strncmp(command, "help", 4) == 0) {
        showHelp();
    }
    else if (strncmp(command, "look", 4) == 0) {
        cmdLook(player);
    }
    else if (strncmp(command, "next", 4) == 0) {
        cmdNext(player, roomCount);
    }
    else if (strncmp(command, "back", 4) == 0) {
        cmdBack(player);
    }
    else if (strncmp(command, "inventory", 9) == 0) {
        cmdInventory(player);
    }
    else if (strncmp(command, "take ", 5) == 0) {
        cmdTakeItem(player, command + 5);
    }
    else if (strncmp(command, "drop ", 5) == 0) {
        cmdDropItem(player, command + 5);
    }
    else if (strncmp(command, "attack", 6) == 0) {
        cmdAttack(player);
    }
    else {
        printf("Unknown command: %s\n", command);
        printf("Type 'help' to see available commands.\n");
    }
}

// COMMAND SECTION
// This game take a input and managed the game
void cmdLook(Player *player) {
    Room *r = player->currentRoom;
    if (!r) return;

    printf("\n--- %s (Level %d) ---\n", r->name, r->level);
    printf("%s\n", r->description);
    
    // Yaratıklar
    if (r->creatureCount > 0) {
        for (int i = 0; i < r->creatureCount; i++) {
            Creature *c = r->creatures[i];
            if (c->isAlive) {
                printf("There is a creature here: %s (HP: %d). %s\n",
                       c->name, c->health, c->description);
            }
        }
    } else {
        printf("No living creatures here.\n");
    }

    // Eşya
    if (r->item) {
        printf("An item is on the ground: %s.\n", r->item->name);
        printf("(%s) [HP Boost: %d, Mana Boost: %d]\n", 
               r->item->description, r->item->hpBoost, r->item->manaBoost);
    }
}

void cmdNext(Player *player, int roomCount) {
    Room *r = player->currentRoom;
    if (!r || !r->next) {
        printf("No next room!\n");
        return;
    }
    // Odayı "cleared" saymak için odadaki tüm yaratıkların ölü olması gerekir
    bool allDead = true;
    for (int i = 0; i < r->creatureCount; i++) {
        if (r->creatures[i]->isAlive) {
            allDead = false;
            break;
        }
    }
    if (!allDead) {
        printf("You must defeat all creatures in this room before moving on!\n");
        return;
    }

    r->cleared = true; // oda temizlendi
    player->currentRoom = r->next;
    printf("You move to the next room: %s\n", player->currentRoom->name);
    cmdLook(player);
}

void cmdBack(Player *player) {
    Room *r = player->currentRoom;
    if (!r || !r->prev) {
        printf("No previous room!\n");
        return;
    }
    player->currentRoom = r->prev;
    printf("You go back to: %s\n", player->currentRoom->name);
    cmdLook(player);
}

void cmdInventory(Player *player) {
    printf("\n--- %s's Inventory ---\n", player->name);
    bool empty = true;
    for (int i = 0; i < MAX_INVENTORY; i++) {
        if (player->inventory[i]) {
            empty = false;
            Item *it = player->inventory[i];
            printf("- %s [HP+%d, Mana+%d]\n", it->name, it->hpBoost, it->manaBoost);
        }
    }
    if (empty) {
        printf("Inventory is empty.\n");
    }
}

void cmdTakeItem(Player *player, const char *itemName) {
    Room *r = player->currentRoom;
    if (!r->item) {
        printf("No item to take in this room.\n");
        return;
    }
    if (strcmp(r->item->name, itemName) != 0) {
        printf("That item is not here.\n");
        return;
    }
    // Envanterde boş yer var mı?
    for (int i = 0; i < MAX_INVENTORY; i++) {
        if (!player->inventory[i]) {
            player->inventory[i] = r->item;
            printf("You picked up '%s'.\n", r->item->name);
            r->item = NULL;
            return;
        }
    }
    printf("Your inventory is full! Drop something first.\n");
}

void cmdDropItem(Player *player, const char *itemName) {
    Room *r = player->currentRoom;
    if (r->item != NULL) {
        printf("There's already an item on the ground. You can't drop here.\n");
        return;
    }
    // Envanterde item var mı?
    for (int i = 0; i < MAX_INVENTORY; i++) {
        if (player->inventory[i] && strcmp(player->inventory[i]->name, itemName) == 0) {
            r->item = player->inventory[i];
            player->inventory[i] = NULL;
            printf("You dropped '%s'.\n", itemName);
            return;
        }
    }
    printf("You don't have that item in your inventory.\n");
}

void cmdAttack(Player *player) {
    Room *r = player->currentRoom;
    if (!r || r->creatureCount == 0) {
        printf("No creatures to attack here.\n");
        return;
    }

    // Seçim yapmadık, basit bir mantıkla ilk yaşayan yaratığa saldırı
    Creature *target = NULL;
    for (int i = 0; i < r->creatureCount; i++) {
        if (r->creatures[i]->isAlive) {
            target = r->creatures[i];
            break;
        }
    }
    if (!target) {
        printf("No living creatures here.\n");
        return;
    }

    int playerDamage = 10 + (player->level * 2);
    target->health -= playerDamage;
    printf("You attacked '%s' for %d damage.\n", target->name, playerDamage);

    if (target->health <= 0) {
        target->health = 0;
        target->isAlive = false;
        printf("You have slain %s!\n", target->name);
        // XP kazan
        player->xp += target->xpValue;
        printf("You gained %d XP! (Total: %d)\n", target->xpValue, player->xp);

        // Seviye atlama kontrolü
        checkLevelUp(player);

        // Loot: Bu örnekte item odanın yerdeki item'ı zaten. 
        // Özel loot sistemi isterseniz ekleyebilirsiniz.
    } else {
        // Karşı saldırı
        player->currentHP -= target->attackPower;
        printf("%s strikes back for %d damage! Your HP: %d/%d\n",
               target->name, target->attackPower, player->currentHP, player->maxHP);
        if (player->currentHP <= 0) {
            player->currentHP = 0;
            player->isAlive = false;
            printf("You were killed by %s...\n", target->name);
        }
    }
}

// Help command
void showHelp(void) {
    printf("\n--- Available Commands ---\n");
    printf("help         : Show this help menu\n");
    printf("look         : Examine the current room\n");
    printf("next         : Move to the next room (if cleared)\n");
    printf("back         : Move to the previous room (if exists)\n");
    printf("attack       : Attack a living creature in the room\n");
    printf("take <item>  : Pick up an item from the ground\n");
    printf("drop <item>  : Drop an item from your inventory\n");
    printf("inventory    : Show your inventory\n");
    printf("quit         : Exit the game\n");
}

//Level control
void checkLevelUp(Player *player) {
    while (player->xp >= 100) {
        player->xp -= 100;
        player->level++;
        player->maxHP   += 20;
        player->maxMana += 10;
        player->currentHP   = player->maxHP;
        player->currentMana = player->maxMana;
        printf("\n*** Level Up! You are now level %d ***\n", player->level);
        printf("MaxHP = %d, MaxMana = %d\n", player->maxHP, player->maxMana);
    }
}

//If the player dead, player born on the first room.
void resetPlayerOnDeath(Player *player) {
    
    // Find the first room
    if (!player->currentRoom) return;
    while (player->currentRoom->prev != NULL) {
        player->currentRoom = player->currentRoom->prev;
    }
    player->currentHP   = player->maxHP;
    player->currentMana = player->maxMana;
    player->isAlive     = true;
    printf("You have been dead and sent back to the first room: %s\n", player->currentRoom->name);
}
