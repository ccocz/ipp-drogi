#include "map.h"
#include "heap.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void freeMap(Map *map) {
  City *aux, *help;
  Road *roads, *helpRoads;
  for (int i = 0; i < N; i++) {
    aux = map->cities[i];
    while (aux) {
      help = aux;
      roads = aux->roads;
      while (roads) {
        helpRoads = roads;
        roads = roads->next;
        free(helpRoads);
      }
      aux = aux->next;
      free(help);
    }
  }
  Route *route, *helpRoute;
  for (int i = 0; i < R; i++) {
    route = map->routes[i];
    while (route) {
      helpRoute = route;
      route = route->next;
      free(helpRoute);
    }
  }
  free(map);
}

Map *newMap(void) {
  Map *aux = malloc(sizeof(Map));
  if (!aux) {
    return aux;
  } else {
    for (int i = 0; i < N; i++) {
      aux->cities[i] = NULL;
    }
    for (int i = 0; i < R; i++) {
      aux->routes[i] = NULL;
    }
    return aux;
  }
}

bool badName(const char *city) {
  int n = strlen(city);
  for (size_t i = 0; i < n; i++) {
    if (city[i] <= 31 || city[i] == ';') {
      return true;
    }
  }
  return false;
}

int hashIt(const char *s) {
  int n = strlen(s);
  int mod = N;
  int base = 1453;
  int hash = 0;
  for (size_t i = 0; i < n; i++) {
    hash = (hash + (((s[i] % mod) * base) % mod)) % mod;
    base = (base * 1453) % mod;
  }
  return hash;
}

City *cityExists(Map *map, const char *city) {
  int hash = hashIt(city);
  City *aux = map->cities[hash];
  while (aux) {
    if (!strcmp(aux->name, city)) {
      return aux;
    }
    aux = aux->next;
  }
  return NULL;
}

City *addCity(Map *map, const char *city) {
  int n = strlen(city);
  int hash = hashIt(city);
  City *where = map->cities[hash];
  City *aux = malloc(sizeof(City));
  aux->name = malloc(sizeof(char) * n);
  aux->name = city;
  aux->roads = NULL;
  aux->heapNode = NULL;
  aux->next = where;
  if (where) {
    where->prev = aux;
  }
  map->cities[hash] = aux;
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

inline int maxi(int x, int y) {
  return x > y ? x : y;
}

Route *Dijkstra(Map *map, City *source, City *destination) {
  Heap *Q = newHeap(source);
  City *aux;
  for (int i = 0; i < N; i++) {
    aux = map->cities[i];
    while (aux) {
      if (aux != source) {
        insertHeap(Q, aux);
      }
      aux = aux->next;
    }
  }
  HeapNode *best;
  Road *adj;
  while (Q->root) {
    heapifyMin(Q->root);
    best = minHeap(Q);
    adj = best->city->roads;
    while (adj) {
      if (!adj->to->heapNode->visited) {
        if (best->distance + adj->length < adj->to->heapNode->distance) {
          decreaseValue(adj->to->heapNode, best->distance + adj->length, maxi(best->year, adj->year));
          adj->to->heapNode->from = best->city;
          adj->to->heapNode->lastYear = adj->year;
        } else if (best->distance + adj->length == adj->to->heapNode->distance) {
          if (maxi(best->year, adj->year) > adj->to->heapNode->year) {
            decreaseValue(adj->to->heapNode, best->distance + adj->length, maxi(best->year, adj->year));
            adj->to->heapNode->from= best->city;
            adj->to->heapNode->lastYear = adj->year;
          }
        }
      }
      adj = adj->next;
    }
    freeMe(Q, best);
  }
  bool ok = true;
  Road *toDest = destination->roads;
  while (ok && toDest) {
    if (toDest->to != destination->heapNode->from) {
      if (toDest->to->heapNode->distance + toDest->length == destination->heapNode->distance &&
          maxi(toDest->to->heapNode->year, toDest->year) == destination->heapNode->year) {
        ok = false;
      }
    }
    toDest = toDest->next;
  }
  Route *ret = NULL, *help;
  if (ok) {
    City *traverse = destination;
    while (traverse) {
      puts(traverse->name);
      help = malloc(sizeof(Route));
      help->city = traverse;
      help->next = ret;
      ret = help;
      traverse = traverse->heapNode->from;
    }
  }
  for (int i = 0; i < N; i++) {
    aux = map->cities[i];
    while (aux) {
      free(aux->heapNode);
      aux = aux->next;
    }
  }
  freeHeap(Q);
  return ret;
}

bool newRoute(Map *map, unsigned routeId,
              const char *city1, const char *city2) {
  if (badName(city1) || badName(city2)) {
    return false;
  }
  if (map->routes[routeId]) {
    return false;
  }
  if (!strcmp(city1, city2)) {
    return false;
  }
  City *first = cityExists(map, city1);
  City *second = cityExists(map, city2);
  if (!first || !second) {
    return false;
  }
  Route *ans = Dijkstra(map, first, second);
  if (!ans) {
    return false;
  } else {
    map->routes[routeId] = ans;
    return true;
  }
}

inline int lgp(unsigned x) {
  int ret = 0;
  while (x) {
    ret++;
    x /= 10;
  }
  return ret;
}

inline void copy(unsigned  x, int from, char *s) {
  while (x) {
    s[from--] = (char)((x % 10) + 'a');
    x /= 10;
  }
}

inline void copyStr(const char *from, char *to, int start) {
  for (int i = start; i < start + (int)strlen(from); i++) {
    to[i] = from[i - start];
  }
}

char const *getRouteDescription(Map *map, unsigned routeId) {
  if (map->routes[routeId] == NULL) {
    return "";
  } else {
    int length = lgp(routeId);
    int totalAlloced = length;
    int lgo;
    char const *ret = malloc(sizeof(char) * length);
    copy(routeId, length - 1, ret);
    Route *aux = map->routes[routeId];
    Road *help;
    while (aux) {
      if (length + strlen(aux->city->name) > totalAlloced) {
        ret = realloc(ret, sizeof(char) * (totalAlloced * 3 / 2 + 1));
        copyStr(aux->city->name, ret, length);
      }
      if (aux->next) {
        help = isConnected(aux->city, aux->next->city);
        lgo =  lgp(help->length);
        if (length + lgo > totalAlloced) {
          ret = realloc(ret, sizeof(char) * (totalAlloced * 3 / 2 + 1));
        }
        copy(help->length, length + lgo - 1, ret);
        lgo =  lgp(help->year);
        if (length + lgo > totalAlloced) {
          ret = realloc(ret, sizeof(char) * (totalAlloced * 3 / 2 + 1));
        }
        copy(help->length, length + lgo - 1, ret);
      }
      aux->next = NULL;
    }
    return ret;
  }
}
