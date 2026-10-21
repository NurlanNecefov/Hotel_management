#ifndef TP3_H
#define TP3_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct Client
{
    char *last_name;
    char *first_name;
    int age;
    int num_room;
    T_Client *next;
} T_Client;

typedef struct Site
{
    char *city;
    int clients;
    T_Client *first;
    T_Client *next;
} T_Site;

typedef struct Hotel
{
    char *name;
    T_Client *first;
} T_Hotel;

T_Client *createClient(char *last_name, char *first_name, int age, int num_room);
T_Site *createSite(char *city);
T_Hotel *createHotel(char *name);

int addClient(T_Site *site, T_Client *client);

int addSite(T_Hotel *hotel, T_Site *site);

void displayHotel(T_Hotel *hotel);

void displaySite(T_Site *site);

int deleteClient(T_Site *site, char *last_name, char *first_name);

int deleteSite(T_Hotel *hotel, char *city);

void searchClients(T_Hotel *hotel, char *critereaName);

void processingfile(T_Hotel *hotel);

// to free the space of malloc
void freeHotel(T_Hotel *hotel);

#endif