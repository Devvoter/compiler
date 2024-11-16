/**
 * @file test_symtable.c
 * @author Michaela Capíková (xcapikm00)
 * @brief testy pre operacie nad avl stromom
 * 
 * @date 2024-11-08
 * 
 */

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "../src/symtable.h"

char *str_duplicate(const char *src) {
    char *dst = malloc(strlen (src) + 1);
    if (dst == NULL) return NULL;
    strcpy(dst, src);
    return dst;
}

void test_RR_rot() {
    tSymTabNode *root = NULL;
    bool heightChange = false;

    // Vložení prvního uzlu
    tSymTabNode *node1 = create_var_node(false);
    node1->id = str_duplicate("a");
    root = insert_avl(root, node1, &heightChange);
    assert(root != NULL);
    assert(strcmp(root->id, "a") == 0);
    assert(root->height == 0);

    // Vložení druhého uzlu, zprava
    tSymTabNode *node2 = create_var_node(false);
    node2->id = str_duplicate("b");
    root = insert_avl(root, node2, &heightChange);
    assert(root != NULL);
    assert(strcmp(root->right->id, "b") == 0);
    assert(root->height == 1);  // Výška stromu by se mela aktualizovat

    // Vložení třetího uzlu, zprava, vyžaduje RR rotaci
    tSymTabNode *node3 = create_var_node(false);
    node3->id = str_duplicate("c");
    root = insert_avl(root, node3, &heightChange);
    assert(root != NULL);
    assert(strcmp(root->id, "b") == 0);
    assert(strcmp(root->left->id, "a") == 0);
    assert(strcmp(root->right->id, "c") == 0);
    assert(root->height == 0);

    printf("Testy pro insert_avl OK!\n");
}

void test_search_symbol() {
    tFrameStack fs;
    init_frame_stack(&fs);
    bool heightChange = false;

    // Vytvoříme nový frame, vložíme uzel
    push_frame(&fs, false);
    tSymTabNode *node1 = create_var_node(false);
    node1->id = str_duplicate("a");
    fs.current->symTable = insert_avl(fs.current->symTable, node1, &heightChange);

    // Vyhledání existujícího symbolu
    tSymTabNode *found = search_symbol(&fs, "a");
    assert(found != NULL);
    assert(strcmp(found->id, "a") == 0);

    // Vyledání v 2. úrovni tabulky
    push_frame(&fs, false);
    tSymTabNode *node2 = create_var_node(false);
    node2->id = str_duplicate("b");
    fs.current->symTable = insert_avl(fs.current->symTable, node2, &heightChange);

    // Vyhledání existujícího symbolu
    found = search_symbol(&fs, "b");
    assert(found != NULL);
    assert(strcmp(found->id, "b") == 0);

    // Vyhledání neexistujícího symbolu
    found = search_symbol(&fs, "c");
    assert(found == NULL);

    printf("Testy pro search_symbol OK!\n");
}

void test_LL_rot() {
    tSymTabNode *root = NULL;
    bool heightChange = false;

    tSymTabNode *node1 = create_var_node(false);
    node1->id = str_duplicate("c");
    root = insert_avl(root, node1, &heightChange);

    tSymTabNode *node2 = create_var_node(false);
    node2->id = str_duplicate("b");
    root = insert_avl(root, node2, &heightChange);
    assert(root->height == -1);

    tSymTabNode *node3 = create_var_node(false);
    node3->id = str_duplicate("a");
    root = insert_avl(root, node3, &heightChange);

    assert(strcmp(root->id, "b") == 0);
    assert(strcmp(root->left->id, "a") == 0);
    assert(strcmp(root->right->id, "c") == 0);
    assert(root->height == 0);

    printf("Testy LL rotace OK!\n");
}

void test_frame_stack() {
    tFrameStack fs;
    init_frame_stack(&fs);

    // Push
    tFrame *frame1 = push_frame(&fs, false);
    assert(frame1 != NULL);

    tFrame *frame2 = push_frame(&fs, true);
    assert(frame2 != NULL);
    assert(fs.current == frame2);
    assert(fs.global == frame1);

    // Pop
    tFrame *poppedFrame = pop_frame(&fs);
    assert(poppedFrame == frame2);
    free(poppedFrame);

    assert(fs.current == frame1);
    poppedFrame = pop_frame(&fs);
    free(poppedFrame);

    assert(fs.current == NULL);
    assert(fs.global == NULL);

    printf("Testy frame stack OK!\n");
}

void test_dlr() {
    tSymTabNode *root = NULL;
    bool heightChange = false;

    tSymTabNode *node1 = create_var_node(false);
    node1->id = str_duplicate("c");
    root = insert_avl(root, node1, &heightChange);

    tSymTabNode *node2 = create_var_node(false);
    node2->id = str_duplicate("a");
    root = insert_avl(root, node2, &heightChange);

    tSymTabNode *node3 = create_var_node(false);
    node3->id = str_duplicate("b");
    root = insert_avl(root, node3, &heightChange);

    assert(strcmp(root->id, "b") == 0);
    assert(strcmp(root->left->id, "a") == 0);
    assert(strcmp(root->right->id, "c") == 0);
    assert(root->height == 0);
    assert(root->right->height == 0);

    printf("Testy dlr OK!\n");
}

void test_drl() {
    tSymTabNode *root = NULL;
    bool heightChange = false;

    tSymTabNode *node1 = create_var_node(false);
    node1->id = str_duplicate("a");
    root = insert_avl(root, node1, &heightChange);

    tSymTabNode *node2 = create_var_node(false);
    node2->id = str_duplicate("c");
    root = insert_avl(root, node2, &heightChange);

    tSymTabNode *node3 = create_var_node(false);
    node3->id = str_duplicate("b");
    root = insert_avl(root, node3, &heightChange);

    assert(strcmp(root->id, "b") == 0);
    assert(strcmp(root->left->id, "a") == 0);
    assert(strcmp(root->right->id, "c") == 0);
    assert(root->height == 0);
    assert(root->right->height == 0);

    printf("Testy drl OK!\n");
}

void test_LL_more_nodes() {
    tSymTabNode *root = NULL;
    bool heightChange = false;

    tSymTabNode *node1 = create_var_node(false);
    node1->id = str_duplicate("e");
    tSymTabNode *node2 = create_var_node(false);
    node2->id = str_duplicate("f");
    tSymTabNode *node3 = create_var_node(false);
    node3->id = str_duplicate("c");
    tSymTabNode *node4 = create_var_node(false);
    node4->id = str_duplicate("d");
    tSymTabNode *node5 = create_var_node(false);
    node5->id = str_duplicate("b");
    tSymTabNode *node6 = create_var_node(false);
    node6->id = str_duplicate("a");

    root = insert_avl(root, node1, &heightChange);
    root = insert_avl(root, node2, &heightChange);
    root = insert_avl(root, node3, &heightChange);
    root = insert_avl(root, node4, &heightChange);
    root = insert_avl(root, node5, &heightChange);
    root = insert_avl(root, node6, &heightChange);

    assert(strcmp(root->id, "c") == 0);
    assert(root->height == 0);
    assert(strcmp(root->right->id, "e") == 0);
    assert(root->right->height == 0);
    assert(strcmp(root->right->left->id, "d") == 0);

    printf("Testy ll_more_nodes OK!\n");
}

void test_dlr_more_nodes() {
    tSymTabNode *root = NULL;
    bool heightChange = false;

    tSymTabNode *node1 = create_var_node(false);
    node1->id = str_duplicate("e");
    tSymTabNode *node2 = create_var_node(false);
    node2->id = str_duplicate("f");
    tSymTabNode *node3 = create_var_node(false);
    node3->id = str_duplicate("b");
    tSymTabNode *node4 = create_var_node(false);
    node4->id = str_duplicate("a");
    tSymTabNode *node5 = create_var_node(false);
    node5->id = str_duplicate("c");
    tSymTabNode *node6 = create_var_node(false);
    node6->id = str_duplicate("d");

    root = insert_avl(root, node1, &heightChange);
    root = insert_avl(root, node2, &heightChange);
    root = insert_avl(root, node3, &heightChange);
    root = insert_avl(root, node4, &heightChange);
    root = insert_avl(root, node5, &heightChange);
    assert(strcmp(root->left->right->id, "c") == 0);
    root = insert_avl(root, node6, &heightChange);

    assert(strcmp(root->id, "c") == 0);
    assert(root->height == 0);
    assert(strcmp(root->right->id, "e") == 0);
    assert(root->right->height == 0);
    assert(strcmp(root->right->left->id, "d") == 0);

    printf("Testy dlr_more_nodes OK!\n");
}

int main() {
    test_RR_rot();
    test_search_symbol();
    test_LL_rot();
    test_frame_stack();
    test_drl();
    test_dlr();
    test_LL_more_nodes();
    test_dlr_more_nodes();
    return 0;
}