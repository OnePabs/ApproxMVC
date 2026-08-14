uint32_t is_node_in_cover(uint64_t node_id, uint32_t* cover_boolean_int_arr_ptr);
void add_node_to_cover(uint64_t node_id, uint32_t* cover_boolean_int_arr_ptr);
uint32_t* create_cover_boolean_arr(uint64_t num_nodes);
void print_nodes_in_cover(uint64_t num_nodes, uint32_t* cover_boolean_int_arr_ptr);