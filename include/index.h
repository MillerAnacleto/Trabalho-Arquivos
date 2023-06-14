#ifndef INDEX_H_
#define INDEX_H_

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h> 
#include <aux.h>
#include <data.h>
#include <header.h>
#include <input_output.h>
#include <structs.h>
#include <math.h>

#define NODE_SIZE 76
#define BT_ORDER 5
#define BT_KEY_NUM (BT_ORDER - 1)
#define NO_OFFSET -10


void indexBTreeCreate(FILE* index_file);

Index_BTPage* indexBTPageRead(FILE* node_file, int RRN);
int indexBTPageWrite(FILE* index_file, Index_BTPage* index_bt_page, int rrn);

bool indexBTPageInnerSearch(Index_BTPage* node, int search_key, int64_t* key_offset, int* rrn);
int64_t indexBTPageSearch(FILE* index_file, int root_rrn, int search_key);


Index_BTPage* indexBTPageRead(FILE* node_file, int RRN);

Index_Header* indexKeyInsert(FILE* index_file, int new_id_key, int new_key_offset);

bool indexKeyRecursiveInsert(FILE* index_file, Index_Header* header, Index_Key** new_key, 
    Index_BTPage* node, Index_BTPage* parent, bool* promotion, int rrn, int* inserted_point);

bool twoToThreeSplit(FILE* index_file, Index_Header* header, Index_BTPage* parent, 
    Index_BTPage* left_node, Index_BTPage* right_node, Index_Key** new_key, int* inserted_point, bool promotion);

bool oneToTwoSplit(FILE* index_file, Index_Header* header, Index_BTPage* parent, 
    Index_BTPage* node, Index_Key** new_key, int* inserted_point, bool promotion);

bool rootSplit(FILE* index_file, Index_Header* header, Index_BTPage* node, 
    Index_Key** new_key, int* inserted_point, bool promotion);

void indexInnerKeyInsert(FILE* index_file, Index_BTPage* node, Index_Key* inserted, int* inserted_point);

bool leftSiblingRedistribuition(FILE* index_file, Index_BTPage* node, Index_BTPage* parent_node, int sibling_rrn, 
    Index_Key* inserted_key, int* inserted_point);

bool rightSiblingRedistribuition(FILE* index_file, Index_BTPage* node, 
    Index_BTPage* parent_node, int right_sib_rrn, Index_Key* inserted_key, int* inserted_point);

void arrayShiftInsert(Index_Key** keys, int* point, int count, int insert_pos, 
    Index_Key* insert_keys, int insert_point);


int bbin(Index_Key** keys, int beg, int end, int inserted);
Index_BTPage* indexBTPageRead(FILE* node_file, int RRN);
int indexBTPageWrite(FILE* index_file, Index_BTPage* node, int rrn);
bool indexBTPageInnerSearch(Index_BTPage* node, int search_key, int64_t* key_offset, int* rrn);
int64_t indexBTPageSearch(FILE* index_file, int root_rrn, int search_key);
int indexNodeRightSibling(Index_BTPage* parent_node, Index_BTPage* node);
int indexNodeLeftSibling(Index_BTPage* parent_node, Index_BTPage* node);

//debug
void indexBTPagePrintByRrn(FILE* node_file, int RRN);
void indexBTPagePrint(Index_BTPage* node);


#endif