# CLOTH SIMULATION


### A very light-weight cloth simulation, done in SDL2 & C.

- Runs on an Intel Celeron with two physical and logical cores, @1.10 Ghz. Adds something like 5% CPU usage to the system (and 2% on a ryzen 5 3600 @3.7 Ghz, according to my tests)


- Mass & Spring system, simulated by Hooke's law. See `update_sticks` function in `src/sticks.c`

<br />
<br />



## **Requirements**

- SDL2 is quite obviously a must, since the rendering lib used is SDL2. You can download it on [SDL](https://www.libsdl.org/) (make sure you linked the .dll file correctly, of course) if you're on Windows, or just do 
```ps 
sudo apt get libsdl2-dev
```
if you're on Linux.

On MacOS, I have no clue if it works since I couldn't test it.

- GCC. or any C compiler should do the job. I've tested with GCC and Clang, and it seems to work fine for me. Don't be afraid to open an issue if it doesn't.

<br />
<br />

## How to install it on your computer ?

- Just clone it wherever you want in your computer.

```ps
git clone https://github.com/Esteban795/physics-simulations.git
```

- After that, head to the directory you cloned it in, go to `cloth_simulation` and type
```ps
make build
``` 
- Then, use 
```ps
make run ROWS="NumberOfRowsItShouldHave" COLUMNS="SameButColumns"
```

## How to use it ?

| Keys        | What they do                  |
|-------------|-------------------------------|
| Left click  | Can drag the cloth            |     |
| Right click | Tears cloth apart             |
| SCROLLWHEEL | Resizes the cursor for tearing| 
| LEFT_ARROW  | Wind impulse, to the.. left.  |
| RIGHT_ARROW | Wind impulse, but to the.. right.|
| ESC         | Exits the program              |

<br />
<br />

## What's left to do ?

- Allow the user to tear apart the cloth when it is dragged too far
- Why not try in 3D ? I'll see that I guess