int binarySearchDeleteIndexArray(FILE* index_file, FILE* binary_file, Index_Data_t** array,  
    int parameter_num, int parameter_index){

    int found = 0;
    
    Index_Header_t* index_header = indexHeaderRead(index_file);
    int size = indexHeaderGetNum(index_header);
    int unique_node_num = 0;
    int index_pos = 0;

    Index_Node_t** node_array = indexArrayCreate(size);
    int param = indexDataGetParam(array[parameter_index]);
    if(param <= 1){
        dataIndexArrayIntRead(index_file, node_array, size, &unique_node_num);
        int field_val = indexDataGetIntKey(array[parameter_index]);
        index_pos = binarySearchIndexInt(node_array, 0, unique_node_num, field_val);
    }
    else{
        dataIndexArrayStrRead(index_file, node_array, size, &unique_node_num);
        char* field_val = indexDataGetStrKey(array[parameter_index]);
        index_pos = binarySearchIndexStr(node_array, 0, unique_node_num, field_val);
    }

    if(index_pos == -1){
        
        indexArrayDestroy(node_array, size, unique_node_num);
        free(index_header);
        return 0;
    }

    Index_Node_t* n = node_array[index_pos];

    found = nodeListCompareDelete(n, array, binary_file, parameter_num);
    printf("found: %d\n", found);

    indexHeaderSetNum(index_header, indexHeaderGetNum(index_header) + found);

    indexArrayDestroy(node_array, size, unique_node_num);
    free(index_header);

    return found;
}

int nodeListCompareDelete(Index_Node_t* node, Index_Data_t** array,
    FILE* binary_file, int parameter_num){
    int found = 0;
    
    while(node != NULL){
        int64_t offset = indexDataGetOffset(indexNodeGetData(node));
        fseek(binary_file, offset, SEEK_SET);
        Bin_Data_t* bin_data = dataBinaryRead(binary_file);
        dataPrintCsvStyle(bin_data);
        putchar('\n');
        printf("compare: %d\n", dataParamCompare(bin_data, array, parameter_num));
    
        if(dataParamCompare(bin_data, array, parameter_num)){

            if(dataGetRemoved(bin_data) == '1'){
                dataDestroy(bin_data);            
                node = indexNodeGetNext(node);
                continue;
            }

            found++;

            int64_t str_size = varStrSize(bin_data);
            int64_t data_offset =  str_size + DATA_BASE_SIZE;
            dataPrintCsvStyle(bin_data);
            putchar('\n');
            dataMarkDeleted(binary_file, data_offset);
        }
        dataDestroy(bin_data);
        node = indexNodeGetNext(node);
    }

    return found;
}