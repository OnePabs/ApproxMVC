#include <stdio.h>
#include <stdint.h>
#include "../helpers.h"


void maximal_matching(uint64_t* edges_ptr, uint32_t* cover_ptr, uint64_t num_nodes, uint64_t num_edges){
    printf("Maximal Matching algorithm\n");

    // create cover boolan array
    //uint32_t* cover_boolean_int_arr_ptr = create_cover_boolean_arr(num_nodes);

    // Go through each edge
    for(uint64_t i=0; i<num_edges*2; i=i+2){
        // check if node 1 is covered
        if(!is_node_in_cover(edges_ptr[i],cover_ptr)){
            // node1  is not in cover
            // check if node 2 is in cover 
            if(!is_node_in_cover(edges_ptr[i+1],cover_ptr)){
                // node 2 is also not in the cover. 
                // add both nodes to the cover
                add_node_to_cover(edges_ptr[i],cover_ptr);
                add_node_to_cover(edges_ptr[i+1],cover_ptr);
            }
        }
    }

    // print the nodes in the cover
    //print_nodes_in_cover(num_nodes,cover_boolean_int_arr_ptr);

    
}