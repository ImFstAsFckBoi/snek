#include "snek.hh"



void nothing() {}

const char *hello() {return "HELLO!";}

int meaning_of_life() {return 42;}

PyObject *tup() {return snek::ToPyValue(1, 2, 3);}

PyObject *lis() {return snek::ToPyList(1, 2, 3);}

int mult(int a, int b) {return a * b;}

SNEK_ExportMod(testmod, "") {
    snek::ExportFn<nothing>("nothing");
    snek::ExportFn<hello>("hello");
    snek::ExportFn<meaning_of_life>("meaning_of_life");
    snek::ExportFn<tup>("tup");
    snek::ExportFn<lis>("lis");
    snek::ExportFn<mult>("mult");
}