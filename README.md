# thalc  
That Calculator -- I needed one that didn't use up my internet data with it's spyware.  
  
  
This has been tested to work on windows only so far.  But since I'm using Raylib 4.5 library, change the PLATFORM_DESKTOP to what you need for other operating systems / devices. Read the raylib docs to know more, or simply ask the raylib group in their discord channel.  
  
  
For windows users, as long as your gcc.exe (mingw) is in your path, you should be able to just simply run the bat file.  
  
UPDATE LOG 8/27/2023 :  
   FIXED - Embedded the Font as a C source file & added bin2file folder with source so you can examine how I converted the  binary TTF file to a C-header source file.  (NOTE : If you want to see debug information in the console, add DEBUGME as a DEFINE "-DDEBUGME" in the build.bat file.)  
   
   FIXED - Can continue new math set without requiring a CLEAR "C" button being pressed.  
  
  
  ![That Calculator](progress.png)  
  