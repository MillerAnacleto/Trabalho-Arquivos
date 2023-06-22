
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <aux.h>
#include <header.h>
#include <index.h>
#include <input_output.h>
#include <structs.h>

#define sup(A, B) A > B ? 1 : 0
#define NO_OFFSET -10

int main(int argc, char** argv){
    
    FILE* index_file_old = fopen("bin/indice1.bin", "r+b");
    
    char rewrite = 0;
    char printsert = 0;
    char show = 1;

    int kkey = 890;
    int ooffset = 800;

    if(rewrite){
        Index_Header* wh = indexHeaderCreate();
        indexHeaderSetNumKeys(wh, 0);
        indexHeaderSetNumLevels(wh, 1);
        indexHeaderSetRnnNextNode(wh, 1);
        indexHeaderSetRootNode(wh, 0);
        indexHeaderSetStatus(wh, '1');

        indexHeaderWrite(index_file_old, wh, -1);

        Index_BTPage* root = indexBTPageCreate();

        indexBTPageSetNodeLevel(root, 1);
        indexBTPageSetNumKeys(root, 0);
        indexBTPageSetRrn(root, 0);

        indexBTPageWrite(index_file_old, root, 0);
    }

    //FILE* index_file_old = fopen("bin/indice1.bin", "r+b");
    
   

    if(show){
        for(int i = 0; i <= 463; i++){

            Index_BTPage* idx = indexBTPageRead(index_file_old, i);
            indexBTPagePrint(idx);
        }
    }

    if(1){
        fseek(index_file_old, 0, SEEK_SET);
        Index_Header* header = indexHeaderRead(index_file_old);
        Index_BTPage* node = indexBTPageRead(index_file_old, 0);
        

        printf("status: %c\n", indexHeaderGetStatus(header));
        printf("root_node: %d\n", indexHeaderGetRootNode(header));
        printf("RRN_next_node: %d\n", indexHeaderGetRnnNextNode(header));
        printf("num_keys: %d\n", indexHeaderGetNumKeys(header));
        printf("num_levels: %d\n", indexHeaderGetNumLevels(header));
        printf("----------------//----------------\n");

        node = indexBTPageRead(index_file_old, 0);

        indexBTPagePrint(node);
    }
    // int64_t pointer = -1;
    // Index_BTPage* notonode = indexBTPageRead(index_file_old, 301);
    // //indexBTPagePrint(notonode);
    // //pointer = indexKeyInsert(index_file_old, header, 1534, 373737);
    // //pointer = indexBTSearch(index_file_old, indexHeaderGetRootNode(header), 1532);

    // Index_BTPage* nnode = indexBTPageCreate();
    // for(int i = 0; i < BT_KEY_NUM; i++){
    //     int m = 10 + 50*i;
    //     int offset = 1500 + m;
    //     indexBTPageSetKeys(nnode, m, offset, i);
    // }
    // for(int i = 0; i < BT_ORDER; i++ ){
    //     int m = 40 - i;
    //     indexBTPageSetPointers(nnode, m, i);
    // }
    // indexBTPageSetNodeLevel(nnode, 5);
    // indexBTPageSetNumKeys(nnode, 4);
    // indexBTPageSetRrn(nnode, 1552);

    // Index_BTPage* new_node = indexBTPageCreate();

    // for(int i = 0; i < BT_KEY_NUM; i++){
    //     int m = 20 + 5*i;
    //     int offset = 2500 + m;
    //     indexBTPageSetKeys(new_node, m, offset, i);
    // }
    
    // for(int i = 0; i < BT_ORDER; i++ ){
    //     int m = 10 - i;
    //     indexBTPageSetPointers(new_node, m, i);
    // }
    
    // indexBTPageSetNodeLevel(new_node, 5);
    // indexBTPageSetNumKeys(new_node, 4);
    // indexBTPageSetRrn(new_node, 890);


    // Index_BTPage* nnew_node = indexBTPageCreate();

    // for(int i = 0; i < BT_KEY_NUM; i++){
    //     int m = 62 + 4*i;
    //     int offset = 100 + m;
    //     indexBTPageSetKeys(nnew_node, m, offset, i);
    // }
    
    // for(int i = 0; i < BT_ORDER; i++ ){
    //     int m = 100 - 10*i;
    //     indexBTPageSetPointers(nnew_node, m, i);
    // }
    
    // indexBTPageSetNodeLevel(nnew_node, 90);
    // indexBTPageSetNumKeys(nnew_node, 4);
    // indexBTPageSetRrn(nnew_node, 131313);

    // Index_Key* idxx_key = indexKeyCreate();
    // indexKeySetId(idxx_key, 200);
    // indexKeySetOffset(idxx_key, 2515);


    // indexBTPagePrint(nnode);
    // indexBTPagePrint(new_node);
    // indexBTPagePrint(nnew_node);

    // int idxx_key_ptr = 750;

    // printf("idxx_key.id = %d\n", indexKeyGetId(idxx_key));
    // //twoToThreeSplit(index_file_old, header, nnode, new_node, nnew_node, &idxx_key);
    // //oneToTwoSplit(index_file_old, header, nnode, new_node, &idxx_key, &idxx_key_ptr);
    // rootSplit(index_file_old, header, nnode, &idxx_key, &idxx_key_ptr);

    // if(idxx_key != NULL) printf("\nidxx_key.id = %d\n", indexKeyGetId(idxx_key));
    // else printf("NUL0\n");

    // printf("%ld\n", pointer);

    // indexBTPageDestroy(node);
    // // fclose(index_file);
    // fclose(index_file_old);
    // indexHeaderDestroy(header);

    fclose(index_file_old);

}


//                                                        node
//    [1524]  [1525]  [1526]    | [1527] |    [1528]  [1529]  [1530]  [1531]    | [1532] |    [1533]  [1534]  [1535]    
// [-1]    [-1]    [-1]    [-1]            [-1]    [-1]    [-1]    [-1]    [-1]            [-1]    [-1]    [-1]    [-1]

//vai que precisa ( espero que não o.O )

// while(rrn != -1){
    //     indexBTPageDestroy(parent_node);
    //     parent_node = node;
    //     node = indexBTPageRead(index_file, rrn);

    //     if(indexBTPageInnerSearch(node, new_id_key, &key_offset, &rrn)){
    //         indexBTPageDestroy(parent_node);
    //         indexBTPageDestroy(node);
    //         indexKeyDestroy(new_key);
    //         return FALSE;
    //     }
    // }

    // if(indexBTPageGetNumKeys(node) < BT_KEY_NUM){
    //     printf("tem espaco\n");
        
    //     indexInnerKeyInsert(node, new_key);
    // }
    // else{

    //     bool redistribution = FALSE;
    //     int right_sib_rrn = indexNodeRightSibling(parent_node, node);
    //     int left_sib_rrn = indexNodeLeftSibling(parent_node, node);

    //     if(left_sib_rrn >= 0 && !redistribution){
    //         redistribution = leftSiblingRedistribuition(index_file, node, parent_node, left_sib_rrn, new_key);
    //         //printf("red = %d\n", redistribution);
    //     }
    //     if(right_sib_rrn >= 0 && !redistribution){
    //         redistribution = rightSiblingRedistribuition(index_file, node, parent_node, right_sib_rrn, new_key);
    //     }
        
    //     if(!redistribution){
    //         if(parent_node == NULL){
    //             //é raiz? sim-> split 1:2
    //         }
    //         else{
    //             //não? -> split 2:3
    //         }
    //     }
    // }
    
    // indexBTPageDestroy(node);
    // indexBTPageDestroy(parent_node);

    // return TRUE;