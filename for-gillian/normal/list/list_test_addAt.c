#include "list.h"
#include <gillian-c/gillian-c.h>

static List *list1;
static List *list2;

int va, vb, vc, vd, ve, vf, vg, vh;

void setup_tests() {
    list_new(&list1), list_new(&list2);

    va = __builtin_annot_intval("symb_int", va);
    vb = __builtin_annot_intval("symb_int", vb);
    vc = __builtin_annot_intval("symb_int", vc);
    vd = __builtin_annot_intval("symb_int", vd);
    ve = __builtin_annot_intval("symb_int", ve);
    vf = __builtin_annot_intval("symb_int", vf);
    vg = __builtin_annot_intval("symb_int", vg);
    vh = __builtin_annot_intval("symb_int", vh);

    int *a = (int *)malloc(sizeof(int));
    int *b = (int *)malloc(sizeof(int));
    int *c = (int *)malloc(sizeof(int));
    int *d = (int *)malloc(sizeof(int));

    *a = va;
    *b = vb;
    *c = vc;
    *d = vd;

    list_add(list1, a);
    list_add(list1, b);
    list_add(list1, c);
    list_add(list1, d);

    a = (int *)malloc(sizeof(int));
    b = (int *)malloc(sizeof(int));
    c = (int *)malloc(sizeof(int));
    d = (int *)malloc(sizeof(int));

    *a = ve;
    *b = vf;
    *c = vg;
    *d = vh;

    list_add(list2, a);
    list_add(list2, b);
    list_add(list2, c);
    list_add(list2, d);
}

void teardown_test() {
    list_destroy_cb(list1, free);
    list_destroy(list2);
}

int main() {
    setup_tests();

    int *ins = (int *)malloc(sizeof(int));
    *ins = __builtin_annot_intval("symb_int", *ins);

    list_add_at(list1, ins, 3);
    ASSERT(5 == list_size(list1));

    int *new;
    list_get_at(list1, 3, (void *)&new);
    ASSERT(*ins == *new);

    teardown_test();
}
