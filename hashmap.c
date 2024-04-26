#include "hashmap.h"
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct HashMap HashMap;
int enlarge_called = 0;

struct HashMap {
  Pair **buckets;
  long size;     // cantidad de datos/pairs en la tabla
  long capacity; // capacidad de la tabla
  long current;  // indice del ultimo dato accedido
};

Pair *createPair(char *key, void *value) {
  Pair *new = (Pair *)malloc(sizeof(Pair));
  new->key = key;
  new->value = value;
  return new;
}

long hash(char *key, long capacity) {
  unsigned long hash = 0;
  char *ptr;
  for (ptr = key; *ptr != '\0'; ptr++) {
    hash += hash * 32 + tolower(*ptr);
  }
  return hash % capacity;
}

int is_equal(void *key1, void *key2) {
  if (key1 == NULL || key2 == NULL)
    return 0;
  if (strcmp((char *)key1, (char *)key2) == 0)
    return 1;
  return 0;
}

long obtenerPosicionValida(HashMap *map, char *key, void *value) {
  long posicion = hash(key, map->capacity);
  while (map->buckets[posicion] != NULL &&
         strcmp(map->buckets[posicion]->key, key) != 0) {
    posicion = (posicion + 1) % map->capacity;
  }
  return posicion;
}

void insertMap(HashMap *map, char *key, void *value) {
  long posicion = obtenerPosicionValida(map, key, value);
  map->buckets[posicion] = createPair(key, value);
  (map->size)++;
  map->current = posicion;
}

void enlarge(HashMap *map) {
  enlarge_called = 1;
  Pair **antiguoMap = map->buckets;
  long nuevaCapacidad = map->capacity *= 2;
  HashMap *nuevoMap = createMap(nuevaCapacidad);

  for (int i = 0; i < map->capacity; i++) {
    if (antiguoMap[i] != NULL) {
      insertMap(nuevoMap, antiguoMap[i]->key, antiguoMap[i]->value);
      free(antiguoMap[i]);
      (nuevoMap->size)++;
      
    }
  }
  free(antiguoMap);
  map->buckets = nuevoMap->buckets;
  map->capacity = nuevaCapacidad;
  map->size = nuevoMap->size;
}

HashMap *createMap(long capacity) {
  HashMap *mapa = (HashMap *)malloc(sizeof(HashMap));
  mapa->buckets = (Pair **)calloc(capacity, sizeof(Pair *));
  mapa->size = 0;
  mapa->capacity = capacity;
  mapa->current = -1;
  return mapa;
}

void eraseMap(HashMap *map, char *key) {
  long posicion = hash(key, map->capacity);
  while (map->buckets[posicion] != NULL &&
         strcmp(map->buckets[posicion]->key, key) != 0) {
    posicion = (posicion + 1) % map->capacity;
  }
  if (map->buckets[posicion] != NULL &&
      strcmp(map->buckets[posicion]->key, key) == 0) {
    map->current = posicion;
    (map->size)--;
    map->buckets[posicion]->key = NULL;
  }
}

Pair *searchMap(HashMap *map, char *key) {
  long posicion = hash(key, map->capacity);
  while (map->buckets[posicion] != NULL &&
         strcmp(map->buckets[posicion]->key, key) != 0) {
    posicion = (posicion + 1) % map->capacity;
  }
  if (map->buckets[posicion] == NULL) {
    map->current = posicion;
    return NULL;
  } else {
    map->current = posicion;
    return map->buckets[posicion];
  }
}

Pair *firstMap(HashMap *map) {
  for (long i = 0; i < map->capacity; i++) {
    Pair *pair = map->buckets[i];
    if (pair != NULL && pair->key != NULL) {
      map->current = i;
      return pair;
    }
  }
  return NULL;
}

Pair *nextMap(HashMap *map) {

  for (long i = map->current + 1; i < map->capacity; i++) {
    Pair *pair = map->buckets[i];
    if (pair != NULL && pair->key != NULL) {
      map->current = i;
      return pair;
    }
  }
  return NULL;
}
