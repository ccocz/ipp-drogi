#include "map.h"
#include "heap.h"
#include "stack.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// checkYear

void freeMap(Map *map) {
  City *aux, *help;
  Edges *edges, *helpEdges;
  for (int i = 0; i < N; i++) {
    aux = map->cities[i];
    while (aux) {
      help = aux;
      edges = aux->edges;
      while (edges) {
        helpEdges = edges;
        edges = edges->next;
        free(helpEdges);
      }
      free(aux->name);
      aux = aux->next;
      free(help);
    }
  }
  Route *route;
  for (int i = 0; i < R; i++) {
    route = map->routes[i];
    if (route) {
      edges = route->edges;
      while (edges) {
        helpEdges = edges;
        edges = edges->next;
        free(helpEdges);
      }
      free(route);
    }
  }
  Road *roads = map->roads, *helpRoads;
  while (roads) {
    helpRoads = roads;
    roads = roads->next;
    free(helpRoads);
  }
  free(map);
}

void deleteMap(Map *map) {
  freeMap(map);
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
    aux->roads = NULL;
    return aux;
  }
}

bool badName(const char *city) {
  size_t n = strlen(city);
  for (size_t i = 0; i < n; i++) {
    if (city[i] <= 31 || city[i] == ';') {
      return true;
    }
  }
  return false;
}

int hashIt(const char *s) {
  size_t n = strlen(s);
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

char *makeCopy(const char *city) {
  size_t n = strlen(city);
  char *copy = malloc(n + 1);
  for (size_t i = 0; i < n; i++) {
    copy[i] = city[i];
  }
  copy[n] = '\0';
  return copy;
}

City *addCity(Map *map, const char *city) {
  int hash = hashIt(city);
  City *where = map->cities[hash];
  City *aux = malloc(sizeof(City));
  aux->name = makeCopy(city);
  aux->edges = NULL;
  aux->heapNode = NULL;
  aux->next = where;
  map->cities[hash] = aux;
  return aux;
}

Road *isConnected(City *city1, City *city2) {
  Edges *edges = city1->edges;
  while (edges) {
    if (edges->road->from == city2 || edges->road->to == city2) {
      return edges->road;
    }
    edges = edges->next;
  }
  return NULL;
}

void addEdge(City *city, Road *road) {
  Edges *help = malloc(sizeof(Edges));
  help->road = road;
  help->next = city->edges;
  if (city->edges) {
    city->edges->prev = help;
  }
  city->edges = help;
}

void connectCities(Map *map, City *city1, City *city2,
                   unsigned length, int builtYear) {
  Road *aux = malloc(sizeof(Road));
  aux->from = city1;
  aux->to = city2;
  aux->length = length;
  aux->year = builtYear;
  aux->next = map->roads;
  if (map->roads) {
    map->roads->prev = aux;
  }
  map->roads = aux;
  addEdge(city1, aux);
  addEdge(city2, aux);
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
    first = addCity(map, city1);
  }
  if (!(second = cityExists(map, city2))) {
    second = addCity(map, city2);
  }
  if (isConnected(first, second)) {
    return false;
  }
  connectCities(map, first, second, length, builtYear);
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
  if (!go) {
    return false;
  }
  if (go->year > repairYear) {
    return false;
  }
  go->year = repairYear;
  return true;
}

inline int maxi(int x, int y) {
  return x > y ? x : y;
}

City *toCity(Road *road, City *from) {
  if (road->to == from) {
    return road->from;
  } else {
    return road->to;
  }
}

void dijkstra(Heap *Q) {
  HeapNode *best;
  Edges *adj;
  Road *adjRoad;
  City *adjCity;
  while (Q->root) {
    heapifyMin(Q->root);
    best = minHeap(Q);
    adj = best->city->edges;
    while (adj) {
      adjRoad = adj->road;
      adjCity = toCity(adjRoad, best->city);
      if (!adjCity->heapNode->visited) {
        if (best->distance + adjRoad->length < adjCity->heapNode->distance) {
          decreaseValue(adjCity->heapNode, best->distance + adjRoad->length, maxi(best->year, adjRoad->year));
          adjCity->heapNode->from = adjRoad;
          adjCity->heapNode->lastYear = adjRoad->year;
        } else if (best->distance + adjRoad->length == adjCity->heapNode->distance) {
          if (maxi(best->year, adjRoad->year) > adjCity->heapNode->year) {
            decreaseValue(adjCity->heapNode, best->distance + adjRoad->length, maxi(best->year, adjRoad->year));
            adjCity->heapNode->from = adjRoad;
            adjCity->heapNode->lastYear = adjRoad->year;
          }
        }
      }
      adj = adj->next;
    }
    freeMe(Q, best);
  }
}

bool checkUnique(City *destination) {
  Road *adjRoad;
  City *adjCity;
  Edges *toDest = destination->edges;
  bool ok = true;
  while (ok && toDest) {
    adjCity = toCity(toDest->road, destination);
    adjRoad = toDest->road;
    if (adjRoad != destination->heapNode->from &&
        adjCity->heapNode->distance + adjRoad->length == destination->heapNode->distance &&
        maxi(adjCity->heapNode->year, adjRoad->year) == destination->heapNode->year) {
      ok = false;
    }
    toDest = toDest->next;
  }
  return ok;
}

Route *makeRoute(City *source, City *destination) {
  Edges *edgesHelp;
  Route *ret = malloc(sizeof(Route));
  ret->start = source;
  ret->end = destination;
  ret->edges = NULL;
  City *traverse = destination;
  while (traverse) {
    puts(traverse->name);
    edgesHelp = malloc(sizeof(Edges));
    edgesHelp->road = traverse->heapNode->from;
    edgesHelp->next = ret->edges;
    if (ret->edges) {
      ret->edges->prev = edgesHelp;
    }
    ret->edges = edgesHelp;
    if (traverse->heapNode->from) {
      traverse = toCity(traverse->heapNode->from, traverse);
    } else {
      traverse = NULL;
    }
  }
  return ret;
}

void addHeap(Heap *Q, City *source) {
  Stack *stack = NULL;
  addStack(&stack, source);
  Edges *edges;
  City *adjCity;
  while (!isEmptyStack(stack)) {
    source = popStack(&stack);
    edges = source->edges;
    while (edges) {
      adjCity = toCity(edges->road, source);
      if (!adjCity->heapNode) {
        insertHeap(Q, adjCity);
        addStack(&stack, adjCity);
      }
      edges = edges->next;
    }
  }
}

Route *startDijkstra(Map *map, City *source, City *destination) {
  Heap *Q = newHeap(source);
  addHeap(Q, source);
  dijkstra(Q);
  bool ok = true;
  Route *ret = NULL;
  if (!destination->heapNode->from || !checkUnique(destination)) {
    ok = false;
  }
  if (ok) {
    ret = makeRoute(source, destination);
  }
  City *aux;
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
  Route *ans = startDijkstra(map, first, second);
  if (!ans) {
    return false;
  } else {
    map->routes[routeId] = ans;
    return true;
  }
}

