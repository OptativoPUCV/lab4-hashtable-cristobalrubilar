#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "hashmap.h"


typedef struct HashMap HashMap;
int enlarge_called=0;

struct HashMap {
    Pair ** buckets;
    long size; //cantidad de datos/pairs en la tabla
    long capacity; //capacidad de la tabla
    long current; //indice del ultimo dato accedido
};

Pair * createPair( char * key,  void * value) {
    Pair * new = (Pair *)malloc(sizeof(Pair));
    new->key = key;
    new->value = value;
    return new;
}

long hash( char * key, long capacity) {
    unsigned long hash = 0;
     char * ptr;
    for (ptr = key; *ptr != '\0'; ptr++) {
        hash += hash*32 + tolower(*ptr);
    }
    return hash%capacity;
}

int is_equal(void* key1, void* key2){
    if(key1==NULL || key2==NULL) return 0;
    if(strcmp((char*)key1,(char*)key2) == 0) return 1;
    return 0;
}


void insertMap(HashMap * map, char * key, void * value) 
{
  long posicion = hash(key,map->capacity);
  if (map->buckets[posicion] == NULL)
  {
    map->buckets[posicion] = createPair(key, value);
    (map->size)++;
    (map->current) = posicion;
    return;
  }
  else
  {
    for (long i = posicion; i < map->capacity || i < posicion; i++)
      {
        if (map->buckets[i] == NULL )
        {
          map->buckets[i] = createPair(key, value);
          (map->size)++;
          (map->current) = i;
          return;
        }
      }
  }
}

void enlarge(HashMap * map) 
{
  enlarge_called = 1; //no borrar (testing purposes
  Pair ** antiguoMap = map->buckets;
  long antiguaCapacidad = map->capacity;
  long nuevaCapacidad = antiguaCapacidad * 2;
  map->buckets = (Pair **)calloc(nuevaCapacidad, sizeof(Pair *));
  map->capacity = nuevaCapacidad;
  map->size = 0;
  for (long i = 0 ; i < map->capacity ; i++)
    {
      if (antiguoMap[i] != NULL)
      {
        insertMap(map, antiguoMap[i]->key, antiguoMap[i]->value);
      }
    }
}


HashMap * createMap(long capacity)
{
  HashMap * mapa = (HashMap*)malloc(sizeof(HashMap));
  mapa->buckets = (Pair **)calloc(capacity,sizeof(Pair *));
  mapa->size = 0;
  mapa->capacity = capacity;
  mapa->current = -1;
  return mapa;
}

void eraseMap(HashMap * map,  char * key) 
{    
  

}

Pair * searchMap(HashMap * map,  char * key) 
{   

  long posicion = hash(key,map->capacity);
  if(map->buckets[posicion] != NULL)
  {
    if(is_equal(map->buckets[posicion]->key,key))
    {
      return map->buckets[posicion]; 
      map->current = posicion;
    }
  }

  for (long i = posicion; i < map->capacity || i < posicion; i++)
    {
      if (map->buckets[i] != NULL )
      {
        if(is_equal(map->buckets[i]->key,key))
        {
          (map->current) = i;
          return map->buckets[i];
        }
      }
    }
  return NULL;
}

Pair * firstMap(HashMap * map) 
{
    for (long i = 0; i < map->capacity; i++)
      {
        Pair* pair = map->buckets[i];
        if (pair != NULL && pair->key != NULL)
        {
          map->current = i;
          return pair;
        }
      }
  return NULL;
}

Pair * nextMap(HashMap * map) {

  for (long i = map->current + 1; i < map->capacity; i++)
        {
          Pair* pair = map->buckets[i];
          if (pair != NULL && pair->key != NULL)
          {
            map->current = i;
            return pair;
          }
        }
    return NULL;
  }
