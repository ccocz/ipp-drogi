#include "map.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

Map *newMap(void) {
  Map *aux = malloc(sizeof(Map));
  if (!aux) {
    return aux;
  } else {
    aux->cities = NULL;
    return aux;
  }
}

bool badName(const char *city) {
  for (int i = 0; i < strlen(city); i++) {
    if (city[i] <= 31 || city[i] == ';') {
      return true;
    }
  }
  return false;
}

City *cityExists(Map *map, const char *city) {
  City *aux = map->cities;
  while (aux) {
    if (!strcmp(aux->name, city)) {
      return aux;
    }
    aux = aux->next;
  }
  return NULL;
}

City *addCity(Map *map, const char *city) {
  City *aux = malloc(sizeof(City));
  aux->name = city;
  aux->roads = NULL;
  aux->next = map->cities;
  map->cities = aux;
  return aux;
}

Road *isConnected(City *city1, City *city2) {
  Road *aux = city1->roads;
  while (aux) {
    if (aux->to == city2) {
      return aux;
    }
    aux = aux->next;
  }
  return NULL;
}

void addEdge(City *from, City *to,
             unsigned length, int builtYear) {
  Road *aux = malloc(sizeof(Road));
  aux->to = to;
  aux->length = length;
  aux->year = builtYear;
  aux->next = from->roads;
  from->roads = aux;
}

void connectCities(City *city1, City *city2,
                   unsigned length, int builtYear) {
  addEdge(city1, city2, length, builtYear);
  addEdge(city2, city1, length, builtYear);
}

bool addRoad(Map *map, const char *city1, const char *city2,
             unsigned length, int builtYear) {
  if (badName(city1) || badName(city2)) {
    return false;
  }
  if (!strcmp(city1, city2)) {
    return false;
  }
  City *first, *second;
  if (!(first = cityExists(map, city1))) {
    first = addCity(map, city1); // check allocation
  }
  if (!(second = cityExists(map, city2))) {
    second = addCity(map, city2); // check allocation
  }
  if (isConnected(first, second)) {
    return false;
  }
  connectCities(first, second, length, builtYear);
  // check allocation
  return true;
}

bool repairRoad(Map *map, const char *city1,
                const char *city2, int repairYear){
  if (badName(city1) || badName(city2)) {
    return false;
  }
  City *first = cityExists(map, city1);
  City *second = cityExists(map, city2);
  if (!first || !second) {
    return false;
  }
  Road *go = isConnected(first, second);
  Road *come = isConnected(second, first);
  if (!go || !come) {
    return false;
  }
  if (go->year > repairYear) {
    return false;
  }
  go->year = come->year = repairYear;
  return true;
}