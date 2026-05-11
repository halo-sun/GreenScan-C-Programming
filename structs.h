/* structs.h - shared data structures for GreenScan */

#ifndef STRUCTS_H
#define STRUCTS_H

#include <stdio.h>

typedef struct {
    char barcode[20];
    char name[100];
    char category[50];
    char subcategory[50];
    char variantGroup[100];
    char ingredients[300];
    char packaging[50];
    int greenScore;
} Product;

typedef struct {
    char username[50];
    char password[50];
    char role[20];
} User;

typedef struct {
    char username[50];
    char barcode[20];
    char productName[100];
    char timestamp[50];
} History;

typedef struct {
    char barcode[20];
    char productName[100];
    int count;
} SearchCount;

#endif // STRUCTS_H
