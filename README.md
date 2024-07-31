# CHIP-8 virtual machine emulator

## Overview

CHIP-8 is an interpreted programming language (and, therefore, also an interpreter of itself) originally designed by Joseph Weisbecker to develop programs (in this case, video games) for the COSMAC VIP microcomputer - which mounted an RCA 1802 microprocessor, also known as COSMAC.

The programming language is used to write the programs (erroneously - because they are expected not to be overwritten, while some CHIP-8 programs are - called ROMs) that the interpreter will later execute. These programs were commonly stored on tapes or cartridges - which were later loaded into the microcomputer's RAM memory - and, in the case of the COSMAC VIP microcomputer, could also be coded directly.

The interpreter (also called virtual machine) is a program specialized in executing programs coded in the CHIP-8 programming language. In the original microcomputers, the interpreter had to be coded for the instruction set of the corresponding microprocessor and it was stored in ROM memory and later loaded into RAM memory to be executed by the microprocessor. It is also called virtual machine because it is not only in charge of interpreting program instructions but also recreates a complete machine (in this case purely virtual; it is a mere concept, physically non-existent): processor (instruction cycle, program counter, index register, general purpose registers, timers), memory (RAM, call stack), etc.

[!IMPORTANT]
The microcomputer executes the interpreter (written with the corresponding microprocessor instruction set) and the interpreter executes the ROM (written with the CHIP-8 instruction set).

[!IMPORTANT]
The role of the CHIP-8 interpreter consists of translating the instructions written in the CHIP-8 programming language into instructions that can be processed by the corresponding microprocessor. Thus, an instruction in the CHIP-8 programming language could have as many interpretations as there are different instruction sets for the different microprocessors on which it can be executed. This, together with the fact that different microprocessors (and their corresponding microcomputers) have different architectures, instruction execution times are not unique (although an average value per instruction could be estimated to some extent).

## Versions

Extensions:

-   CHIP-10 and Hi-Res CHIP-8: higher resolution.
-   CHIP-8C and CHIP-8X: (limited) color.

Improved versions:

-   CHIP-48 (1990; by Andreas Gustafsson).
-   SUPER-CHIP (a.k.a. SCHIP / S-CHIP; 1990; by Erik Bryntse).

## Hardware (host)

### COSMAC VIP (original)

-   Microcomputer
-   RCA - Joseph Weisbecker
-   1977, USA
-   Microprocessor: RCA 1802 (a.k.a. COSMAC), 8-bit (1 B) word

### Telmac 1800

-   Microcomputer
-   Telercas Oy
-   1977, Finland
-   Microprocessor: RCA 1802.

### DREAM 6800

-   Hobby computer
-   Michael Bauer
-   1978, Australia
-   Microprocessor: Motorola (MC)6800, 8-bit word

## Virtual machine

[!IMPORTANT]
Following specifications are about the requirements of the interpreter (although it is certainly very much conditioned by the original hardware for which it was designed).
Thus, the performance of host microcomputers could be superior, although in this type of dedicated systems it would not make any sense (i.e. resources would be wasted).

[!NOTE]
The hexadecimal number system notation supports the forms `0xValue` and `$Value`

[!NOTE]
As the total memory range (4096) is represented by a 4 digit number in the hexadecimal number system ($1000) the following numerical data is expressed with 4 digits to maximize readability (e.g. $0200) although it is not really necessary (i.e. left zeros have no weight).

### RAM Memory

Size: 4 kB (i.e. 4096 ($1000) B).

[!NOTE]
`4 kB == 4 * 2**10 B == 4 * 1024 B == 4096 B` or
`4 kB == 4 * 2**10 B == 2**2 * 2**10 B == 2**12 B == 4096 B`

Memory range: $0000 (0) - $x0FFF (4095)

```
$0000 (0)    - $0FFF (4095) : 4096 ($1000) B : Total
$0000 (0)    - $01FF (511)  : 512 ($0200) B  : Interpreter (+ Fonts)
$0200 (512)  - $0E9F (3743) : 3232 ($0CA0) B : Program
$0EA0 (3744) - $0EFF (3839) : 96 ($0060) B   : Call stack, internal use.
$0F00 (3840) - $0FFF (4095) : 256 ($00FF) B  : Framebuffer
```

#### Interpreter

First 512 ($0200) B are reserved for the CHIP-8 interpreter program.

Memory range: $0000 (0) - $01FF (511)

[!NOTE]
In the original microcomputers the CHIP-8 interpreter was loaded into this memory range. In modern high-level implementations (i.e. any implementation that is not going to be stored and executed on a miniaturized computer) this is not necessary and this space is left free. Font data is commonly stored here in the range $0050 (80) - $009F (159) i.e. 80 ($0050) B

#### Program

It starts on memory location 0x0200 (512), with an available space of 3232 ($0CA0) B - it does not access any memory under this location.

Memory range: $0200 (512) - $0E9F (3743)

[!NOTE]
In the original implementations the program does not have the rest of the memory space as it might seem a priori; other resources - such as the call stack or the framebuffer - are stored in the higher memory spaces. In modern implementations all these resources are normally managed outside the virtualized RAM, so this constraint would not be given (in any case it would depend on the design chosen by the developer).

#### Other

Uppermost 256 ($00FF) B reserved for the framebuffer.

Memory range: $0F00 (3840) - $0FFF (4095)

[!TIP]
Why 256 B? Because MONOCHROME (true/false value per px) display resolution is 64x32 px i.e. 2048 px, which can be tightly represented with 256 B (8 px encoded per byte)

[!WARNING]
Not the best implementation approach (besides complexity); use a separate array of 2048 (64x32) elements.

Below 96 ($0060) B reserved for the call stack, internal use and other variables.

Memory range: $0EA0 (3744) - $0EFF (3839)

[!WARNING]
Not the best implementation approach (besides complexity); use a separate array of n (number of maximum nesting levels) elements.

### Registers

General purpose register `V`:

-   x16 (V0 - VF)
-   Size: 8-bit (1 B).

VF special use as flag register:

-   Add operation: carry flag.
-   Subtract operation: no-borrow flag.
-   Drawing operation: pixel (px) collision flag.

[!WARNING]
Avoid writing to it from programs.

Address (or index) register (to point at memory locations) `I`:

-   x1
-   SIze: 12-bit.

[!TIP]
Why 12-bit? Because the memory size is 4KB (i.e. 4096 B or, in other words, `2**12` B).
When running on a microprocessor with 8-bit (1 B) words, the index register will use 2 words (i.e. a total of 16-bit - 2 B), so it will be partially wasted.

### Call stack

Store return addresses (i.e. PC value) when subroutines are called; LIFO (Last In First Out) architecture:

-   Nesting levels: x12.
-   Size (total): 48 B (16-bit - 2 B - per level)

[!TIP]
Would really only be using the 12 LSB (i.e. 12-bit).

[!WARNING]
In modern implementations may have more nesting levels (e.g. x16 or whatever).

Stack Pointer `SP`: 8-bit (1 B) - point to the topmost level of the stack.

[!TIP]
Stack values do not need to be removed on poping (because it will be overwritten by another process soon).

### Timers

Count down (decremented by 1) at a frequency of 60 Hz (cycles/second) until reach 0.

[!WARNING]
Independent of the CPU instruction cycle execution speed.

Delay timer `DT`: timing events; value can be set (write) and get (read):

-   x1
-   Size: 8-bit (1 B)

Sound timer `ST`: sound effects (play sound while value is nonzero); value can only be set.

-   x1
-   Size: 8-bit (1 B)

[!TIP]
Produced sound has only one tone (beep), whose frequency is chosen by the developer.

### Input

HEX Keyboard: 16 keys (0 - F).

```
1 2 3 C
4 5 6 D
7 8 9 E
A 0 B F
```

Commonly replaced with standard QERTY keyboard layout:

```
1 2 3 4
Q W E R
A S D F
Z X C V
```

### Graphics

Display

-   Resolution: 64x32 (width-height) px
-   Monochrome (i.e. black or white)
-   Originally updated (by the interpreter) at 60Hz (i.e. 60 FPS)

[!WARNING]
In the implementation it would be more convenient to limit the display update to when a drawing command is executed to maximize efficiency.

Sprite

-   Width: 8 px (i.e. each of the bits of corresponding level byte).
-   Height: 1-15 px (i.e. each of the levels - bytes - of the sprite).

Drawing

-   XOR with corresponding screen pixel (i.e. flip corresponding screen pixel if sprite bit is set; otherwise, do nothing).
-   Collision detection: VF is set to 1 if a screen pixel is unset.

### Font

Character sprite: x16 ($0-$F); each 5 B in 8x5 px format.

```
0xF0, 0x90, 0x90, 0x90, 0xF0, // $0
0x20, 0x60, 0x20, 0x20, 0x70, // $1
0xF0, 0x10, 0xF0, 0x80, 0xF0, // $2
0xF0, 0x10, 0xF0, 0x10, 0xF0, // $3
0x90, 0x90, 0xF0, 0x10, 0x10, // $4
0xF0, 0x80, 0xF0, 0x10, 0xF0, // $5
0xF0, 0x80, 0xF0, 0x90, 0xF0, // $6
0xF0, 0x10, 0x20, 0x40, 0x40, // $7
0xF0, 0x90, 0xF0, 0x90, 0xF0, // $8
0xF0, 0x90, 0xF0, 0x10, 0xF0, // $9
0xF0, 0x90, 0xF0, 0x90, 0x90, // $A
0xE0, 0x90, 0xE0, 0x90, 0xE0, // $B
0xF0, 0x80, 0x80, 0x80, 0xF0, // $C
0xE0, 0x90, 0x90, 0x90, 0xE0, // $D
0xF0, 0x80, 0xF0, 0x80, 0xF0, // $E
0xF0, 0x80, 0xF0, 0x80, 0x80  // $F
```

```
  ($0)        ($1)                ($F)
11110000    00100000            11110000
10010000    10100000            10000000
10010000    00100000     ...    11110000
10010000    00100000            10000000
11110000    01110000            10000000
```

[!WARNING]
A program may use or not built-in font, but it must be provided by the implementation anyway.

### Opcode

Program Counter `PC`: point at next opcode memory location.

-   x1
-   Size: 12-bit (2 B)

Opcode

-   Size: 16-bit (2 B)
-   Endianness: big-endian

```
Memory arrangement: ..., B-n ($AB), B-n+1 ($CD), ...
Decode (2 B): N1N2N3N4 == ABCD (where $B-nB-n+1 == $ABCD)
```

[!NOTE]
N2,N3 and N4 meaning depends on instruction type.

Decoding

-   N1: instruction type
-   N2: X (register identifier - VX)
-   N3: Y (register identifier - VY)
-   N4: N (4-bit constant)
-   N3N4: NN (8-bit constant)
-   N2N3N4: NNN (12-bit memory address)

[!TIP]
It is recommended to perform instruction decoding in separate functions so in case there is any mistake there is just a single place where it should be fixed.

[!NOTE]
In the interpreter, RAM memory will be represented with an array; as in real hardware (same concept but on much larger scale), memory locations (i.e. addresses) will correspond to the positions (or the indices) within the array.

Timing: 700 CHIP-8 instructions per second works well for most old programs.

### Instruction

-   x35
-   Size: 16-bit (2 B)

#### Table

-   0NNN: call machine code routine at address NNN (not necessary unless emulating a whole microcomputer - actually its microprocessor)
-   00E0: clear screen
-   00EE: return form subroutine (`PC = Stack.Pop()`)
-   1NNN: jump to address NNN (`PC = NNN`)
-   2NNN: call subroutine at address NNN (`Stack.Push(PC); PC = NNN`)
-   3XNN: skip next instruction (`PC += 2`) if `VX == NN`
-   4XNN: skip next instruction if `VX != NN`
-   5XY0: skip next instruction if `VX == VY`
-   6XNN: `VX = NN`
-   7XNN: `VX += NN` (VF is not affected)
-   8XY0: `VX = VY`
-   8XY1: `VX = (VX OR VY)`
-   8XY2: `VX = (VX AND VY)`
-   8XY3: `VX = (VX XOR VY)`
-   8XY4: `VX += VY` (set VF if overflow; otherwise, unset).
-   8XY5: `VX -= VY` (unset VF if underflow; otherwise set).
-   8XY6: `VX = VX >> 1` (set VF if previous VX LSB was set). [ALTERNATIVE: first `VX = VY`; then, proceed normally]
-   8XY7: `VX = VY - VX` (unset VF if underflow; otherwise set).
-   8XYE: `VX = VX << 1` (set VF if previous VX MSB was set). [ALTERNATIVE: first `VX = VY`; then, proceed normally]
-   9XY0: skip next instruction if `VX != VY`
-   ANNN: `I = NNN`
-   BNNN: jump to address NNN plus V0 (`PC = NNN + V0`) [ALTERNATIVE BXNN: jump to address XNN plus VX `PC = NNN + VX`]
-   CXNN: `VX = (RANDOM(8-bit) AND NN)`
-   DXYN: draw sprite 8xN (start reading at I) at coordinate (VX, VY); set VF if collision; otherwise, unset; starting drawing coordinate must be wrapped; sprite drawing must be clipped against the border [ALTERNATIVE: sprite drawing must be wrapped - and not clipped - against the border]
-   EX9E: skip next instruction if `IsKeyPressed(VX)`
-   EXA1: skip next instruction if `!IsKeyPressed(VX)`
-   FX07: `VX = DT`
-   FX0A: `VX = OnKeyPressed()` (blocking operation - await to key press); timers should keep decremeting if active [ALTERNATIVE: await to key press & release]
-   FX15: `DT = VX`
-   FX18: `ST = VX`
-   FX1E: `I += VX` (VF is not affected) [ALTERNATIVE: set VF if 12-bit overflow; otherwise, unset]
-   FX29: `I = GetFontSprite(VX)` (use low nibble from VX)
-   FX33: store BCD (Binary-Coded Decimal; convert to decimal first) of VX; hundreds at memory location `I`, tens at `I+1`, and ones at `I+2`.
-   FX55: Store range V0-VX (included) in memory; starting at memory location I and offseting by 1 B for each value (I is not affected). [ALTERNATIVE: update I on each step]
-   FX65: Fill range V0-VX (included) from memory; starting at memory location I and offseting by 1 B for each value (I is not affected). [ALTERNATIVE: update I on each step]

## Extra

### Endianness

Determine the order in which a set of bytes is to be processed (e.g. write to/read from memory, transmit/receive data).

-   Big-endian (BE): high (or most significant) byte first.
-   Little-endian (LE): low (or less significant) byte first.

[!WARNING]
Incorrect interpretation of a set of bytes can lead to malfunctioning of the application.

Example: IBM Logo

Memory: 00 E0 A2 2A 60 0C 61 08 D0 1F 70 09 A2 39 D0 1F ...

Big-Endian: it does make sense

```
00E0 - CLEAR SCREEN
A22A - I = $22A
600C - V0 = $0C
6108 - V1 = #08
D01F - DRAW AT (V0,V1) SPRITE 8xF (START AT I)
7009 - V0 += $09
A239 - I = $239
D01F - DRAW AT (V0,V1) SPRITE 8xF (START AT I)
```

Little-Endian: it does not make sense!

```
E000 - UNKNOWN
2AA2 - (2NNN) CALL SUBROUTINE AT $AA2
0C60 - (0NNN) CALL MACHINE CODE AT $C60
0861 - (0NNN) CALL MACHINE CODE AT $861
1FD0 - (1NNN) JUMP TO ADDRESS $FD0
0970 - (0NNN) CALL MACHINE CODE AT $970
39A2 - (3XNN) SKIP NEXT INSTRUCTION IF V9 == $A2
1FD0 - (1NNN) JUMP TO ADDRESS $FD0
```

### DXYN instruction

Difficult because two really complicated tasks:

-   Manage memory cache or buffer (calculate positions, update position, fit outbounds constraints, detect collisions, etc.)
-   Render (knowledge on graphics API (e.g. OpenGL) or higher level libraries/frameworks for rendering (e.g. SDL))

[!WARNING]
Axes orientation on both stages (buffering and drawing) must match; otherwise, mirroring will happen on rendering.

-   Commonly, when graphing X-Y axes people use following convention: +X left to right; +Y down to up.
-   Screen coordinates axes orientation convention is different to this one: +X left to right (same); +Y up to down.

Depending on which utility you will use for drawing from your buffer, it is important to store and manage data accordingly.
In the case of OpenGL, textures are processed as screen coordinates convention: +X left to right; +Y up to down
So, I should manage my data (i.e. how I store and access it in memory) following that convention too.

In this case, the display is 64 px width by 32 px height: 2048 px (64x32)

```
Common convention:

     (+Y)
     ^
[31] |(0,31)                     (63,31)
     |
     |
    ...
     |
     |
     |(0,0)                       (63,0)
 [0] --------------- ... ---------------> (+X)
      [0]                           [63]

     (+Y)
     ^
[31] |(1984)                      (2047)
     |
     |
    ...
     |
     |
     |(0)                           (63)
 [0] --------------- ... ---------------> (+X)
      [0]                           [63]

               [r0 ...           , r1 ...                    ...    , r30                        , r31 ...                     ]
      indics = [0, 1, ..., 62, 63, 64, 65, ..., 126, 127,    ...    , 1920, 1921, ..., 1982, 1983, 1984, 1985, ..., 2046, 2047 ]
      buffer = [0, 1, ..., 62, 63, 64, 65, ..., 126, 127,    ...    , 1920, 1921, ..., 1982, 1983, 1984, 1985, ..., 2046, 2047 ]

Screen coordinates convention:

      [0]                           [63]
 [0] --------------- ... ---------------> (+X)
     |(0,0)                       (63,0)
     |
     |
    ...
     |
     |
[31] |(0,31)                     (63,31)
     v
     (+Y)

      [0]                           [63]
 [0] --------------- ... ---------------> (+X)
     |(0)                           (63)
     |
     |
    ...
     |
     |
[31] |(1984)                      (2047)
     v
     (+Y)

               [r0 ...                     , r1 ...                          ...    , r30                        , r31 ...                     ]
      indics = [0,    1,    ..., 62,   63,   64,   65,   ..., 126,  127,     ...    , 1920, 1921, ..., 1982, 1983, 1984, 1985, ..., 2046, 2047 ]
      buffer = [1984, 1985, ..., 2046, 2047, 1920, 1921, ..., 1982, 1983,    ...    , 64,   65,   ..., 126,  127,  0,    1,    ..., 62,   63   ]
```

## Reference

-   [Tobias V. Langhoff](https://tobiasvl.github.io/blog/write-a-chip-8-emulator/): full concept guide (non-code) and technical support.
-   [Austin Morlan](https://austinmorlan.com/posts/chip8_emulator/): brief implementation guide in C++.
-   [David Winter](https://www.pong-story.com/chip8/): outstanding partner (interpreter, disassembler, and extended technical documentation).
-   [Cowgod](http://devernay.free.fr/hacks/chip8/C8TECH10.HTM): CHIP-8 instruction set definition.
-   [Jackson Sommerich](https://jackson-s.me/2019/07/13/Chip-8-Instruction-Scheduling-and-Frequency.html): CHIP-8 instruction set timing.
-   [Wikipedia](https://es.wikipedia.org/wiki/CHIP-8): general concepts.
