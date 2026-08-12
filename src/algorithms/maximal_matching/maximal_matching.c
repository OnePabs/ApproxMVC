#include <stdio.h>
#include <stdint.h>
#include <sys/mman.h>
#include <stdbool.h>

int is_node_in_cover(long node_id, int* cover_boolean_int_arr_ptr){
    long offset_in_array = node_id >> 5;
    long offset_in_byte = node_id & 0x31UL;
    return cover_boolean_int_arr_ptr[offset_in_array] & (1UL << offset_in_byte);

}

void add_node_to_cover(long node_id, int* cover_boolean_int_arr_ptr){
    long offset_in_array = node_id >> 5;
    long offset_in_byte = node_id & 0x31UL;
    cover_boolean_int_arr_ptr[offset_in_array] = cover_boolean_int_arr_ptr[offset_in_array] | (1UL << offset_in_byte);
}



void maximal_matching(uint64_t* edges_ptr, long num_nodes, long num_edges){
    printf("Maximal Matching algorithm\n");

    // create memory mapping for an "array" of 1 bit booleans. 
    // Each entry is 1 bit.
    // Entry at (base_address + i bits) is
    //      0 if node i is not in the cover
    //      1 if node i is in the cover
    // Since we cannot access individual bits, 
    // we will access the boolean array values in sets of 32, 
    // represented by an integer
    size_t num_int_entries_needed = (num_nodes+31) >> 5; // take the ceiling of dividing num_nodes by 8
    size_t bytes_needed = num_int_entries_needed*4;

    // create the array of booleans 
    // By setting MAP_ANONYMOUS its contents are initialized to zero.
    void* cover_boolean_arr_ptr = mmap(NULL,bytes_needed, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0); 

    // Convert to int array to access in chuncks of 32
    int* cover_boolean_int_arr_ptr = (int*)cover_boolean_arr_ptr;


    // Go through each edge (node1,node2)

    for(long i=0; i<num_edges*2; i=i+2){

        // check if node 1 is covered
        if(!is_node_in_cover(edges_ptr[i],cover_boolean_int_arr_ptr)){
            // node1  is not in cover
            // check if node 2 is in cover 
            if(!is_node_in_cover(edges_ptr[i+1],cover_boolean_int_arr_ptr)){
                // node 2 is also not in the cover. 
                // add both nodes to the cover
                add_node_to_cover(edges_ptr[i],cover_boolean_int_arr_ptr);
                add_node_to_cover(edges_ptr[i+1],cover_boolean_int_arr_ptr);
            }
        }
        
        printf("edge %lu = (%lu,%lu)\n",edge_num, edges_ptr[i],  edges_ptr[i+1]);
    }


}