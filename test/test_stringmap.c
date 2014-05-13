#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "util/stringmap.h"

typedef struct simple_data {
  int x, y;
} simple_data;

static simple_data* make_simple_data(int x, int y) {
  simple_data *data = malloc(sizeof(simple_data));
  data->x = x;
  data->y = y;
  return data;
}

typedef struct complex_data {
  int x;
  char *string;
} complex_data;

static complex_data* make_complex_data(int x, const char *string) {
  complex_data *data = malloc(sizeof(complex_data));
  data->x = x;
  data->string = strdup(string);
  return data;
}

static void free_complex_data(complex_data *data) {
  free(data->string);
  free(data);
}

// test stringmap functionality
int main(int argc, char *argv[]) {
  /* Simple Data Test**********************************************************/
  stringmap *simple_map = stringmap_new(free);
  assert(simple_map != NULL);
  // test insertion
  stringmap_add(simple_map, "first", make_simple_data(1, 2));
  stringmap_add(simple_map, "second", make_simple_data(3, 4));
  stringmap_add(simple_map, "third", make_simple_data(5, 6));
  // look for entries that were inserted
  simple_data *d1 = stringmap_find(simple_map, "first");
  simple_data *d2 = stringmap_find(simple_map, "second");
  assert(d1 != NULL);
  assert(d2 != NULL);
  assert(d1->x == 1 && d1->y == 2);
  assert(d2->x == 3 && d2->y == 4);
  // look for an entry that was not inserted
  simple_data *d3 = stringmap_find(simple_map, "noentry");
  assert(d3 == NULL);
  // remove a value then try to find it. should get NULL
  stringmap_remove(simple_map, "second");
  assert(stringmap_find(simple_map, "second") == NULL);
  // free the whole map
  stringmap_free(simple_map);

  /* Complex Data Test*********************************************************/
  stringmap *complex_map = stringmap_new((list_lambda)free_complex_data);
  assert(complex_map != NULL);
  // insert some values
  stringmap_add(complex_map, "first",  make_complex_data(1, "data1"));
  stringmap_add(complex_map, "second", make_complex_data(3, "data2"));
  stringmap_add(complex_map, "third",  make_complex_data(5, "data3"));
  // look for entries that were inserted
  complex_data *d4 = stringmap_find(complex_map, "first");
  complex_data *d5 = stringmap_find(complex_map, "second");
  assert(d4 != NULL);
  assert(d5 != NULL);
  assert(d4->x == 1 && (strcmp("data1", d4->string) == 0));
  assert(d5->x == 3 && (strcmp("data2", d5->string) == 0));
  // look for an entry that was not inserted
  simple_data *d6 = stringmap_find(complex_map, "noentry");
  assert(d6 == NULL);
  // remove a value then try to find it. should get NULL
  stringmap_remove(complex_map, "second");
  assert(stringmap_find(complex_map, "second") == NULL);
  // free the whole map
  //stringmap_free(complex_map);
}
