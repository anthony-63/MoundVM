# MoundVM Assembler

## Basic example
```
func sqrt {
    pop r0
    mul r0 r0
    push r0
    ret
}

mov #0E r0
push r0
call sqrt
```

## How it works
There are two sections
- A main section (bank `f0-f9`)
- A function section (bank `fa-ff`)