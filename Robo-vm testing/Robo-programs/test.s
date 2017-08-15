(PASTE "test_api.s")

MOV eax [.world_width]
MUL .world_height
ADD eax .world_data_start
MOV esp eax
.start: 
    CALL .init_world
    ;CALL .print_world
    HLT

.print_world:       
    MOV eax #0
    .print_world_y_loop:
        MOV ebx #0
        .print_world_x_loop:
            PUSH eax
            CALL .world_coords_to_addr
            MOV eax [eax]
            CALL .print_number_to_end   
            CMP eax ecx
            POP eax
            JE .print_world_cell_is_dead
            MOV ecx [.alive_char]
            JMP .print_world_draw_cell
            .print_world_cell_is_dead: MOV ecx [.dead_char]
            ;.print_world_draw_cell: CALL .print_char_to_loc
            INC ebx
            CMP ebx .world_width
            JNE .print_world_x_loop
        INC eax
        CMP eax .world_height
        JNE .print_world_y_loop
    MOV eax #100
    CALL .print_number_to_end
    ;CALL .draw
    RET

.world_coords_to_addr:
    MUL .world_width
    ADD eax ebx
    ADD eax .world_data_start
    RET

.init_world:    ;put init shape into world data
    MOV eax #0
    MOV ecx #0
    .init_world_y_loop:
        MOV ebx #0
        .init_world_x_loop:
            PUSH eax
            CALL .world_coords_to_addr
            MOV [eax] ecx
            CALL .print_number_to_end MOV eax .space_char CALL .print_to_end
            POP eax
            INC ebx
            CMP ebx .world_width
            JNE .init_world_x_loop
        INC eax
        CMP eax .world_height
        JNE .init_world_y_loop
 
    ;MOV ebx #1
    ;MOV ecx .world_init_shape
    ;MOV edx .world_init_shape
    ;ADD edx .world_init_shape
    ;.init_world_loop:
    ;    INC eax
    ;    MOV eax [ecx]
    ;    INC ecx
    ;    MOV ebx [ecx]
    ;    CALL .world_coords_to_addr
    ;    MOV [eax] ebx
    ;    CMP ecx edx
    ;    JNE .init_world_loop
    ;MOV eax #210
    ;CALL .print_number_to_end MOV eax .space_char CALL .print_to_end
    RET

.dead_char: DB "-"
.alive_char: DB "o"
.space_char: DB " " 
.world_data_start: DB .end
.world_width: DB 20
.world_height: DB 20
.world_init_shape: DB [0,1,1,2,2,0,2,1,2,2] ; initial shape, stored as y,x pairs of cells to start as alive

