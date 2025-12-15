#include <stdio.h>
#include <string.h>

#include "sort_util.h"

//삽입정렬을 이용하여 무게 기준으로 수화물 정렬 (내림차순)
void sortCargoByWeight(Cargo* cargos[], int count) {
    for (int i = 1; i < count; i++) {
        Cargo* key = cargos[i];
        int j = i - 1;
        while (j >= 0 && cargos[j]->weight < key->weight) {
            cargos[j + 1] = cargos[j];
            j--;
        }
        cargos[j + 1] = key;
    }
}

//버블정렬을 이용하여 이름 기준으로 수화물 정렬 (오름차순)
void sortCargoByName(Cargo* cargos[], int count) {
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (strcmp(cargos[j]->owner_name, cargos[j + 1]->owner_name) > 0) {
                Cargo* temp = cargos[j];
                cargos[j] = cargos[j + 1];
                cargos[j + 1] = temp;
            }
        }
    }
}

