# MoundVM Specifications

- 255 32-bit registers

- `0xFFFF` 32-bit spaces in the stack

- `0xFF` Banks of `0xFFFF` bytes of memory

## Memory Layout
### Adressing format: Bank Address (ex. `f0` `0001`)
`00`-`df` `0000`-`ffff`: General purpose memory to store things in

`e0`-`ef` `0000`-`ffff`: Video memory (WIP)

`f0`-`ff` `0000`-`ffff`: Program memory