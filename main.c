#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include "data_types.h"
#include "sort_util.h"
#include "util.h"

int main(void) {
    //setbuf(stdout, NULL); //디버깅을 위한 버퍼 비활성화
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);;

    BayArea* bayArea = createBayArea();
    int selection;

    while (1) {
        system("cls");
        printf("===== 화물 관리 시스템 =====\n");
        printf("\n--- 메뉴 ---\n");
        printf("1. 화물 적재\n");
        printf("2. 화물 검색\n");
        printf("3. 하역 순서 조회\n");
        printf("4. Bay 적재 현황 조회\n");
        printf("0. 종료\n");
        printf("선택: ");

        if (scanf("%d", &selection) != 1) {
            while (getchar() != '\n');
            printf("잘못된 입력입니다.\n");
            getchar();
            continue;
        }
        while (getchar() != '\n');

        switch (selection) {
            case 1:
                menu_loadCargo(bayArea);
                break;
            case 2:
                menu_searchCargo(bayArea);
                break;
            case 3:
                menu_showUnloadOrder(bayArea);
                break;
            case 4:
                menu_showBayStatus(bayArea);
                break;
            case 0:
                printf("\n프로그램을 종료합니다.\n");
                freeBayArea(bayArea);
                return 0;
            default:
                printf("잘못된 선택입니다.\n");
                system("pause");
        }

    }

    return 0;
}