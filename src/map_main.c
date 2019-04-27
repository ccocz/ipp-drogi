#include "map.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#undef NDEBUG

#include <assert.h>
#include "map.h"

int main() {
  char const* str;
  Map* m = newMap();
  assert(m);
  /*
  assert(addRoad(m, "A", "B", 1, 2020));
  assert(addRoad(m, "B", "C", 1, 2030));
  assert(addRoad(m, "A", "F", 2, 2023));
  assert(addRoad(m, "F", "C", 3, 2023));
  assert(newRoute(m, 1, "A", "C"));
  assert(repairRoad(m, "B", "A", 2020));
  assert(extendRoute(m, 1, "F"));
  assert(removeRoad(m, "F", "C"));*/
  assert(addRoad(m, "Alinow", "Bor", 1, 2020));
  assert(addRoad(m, "Bor", "Cielinsk-Nieklansk", 2, 2020));
  assert(addRoad(m, "Bor", "Dab Stary", 3, 2020));
  assert(addRoad(m, "Cielinsk-Nieklansk", "Emiliew", 4, 2020));
  assert(addRoad(m, "Dab Stary", "Emiliew", 5, 2020));
  assert(addRoad(m, "Emiliew", "Bor", 8, 2020));
  assert(addRoad(m, "Emiliew", "Fraznik Nowy", 3, 2020));
  assert(!repairRoad(m, "Emiliew", "Cielinsk-Nieklansk", 2019));
  assert(repairRoad(m, "Emiliew", "Cielinsk-Nieklansk", 2021));
  assert(!repairRoad(m, "Emiliew", "Alinow", 2020));
  assert(addRoad(m, "Fraznik Nowy", "Grzegrzewo", 4, 2020));
  assert(addRoad(m, "Alinow", "Grzegrzewo", 10, 2020));
  assert(addRoad(m, "Homilcin", "Grzegrzewo", 5, 2020));
  assert(addRoad(m, "Fraznik Nowy", "Cielinsk-Nieklansk", 2, 2020));
  assert(!addRoad(m, "Fraznik Nowy", "Cielinsk-Nieklansk", 2, 2020));
  assert(!addRoad(m, "Cielinsk-Nieklansk", "Fraznik Nowy", 2, 2020));
  assert(!repairRoad(m, "Emiliew", "Bor", 2018));
  assert(repairRoad(m, "Emiliew", "Cielinsk-Nieklansk", 2021));
  assert(repairRoad(m, "Emiliew", "Fraznik Nowy", 2023));
  assert(addRoad(m, "Homilcin", "Cielinsk-Nieklansk", 3, 2020));
  assert(newRoute(m, 10, "Alinow", "Emiliew"));
  assert(extendRoute(m, 10, "Homilcin"));
  str = getRouteDescription(m, 10);
  puts(str);
  free(str);
  deleteMap(m);
  //deleteMap(m);
  //assert(addRoad(m, "Alinów", "Bór", 1, 2020));

  /*
  assert(addRoad(m, "Alinów", "Bór", 1, 2020));
  assert(addRoad(m, "Bór", "Cielińsk-Niekłańsk", 2, 2020));
  assert(addRoad(m, "Bór", "Dąb Stary", 3, 2020));
  assert(addRoad(m, "Cielińsk-Niekłańsk", "Emiliew", 4, 2020));
  assert(addRoad(m, "Dąb Stary", "Emiliew", 5, 2020));
  assert(addRoad(m, "Emiliew", "Bór", 8, 2020));
  assert(addRoad(m, "Emiliew", "Fraźnik Nowy", 3, 2020));
  assert(!repairRoad(m, "Emiliew", "Cielińsk-Niekłańsk", 2019));
  assert(repairRoad(m, "Emiliew", "Cielińsk-Niekłańsk", 2021));
  assert(!repairRoad(m, "Emiliew", "Alinów", 2020));
  assert(addRoad(m, "Fraźnik Nowy", "Grzegrzewo", 4, 2020));
  assert(addRoad(m, "Alinów", "Grzegrzewo", 10, 2020));
  assert(addRoad(m, "Homilcin", "Grzegrzewo", 5, 2020));
  assert(addRoad(m, "Fraźnik Nowy", "Cielińsk-Niekłańsk", 2, 2020));
  assert(!addRoad(m, "Fraźnik Nowy", "Cielińsk-Niekłańsk", 2, 2020));
  assert(!addRoad(m, "Cielińsk-Niekłańsk", "Fraźnik Nowy", 2, 2020));
  assert(!repairRoad(m, "Emiliew", "Bór", 2018));
  assert(repairRoad(m, "Emiliew", "Cielińsk-Niekłańsk", 2021));
  assert(repairRoad(m, "Emiliew", "Fraźnik Nowy", 2023));
  assert(addRoad(m, "Homilcin", "Cielińsk-Niekłańsk", 3, 2020));
  assert(newRoute(m, 10, "Alinów", "Emiliew"));
  str = getRouteDescription(m, 10);
  assert(strcmp(str, "10;Alinów;1;2020;Bór;2;2020;Cielińsk-Niekłańsk;4;2021;Emiliew") == 0);
  free((void *)str);

  assert(extendRoute(m, 10, "Homilcin"));

  str = getRouteDescription(m, 10);
  assert(strcmp(str, "10;Alinów;1;2020;Bór;2;2020;Cielińsk-Niekłańsk;4;2021;Emiliew"
                     ";3;2023;Fraźnik Nowy;4;2020;Grzegrzewo;5;2020;Homilcin") == 0);
  free((void *)str);

  deleteMap(m);
  */
  return 0;
}
