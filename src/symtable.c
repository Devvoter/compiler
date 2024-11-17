/**
 * Project : Prekladac imperativniho jazyka IFJ24
 * @file symtable.c
 * @brief Tabulka symbolu
 * 
 * @author Michaela Capíková (xcapikm00)
 * @date 2024-10-19
 */

#include "symtable.h"

void init_frame_stack (tFrameStack *fs) {
    fs->current = NULL;
    fs->global = NULL;
}

tFrame* push_frame (tFrameStack *fs, bool isFun) {
    tFrame *newFrame = malloc(sizeof(tFrame));
    if (newFrame == NULL) return NULL;

    newFrame->prev = fs->current;
    newFrame->symTable = NULL;
    newFrame->isFun = isFun;
    newFrame->retType = NOT_DEF;
    newFrame->calledReturn = false;

    fs->current = newFrame;
    if (fs->global == NULL) fs->global = newFrame;
    return newFrame;
}

tFrame* pop_frame (tFrameStack *fs) {
    tFrame *frame = NULL;
    if (fs->current != NULL) {
        frame = fs->current;
        fs->current = fs->current->prev;
        if (fs->current == NULL) fs->global = NULL;
    }
    return frame;
}

tSymTabNode* search_avl (tSymTabNode *node, char *key) {
    if (node == NULL) return NULL;
    int match = strcmp(node->id, key);

    if (match == 0) return node;
    else if (match > 0) return search_avl(node->left, key);
    else return search_avl(node->right, key);
}

tSymTabNode* search_symbol (tFrameStack *fs, char *key) {
    tFrame *frame = fs->current;
    tSymTabNode *found = search_avl(frame->symTable, key);

    while (found == NULL && frame->prev != NULL) {
        frame = frame->prev;
        found = search_avl(frame->symTable, key);
    }
    return found;
}

tSymTabNode* rotate_ll (tSymTabNode *root) {
    tSymTabNode *tmp = root;
    root = root->left;
    tmp->left = root->right;
    root->right = tmp;
    root->height = 0;
    tmp->height = 0;
    return root;
}

tSymTabNode* rotate_rr (tSymTabNode *root) {
    tSymTabNode *tmp = root;
    root = root->right;
    tmp->right = root->left;
    root->left = tmp;
    root->height = 0;
    tmp->height = 0;
    return root;
}

tSymTabNode* rotate_dlr (tSymTabNode *root) {
    tSymTabNode *tmpB = root->left;
    tSymTabNode *tmpC = tmpB->right; 
    tmpB->right = tmpC->left;
    root->left = tmpC->right;
    tmpC->left = tmpB;
    tmpC->right = root;
    // Korekce váhy
    if(tmpC->height == 1) {
        tmpB->height = -1;
        root->height = 0;
    }
    else if(tmpC->height == -1) {
        tmpB->height = 0;
        root->height = 1;
    }
    else {
        tmpB->height = 0;
        root->height = 0;
    }
    tmpC->height = 0;
    return tmpC;
}

tSymTabNode* rotate_drl (tSymTabNode *root) {
    tSymTabNode *tmpB = root->right;
    tSymTabNode *tmpC = tmpB->left; 
    tmpB->left = tmpC->right;
    root->right = tmpC->left;
    tmpC->right = tmpB;
    tmpC->left = root;
    // Korekce váhy
    if(tmpC->height == 1) {
        tmpB->height = 0;
        root->height = -1;
    }
    else if(tmpC->height == -1) {
        tmpB->height = 1;
        root->height = 0;
    }
    else {
        tmpB->height = 0;
        root->height = 0;
    }
    tmpC->height = 0;
    return tmpC;
}

tSymTabNode* insert_avl (tSymTabNode *root, tSymTabNode *node, bool *heightChange) {
    // vložit ako kořen
    if(root == NULL) {
        *heightChange = true;
        return node;
    }
    // vložit do levého podstromu
    else if (strcmp(root->id, node->id) > 0) {
        root->left = insert_avl(root->left, node, heightChange);

        if (*heightChange) {
            root->height--;

            // if(root->height == -1) změna výšky putuje dál do otcovského uzlu
            if(root->height == 0) *heightChange = false;
            if(root->height == -2) {
                // Kritický uzel, musíme rotovat
                // Situace LL
                if (root->left->height < 0) root = rotate_ll(root);
                // Situace LR
                else root = rotate_dlr(root);
                *heightChange = false;
            }
        }
    }
    // vložit do pravého podstromu
    else {
        root->right = insert_avl(root->right, node, heightChange); 
        
        if (*heightChange) {
            root->height++;

            // if(root->height == 1) změna výšky putuje dál do otcovského uzlu
            if(root->height == 0) *heightChange = false;
            if(root->height == 2) { 
                // Kritický uzel, musíme rotovat
                // Situace RR
                if (root->right->height > 0) root = rotate_rr (root);
                // Situace RL
                else root = rotate_drl (root);
                *heightChange = false;
            }
        }
    }
    return root;
}

tSymTabNode* create_var_node (bool isConst) {
    tSymTabNode *newVar = malloc(sizeof(tSymTabNode));
    if (newVar == NULL) return NULL;

    newVar->varData = malloc(sizeof(tVar));
    if (newVar->varData == NULL) { 
        free(newVar);
        return NULL;
    }
    newVar->varData->isConst = isConst;
    newVar->varData->isUsed = false;
    newVar->varData->isDef = false;
    newVar->varData->isNull = false;
    newVar->varData->dataType = NOT_DEF;

    newVar->id = NULL;
    newVar->isFun = false;
    newVar->funData = NULL;

    newVar->height = 0;
    newVar->left = NULL;
    newVar->right = NULL;
    
    return newVar;
}

tSymTabNode* create_fun_node () {
    tSymTabNode *newFun = malloc(sizeof(tSymTabNode));
    if (newFun == NULL) return NULL;

    newFun->funData = malloc(sizeof(tVar));
    if (newFun->funData == NULL) { 
        free(newFun);
        return NULL;
    }
    newFun->paramCnt = 0;
    newFun->paramTypes = NULL;
    newFun->retType = NOT_DEF;

    newFun->isFun = true;
    newFun->id = NULL;
    newFun->varData = NULL;
    newFun->height = 0;
    newFun->left = NULL;
    newFun->right = NULL;

    return newFun;
}

bool insert_symbol (tFrameStack *fs, tSymTabNode *node) {
    if(search_symbol(fs, node->id) != NULL) { 
        // semanticka chyba - duplicitny identifikator
        return false;
    }
    bool heightChange = false;
    fs->current->symTable = insert_avl(fs->current->symTable, node, &heightChange);
    return true;
}

/* Konec souboru symtable.c */