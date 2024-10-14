// IMPLEMENTATION OF A SINGLY LINKED LIST
// to be used in the graph's adjacency lists

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdbool.h>

typedef struct element {
    int info;
    struct element *next;
} element, *list;

/* Initializes an empty singly linked list */
void ini(list* h) {
  *h = NULL;
}

/* Creates a new node for the singly linked list
 * and returns its address.
 */
list new_node(int a) {
  list u;
  u = malloc(sizeof(element));
  if (u == NULL) exit(1);
  u->next = NULL;
  u->info = a;
  return u;
}

/* Inserts an element at the beginning of the singly linked list */
void add(list* h, list u) {
  u->next = *h;
  *h = u;
}

/* Removes the element at the beginning of the singly linked list,
 * if the list is not empty.
 */
void delete(list* h) {
  list u;
  if (*h != NULL) {
    u = *h;
    *h = (*h)->next;
    free(u);
  }
}

/* Removes all elements from the singly linked list */
void clear(list* h) {
  while (*h != NULL)
    delete(h);
}

/* Searches the singly linked list for an element
 * whose `info` field equals `a`.
 * Returns `NULL` if such an element is not found.
 */
list search(list h, int a) {
  while (h != NULL && h->info != a)
    h = h->next;
  return h;
}

list search_and_delete(list *h, int a) {
  list previous = NULL;
  list current = *h;

  while (current != NULL && current->info != a) {
    previous = current;
    current = current->next;
  }

  if (current != NULL) {
    if (previous != NULL) {
      previous->next = current->next;
    } else {
      *h = current->next;
    }

    free(current);
  }

  return *h;
}

/* Calculates the number of elements in the singly linked list */
int size(list h) {
  int result = 0;
  while (h != NULL) {
    ++result;
    h = h->next;
  }
  return result;
}

/* Merges two singly linked lists into one,
 * appending `l2` to the end of `l1`. Both lists may be NULL.
 * After merging, the address of list `l2` is set to NULL.
 */
void merge(list* l1, list* l2) {
  list l = *l1;
  if (*l2 != NULL) {
    if (*l1 == NULL) {
      *l1 = *l2;
    } else {
      while (l->next != NULL)
        l = l->next;
      l->next = *l2;
    }
    *l2 = NULL;
  }
}

void print_list(list u)
// prints the singly linked list
{
    while (u != NULL) {
        printf("%d ", u->info);
        u = (*u).next; // or u = u->next;
    }
    printf("\n");
}

#define N 6 // number of vertices in the graph

char path[] = "C:/Users/ziela/Documents/C_cwiczenia/topological_sort_HD/graph.txt"; // name of the file with the graph
list graph[N];

void read_graph() {
    FILE *f;
    int i, s, t;
    f = fopen(path, "r");
    for (i = 0; i < N; i++)
        graph[i] = NULL;
    while (fscanf(f, "%d %d", &s, &t) > 0) {
        list u;
        u = malloc(sizeof(element));
        u->info = t;
        u->next = graph[s];
        graph[s] = u;
    }
    fclose(f);
}

void print_graph() {
    int i;
    list u;
    printf("Graph:");
    for (i = 0; i < N; i++) {
        printf("\n%2d: ", i);
        u = graph[i];
        while (u != NULL) {
            printf("%3d", u->info);
            u = u->next;
        }
    }
    printf("\n");
}

list nodes_without_incoming_edges(list *S, int k) {
    int i;
    bool have_inc_edge = false;
    for (i = 0; i < N; i++) {
        if (search(graph[i], k) != NULL) {
            have_inc_edge = true;
        }
    }
    if (!have_inc_edge) {
        add(S, new_node(k));
    }

    return *S;
}

list top_sort(list *S, list *L) {
    while (*S != NULL) {
        int v = (*S)->info;
        *S = (*S)->next;

        add(L, new_node(v));

        list u = graph[v];
        while (u != NULL) {
            int m = u->info;
            u = u->next;

            search_and_delete(&graph[v], m);
            nodes_without_incoming_edges(S, m);
        }
    }
    for (int i = 0; i < N; i++) {
        if (graph[i] != NULL) {
            printf("The graph has a cycle.\n");
            return *L;
        }
    }

    return *L;
}

int main(void) {
    list S;
    list L;
    int i = 0;
    read_graph();
    printf("Graph read:\n");
    print_graph();
    ini(&S);
    ini(&L);
    for (i = 0; i < N; i++) {
        nodes_without_incoming_edges(&S, i);
    }
    printf("List of vertices without incoming edges: \n");
    print_list(S);
    L = top_sort(&S, &L);
    printf("Result of the topological sort using Kahn's 1962 algorithm: \n");
    print_list(L);
    printf("Graph after topsort: \n");
    print_graph();
    for (i = 0; i < N; i++) {
        clear(&graph[i]);
    }
    clear(&S);
    clear(&L);

    return 0;
}
