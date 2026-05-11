/* greenscore.c - green score calculation implementation */

#include "greenscore.h"
#include <string.h>

int calculateGreenScore(Product p) {
    int score = 0;

    // 1. Category base scores
    if (strstr(p.category, "Biscuits")) {
        score = 55;
    } else if (strstr(p.category, "Soft Drinks")) {
        score = 30;
    } else if (strstr(p.category, "Dairy")) {
        score = 50;
    } else if (strstr(p.category, "Health Drink")) {
        score = 60;
    } else if (strstr(p.category, "Snacks")) {
        score = 45;
    } else if (strstr(p.category, "Instant Food")) {
        score = 35;
    } else {
        score = 50;  // Default base score
    }

    // 2. Ingredient penalties
    if (strstr(p.ingredients, "Palm Oil")) score -= 12;
    if (strstr(p.ingredients, "Artificial Color")) score -= 6;
    if (strstr(p.ingredients, "Preservatives")) score -= 5;
    if (strstr(p.ingredients, "Refined Sugar")) score -= 6;

    // 3. Ingredient bonuses
    if (strstr(p.ingredients, "Millet")) score += 10;
    if (strstr(p.ingredients, "Oats")) score += 8;
    if (strstr(p.ingredients, "Whole Wheat")) score += 8;
    if (strstr(p.ingredients, "Organic")) score += 10;
    if (strstr(p.ingredients, "Ragi")) score += 8;

    // 4. Packaging modifier
    if (strstr(p.packaging, "Glass")) {
        score += 20;
    } else if (strstr(p.packaging, "Paper")) {
        score += 15;
    } else if (strstr(p.packaging, "Metal")) {
        score += 10;
    } else if (strstr(p.packaging, "Plastic")) {
        score += 0;  // Baseline
    }

    // 5. Clamp result to 0-100
    if (score < 0) score = 0;
    if (score > 100) score = 100;

    return score;
}
