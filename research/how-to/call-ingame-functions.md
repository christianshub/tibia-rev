# Calling ingame functions

The following steps will guide you through the process of making a simple in-game function call using x64dbg. 

## Calling a simple game function

Make sure x64dbg and a game you want to reverse is running. Now follow the steps below:

1) Do something in the game to trigger a message appearing (e.g.: "sorry not possible" in Tibia when hitting a wall)
1) Through x64dbg, select the game's module (`alt+E`)
1) Analyze the code - guess the functions and calls (`ctrl+A`)
1) CPU -> Right-click -> Search for -> Current module -> String ref. -> Search for the str that appeared.
1) Put a breakpoint on the address you find interesting, see if bp gets hit when triggering the message. 
1) Assuming you found the correct address, follow the call (`Enter`) and you found the print function.
1) Now find out what the calling convention is ([MSDN](https://docs.microsoft.com/en-us/cpp/cpp/argument-passing-and-naming-conventions?view=msvc-170)). We see the string is moved into `ecx`, thus, it's probably either a `__fastcall` or a `__thiscall`. Looking at the end of the function, we see it's popping a lot of registers from the stack:

    ```ASM
    pop     ec      ; <-----
    pop     edi     ; <-----
    pop     esi     ; <-----
    pop     ebx     ; <-----
    mov     ecx, [ebp+var_14]
    xor     ecx, ebp
    ...
    retn
    ```
    And thus, it's likely a fastcall, as, according to [MSDN](https://docs.microsoft.com/en-us/cpp/cpp/fastcall?view=msvc-170) about fastcalls: 

    Argument-passing order:
    > The first two DWORD or smaller arguments that are found in the argument list from left to right *are passed in ECX and EDX registers*; all other arguments are passed on the stack from right to left.

    Stack-maintenance responsibility:

    > Called function *pops the arguments from the stack*.
1) Go to your Payload project and type in the function prototype for the newfound function like so:

    ```cpp
    namespace print
    {
        namespace
        {
            typedef void(__fastcall* print)(const char* ecx);
            print o_print = (print)(globals::moduleBase + 0x214540);
        }

        void Print(const char* text)
        {
            std::cout << "print::Print(" << text << ")" << "\n";
            o_print(text);
        }

        void GetAddress()
        {
            std::cout << "print::GetAddress(): " << std::hex << o_print << "\n";
        }
    }
    ```

    The snippet above creates a typedefinition of the print function. We then declare a function prototype, `o_print`, and then points to where `o_print`'s address is. It's at `Tibia.exe`'s modulebase plus a relative offset from the print function's address to the modulebase. We then create two functions from where we can call the print function, `Print`, and a function, `GetAddress`, that will print at exactly what address the print function was found.

## Calling an ingame function that is also sent as a packet

Make sure x64dbg and a game you want to reverse is running. Now follow the steps below:

1) From x64dbg go to the modules section (`Alt+E`), search for the module `ws2_32.dll`.
    - `ws2_32.dll` contains the Windows Sockets API used by most applications leveraging network in Windows [MSDN](https://docs.microsoft.com/en-us/windows/win32/winsock/windows-sockets-start-page-2). Find the `send` function (some games use `WSASend`) and set a bp. What's important is that the bp gets hit when something happens inside the game.
1) From within the game, enable the bp and write `hello!` in the chatbox. Be quick, as heartbeat packets will trigger the bp too.
1) Check out the Call Stack when saying hello (chat function):

    Ingame chat:
    ```
         Address  To       From     Siz Comment              Party  
    L01: 00D9E620 006E30A1 75D958A0 18  ws2_32.75D958A0      User   
    L02: 00D9E638 00626DB3 006E30A1 1AC tibia.sub_6E308C+15  User   
    L03: 00D9E7E4 004909EF 00626DB3 18C tibia.sub_626B10+2A3 User   
    L04: 00D9E970 004D7682 004909EF 7AC tibia.sub_4909CA+25  User    ; <-- Note! 
    L05: 00D9F11C 004D3966 004D7682 1D8 tibia.004D7682       User   
    ...
    L12: 00D9FE04 773B7A9E 75E0FA29 5C  kernel32.75E0FA29    System 
    L13: 30D9FE60 773B7A6E 773B7A9E 10  ntdll.773B7A9E       System 
    L14: 00D9FE70 00000000 773B7A6E     ntdll.773B7A6E       User   
    ```

    Notice, the second (`tibia.sub_6E308C+15`) and third (`tibia.sub_626B10+2A3`) function call is most likely the internal chat function (see notes as to why below under *References*). 

1) Check out the Call Stack when taking a step left ingame (step function). By comparing the two call stacks, it's easy to see we have a `send` function at the top, then it's likely two `send` wrappers on line 2 and 3 (as they are the exact same functions and offsets). So what's interesting is the call stack's 4th line (highlighted below):

    Ingame step (a movement):

    ```
         Address  To       From     Siz Comment              Party  
    L01: 00D9EFC4 006E30A1 75D958A0 18  ws2_32.75D958A0      User   
    L02: 00D9EFDC 00626DB3 006E30A1 1AC tibia.sub_6E308C+15  User   
    L03: 00D9F188 0048D5E8 00626DB3 154 tibia.sub_626B10+2A3 User   
    L04: 00D9F2DC 005F11C1 0048D5E8 11C tibia.0048D5E8       User   ; <-- Note! 
    L05: 00D9F3F8 005EFB4B 005F11C1 128 tibia.sub_5F1140+81  User   
    ...
    L12: 00D9FE04 773B7A9E 75E0FA29 5C  kernel32.75E0FA29    System 
    L13: 00D9FE60 773B7A6E 773B7A9E 10  ntdll.773B7A9E       System 
    L14: 00D9FE70 00000000 773B7A6E     ntdll.773B7A6E       User   
    ```
1) Function calls deviates between the two call stacks at line 4, so we set a breakpoint to find the chat function at line 4 at address `4909CA+25`, we then scroll to the top of the function and set a bp.
1) We write `hello` again and now let's see what's in our registers:

    Top of the function:

    ```
    004906C0 | 55    | push ebp         |
    004906C1 | 8BEC  | mov ebp, esp     |
    004906C3 | 6A FF | push 0xFFFFFFFF  |
    ```

    Registers:

    ```
    EAX : 00000001
    EBX : 04EF2540
    ECX : 00000001
    EDX : 04FB6E50     "hello"
    EBP : 00D9F118
    ESP : 00D9E970
    ESI : 04FB6E50     "hello"
    EDI : 00000001
    EIP : 004906C0     tibia.004906C0
    ```

    Stack:
    ```
    00D9E970  004D7682  return to tibia.004D7682 from tibia.sub_49057B+145
    ```
1) The bp got hit, so we now go to the stack and hit `enter` to see it's arguments:
    ```
    004D7679 | 8BD6         | mov edx, esi       | edx:"hello", esi:"hello"
    004D767B | 8BCF         | mov ecx, edi       |
    004D767D | E8 3E90FBFF  | call tibia.4906C0  |
    ```

    It's clearly a `__fastcall` where the chat message (`1`) is stored in `edx` and some kind of chat code perhaps is stored in `ecx`. Those are the two arguments.

1) We can now create the function call in our payload like so:

    ```cpp
    namespace chat
    {
        namespace
        {
            typedef void(__fastcall* Chat)(unsigned int chatCode, const char* message);
            Chat originalAddress = (Chat)(globals::moduleBase + 0x206C0);
        }

        void Send(unsigned int chatCode, const char* text)
        {
            std::cout << "chat::Chat(" << text << ")" << "\n";
            originalAddress(1, text);
        }

        void GetAddress()
        {
            std::cout << "chat::GetAddress(): " << std::hex << originalAddress << "\n";
        }
    }
    ```


## References

- *2020*, [*OllyDBG Tibia Reverse Engineering & Function Calling Tutorial 1/3*](https://www.youtube.com/watch?v=zldW5gBc-JE)

- *2016, Nick Cano, Game Hacking, No Starch Press,  (p. 215-216), about sending packets:*

    TL;DR: We see two call stacks below, one with a `step()` function, and one with a `castSpell()`. The stacks are similar except from `step()` and `castSpell()` addresses, thus, we found our two actor functions.

    ```
            0x70707070  ->  send()
            0x60606060  ->  connection->send()
            0x50505050  ->  network->send()
                        | 
             -----------------------
            |                       |                           
    0xDEADBEEF -> step()    0xBADF00D -> castSpeel()
            |                       |                           
             -----------------------
                        |
            0x40404040  ->  processKeyboardInput()
            0x30303030  ->  processInput()
            0x20202020  ->  doFrame()
            0x10101010  ->  main()              
    ```
    ----------------
    Nick Cano's thoughts about sending packets and calling in-game functions: 
     
    > Before a game draws a frame, it checks for keyboard and mouse input.
    When it receives input that results in an action, such as moving around or
    casting a spell, it checks to make sure the action is possible and, if so, tells
    the game server that the action has been performed. 


    So, the game code checks for events and alerts the server through some kind of function (here called `processInput`).

    >The processInput() function is called every frame. The function iterates
    over all pending inputs and dispatches different types of inputs to their relevant handlers. In this case, when keyboard input is received, it's dispatched to the processKeyboardInput() function. This handler then checks whether
    the key is either W or S, and, if so, calls step() to move the player in the corresponding direction.
    Since step() is used to perform an action, it is called an actor function.
    The invocation of an actor function is called actuation. You can directly call
    a game's actor functions from your bot to perform an action while completely bypassing the input layer.
    Before you can call an actor, though, you must find its address. To do
    this, you can attach OllyDbg to the game, open the command line, and
    enter bp send. This will place a breakpoint on the send() function, which is
    used to send data over the network. When you play the game, every time
    you take a step, cast a spell, pick up loot, or do anything else, your breakpoint should trigger, and you can note each function in the call stack.

    Note:
    >The game should call send() every time you do anything while playing. Pay attention
    to what you did before each send() breakpoint is hit, as that will give you a rough
    idea of what action each call is communicating to the server, and, ultimately, what
    the actor you find is responsible for. 

    Comparing call stacks:

    >Once you have a few different call stacks, you can compare them to
    locate the actor functions. 

    > ... The call stacks you find should be identical at the
    top, sharing a couple of common functions responsible for generic network
    transmission. They should also be identical on the bottom, since each call
    to send() should have originated from the processInput() function. Each
    stack should have some unique functions between these identical regions, though, 
    and those are the actor functions you're looking for. Typically, the
    function of interest is immediately beneath the common network calls. In
    this case, the two actors are the step() and castSpell() functions.
    After hacking the same game for a while, you'll learn how far up
    the stack the actor functions are from the send() call. In Figure 10-3, for
    example, the actors happen three calls before the send() call. Knowing this,
    you could just climb the stack in OllyDbg (ctrl-F9 followed by F8) three
    times when your send() breakpoint is hit and be inside the actor function
    that sent the data.

    Make sure the actor functions are calling from same the same thread as the game:

    > Just make sure to call the actor functions
    from inside the same thread as the game, or you can run into threading
    issues. The best way to do this is to call the actors from a hook on a major
    function like Direct3D's EndScene() or the Windows API's PeekMessage() function, as these functions will usually be called only from the game's main
    thread.

    About `__thiscall` and packets: 

    > If you try to call an actor function that's a nonstatic member of a class, the
    function will have a _thiscall calling convention, which means you'll need to
    pass the instance of the class on the ECX register. (You can brush up on calling
    conventions in "Function Calls" on page 94.) Passing the instance is straightforward, but you'll have to locate a pointer chain to the class instance first.
    To find the pointer chain, you can drop a breakpoint on the actor function,
    grab the class instance value from ECX when the breakpoint kicks, and throw
    that value into a Cheat Engine pointer scan. Then, to call the function, you'd
    walk the pointer chain, obtain the current instance address, and use inline
    assembly to set up ECX and make the actual function call. This process works
    similarly to the way VF hook callbacks call their original counterparts, as shown
    in "Writing a VF Table Hook" on page 156.