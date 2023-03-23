# MoundVM Assembler

## Basic example
Also, sorry for the shitty syntax highlighting, markdown does not like assembly very much.
```arm
-org f1 0000 ; this part does NOT work yet
sqrt:
    pop r0
    mul r0 r0
    push r0
    ret

-org start ; this also doesnt work but it would takes you to the default program memory address (f0 0000)

mov #0E r0
push r0
call sqrt
pop r0
```

## How it works
There are two sections
- A main section (bank `f0-f9`)
- A function section (bank `fa-ff`)