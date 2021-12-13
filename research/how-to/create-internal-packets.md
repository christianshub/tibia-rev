# How to create internal packets

The following gudie will show how to find the internal send packet function in Tibia with x64dbg. We want to send a packet that triggers the `look` functionality. The `look` functionality ingame is just writing to the screen what we see when clicking on an object. I chose this functionality as it's easy to trigger and look for in the debugger.

## Steps

1) Go to x64dbg and search for `send` within the `ws2_32.dll` module (press `alt+E`)
1) Set a bp and trigger the function.
1) Do two similar things that triggers `ws2_32.send` and compare their call stacks. 

    Call stack when triggering `look` ingame:

    ```
    Address  To       From     Siz Comment              Party MyComment
    0115E698 006E30A1 75D958A0 18  ws2_32.75D958A0      User  send
    0115E6B0 00626DB3 006E30A1 1AC tibia.sub_6E308C+15  User  send wrapper
    0115E85C 00490393 00626DB3 11C tibia.sub_626B10+2A3 User  encrypt packets
    0115E978 0050DAB6 00490393 4DC tibia.sub_490320+73  User  unencrypted packets
    ...
    ```

    Call stack when triggering `follow` ingame:

    ```
    Address  To       From     Siz Comment              Party MyComment
    0115E6A0 006E30A1 75D958A0 18  ws2_32.75D958A0      User  send
    0115E6B8 00626DB3 006E30A1 1AC tibia.sub_6E308C+15  User  send wrapper
    0115E864 00492772 00626DB3 120 tibia.sub_626B10+2A3 User  encrypt packets
    0115E984 0050DC08 00492772 4D0 tibia.00492772       User  unencrypted packets
    ...
    ```

    Notice by looking at the two call stacks, that the first three function calls are identical. The three calls are the same whether we call an ingame `look` command or an ingame `follow` command.\
    The second function call (`sub_6E308C+15`) is likely Tibia's own `ws2_32.send` wrapper.\
    The third function call is may be where the packet gets encrypted.\
    The fourth function calls (`look`: `tibia.sub_490320+73` / `follow`: `tibia.00492772`) are interesting here as it's the first time we see a difference when comparing the two call stacks.
1) Following `look`'s fourth function call in the call stack (`look`: `tibia.sub_490320+73`) we will land here:

    ```ASM
    ...
    0049035C | mov ecx, 0x8C              |
    00490361 | call <tibia.sub_625C10>    | look pkg (packet id)
    00490366 | mov ecx, edi               |
    00490368 | call tibia.626180          | unk2
    0049036D | mov ecx, esi               |
    0049036F | call tibia.626180          | unk3
    00490374 | mov ecx, [ebp + 0x8]       |
    00490377 | call tibia.625FD0          | unk4
    0049037C | mov ecx, [ebp + 0xC]       |
    0049037F | call tibia.626180          | unk5
    00490384 | mov ecx, [ebp + 0x10]      |
    00490387 | call tibia.625FD0          | unk6
    0049038C | mov cl, 0x1                |
    0049038E | call <tibia.sub_626B10>    | packetFlag
    ...
    ```
1) This is where the packet is being created, and we can now set a bp at every call and inspect what's moved into `ecx`.
1) By inspecting the calling convention and what's happening inside the functions at the end, it sure looks like they are all `__fastcall`'s since arguments are moved into `ecx` and the callee function is popping it's registers at the end (about `__fastcall`, see [MSDN](https://docs.microsoft.com/en-us/cpp/cpp/fastcall?view=msvc-170)). 
    - NOTE: It does not matter if it's a `__thiscall` or a `__fastcall` in this situation as only one argument is passed to each function (`mov ecx xxx`).
1) Now follow all the functions, and grab their `RVA`'s (Relative Virtual Address)\
    - NOTE: Don't get the address from within the function (`tibia.sub_490320+73`) above, but follow each function and grab it's RVA from there.
1) We are now going to recreate the internal and unencrypted packets like so: 

    ```cpp
    namespace lookPacket
    {
        namespace
        {
            // Packet1
            typedef void(__fastcall* _pkgId)(unsigned int id);
            _pkgId PktId = (_pkgId)(globals::moduleBase + 0x1B5C10);

            // Packet2
            typedef void(__fastcall* _pktUnk2)(int num);
            _pktUnk2 PktUnk2 = (_pktUnk2)(globals::moduleBase + 0x1B6180);

            // Packet3
            typedef void(__fastcall* _pktUnk3)(int num);
            _pktUnk3 PktUnk3 = (_pktUnk3)(globals::moduleBase + 0x1B6180);

            // Packet4
            typedef void(__fastcall* _pktUnk4)(int num);
            _pktUnk4 PktUnk4 = (_pktUnk4)(globals::moduleBase + 0x1B5FD0);

            // Packet5
            typedef void(__fastcall* _pktUnk5)(int num);
            _pktUnk5 PktUnk5 = (_pktUnk5)(globals::moduleBase + 0x1B6180);

            // Packet6
            typedef void(__fastcall* _pktUnk6)(int num);
            _pktUnk6 PktUnk6 = (_pktUnk6)(globals::moduleBase + 0x1B5FD0);

            // Packet7
            typedef void(__fastcall* _pktFlag)(BYTE num);
            _pktFlag PktFlag = (_pktFlag)(globals::moduleBase + 0x1B6B10);
        }

        void Send()
        {
            PktId(0x8C);
            PktUnk2(0x00007D50);
            PktUnk3(0x00007DC8);
            PktUnk4(0x00000007);
            PktUnk5(0x00000063);
            PktUnk6(0x00000001);
            PktFlag(0x1);
        }
    }
    ```

