#include "list.h"
#include <klee/klee.h> 

static List *list1;
static List *list2;

void setup_tests() { list_new(&list1), list_new(&list2); }

void teardown_test() {
    list_destroy(list1);
    list_destroy(list2);
}

int main() {
    setup_tests();

    char s1; klee_make_symbolic(&s1, sizeof(char), "s1");

    char str_s1[] = {s1, '\0'};

    char s2; klee_make_symbolic(&s2, sizeof(char), "s2");

    char str_s2[] = {s2, '\0'};

    char s3; klee_make_symbolic(&s3, sizeof(char), "s3");

    char str_s3[] = {s3, '\0'};

    char s4; klee_make_symbolic(&s4, sizeof(char), "s4");

    char str_s4[] = {s4, '\0'};

    klee_assert(CC_OK == list_add(list1, str_s1));
    klee_assert(CC_OK == list_add(list1, str_s2));
    klee_assert(CC_OK == list_add(list1, str_s3));
    klee_assert(CC_OK == list_add(list1, str_s4));

    void *e;
    list_get_first(list1, &e);
    klee_assert(e != NULL);

    list_get_last(list1, &e);
    klee_assert(e != NULL);

    teardown_test();
}
