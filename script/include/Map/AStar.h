#include "raylib.h"
#include "raymath.h"
#include<stdbool.h>
#include<stdlib.h>
#include<stdio.h>
#include "math.h"

#include "./Map.h"
#include "../System/PacMath.h"


#pragma once


/***** DATA STRUCTURES ******/

/** @brief Estrutura que modela uma posição para o algorítmo A* */
typedef struct TNode {
    GridVector position;
    int gCost;  // distancia de manhathan a partir do inicio
    int hCost;  // um chute da distância de manhatan até o fim
    int fCost;  // uma heuristica pra prioziar os caminhos. gCost + fCost
    bool hasBeenVisited;    // Se os custos do node já foram avaliados alguma vez
    struct TNode* parent;   // Referência para o nó anterior no caminho | !!!!!! O caminho é construido através da hierarquia dos parents !!!!! 
} Node;


typedef struct TNodeElement {
    Node node;
    struct TNodeElement* next;
} NodeListElement;


typedef struct {
    NodeListElement* start;
    int size;
} NodeList;


/** @brief Calcula o gcost, hcost e fcost de um dado node */
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


bool insertIntoStartOfNodeList(NodeList* list, Node node){
    return insertIntoNodeList(list, node, 0);
}


/** @brief Inicializa um node com todos os seus valores e o adiciona numa lista */
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


/** @brief Aloca uma matriz de Nodes na memória com um chute dos seus custos calculados */
Node** setUpNodeMap(GridVector start, GridVector end){
    Node** nodeMap = (Node**)malloc((WIDTH*PIX2GRID)*sizeof(Node*));
    for(int i=0; i<WIDTH*PIX2GRID; i++){
        nodeMap[i] = (Node*)malloc((HEIGHT*PIX2GRID)*sizeof(Node));
        for(int j=0; j<HEIGHT*PIX2GRID; j++)
            innitNode(&nodeMap[i][j], (GridVector){i, j}, start, end);
    }
    return nodeMap;
}


void freeNodeMap(Node** nodeMap){
    for(int i=0; i<20; i++){
        free(*(nodeMap+i));
    }
    free(nodeMap);
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


/** @brief Transforma um caminho salvo através da hierarquia de nodes em uma NodeList */
NodeList getPathFromNodePile(Node finalNode, Node** nodeMap){
    NodeList path;
    innitNodeList(&path);

    Node currentNode = finalNode;
    
    while(currentNode.parent != NULL){
        insertIntoNodeList(&path, currentNode, 0);
        currentNode = *currentNode.parent;
    }
    insertIntoNodeList(&path, currentNode, 0);

    freeNodeMap(nodeMap);
    return path;
}



/** @brief Encontra o menor caminho de start a end no map através do algorítmo A* */
NodeList findPath(GridVector start, GridVector end, Map map){
    NodeList possibleNodes;                                         // Lista de todos os nodes tiveram seus custos g, h e f avaliados
    innitNodeList(&possibleNodes);

    Node** nodeMap = setUpNodeMap(start, end);

    innitNodeInsideNodeList(&possibleNodes, 0, start, start, end);  // Cria o node do início
    nodeMap[start.x][start.y].hasBeenVisited = true;

    while(possibleNodes.size != 0){
        Node* currentNode = getBestNode(&possibleNodes);
        currentNode->hasBeenVisited = true;

        if(gridVectorEquals(currentNode->position, end)){
            return getPathFromNodePile(*currentNode, nodeMap);
        }
        
        GridVector directions[4] = {{-1,0}, {1,0}, {0,-1}, {0,1}};  // Todas as direções em que um vizinho pode estar
        for(int i=0; i<4; i++){
            GridVector portalPosition = (GridVector){modulate(currentNode->position.x+directions[i].x, WIDTH*PIX2GRID), 
                                                     modulate(currentNode->position.y+directions[i].y, HEIGHT*PIX2GRID)};
            if(readCoordinatesInMap(portalPosition, map, (GridVector){0,0}) == '#')
                continue;

            Node* neighbor = &nodeMap[portalPosition.x][portalPosition.y];
            if(neighbor->hasBeenVisited)
                continue;
            neighbor->hasBeenVisited = true;

            if(neighbor->parent == NULL || currentNode->gCost + 1 < neighbor->gCost){
                neighbor->parent = &nodeMap[currentNode->position.x][currentNode->position.y];
                calculateNodeCosts(neighbor, start, end);

                if(!isNodeOnList(&possibleNodes, neighbor))
                    insertIntoStartOfNodeList(&possibleNodes, *neighbor);
            }
        }
        removeNodeFromNodeList(&possibleNodes, currentNode);
    }
    freeNodeMap(nodeMap);
    return (NodeList){NULL, 0};
}
