#ifndef WALKWAYS_H
#define WALKWAYS_H

typedef struct hashtable_s HashTable;
typedef struct list_s List;

struct walkway_manager {
    HashTable *walkways;
};

typedef struct walkway_entrance_s {
    char *model_from;
    char *model_to;
    List *from_coords;
    List *destination_coords;
} walkway_entrance;

void walkways_init();
void walkways_add(char *model_from, char *model_to, char *from_coords, char *destination_coords);
List *walkways_get_coords(char *coords);

#endif