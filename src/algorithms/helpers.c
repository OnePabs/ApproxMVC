#include <stdio.h>
#include <sys/mman.h>
#include <stdint.h>


/////////////////////////////////////
// Function helpers for algorithms //
/////////////////////////////////////

uint64_t get_offset_in_array(uint64_t node_id){
    return node_id >> 5;
}

uint64_t get_offset_in_int(uint64_t node_id){
    return node_id - ((node_id >> 5)<<5); // remainder r of node_id = m(2^5) + r
}

uint32_t is_node_in_cover(uint64_t node_id, uint32_t* cover_boolean_int_arr_ptr){
    //printf("is_node_in_cover %lu\n",node_id);
    uint64_t offset_in_array = get_offset_in_array(node_id);
    //printf("offset_in_array %lu\n",offset_in_array);
    uint64_t offset_in_int = get_offset_in_int(node_id);
    //printf("offset_in_int %lu\n",offset_in_int);
    uint32_t is_it = cover_boolean_int_arr_ptr[offset_in_array] & (INT32_C(1) << offset_in_int);
    return is_it; // 0 if false, otherwise true (follows convention)
}

void add_node_to_cover(uint64_t node_id, uint32_t* cover_boolean_int_arr_ptr){
    //printf("add_node_to_cover %lu\n",node_id);
    uint64_t offset_in_array = get_offset_in_array(node_id);
    //printf("offset_in_array %lu\n",offset_in_array);
    uint64_t offset_in_int = get_offset_in_int(node_id);
    //printf("offset_in_int %lu\n",offset_in_int);
    cover_boolean_int_arr_ptr[offset_in_array] = cover_boolean_int_arr_ptr[offset_in_array] | (INT32_C(1) << offset_in_int);
}

uint64_t calc_num_int_entries_needed(uint64_t num_nodes){
    return (num_nodes+31) >> 5; // take the ceiling of dividing num_nodes by 32
}

uint32_t* create_cover_boolean_arr(uint64_t num_nodes){
    // create memory mapping for an "array" of 1 bit booleans. 
    // Each entry is 1 bit.
    // Entry at (base_address + i bits) is
    //      0 if node i is not in the cover
    //      1 if node i is in the cover
    // Since we cannot access individual bits, 
    // we will access the boolean array values in sets of 32, 
    // represented by an integer
    uint64_t num_int_entries_needed = calc_num_int_entries_needed(num_nodes);
    //printf("num_int_entries_needed %lu\n",num_int_entries_needed);
    uint64_t bytes_needed = num_int_entries_needed*4;

    // create the array of booleans 
    // By setting MAP_ANONYMOUS its contents are initialized to zero.
    void* cover_boolean_arr_ptr = mmap(NULL,bytes_needed, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0); 

    // Convert to int array to access in chuncks of 32
    uint32_t* cover_boolean_int_arr_ptr = (uint32_t*)cover_boolean_arr_ptr;
    return cover_boolean_int_arr_ptr;
}


void print_nodes_in_cover(uint64_t num_nodes, uint32_t* cover_boolean_int_arr_ptr){
    // print the nodes in the cover
    printf("\nPrinting nodes in cover...\n");
    uint32_t is_in_cover = 0;
    uint64_t node_id = 0;
    for(uint64_t i=0;i<calc_num_int_entries_needed(num_nodes);i++){
        //printf("i: %lu\n",i);
        uint64_t num_id_base = i*32;
        //printf("num_id_base %lu\n", num_id_base);
        uint64_t nodes_remaining = num_nodes - num_id_base;
        //printf("nodes_remaining %lu\n", nodes_remaining);
        uint64_t max_offset = (nodes_remaining >= 32) ? 31:nodes_remaining-1;
        //printf("max_offset %lu\n", max_offset);
        for(uint64_t offset_in_int = 0; offset_in_int <= max_offset; offset_in_int++){
            //printf("offset_in_int %d\n", offset_in_int);
            node_id = num_id_base + offset_in_int; 
            //printf("node_id %lu\n", node_id);
            is_in_cover = (cover_boolean_int_arr_ptr[i] >> offset_in_int) & INT32_C(1);
            if(is_in_cover){
                // node is in cover
                printf("node %lu is in cover \n",node_id);
            }else{
                // node is not in cover
                printf("node %lu is NOT in cover \n",node_id);
            }
        }
    }
}
