#include "map.h"
#include "heap.h"
#include "stack.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <pthread.h>

// checkYear

void freeRoutes(Routes *routes) {
  Routes *help;
  while (routes) {
    help = routes;
    routes = routes->next;
    free(help);
  }
}

void freeEdges(Edges *this) {
  Edges *edges = this, *helpEdges;
  while (edges) {
    helpEdges = edges;
    edges = edges->next;
    free(helpEdges);
  }
}

void freeRoute(Route *route) {
  freeEdges(route->edges);
  free(route);
}

void freeMap(Map *map) {
  City *aux, *help;
  for (int i = 0; i < N; i++) {
    aux = map->cities[i];
    while (aux) {
      help = aux;
      freeEdges(aux->edges);
      free(aux->name);
      aux = aux->next;
      free(help);
    }
  }
  Route *route;
  for (int i = 0; i < R; i++) {
    route = map->routes[i];
    if (route) {
      freeRoute(route);
    }
  }
  Road *roads = map->roads, *helpRoads;
  while (roads) {
    helpRoads = roads;
    freeRoutes(roads->routes);
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
  aux->allowed = true;
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
  aux->routes = NULL;
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
      if (adjCity->allowed && !adjCity->heapNode->visited ) {
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
    if (adjRoad != destination->heapNode->from && adjCity->allowed &&
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
  ret->totalCost = destination->heapNode->distance;
  ret->year = destination->heapNode->year;
  City *traverse = destination;
  while (traverse) {
    if (traverse->heapNode->from) {
      edgesHelp = malloc(sizeof(Edges));
      edgesHelp->road = traverse->heapNode->from;
      edgesHelp->next = ret->edges;
      if (ret->edges) {
        ret->edges->prev = edgesHelp;
      }
      ret->edges = edgesHelp;
      traverse = toCity(traverse->heapNode->from, traverse);
    } else {
      traverse = NULL;
    }
  }
  ret->edges->prev = NULL;
  return ret;
}

void addHeap(Heap *Q, City *source) {
  Edges *edges;
  City *adjCity;
  edges = source->edges;
  while (edges) {
    adjCity = toCity(edges->road, source);
    if (!adjCity->heapNode && adjCity->allowed) {
      insertHeap(Q, adjCity);
      addHeap(Q, adjCity);
    }
    edges = edges->next;
  }
  /*
  Stack *stack = NULL;
  addStack(&stack, source);
  Edges *edges;
  City *adjCity;
  while (!isEmptyStack(stack)) {
    source = popStack(&stack);
    edges = source->edges;
    while (edges) {
      adjCity = toCity(edges->road, source);
      if (!adjCity->heapNode && adjCity->allowed) {
        insertHeap(Q, adjCity);
        addStack(&stack, adjCity);
      }
      edges = edges->next;
    }
  }*/
}

void freeNodes(Heap *Q, City *source) {
  Edges *edges;
  City *adjCity;
  edges = source->edges;
  free(source->heapNode);
  source->heapNode = NULL;
  while (edges) {
    adjCity = toCity(edges->road, source);
    if (adjCity->heapNode) {
      freeNodes(Q, adjCity);
    }
    edges = edges->next;
  }
}

Route *startDijkstra(Map *map, City *source, City *destination) {
  Heap *Q = newHeap(source);
  addHeap(Q, source);
  bool ok = true;
  Route *ret = NULL;
  City *aux;
  if (destination->heapNode) {
    dijkstra(Q);
  }
  if (!destination->heapNode || !destination->heapNode->from || !checkUnique(destination)) {
    ok = false;
  }
  if (ok) {
    ret = makeRoute(source, destination);
  }
  freeNodes(Q, source);
  freeHeap(Q);
  return ret;
}

void giveId(Route *route, unsigned routeId) {
  City *start = route->start;
  Edges *edges = route->edges;
  Routes *aux;
  while (start) {
    if (edges) {
      start = toCity(edges->road, start);
      aux = malloc(sizeof(Routes));
      aux->routeId = routeId;
      aux->next = edges->road->routes;
      edges->road->routes = aux;
      edges = edges->next;
    } else {
      start = NULL;
    }
  }

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
    giveId(ans, routeId);
    return true;
  }
}

void switchAllowed(Route *route, bool allowed) {
  City *start = route->start;
  Edges *edges = route->edges;
  while (start) {
    start->allowed = allowed;
    if (edges) {
      start = toCity(edges->road, start);
      edges = edges->next;
    } else {
      start = NULL;
    }
  }
}

Route *mergeRoutes(Route *a, Route *b, bool start) {
  a->totalCost += b->totalCost;
  a->year = maxi(a->year, b->year);
  if (start) {
    Edges *use = b->edges;
    Edges *help;
    while (use) {
      help = use->next;
      a->edges->prev = use;
      use->next = a->edges;
      a->edges = use;
      use = help;
    }
    a->edges->prev = NULL;
    a->start = b->end;
    free(b);
    return a;
  } else {
    Edges *find = a->edges;
    while (find->next) {
      find = find->next;
    }
    Edges *use = b->edges;
    while (use) {
      find->next = use;
      use->prev = find;
      find = use;
      use = use->next;
    }
    find->next = NULL;
    a->end = b->end;
    free(b);
  }
}

bool extendRoute(Map *map, unsigned routeId, const char *city) {
  if (badName(city)) {
    return false;
  }
  if (!map->routes[routeId]) {
    return false;
  }
  City *first = cityExists(map, city);
  if (!first) {
    return false;
  }
  Route *my = map->routes[routeId];
  switchAllowed(map->routes[routeId], false);
  if (!first->allowed) {
    switchAllowed(my, true);
    return false;
  }
  Route *fromHead = startDijkstra(map, my->start, first);
  Route *fromTail = startDijkstra(map, my->end, first);
  switchAllowed(my, true);
  if (fromHead->totalCost < fromTail->totalCost) {
    map->routes[routeId] = mergeRoutes(my, fromHead, true);
    freeRoute(fromTail);
  } else if (fromHead->totalCost > fromTail->totalCost) {
    map->routes[routeId] = mergeRoutes(my, fromTail, false);
    freeRoute(fromHead);
  } else {
    if (fromHead->totalCost == fromTail->totalCost && fromHead->year > fromTail->year) {
      map->routes[routeId] = mergeRoutes(my, fromHead, true);
      freeRoute(fromTail);
    } else if (fromHead->totalCost == fromTail->totalCost && fromHead->year < fromTail->year) {
      map->routes[routeId] = mergeRoutes(my, fromTail, false);
      freeRoute(fromHead);
    } else {
      freeRoute(fromHead);
      freeRoute(fromTail);
      return false;
    }
  }
  return true;
}

void deleteEdge(City *city, Road *road) {
  Edges *edges = city->edges;
  bool found = false;
  while (edges && !found) {
    if (edges->road == road) {
      if (edges->prev) {
        edges->prev->next = edges->next;
      }
      else {
        city->edges = edges->next;
      }
      free(edges);
      found = true;
    }
    edges = edges->next;
  }
}

void changeRoute(Route *route, Route *with, Road *road) {
  bool found = false;
  Edges *edges = route->edges;
  while (edges && !found) {
    if (edges->road == road) {
      if (edges->prev) {
        edges->prev->next = with->edges;
      } else {
        route->edges = with->edges;
      }
      free(edges);
      found = true;
    }
    edges = edges->next;
  }
}

void deleteRoad(Map *map, Road *road) {
  if (road->prev) {
    road->prev = road->next;
  } else {
    map->roads = road->next;
  }
  free(road);
}

bool removeRoad(Map *map, const char *city1, const char *city2) {
  if (badName(city1) || badName(city2)) {
    return false;
  }
  City *first = cityExists(map, city1);
  City *second = cityExists(map, city2);
  if (!first || !second) {
    return false;
  }
  Road *connects = isConnected(first, second);
  if (!connects) {
    return false;
  }
  puts(connects->from->name);
  puts(connects->to->name);
  Routes *use = connects->routes;
  while (use) {
    switchAllowed(map->routes[use->routeId], false);
    use = use->next;
  }
  Route *new = startDijkstra(map, first, second);
  if (new == NULL) {
    use = connects->routes;
    while (use) {
      switchAllowed(map->routes[use->routeId], true);
      use = use->next;
    }
    free(new);
    return false;
  }
  use = connects->routes;
  while (use) {
    switchAllowed(map->routes[use->routeId], true);
    changeRoute(map->routes[use->routeId], new, connects);
    use = use->next;
  }
  deleteEdge(first, connects);
  deleteEdge(second, connects);
  deleteRoad(map, connects);
  free(new);
  return true;
}