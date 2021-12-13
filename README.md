# TibiaRev

NOTE: This repo should never be used to cause a bad player experience for anyone in any way.

## Why Tibia 

Tibia (a MMORPG) is a great starting point in understanding reverse engineering of packets as these are not obfuscated in this game.  

We will download the client from the private server aurera-global.com as it's uptime is high and there is no anti-cheat detection on this server.

## How to build this project

The solution `tibia.sln` consists of two projects. The project, `payload`, is to be injected into the game, `Tibia.exe`, by the project, `injector.exe`.

**Steps:** 

1) Install Tibia.exe from [aurera-global.com](https://aurera-global.com/)
2) Open `tibia.sln` and go to `Configuration Properties` (right-click on the solution), make sure the follow is set: 
    - Character Set: Use Multi-Byte Character Set
    - C++ Language Standard: ISO C++20 Standard
    - Precompiled Header: Not Using Precompiled Headers
3) Compile the solution
4) Navigate to `<fullpath>\TibiaRev\src\Release\` and run `injector.exe`, an `Injector` directory should have been created on your desktop.
5) Navigate to `Desktop\Injector\config.ini` and insert the following: 
 
    > DLLs=<fullpath>\TibiaRev\src\Release\payload.dll \
     msDelay=500 \
     TargetProcess=Tibia.exe
6) Open the game, `Tibia.exe`
7) Run `injector.exe`
8) The `payload.dll` should now have been injected and an extra console should have been created from where you can read it's instruction guide (e.g. pressing F1, ..., F12 to do different actions)
