(PASTE "test_api.s")

MOV esp .end
MOV eax [.COLOR_MAGENTA] MOV ebx [.COLOR_YELLOW]
CALL .set_color
MOV eax .string
CALL .print_to_end
HLT

.string: DB "hello"
