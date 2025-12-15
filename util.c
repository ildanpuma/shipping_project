#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "util.h"
#include "data_types.h"

void menu_loadCargo(BayArea* bayArea) {
    system("cls");
    printf("\n=== 화물 적재 ===\n");
    printf("1. 직접 입력\n");
    printf("2. CSV 파일에서 불러오기\n");
    printf("선택: ");
    
    int selection;
    if (scanf("%d", &selection) != 1) {
        while (getchar() != '\n');
        printf("잘못된 입력입니다.\n");
        system("pause");
        return;
    }
    
    Cargo** cargos = NULL;
    int cargoCount = 0;
    
    // 직접 입력
    if (selection == 1) {
        printf("적재할 화물 개수: ");
        scanf("%d", &cargoCount);
        
        if (cargoCount <= 0 || cargoCount > MAX_CARGO) {
            printf("잘못된 개수입니다. (1-%d)\n", MAX_CARGO);
            return;
        }
        
        cargos = (Cargo**)malloc(sizeof(Cargo*) * cargoCount);
        
        for (int i = 0; i < cargoCount; i++) {
            int weight;
            char owner[20];
            
            printf("\n[화물 %d]\n", i + 1);
            
            // 무게 입력 (100kg 이하로 제한)
            do {
                printf("무게(kg, 1-%d): ", MAX_CARGO_WEIGHT);
                scanf("%d", &weight);
                
                if (weight <= 0 || weight > MAX_CARGO_WEIGHT) {
                    printf("잘못된 무게입니다. 1-%dkg 사이로 입력하세요.\n", MAX_CARGO_WEIGHT);
                }
            } while (weight <= 0 || weight > MAX_CARGO_WEIGHT);
            
            printf("소유자 이름: ");
            scanf("%s", owner);
            
            cargos[i] = createCargo(i + 1, weight, owner);
        }
    } 
    // CSV 파일에서 로드
    else if (selection == 2) {
        char filename[100];
        printf("CSV 파일명 입력: ");
        scanf("%s", filename);
        
        cargoCount = loadCargosFromCSV(filename, &cargos);
        if (cargoCount == 0) {
            printf("적재할 화물이 없습니다.\n");
            system("pause");
            return;
        }
    }
    else {
        printf("잘못된 선택입니다.\n");
        system("pause");
        return;
    }
    
    loadCargoToBayArea(bayArea, cargos, cargoCount);
    printf("\n%d개 화물이 적재되었습니다.\n", cargoCount);
    system("pause");
    free(cargos);
}

// 화물 검색 메뉴
void menu_searchCargo(BayArea* bayArea) {
    system("cls");
    
    if (bayArea->top == -1) {
        printf("\n적재된 화물이 없습니다.\n");
        system("pause");
        return;
    }
    
    while(1) {
        system("cls");
        printf("\n=== 화물 검색 ===\n");
        printf("1. 소유자 이름으로 검색\n");
        printf("2. ID로 검색\n");
        printf("3. 종료\n");
        printf("선택: ");
        
        int searchType;
        scanf("%d", &searchType);
        
        if (searchType == 3) {
            break;
        }
        
        if (searchType == 1) {
            // 이름으로 검색
            char searchName[20];
            system("cls");
            printf("\n=== 소유자 이름으로 검색 ===\n");
            printf("검색할 소유자 이름: ");
            scanf("%s", searchName);
            
            int found = 0;
            for (int i = 0; i <= bayArea->top; i++) {
                Bay* bay = bayArea->bays[i];
                for (int j = 0; j <= bay->top; j++) {
                    if (strcmp(bay->tiers[j]->owner_name, searchName) == 0) {
                        printf("\n[발견] Bay %d, 위치 %d\n", i, j);
                        printf("  ID: %d\n", bay->tiers[j]->id);
                        printf("  무게: %dkg\n", bay->tiers[j]->weight);
                        printf("  소유자: %s\n", bay->tiers[j]->owner_name);
                        found = 1;
                    }
                }
            }
            
            if (!found) {
                printf("\n'%s' 이름의 화물을 찾을 수 없습니다.\n", searchName);
            }
            system("pause");
        }
        else if (searchType == 2) {
            // ID로 검색
            int searchId;
            system("cls");
            printf("\n=== ID로 검색 ===\n");
            printf("검색할 ID: ");
            scanf("%d", &searchId);
            
            int found = 0;
            for (int i = 0; i <= bayArea->top; i++) {
                Bay* bay = bayArea->bays[i];
                for (int j = 0; j <= bay->top; j++) {
                    if (bay->tiers[j]->id == searchId) {
                        printf("\n[발견] Bay %d, 위치 %d\n", i, j);
                        printf("  ID: %d\n", bay->tiers[j]->id);
                        printf("  무게: %dkg\n", bay->tiers[j]->weight);
                        printf("  소유자: %s\n", bay->tiers[j]->owner_name);
                        found = 1;
                        break;
                    }
                }
                if (found) break;
            }
            
            if (!found) {
                printf("\nID %d인 화물을 찾을 수 없습니다.\n", searchId);
            }
            system("pause");
        }
        else {
            printf("잘못된 선택입니다.\n");
            system("pause");
        }
    }    
    system("pause");
}

// 하역 순서 조회 메뉴
void menu_showUnloadOrder(BayArea* bayArea) {
    system("cls");
    printf("\n=== 하역 순서 조회 ===\n");
    printf("1. 하역 순서 (Bay별)\n");
    printf("선택: ");
    
    int selection;
    scanf("%d", &selection);
    
    if (selection == 1) {
        Cargo** unloadIndex = createCargoUnloadIndex(bayArea);
        int totalCount = 0;
        for (int i = 0; i <= bayArea->top; i++) {
            totalCount += bayArea->bays[i]->top + 1;
        }
        
        printf("\n--- 하역 순서 (총 %d개) ---\n", totalCount);
        for (int i = 0; i < totalCount; i++) {
            printf("%d. ID:%d, %dkg, %s\n", 
                   i + 1, 
                   unloadIndex[i]->id, 
                   unloadIndex[i]->weight, 
                   unloadIndex[i]->owner_name);
        }
        
        printf("\nCSV 파일로 저장하시겠습니까?\n");
        printf("1. 예\n");
        printf("2. 아니오\n");
        printf("선택: ");
        
        int saveSelection;
        scanf("%d", &saveSelection);
        
        if (saveSelection == 1) {
            char filename[100];
            printf("저장할 파일명을 입력하세요 : ");
            scanf("%s", filename);
            
            char fullFilename[110];
            snprintf(fullFilename, sizeof(fullFilename), "%s.csv", filename);
            
            FILE* fp = fopen(fullFilename, "w");
            if (fp == NULL) {
                printf("파일을 생성할 수 없습니다: %s\n", fullFilename);
            } else {
                for (int i = 0; i < totalCount; i++) {
                    fprintf(fp, "%d,%d,%s\n",
                            unloadIndex[i]->id,
                            unloadIndex[i]->weight,
                            unloadIndex[i]->owner_name);
                }
                fclose(fp);
                printf("%d개의 하역 순서가 %s 파일에 저장되었습니다.\n", totalCount, fullFilename);
            }
        }
        
        freeCargoUnloadIndex(unloadIndex);
    } 
    else {
        printf("잘못된 선택입니다.\n");
        system("pause");
    }
    system("pause");
}

void menu_showBayStatus(BayArea* bayArea) {
    system("cls");
    printf("\n=== Bay 적재 현황  ===\n\n");
    
    if (bayArea->top == -1) {
        printf("적재된 화물이 없습니다.\n");
        system("pause");
        return;
    }
    
    int totalCargo = 0;
    int totalWeight = 0;
    
    for (int row = 0; row < 3; row++) {
        for (int col = 0; col < 2; col++) {
            int bayIndex = row * 2 + col;
            if (bayIndex < MAX_BAYS) {
                printf("[Bay %d]%-16s", bayIndex, "");
            }
        }
        printf("\n");
        for (int tier = 0; tier < MAX_CARGO_PER_BAY; tier++) {
            for (int col = 0; col < 2; col++) {
                int bayIndex = row * 2 + col;
                if (bayIndex < MAX_BAYS) {
                    if (bayIndex <= bayArea->top) {
                        Bay* bay = bayArea->bays[bayIndex];
                        if (tier <= bay->top) {
                            char cargoInfo[23];
                            snprintf(cargoInfo, sizeof(cargoInfo), "ID:%d %dkg %.5s...", 
                                   bay->tiers[tier]->id,
                                   bay->tiers[tier]->weight,
                                   bay->tiers[tier]->owner_name);
                            printf("%-25s", cargoInfo);
                        } else {
                            printf("%-22s", "---");
                        }
                    } else {
                        printf("%-25s", "---");
                    }
                }
            }
            printf("\n");
        }
        
        // Bay 통계 출력
        for (int col = 0; col < 2; col++) {
            int bayIndex = row * 2 + col;
            if (bayIndex < MAX_BAYS) {
                if (bayIndex <= bayArea->top) {
                    Bay* bay = bayArea->bays[bayIndex];
                    int bayWeight = SumBayWeight(bay);
                    int cargoCount = bay->top + 1;
                    
                    char stats[23];
                    snprintf(stats, sizeof(stats), "(%d개/%dkg)", cargoCount, bayWeight);
                    printf("%-22s", stats);
                    
                    totalCargo += cargoCount;
                    totalWeight += bayWeight;
                } else {
                    printf("%-25s", "(0개/0kg)");
                }
            }
        }
        printf("\n\n");
    }
    
    printf("\n--- 전체 통계 ---\n");
    printf("총 Bay 수: %d\n", bayArea->top + 1);
    printf("총 화물 개수: %d\n", totalCargo);
    printf("총 무게: %dkg\n", totalWeight);
    printf("평균 Bay 무게: %.1fkg\n", (float)totalWeight / (bayArea->top + 1));
    system("pause");
}

// CSV 파일에서 화물 데이터 로드
// 읽어온 화물 개수, cargos에 할당된 배열 저장
int loadCargosFromCSV(const char* filename, Cargo*** cargos) {
    FILE* fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("파일을 열 수 없습니다: %s\n", filename);
        return 0;
    }
    
    int count = 0;
    char line[256];
    while (fgets(line, sizeof(line), fp) != NULL) {
        count++;
    }
    
    if (count == 0) {
        fclose(fp);
        printf("파일이 비어있습니다.\n");
        return 0;
    }
    rewind(fp);
    
    *cargos = (Cargo**)malloc(sizeof(Cargo*) * count);
    
    int idx = 0;
    int ignored = 0;
    int id, weight;
    char owner[20];

    while (fgets(line, sizeof(line), fp) != NULL) {
        line[strcspn(line, "\r\n")] = '\0';
        if (strlen(line) == 0) continue;
        
        int scanResult = sscanf(line, "%d,%d,%19s", &id, &weight, owner);
        if (scanResult == 3) {
            // 100kg 초과 화물은 무시
            if (weight > MAX_CARGO_WEIGHT) {
                printf("[무시] ID:%d, %dkg (100kg 초과): %s\n", id, weight, owner);
                ignored++;
            } else {
                (*cargos)[idx] = createCargo(id, weight, owner);
                idx++;
            }
        } else {
            printf("[경고] 불러오기 실패 (라인 %d): %s\n", idx + 1, line);
        }
    }
    
    fclose(fp);
    if (ignored > 0) {
        printf("CSV 파일에서 %d개 화물을 불러왔습니다. (%d개 무시)\n", idx, ignored);
    } else {
        printf("CSV 파일에서 %d개 화물을 불러왔습니다.\n", idx);
    }
    return idx;
}

void saveCargoToCSV(BayArea* bayArea, const char* filename) {
    FILE* fp = fopen(filename, "w");
    if (fp == NULL) {
        printf("파일을 생성할 수 없습니다: %s\n", filename);
        return;
    }
    
    int totalSaved = 0;
    for (int i = 0; i <= bayArea->top; i++) {
        Bay* bay = bayArea->bays[i];
        for (int j = 0; j <= bay->top; j++) {
            fprintf(fp, "%d,%d,%s\n", 
                    bay->tiers[j]->id, 
                    bay->tiers[j]->weight, 
                    bay->tiers[j]->owner_name);
            totalSaved++;
        }
    }
    
    fclose(fp);
    printf("%d개 화물이 %s 파일에 저장되었습니다.\n", totalSaved, filename);
}
