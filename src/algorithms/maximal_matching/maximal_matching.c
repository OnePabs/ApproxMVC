#include <stdio.h>
#include <stdint.h>
#include "../helpers.h"


void maximal_matching(uint64_t* edges_ptr, long num_nodes, long num_edges){
    printf("Maximal Matching algorithm\n");

    // create cover boolan array
    int* cover_boolean_int_arr_ptr = create_cover_boolean_arr(num_nodes);

    // Go through each edge
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
    }

    // print the nodes in the cover
    print_nodes_in_cover(num_nodes,cover_boolean_int_arr_ptr);

}