#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "../helpers.h"

void pitts(uint64_t* edges_ptr, uint32_t* cover_ptr,  uint64_t num_nodes, uint64_t num_edges){
    printf("Pitt's randomized algorithm\n");

    // seed random number generator
    srand(0);

    // Go through each edge
    for(uint64_t i=0; i<num_edges*2; i=i+2){
        // check if node 1 is covered
        if(!is_node_in_cover(edges_ptr[i],cover_ptr)){
            // node1  is not in cover
            // check if node 2 is in cover 
            if(!is_node_in_cover(edges_ptr[i+1],cover_ptr)){
                // node 2 is also not in the cover. 
                // add one node at random to the cover
                int choice = rand() % 2;
                printf("choice: %d\n",choice);
                add_node_to_cover(edges_ptr[i + choice], cover_ptr);
            }
        }
    }
}