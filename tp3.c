#include "tp3.h"

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

// Nurlan's Part

#include "tp3.h"
#include <string.h>
#include <stdio.h>


int addClient(T_Site *site, T_Client *client){
    T_Client *current_client;

    if (site->first == NULL){
        site->first = malloc(sizeof(T_Client));
        *(site->first) = *client;
        site->first->next = NULL;
    }
    else{
        current_client = site->first;
        while (1){
            if (current_client = NULL) break;
            
            if (strcmp(current_client->last_name, client->last_name) < 0)
            {
                client->next = current_client->next;
                current_client->next = client;
                current_client = client->next;

            }
            else{
                client->next = current_client;
                break;
            }
        }



        // current_client = site->first;

        // while(current_client->next != NULL) current_client = current_client->next;
        // current_client->next = malloc(sizeof(T_Client));
        // current_client = current_client->next;
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
    else return 0; // no clients in site
}

void searchClients(T_Hotel *hotel, char *critereaName){
    T_Site *current_site = hotel->first;
    int result_num = 0;
    T_Client *first_result = NULL;
    T_Client *current_result = first_result;
    

    while (current_site != NULL)
    {
        T_Client *current_client = current_site->first;
        
        for (int i = 0; i < current_site->clients; i++)
        {
            int is_it_result = 1;
            for (int j = 0; j < strlen(critereaName); j++)
            {
                if (current_client->last_name[i] != critereaName[i]) is_it_result = 0;
            }
            if(is_it_result){
                while (1)
                {
                    if (first_result == NULL){
                        first_result = malloc(sizeof(T_Client));
                        first_result = current_client;
                    }
                    else{
                        current_result->next = malloc(sizeof(T_Client));
                        current_result->next = current_client;
                        current_result = current_result->next;
                    }
                }
                result_num++;
            }
        }
        current_site = current_site->next;
    }
    
    // sorting for first_name

    int swap = 0;
    while(swap != 0){
        current_result = first_result;
        swap = 0;

        for (int i = 0; i < result_num; i++)
        {
            if(strcmp(
                current_result->first_name,
                current_result->next->first_name
            ) > 0){
                T_Client *temp_ptr = current_result->next->next;
                current_result->next = temp_ptr;
                current_result->next->next = current_result;
                swap++;
            }
        }
    }

    // print results
    printf("Last name:\t\tFirst name:\t\tAge:\t\tRoom:");
    current_result = first_result;
    for (int i = 0; i < result_num; i++)
    {
        printf("%s\t\t%s\t\t%d\t\t%d", current_result->last_name, current_result->first_name, current_result->age, current_result->num_room);
        current_result = current_result->next;
    }
}

void quit(T_Hotel hotel){
    T_Site *current_site = hotel.first;
    T_Client *current_client = current_site->first;
    while (1)
    {
        for (int i = 0; i < current_site->clients; i++)
        {
            free(current_client);
            current_client = current_client->next;
        }
        if (current_site->next != NULL) current_site = current_site->next;
        else break;        
    }
}