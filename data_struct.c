#include <stdio.h>
#include <stdlib.h>
#include "data_types.h"
#include "sort_util.h"

//저장공간 초기화 및 생성 함수
Cargo* createCargo(int id, int weight, const char* owner_name) {
    Cargo* cargo = (Cargo*)malloc(sizeof(Cargo));
    cargo->id = id;
    cargo->weight = weight;
    snprintf(cargo->owner_name, sizeof(cargo->owner_name), "%s", owner_name);
    return cargo;
}

Bay* createBay() {
    Bay* bay = (Bay*)malloc(sizeof(Bay));
    for (int i = 0; i < MAX_CARGO_PER_BAY; i++) {
        bay->tiers[i] = NULL;
    }
    initBay(bay);
    return bay;
}

BayArea* createBayArea() {
    BayArea* bayArea = (BayArea*)malloc(sizeof(BayArea));
    for (int i = 0; i < MAX_BAYS; i++) {
        bayArea->bays[i] = createBay();
    }
    initBayArea(bayArea);
    return bayArea;
}

//BAY 관련 함수 정의
int isBayFull(Bay* bay) {
    if(bay->top == MAX_CARGO_PER_BAY - 1) {
        return 1;
    }
    else {
        return 0;
    }
}

int isBayEmpty(Bay* bay) {
    if(bay->top == -1) {
        return 1;
    }
    else {
        return 0;
    }
}

void initBay(Bay* bay) {
    bay->top = -1;
}

void pushCargoToBay(Bay* bay, Cargo* cargo) {
    if (isBayFull(bay) == 0) {
        bay->top++;
        bay->tiers[bay->top] = cargo;
    } else {
        printf("Bay가 가득 찼습니다.\n");
    }
}

Cargo* popCargoFromBay(Bay* bay) {
    if (isBayEmpty(bay) == 0) {
        return bay->tiers[bay->top--];
    } else {
        printf("Bay가 비어 있습니다.\n");
        return NULL;
    }
}

//BAY구역 관련 함수 정의
int isBayAreaEmpty(BayArea* bayArea) {
    if(bayArea->top == -1) {
        return 1;
    }
    else {
        return 0;
    }
}

int isBayAreaFull(BayArea* bayArea) {
    if(bayArea->top == MAX_BAYS - 1 ) {
        return 1;
    }
    else {
        return 0;
    }
}

void initBayArea(BayArea* bayArea) {
    bayArea->top = -1;
}

void pushBayToBayArea(BayArea* bayArea, Bay* bay) {
    if (isBayAreaFull(bayArea) == 0) {
        bayArea->top++;
        bayArea->bays[bayArea->top] = bay;
    } else {
        printf("Bay구역이 가득 찼습니다.\n");
    }
}

Bay* popBayFromBayArea(BayArea* bayArea) {
    if (isBayAreaEmpty(bayArea) == 0) {
        return bayArea->bays[bayArea->top--];
    } else {
        printf("Bay구역이 비어 있습니다.\n");
        return NULL;
    }
}

int SumBayWeight(Bay* bay) {
    int totalWeight = 0;
    for(int i = 0; i <= bay->top; i++) {
        totalWeight += bay->tiers[i]->weight;
    }
    return totalWeight;
}

//적재관련 함수 정의
//무거운짐부터 적재하여 균하게 적재
void loadCargoToBayArea(BayArea* bayArea, Cargo* cargos[], int cargoCount) {
    // 무게순으로 정렬 (내림차순)
    sortCargoByWeight(cargos, cargoCount);
    
    int BayWeights[MAX_BAYS] = {0};
    int MinWeight = 0;
    int targetBayIndex = 0;
    for(int i = 0; i < cargoCount; i++) {
        for(int j = 0; j < MAX_BAYS; j++) {
            BayWeights[j] = SumBayWeight(bayArea->bays[j]);
        }

        MinWeight = BayWeights[0];
        targetBayIndex = 0;
        for(int j = 1; j < MAX_BAYS; j++) {
            if(BayWeights[j] < MinWeight) {
                MinWeight = BayWeights[j];
                targetBayIndex = j;
            }
        }
        pushCargoToBay(bayArea->bays[targetBayIndex], cargos[i]);
        
        // BayArea의 top 업데이트 (사용 중인 Bay 중 가장 높은 인덱스)
        if (targetBayIndex > bayArea->top) {
            bayArea->top = targetBayIndex;
        }
    }
}

//인덱스 생성 함수 정의
// BayArea의 모든 수화물을 하역 순서대로 배열에 저장
Cargo** createCargoUnloadIndex(BayArea* bayArea) {
    int totalCount = 0;
    for(int i = 0; i <= bayArea->top; i++) {
        totalCount += bayArea->bays[i]->top + 1;
    }
    
    Cargo** unloadIndex = (Cargo**)malloc(sizeof(Cargo*) * totalCount);
    int idx = 0;
    for(int i = 0; i <= bayArea->top; i++) {
        Bay* bay = bayArea->bays[i];
        for(int j = bay->top; j >= 0; j--) {
            unloadIndex[idx++] = bay->tiers[j];
        }
    }
    return unloadIndex;
}

//이름 순서대로 정렬된 수화물 인덱스 생성
Cargo** createCargoIndexByName(BayArea* bayArea) {
    // 먼저 모든 수화물 수집
    int totalCount = 0;
    for(int i = 0; i <= bayArea->top; i++) {
        totalCount += bayArea->bays[i]->top + 1;
    }
    
    Cargo** nameIndex = (Cargo**)malloc(sizeof(Cargo*) * totalCount);
    int idx = 0;
    for(int i = 0; i <= bayArea->top; i++) {
        Bay* bay = bayArea->bays[i];
        for(int j = bay->top; j >= 0; j--) {
            nameIndex[idx++] = bay->tiers[j];
        }
    }
    
    // 이름순 정렬 (오름차순)
    sortCargoByName(nameIndex, totalCount);
    
    return nameIndex;
}

//메모리 해제 함수 정의
void freeCargo(Cargo* cargo) {
    if (cargo != NULL) {
        free(cargo);
    }
}

void freeBay(Bay* bay) {
    if (bay != NULL) {
        for (int i = 0; i < MAX_CARGO_PER_BAY; i++) {
            if (bay->tiers[i] != NULL) {
                freeCargo(bay->tiers[i]);
            }
        }
        free(bay);
    }
}


void freeBayArea(BayArea* bayArea) {
    if (bayArea != NULL) {
        for (int i = 0; i < MAX_BAYS; i++) {
            if (bayArea->bays[i] != NULL) {
                freeBay(bayArea->bays[i]);
            }
        }
        free(bayArea);
    }
}

void freeCargoUnloadIndex(Cargo** unloadIndex) {
    if (unloadIndex != NULL) {
        free(unloadIndex);
    }
}