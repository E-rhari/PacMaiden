#include "raylib.h"
#include "raymath.h"
#include<stdbool.h>
#include<stdlib.h>
#include<stdio.h>
#include <sys/time.h>
#include "math.h"


#include "./Character.h"
#include "./Ghost.h"
#include "./PacMaiden.h"
#include "../Map/Map.h"

// #pragma once

typedef char** Map;

typedef struct {
    int x;
    int y;
} GridVector;


bool compareGridPositions(GridVector position1, GridVector position2){
    return position1.x == position2.x && position1.y == position2.y;
}


/** DATA STRUCTURES */

typedef struct TNode {
    GridVector position;
    int gCost;  // distancia de manhathan a partir do inicio
    int hCost;  // um chute da distância de manhatan até o fim
    int fCost;  // uma heuristica pra prioziar os caminhos. gCost + fCost
    bool hasBeenVisited;
    struct TNode* parent; // !!!!!! O caminho é construido através da hierarquia dos parents !!!!! 
} Node;


typedef struct TNodeElement {
    Node node;
    struct TNodeElement* next;
} NodeListElement;


typedef struct {
    NodeListElement* start;
    int size;
} NodeList;


void innitNode(Node* node,  GridVector nodePosition, GridVector start, GridVector end){
    node->position = nodePosition;

    node->gCost = abs(start.x - nodePosition.x) + abs(start.y - nodePosition.y); // Distância de Manhatan
    node->hCost = abs(nodePosition.x - end.x) + abs(nodePosition.y - end.y);
    node->fCost = node->hCost + node->gCost;
    node->hasBeenVisited = false;
    node->parent = NULL;
}


void innitNodeListElement(NodeListElement* element, GridVector nodePosition, GridVector start, GridVector end){
    element->next = NULL;
    innitNode(&element->node, nodePosition, start, end);
    element->node.parent = NULL;
}


void innitNodeList(NodeList* list){
    list->start = NULL;
    list->size = 0;
}


bool insertIntoNodeList(NodeList* list, Node node, int index){
    if(index<0 || index > list->size)
        return false;
    
    NodeListElement* newElement = (NodeListElement*) malloc(sizeof(NodeListElement));
        
    newElement->node = node;

    if(index == 0){
        newElement->next = list->start;
        list->start = newElement;
    }
    else{
        NodeListElement* aux = list->start;
        for(int i=0; i<index-1; i++)
            aux = aux->next;
        newElement->next = aux->next;
        aux->next = newElement;
    }

    list->size++;
    return true;
}


bool insertIntoEndOfNodeList(NodeList* list, Node node){
    return insertIntoNodeList(list, node, 0);
}


bool innitInsideNodeList(NodeList* list, int index, GridVector nodePosition, GridVector start, GridVector end){
    Node* node = (Node*) malloc(sizeof(Node));
    innitNode(node, nodePosition, start, end);
    return insertIntoNodeList(list, *node, index);
}


Node* getFromNodeList(NodeList* list, int index){
    if(index<0 || index > list->size)
        return NULL;
    
    NodeListElement* current = list->start;
    for(int i=0; i<index; i++)
        current = current->next;
    return &current->node;
}

bool removeIndexFromNodeList(NodeList* list, int index){
    if(index<0 || index > list->size)
        return false;
    
    NodeListElement* elToRemove;
    if(index == 0){
        elToRemove = list->start;
        list->start = list->start->next;
    }
    else{
        NodeListElement* current = list->start;
        for(int i=0; i<index-1; i++)
            current = current->next;
        elToRemove = current->next;
        current->next = elToRemove->next;
    }
    free(elToRemove);
    list->size--;
    return true;
}


bool isNodeOnList(NodeList* list, Node* node){
    for(int i=0; i<list->size; i++)
        if(getFromNodeList(list, i) == node)
            return true;
    return false;
}


bool removeNodeFromNodeList(NodeList* list, Node* node){
    for(int i=0; i<list->size; i++)
        if(getFromNodeList(list, i) == node)
            return removeIndexFromNodeList(list, i);
    return false;
}


void printNodeList(NodeList list){
    NodeListElement* currentElement = list.start;
    printf("NodeList:[");

    printf("\n\t\tPosition\tgCost\thCost\tHeuristc");
    while(currentElement != NULL){
        printf("\n\t\t(%d, %d)  ", currentElement->node.position.x, currentElement->node.position.y);
        printf("\t%d", currentElement->node.gCost);
        printf("\t%d", currentElement->node.hCost);
        printf("\t%d", currentElement->node.fCost);
        currentElement = currentElement->next;
    }
    printf("\n\t ]");
}


/** A* (A STAR) ALGORITHM */


bool isInsideMap(GridVector gridPosition, Map map, GridVector displacement){
    return (int)gridPosition.y+(int)displacement.y>=0 && (int)gridPosition.y+(int)displacement.y<ALTURA/40
        && (int)gridPosition.x+(int)displacement.x>=0 && (int)gridPosition.x+(int)displacement.x<LARGURA/40;
}


char readCoordinatesInMap(GridVector gridPosition, Map map, GridVector displacement){
    if(isInsideMap(gridPosition, map, displacement))
        return map[(int)gridPosition.y+(int)displacement.y][(int)gridPosition.x + (int)displacement.x];
}


// MUITO ineficiente. Vamos mudar pra uma Heap!!!!
Node* getBestNode(NodeList* list){
    Node* currentBestNode = getFromNodeList(list, 0);
    for(int i=0; i<list->size; i++){
        Node* testingNode = getFromNodeList(list, i);
        if(testingNode->fCost <= currentBestNode->fCost || testingNode->hCost < currentBestNode->hCost)
            currentBestNode = testingNode;
    }
    return currentBestNode;
}


void printPath(Node finalNode){
    printf("---------- FINAL PATH ------------\n");
    Node currentNode = finalNode;
    while(currentNode.parent != NULL){
        printf("(%d, %d) -> ", currentNode.position.x, currentNode.position.y);
        currentNode = *currentNode.parent;
    }
    printf("Start\n");
    printf("----------------------------------");
}


void findPath(GridVector start, GridVector end, Map map){
    NodeList openList;
    innitNodeList(&openList);

    Node** nodeMap = (Node**)malloc((LARGURA/40)*sizeof(Node*));
    for(int i=0; i<LARGURA/40; i++){
        nodeMap[i] = (Node*)malloc((ALTURA/40)*sizeof(Node));
        for(int j=0; j<ALTURA/40; j++)
            innitNode(&nodeMap[i][j], (GridVector){i, j}, start, end);
    }

    innitInsideNodeList(&openList, 0, start, start, end);
    nodeMap[start.x][start.y].hasBeenVisited = true;
    int i = 0;

    while(openList.size != 0){
        printf("\n---- Loop #%d ----\n", i);
        Node* currentNode = getBestNode(&openList);
        printf("Best Node: (%d, %d)\n", currentNode->position.x, currentNode->position.y);
        if(!compareGridPositions(currentNode->position, start))
            printf("Best Node's parent: (%d, %d)\n", currentNode->parent->position.x, currentNode->parent->position.x);
    
        currentNode->hasBeenVisited = true;

        if(compareGridPositions(currentNode->position, end)){
            printPath(*currentNode);
            return;
        }
        
        GridVector directions[4] = {{-1,0}, {1,0}, {0,-1}, {0,1}};
        for(int i=0; i<4; i++){
            printf("Neighbor: (%d, %d)  -  ", currentNode->position.x+directions[i].x, currentNode->position.y+directions[i].y);
            printf("Object type: %c\n", readCoordinatesInMap(currentNode->position, map, directions[i]));
            if(!isInsideMap(currentNode->position, map, directions[i]) || readCoordinatesInMap(currentNode->position, map, directions[i]) == '#')
                continue;

            Node* neighbor = &nodeMap[currentNode->position.x+directions[i].x][currentNode->position.y+directions[i].y];
            // printf("Neighbor: (%d, %d)\n", neighbor->position.x, neighbor->position.y);

            if(neighbor->hasBeenVisited)
                continue;
            neighbor->hasBeenVisited = true;


            int gCostToNeighbor = currentNode->gCost + 1;
            if(neighbor->parent == NULL || gCostToNeighbor < neighbor->gCost){
                neighbor->gCost = gCostToNeighbor;
                neighbor->hCost = abs(neighbor->position.x - end.x) + abs(neighbor->position.y - end.y);
                neighbor->fCost = neighbor->gCost + neighbor->hCost;
                neighbor->parent = &nodeMap[currentNode->position.x][currentNode->position.y];

                if(!isNodeOnList(&openList, neighbor))
                    insertIntoEndOfNodeList(&openList, *neighbor);
            }
        }
        removeNodeFromNodeList(&openList, currentNode);
        printf("Size: %d\n", openList.size);
        printNodeList(openList);
        i++;

        WaitTime(1);
    }
}


GridVector vector2ToGridVector(Vector2 vector){
    return (GridVector){(int)(vector.x/40), (int)(vector.y/40)};
}

void stalkPacmaiden(Ghost* ghost, Map map, PacMaiden* pacmaiden){
    printf("\nStart: (%d, %d)\n End: (%d, %d)\n\n", vector2ToGridVector(ghost->chara.circle.center).x,     vector2ToGridVector(ghost->chara.circle.center).y,
                                                  vector2ToGridVector(pacmaiden->chara.circle.center).x, vector2ToGridVector(pacmaiden->chara.circle.center).y);
    findPath(vector2ToGridVector(ghost->chara.circle.center), vector2ToGridVector(pacmaiden->chara.circle.center), map);
}

// int main(){
//     GridVector pathStart = {14,1};
//     GridVector pathEnd = {22,1};

//     NodeList list;
//     innitNodeList(&list);

//     printf("Start: (%d, %d)\n", pathStart.x, pathStart.y);
//     printf("Start: (%d, %d)\n", pathEnd.x, pathEnd.y);
//     printf("\n");

//     innitInsideNodeList(&list, 0, (GridVector){1,1}, pathStart, pathEnd);
//     innitInsideNodeList(&list, 0, (GridVector){3,1}, pathStart, pathEnd);
//     innitInsideNodeList(&list, 0, (GridVector){1,2}, pathStart, pathEnd);

//     printNodeList(list);
//     printf("\n\n");

//     printf("Best Node: (%d, %d)\n", getBestNode(&list)->position.x, getBestNode(&list)->position.y);

//     Map map = (char**)malloc(sizeof(char*)*20);
//     for(int i=0;i<20;i++)
//         *(map+i) = (char*)malloc(sizeof(char)*40);
//     readMap(1, map);


//     struct timeval t0, t1;
//     gettimeofday(&t0, NULL);

//     // for(int i=0; i<600; i++)
//         findPath(pathStart, pathEnd, map);

//     gettimeofday(&t1, NULL);
//     printf("\nDuration: %.2gs\n", t1.tv_sec - t0.tv_sec + 1E-6 * (t1.tv_usec - t0.tv_usec));
// }