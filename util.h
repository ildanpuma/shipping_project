#ifndef SHIPPING_PROJECT_UTIL_H
#define SHIPPING_PROJECT_UTIL_H

#include <stdio.h>
#include <stdlib.h>
#include "data_types.h"

void menu_loadCargo(BayArea* bayArea);
void menu_searchCargo(BayArea* bayArea);
void menu_showUnloadOrder(BayArea* bayArea);
void menu_showBayStatus(BayArea* bayArea);

// CSV 파일 입출력 함수
int loadCargosFromCSV(const char* filename, Cargo*** cargos);
void saveCargoToCSV(BayArea* bayArea, const char* filename);

#endif //SHIPPING_PROJECT_UTIL_H