# cSoft3D
###### Formerly known as cube2
Note: Current project status could be characterized as "horrifying".\
But since GitHub doesn't have an "unlisted" repo privacy setting ... I present to you: cSoft3D!\
It's supposed to be a software 3d renderer, but right now it just renders a point cloud of vertices. But hey, it looks kinda cool!

Here's a [live demo](https://andai.tv/wasm/sdl/cube2). It doesn't work on phones, and the monkey might be upside down.\
Move the mouse around to rotate the monkey!

## Building
### You will need:

- Windows (or the patience to set things up on Linux yourself)
- Visual Studio 2022, or the Build Tools (for cl.exe in build-win.bat)\
MinGW probably works too, but didn't try yet.\
(Note: You need a different SDL download if you use MinGW!)
- Python (I use the Python one-liner server in run.bat)
- Brave Browser (not shilling for Brave but the path to the browser executable is hardcoded in run.bat... so just change it to whatever you use)
- Emscripten for the web build (build.bat)
- For the Windows build you will need SDL2-2.0.22, preferably at C:\dev\SDL2-2.0.22.\
(You don't need it for the web build: Emscripten ships with SDL built in! How cool is that?)

## TODO

[ ] Clean things up a bit (sorry)
[ ] Update SDL version
[ ] Linux build
[ ] MinGW build
[ ] Make the monkey not be upside down