#include "tp3.h"
#include <string.h>
#include <stdio.h>

// Mehemmed's Part

T_Site *createSite(char *city) {
    if (!city) return NULL;

    T_Site *new_site = (T_Site *)malloc(sizeof(T_Site));
    if (!new_site) return NULL;

    new_site->city = strdup(city);
    new_site->clients = 0;
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
        printf("%-20s | %d\n", curr->city, curr->clients);
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

int addClient(T_Site *site, T_Client *client) {
    if (!site || !client) return 0;

    // if the list is NULL
    if (site->first == NULL || 
        strcmp(client->last_name, site->first->last_name) < 0 || 
        (strcmp(client->last_name, site->first->last_name) == 0 && strcmp(client->first_name, site->first->first_name) < 0)) {
        client->next = site->first;
        site->first = client;
    } else {
        T_Client *curr = site->first;
        while (curr->next != NULL && 
               (strcmp(curr->next->last_name, client->last_name) < 0 || 
               (strcmp(curr->next->last_name, client->last_name) == 0 && strcmp(curr->next->first_name, client->first_name) < 0))) {
            curr = curr->next;
        }
        client->next = curr->next;
        curr->next = client;
    }

    site->clients++;
    return 1;
}




int deleteClient(T_Site *site, char *last_name, char *first_name){
    if (site->clients > 0) {    
        T_Client *current_client = site->first;
        // first client == wanted client
        if(
            strcmp(current_client->last_name, last_name) == 0 && 
            strcmp(current_client->first_name, first_name) == 0
        ) {
            site->first = current_client->next;
            return 1;
        }
        else{
            for (int i = 0; i < site->clients; i++)
            {
                if(current_client->next != NULL) {
                    T_Client *next_client = current_client->next;
                    if (
                        strcmp(next_client->last_name, last_name) == 0 &&
                        strcmp(next_client->first_name, first_name) == 0
                    ) {
                        current_client->next = next_client->next;
                        return 1;
                    }
                    else current_client = current_client->next;
                }
                else return 0; // no wanted client in site
                
            }
        }
    }
    return 0; // no clients in site
}

static int compareClients(const void *a, const void *b)
{
    const T_Client *c1 = *(T_Client * const *)a;
    const T_Client *c2 = *(T_Client * const *)b;

    int result = strcmp(c1->last_name, c2->last_name);

    if (result == 0)
        result = strcmp(c1->first_name, c2->first_name);

    return result;
}

void searchClients(T_Hotel *hotel, char *critereaName)
{
    if (hotel == NULL || critereaName == NULL) {
        printf("Invalid hotel or search criterion.\n");
        return;
    }

    // remove the newline potentially added by fgets()
    size_t len = strcspn(critereaName, "\r\n");

    if (len == 0) {
        printf("Please enter a name to search.\n");
        return;
    }

    T_Client **results = NULL;
    size_t count = 0;
    size_t capacity = 0;

    T_Site *site = hotel->first;

    while (site != NULL) {
        T_Client *client = site->first;

        while (client != NULL) {
            if (client->first_name != NULL &&
                client->last_name != NULL &&
                (strncasecmp(client->first_name, critereaName, len) == 0 ||
                 strncasecmp(client->last_name, critereaName, len) == 0)) {

                if (count == capacity) {
                    size_t new_capacity = (capacity == 0) ? 8 : capacity * 2;

                    T_Client **temp = realloc(
                        results, new_capacity * sizeof(*results)
                    );

                    if (temp == NULL) {
                        free(results);
                        printf("Memory allocation failed.\n");
                        return;
                    }

                    results = temp;
                    capacity = new_capacity;
                }

                results[count] = client;
                count++;
            }

            client = client->next;
        }

        site = site->next;
    }

    if (count == 0) {
        printf("No clients found.\n");
        free(results);
        return;
    }

    qsort(results, count, sizeof(*results), compareClients);

    printf("Found %zu client(s):\n", count);

    for (size_t i = 0; i < count; i++) {
        printf("%s %s | Age: %d | Room: %d\n",
               results[i]->first_name,
               results[i]->last_name,
               results[i]->age,
               results[i]->num_room);
    }

    free(results);
}

void quit(T_Hotel hotel){
    T_Site *current_site = hotel.first;
    T_Client *current_client = current_site->first;
    while (1)
    {
        for (int i = 0; i < current_site->clients; i++)
        {
            T_Client *next_client = current_client->next;

            free(current_client);

            current_client = next_client;
        }
        if (current_site->next != NULL) current_site = current_site->next;
        else break;        
    }
}

T_Client *createClient(char *last_name, char *first_name, int age, int num_room) {
    if (last_name == NULL || first_name == NULL)
        return NULL;

    T_Client *client = malloc(sizeof(T_Client));

    if (client == NULL)
        return NULL;

    client->last_name = strdup(last_name);
    client->first_name = strdup(first_name);

    if (client->last_name == NULL || client->first_name == NULL) {
        free(client->last_name);
        free(client->first_name);
        free(client);
        return NULL;
    }

    client->age = age;
    client->num_room = num_room;
    client->next = NULL;

    return client;
}

void processingfile(T_Hotel *hotel)
{
    if (hotel == NULL)
        return;

    FILE *file = fopen("bookings.txt", "r");

    if (file == NULL) {
        printf("Could not open bookings.txt\n");
        return;
    }

    char line[256];

    while (fgets(line, sizeof(line), file) != NULL) {
        char last[50], first[50], city[100];
        int age, room;

        if (sscanf(line, "%49[^;];%49[^;];%d;%d;%99[^\n]",
                   last, first, &age, &room, city) != 5) {
            continue;
        }

        T_Site *site = hotel->first;

        while (site != NULL &&
               strcasecmp(site->city, city) != 0) {
            site = site->next;
        }

        if (site == NULL) {
            site = createSite(city);

            if (site == NULL || !addSite(hotel, site)) {
                freeSite(site);
                continue;
            }
        }

        T_Client *client = createClient(last, first, age, room);

        if (client == NULL || !addClient(site, client)) {
            freeClient(client);
        }
    }

    fclose(file);
}