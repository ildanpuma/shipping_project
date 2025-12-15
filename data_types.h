#ifndef DATA_TYPES_H
#define DATA_TYPES_H

#include <stdio.h>

#define MAX_CARGO_WEIGHT 100 //수화물 최대 무게
#define MAX_CARGO_PER_BAY 5 //한 BAY에 탑재가능한 수화물갯수

#define MAX_BAYS 6 //최대 BAY 갯수
#define BAY_AREA_COLS 2 //BAY구역의 열 갯수
#define BAY_AREA_ROWS 3 //BAY구역의 행 갯수

#define MAX_CARGO 30 //최대 수화물 갯수

#define GET_BayArea_SIDE(col) ((col) % BAY_AREA_COLS) //BAY의 오른쪽, 왼쪽 반환
#define GET_BayArea_LEVEL(row) ((row) / BAY_AREA_COLS) //BAY의 높이 반환

//수화물, BAY, BAY구역 구조체 정의
typedef struct Cargo{
    int id;
    int weight;
    char owner_name[20];
} Cargo;

typedef struct Bay{
    Cargo* tiers[MAX_CARGO_PER_BAY];
    int top;
} Bay;

typedef struct BayArea{
    Bay* bays[MAX_BAYS];
    int top;
} BayArea;

typedef enum {
    RIGHT = 0,
    LEFT = 1
} BaySide;

//저장공간 초기화 및 생성 함수
Cargo* createCargo(int id, int weight, const char* owner_name);
Bay* createBay();
BayArea* createBayArea();

//BAY 관련 함수 선언
int isBayFull(Bay* bay);
int isBayEmpty(Bay* bay);
void initBay(Bay* bay);
void pushCargoToBay(Bay* bay, Cargo* cargo);
Cargo* popCargoFromBay(Bay* bay);

//BAY구역 관련 함수 선언
int isBayAreaEmpty(BayArea* bayArea);
int isBayAreaFull(BayArea* bayArea);
void initBayArea(BayArea* bayArea);
void pushBayToBayArea(BayArea* bayArea, Bay* bay);
Bay* popBayFromBayArea(BayArea* bayArea);

//적재관련 함수 선언
int SumBayWeight(Bay* bay);
void loadCargoToBayArea(BayArea* bayArea, Cargo* cargos[], int count);

//인덱스 생성 함수 선언
Cargo** createCargoUnloadIndex(BayArea* bayArea);
Cargo** createCargoIndexByName(BayArea* bayArea);

//메모리 해제 함수 선언
void freeCargo(Cargo* cargo);
void freeBay(Bay* bay);
void freeBayArea(BayArea* bayArea);
void freeCargoUnloadIndex(Cargo** unloadIndex);

#endif

