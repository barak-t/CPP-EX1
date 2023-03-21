#include <stdio.h>
#include <stdlib.h>
#include "AdptArray.h"


typedef struct AdptArray_ {
    COPY_FUNC copyFunc;
    DEL_FUNC delFunc;
    PRINT_FUNC printFunc;
    int size;
    PElement* array;

} AdptArray, *PAdptArray;


PAdptArray CreateAdptArray(COPY_FUNC copyFunc, DEL_FUNC delFunc, PRINT_FUNC printFunc) {
    PAdptArray pAdptArray = (PAdptArray) malloc(sizeof(AdptArray));
    if (pAdptArray == NULL) {
       return NULL;
    }

    pAdptArray->copyFunc = copyFunc;
    pAdptArray->delFunc = delFunc;
    pAdptArray->printFunc = printFunc;
    pAdptArray->size = 0;
    pAdptArray->array = NULL;
    return pAdptArray;
}

void DeleteAdptArray(PAdptArray pAdptArray) {
    if (pAdptArray == NULL) {
        return;
    }
    for (int i = 0; i < pAdptArray->size; ++i) {
        if(pAdptArray->array[i] != NULL) {
            pAdptArray->delFunc(pAdptArray->array[i]);
        }
    }
    free(pAdptArray->array);
}

Result SetAdptArrayAt(PAdptArray pAdptArray, int index, PElement pElement) {
    if (pAdptArray == NULL) {
        return FAIL;
    }
    if (index >= pAdptArray->size) { // Need to resize the array
        PElement* newArray = (PElement*) malloc((index + 1) * sizeof(PElement));
        if (newArray == NULL) {
            return FAIL;
        }
        for (int i = 0; i < pAdptArray->size; ++i) { // Copy elements to the new array
            if (pAdptArray->array[i] != NULL) {
                newArray[i] = pAdptArray->array[i];
            } else {
                newArray[i] = NULL;
            }
        }
        free(pAdptArray->array);
        pAdptArray->array = newArray;
        pAdptArray->size = index + 1;
    }

    if (pAdptArray->array[index] != NULL) { // Delete the exising element
        pAdptArray->delFunc(pAdptArray->array[index]);
    }
    pAdptArray->array[index] = pAdptArray->copyFunc(pElement);
    return SUCCESS;
}

PElement GetAdptArrayAt(PAdptArray pAdptArray, int index) {
    if (pAdptArray == NULL) {
        return NULL;
    }
    if (index >= pAdptArray->size) {
        SetAdptArrayAt(pAdptArray, index, NULL);
        return NULL;
    }
    if (pAdptArray->array[index] == NULL) {
        return NULL;
    }
    return pAdptArray->copyFunc(pAdptArray->array[index]);
}

int GetAdptArraySize(PAdptArray pAdptArray) {
    if(pAdptArray == NULL) {
        return -1;
    }
    return pAdptArray->size;
}

void PrintDB(PAdptArray pAdptArray) {
    if (pAdptArray == NULL) {
        return;
    }

    for (int i = 0; i < pAdptArray->size; ++i) {
        if(pAdptArray->array[i] != NULL) {
            pAdptArray->printFunc(pAdptArray->array[i]);
        }
    }
}
