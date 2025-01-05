#include "snek/snek.hh"
#include <iostream>


void nothing() {}

const char *hello() {return "HELLO!";}

int meaning_of_life() {return 42;}

PyObject *tup() {return snek::ToPyValue(1, 2, 3);}

PyObject *lis() {return snek::ToPyList(1, 2, 3);}

int mult(int a, int b) {return a * b;}

double half(double a) {return a / 2;}

SNEK_ExportMod(testmod, "") {
    snek::ExportFn<nothing>("nothing");
    snek::ExportFn<hello>("hello");
    snek::ExportFn<meaning_of_life>("meaning_of_life");
    snek::ExportFn<tup>("tup");
    snek::ExportFn<lis>("lis");
    snek::ExportFn<mult>("mult");
    snek::ExportFn<half>("half");

    std::cerr << snek::GenFnInfo<nothing>("nothing") << "\n"
              << snek::GenFnInfo<lis>("lis") << "\n"
              << snek::GenFnInfo<mult>("mult") << "\n"
              << snek::GenFnInfo<half>("half");
}