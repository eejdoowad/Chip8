# Windows (Visual Studio) Build Instructions

1. Download and install SDL2. Make sure to create a new environment variable called 'SDL2_DIR' with value 'path/to/SDL/Root/Directory'
2. Open the solution file (chip8.sln)
3. Click debug to build the project and load the default PONG ROM. The ROM can be changed via debugger settings in project properties
4. Once you've built the project, chip8.exe will be generated in the bin directory.
5. When chip8.exe is started from the command line, enter the path to the desired ROM as the argument. For example, if chip8.exe is in bin, then run "./chip8 ../roms/PONG"
