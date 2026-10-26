#include "tp3.h"

T_Site *createSite(char *city) {
    if (!city) return NULL;

    T_Site *new_site = (T_Site *)malloc(sizeof(T_Site));
    if (!new_site) return NULL;

    new_site->city = strdup(city);
    new_site->number_clients = 0;
    new_site->first = NULL;
    new_site->next = NULL;

    return new_site;
}

T_Hotel *createHotel(char *name) {
    if (!name) return NULL;

    T_Hotel *new_hotel = (T_Hotel *)malloc(sizeof(T_Hotel));
    if (!new_hotel) return NULL;

    new_hotel->name = strdup(name);
    new_hotel->first = NULL;

    return new_hotel;
}

int addSite(T_Hotel *hotel, T_Site *site) {
    if (!hotel || !site) return 0;

    T_Site *curr = hotel->first;
    T_Site *prev = NULL;

    while (curr != NULL) {
        int cmp = strcasecmp(site->city, curr->city);
        if (cmp == 0) {
            /* Город уже существует */
            return 0;
        }
        if (cmp < 0) {
            break;
        }
        prev = curr;
        curr = curr->next;
    }

    if (prev == NULL) {
        site->next = hotel->first;
        hotel->first = site;
    } else {
        site->next = curr;
        prev->next = site;
    }

    return 1;
}

void displayHotel(T_Hotel *hotel) {
    if (!hotel) return;

    printf("\n%-20s | %s\n", "City", "Number of customers");
    printf("-----------------------------------------\n");

    T_Site *curr = hotel->first;
    while (curr != NULL) {
        printf("%-20s | %d\n", curr->city, curr->number_clients);
        curr = curr->next;
    }
    printf("\n");
}

void displaySite(T_Site *site) {
    if (!site) return;

    printf("\n%-15s | %-15s | %-5s | %-5s\n", "Last name", "First name", "Age", "Room");
    printf("---------------------------------------------------\n");

    T_Client *curr = site->first;
    while (curr != NULL) {
        printf("%-15s | %-15s | %-5d | %-5d\n", 
               curr->last_name, curr->first_name, curr->age, curr->num_room);
        curr = curr->next;
    }
    printf("\n");
}

int deleteSite(T_Hotel *hotel, char *city) {
    if (!hotel || !city) return 0;

    T_Site *curr = hotel->first;
    T_Site *prev = NULL;

    while (curr != NULL) {
        if (strcasecmp(curr->city, city) == 0) {
            if (prev == NULL) {
                hotel->first = curr->next;
            } else {
                prev->next = curr->next;
            }

            freeSite(curr);
            return 1;
        }
        prev = curr;
        curr = curr->next;
    }

    return 0;
}

/* Helper Functions */
void freeClient(T_Client *client) {
    if (!client) return;
    free(client->last_name);
    free(client->first_name);
    free(client);
}

void freeSite(T_Site *site) {
    if (!site) return;
    T_Client *curr = site->first;
    while (curr != NULL) {
        T_Client *tmp = curr;
        curr = curr->next;
        freeClient(tmp);
    }
    free(site->city);
    free(site);
}

void freeHotel(T_Hotel *hotel) {
    if (!hotel) return;
    T_Site *curr = hotel->first;
    while (curr != NULL) {
        T_Site *tmp = curr;
        curr = curr->next;
        freeSite(tmp);
    }
    free(hotel->name);
    free(hotel);
}