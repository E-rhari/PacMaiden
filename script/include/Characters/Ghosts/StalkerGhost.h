#include "raylib.h"
#include "raymath.h"
#include<stdbool.h>
#include<stdlib.h>
#include<stdio.h>
#include <sys/time.h>
#include "math.h"

#include "../Character.h"
#include "./Ghost.h"
#include "../PacMaiden.h"
#include "../../Map/Map.h"

#pragma once


/***** DATA STRUCTURES ******/
typedef struct TNode {
    GridVector position;
    int gCost;  // distancia de manhathan a partir do inicio
    int hCost;  // um chute da distância de manhatan até o fim
    int fCost;  // uma heuristica pra prioziar os caminhos. gCost + fCost
    bool hasBeenVisited;
    struct TNode* parent; // Referência para o nó anterior no caminho | !!!!!! O caminho é construido através da hierarquia dos parents !!!!! 
} Node;


typedef struct TNodeElement {
    Node node;
    struct TNodeElement* next;
} NodeListElement;


typedef struct {
    NodeListElement* start;
    int size;
} NodeList;


void calculateNodeCosts(Node* node, GridVector start, GridVector end){
    if(node->parent != NULL)
        node->gCost = node->parent->gCost + 1;
    else
        node->gCost = __INT_MAX__;
    node->hCost = abs(node->position.x - end.x) + abs(node->position.y - end.y);
    node->fCost = node->gCost + node->hCost;
}


void innitNode(Node* node,  GridVector nodePosition, GridVector start, GridVector end){
    node->position = nodePosition;
    node->parent = NULL;
    calculateNodeCosts(node, start, end);
    node->hasBeenVisited = false;
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


bool innitNodeInsideNodeList(NodeList* list, int index, GridVector nodePosition, GridVector start, GridVector end){
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


bool isNodeOnList(NodeList* list, Node* node){
    for(int i=0; i<list->size; i++)
        if(getFromNodeList(list, i) == node)
            return true;
    return false;
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
    printf("\n\t ]\n");
}


/** A* (A STAR) ALGORITHM */

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
    printf("-------------------- FINAL PATH ----------------------\n");
    Node currentNode = finalNode;
    while(currentNode.parent != NULL){
        printf("(%d, %d) -> ", currentNode.position.x, currentNode.position.y);
        currentNode = *currentNode.parent;
    }
    printf("Start\n");
    printf("-----------------------------------------------------\n");
}


NodeList getPathFromNodePile(Node finalNode){
    Node currentNode = finalNode;

    NodeList path;
    innitNodeList(&path);
    
    while(currentNode.parent != NULL){
        insertIntoNodeList(&path, currentNode, 0);
        currentNode = *currentNode.parent;
    }
    insertIntoNodeList(&path, currentNode, 0);
    return path;
}


Node** setUpNodeMap(GridVector start, GridVector end){
    Node** nodeMap = (Node**)malloc((LARGURA/40)*sizeof(Node*));
    for(int i=0; i<LARGURA/40; i++){
        nodeMap[i] = (Node*)malloc((ALTURA/40)*sizeof(Node));
        for(int j=0; j<ALTURA/40; j++)
            innitNode(&nodeMap[i][j], (GridVector){i, j}, start, end);
    }
    return nodeMap;
}


NodeList findPath(GridVector start, GridVector end, Map map){
    NodeList possibleNodes;                                         // Lista de todos os nodes tiveram seus custos g, h e f avaliados
    innitNodeList(&possibleNodes);

    Node** nodeMap = setUpNodeMap(start, end);

    innitNodeInsideNodeList(&possibleNodes, 0, start, start, end);  // Cria o node do início
    nodeMap[start.x][start.y].hasBeenVisited = true;

    while(possibleNodes.size != 0){
        Node* currentNode = getBestNode(&possibleNodes);
        currentNode->hasBeenVisited = true;

        if(gridVectorEquals(currentNode->position, end))
            return getPathFromNodePile(*currentNode);
        
        GridVector directions[4] = {{-1,0}, {1,0}, {0,-1}, {0,1}};  // Todas as direções em que um vizinho pode estar
        for(int i=0; i<4; i++){
            if(!isInsideMap(currentNode->position, map, directions[i]) || readCoordinatesInMap(currentNode->position, map, directions[i]) == '#')
                continue;

            Node* neighbor = &nodeMap[currentNode->position.x+directions[i].x][currentNode->position.y+directions[i].y];
            if(neighbor->hasBeenVisited)
                continue;
            neighbor->hasBeenVisited = true;

            if(neighbor->parent == NULL || currentNode->gCost + 1 < neighbor->gCost){
                neighbor->parent = &nodeMap[currentNode->position.x][currentNode->position.y];
                calculateNodeCosts(neighbor, start, end);

                if(!isNodeOnList(&possibleNodes, neighbor))
                    insertIntoEndOfNodeList(&possibleNodes, *neighbor);
            }
        }
        removeNodeFromNodeList(&possibleNodes, currentNode);
    }
}


void stalkPacmaiden(Ghost* ghost, Map map, PacMaiden* pacmaiden){
    NodeList path;
    path = findPath(vector2ToGridVector(ghost->chara.circle.center), vector2ToGridVector(pacmaiden->chara.circle.center), map);
    if(path.start == NULL || pacmaiden->state == IMMORTAL){
        chooseDestinationAware(ghost, map);
        return;
    }

    Node nextStep = *getFromNodeList(&path, 1);

    ghost->chara.moveDirection = (Vector2){nextStep.position.x - (int)(ghost->chara.circle.center.x*PIX2GRID), 
                                           nextStep.position.y - (int)(ghost->chara.circle.center.y*PIX2GRID)};
}