.draw:
    PUSH eax
    PUSH ebx

    MOV eax [.MONITOR_ADDRESS]
    DEC eax
    MOV ebx #1
    MOV [eax] ebx

    POP ebx
    POP eax 
    RET

.print:          ; print([eax] to ebx)
    PUSH ecx
    PUSH edx
    MOV edx #0
    .loop:
        MOV ecx [eax]  ; Get char from var
        MOV [ebx] ecx  ; Write to output
        INC eax
        INC ebx
        CMP edx [eax]  ; Check if end
        JNE .loop   ; jump if not
    MOV .last_print_loc ebx
    CALL .draw
    POP edx
    POP ecx
    RET

.print_char_to_loc:
    PUSH eax
    MUL .COLUMNS
    ADD eax ebx
    ADD eax .MONITOR_ADDRESS
    MOV [eax] ecx
    POP eax
    RET

.print_to_screen:   ; print([eax] to the beginning to the screen)
    PUSH ebx
    MOV ebx [.MONITOR_ADDRESS]
    CALL .print
    POP ebx 
    RET

.print_to_end:   ; print([eax] to end of last print)
    PUSH ebx
    MOV ebx [.last_print_loc]
    CALL .print
    POP ebx
    RET

.print_to_loc:      ; print([ecx] to screen at positition y = eax x = ebx)
    PUSH eax PUSH ebx
    MUL .COLUMNS
    ADD eax ebx
    ADD eax .MONITOR_ADDRESS
    MOV ebx eax
    MOV eax ecx
    CALL .print
    POP ebx POP eax
    RET

.number_to_array:
    PUSH ebx PUSH ecx PUSH edx
    MOV ebx #10
    MOV ecx .num_to_array_array
    MOV edx #0
    MOV [ecx] edx
    MOV edx #4
    ADD ecx edx
    MOV edx #0
    .number_to_array_loop:
        PUSH eax
        MOD ebx
        MOV [ecx] eax
        DEC ecx
        POP eax
        DIV ebx
        CMP eax edx
        JNE .number_to_array_loop
    MOV eax .num_to_array_array
    POP edx POP ecx POP ebx
    RET

.print_number_to_end:    ;prints(eax to end)
    PUSH eax PUSH ebx PUSH ecx PUSH edx
    CALL .number_to_array
    MOV ebx eax
    MOV ecx eax
    MOV edx #5
    ADD ecx edx
    MOV edx #0
    .print_number_to_end_loop:
        MOV eax [ebx]
        CMP edx .print_number_to_end_start_flag
        JE .print_number_to_end_dont_start

        .print_number_to_end_draw_char:
        ADD eax .print_number_to_end_ascii_offset
        MOV .print_number_to_end_char eax
        MOV eax .print_number_to_end_char
        CALL .print_to_end
        JMP .print_number_to_end_continue_loop

        .print_number_to_end_dont_start:
        CMP eax edx
        JE .print_number_to_end_continue_loop
        INC edx
        MOV .print_number_to_end_start_flag edx
        DEC edx
        JMP .print_number_to_end_draw_char

        .print_number_to_end_continue_loop: 
        INC ebx       
        CMP ebx ecx
        JNE .print_number_to_end_loop
    MOV .print_number_to_end_start_flag edx
    POP edx POP ecx POP ebx POP eax
    RET
    .print_number_to_end_char: DB "a"
    .print_number_to_end_ascii_offset: DB 48
    .print_number_to_end_start_flag: DB 0


.timer:            ;does eax*.timer_multiplier subtractions
    MUL .timer_multiplier
    .timer_loop: 
        CALL .draw
        DEC eax
        JNE .timer_loop
    RET

.wait_for_key:  ;waits until a key is pressed then eax = key
    PUSH ebx
    PUSH ecx
    MOV ebx #0
    MOV ecx .KEY_ADDRESS
    .key_wait_loop:
        MOV eax [ecx]
        CMP eax ebx
        JE .key_wait_loop
    POP ecx
    POP ebx
    RET

.timer_multiplier: DB 60000
.last_print_loc: DB 61952
.num_to_array_array: DB [4]

.KEY_q: DB 71

.MEM_SIZE: DB 64000
.ROWS: DB 32
.COLUMNS: DB 64
.MONITOR_ADDRESS: DB 61952
.KEY_ADDRESS: DB 61950
