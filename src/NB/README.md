# NB library

The NB (No Bullshit) library provides an interface for the messy
OS specific junk like windows, input, graphics and whatnot.

The library has a few subsystems:
- Windows (GUI kind)
- Input
- Graphics
  - Symbol rendering
- Audio (TODO)
- Networking (TODO)

## Graphics coordinate space

![coordinate_plane](../docs/nb_screencoord.png)

The center of the window is (0,0). How wide a single "point" is depends on dimensions specified in call to nbInit.

## Setting up the window loop
In between calls to nbInit and nbQuit, there is one window or screen. All graphics functions refer to this single output.

In order to have a responsive window, you need to continuously poll the library for window events and update the graphics buffer in a loop. Before that, you need to intialize the library and after that, quiting it to free resources.

The functions used for this:
```c
int nbInit(char* title, unsigned long width, unsigned long height);
void nbSwapBuffers();
int nbWindowIsUp();
void nbPollEvent();
void nbQuit();
```
A simple main function looks like this:
```c
int main(int argc, char* argv[]) {
    if (nbInit("Window title", 1280, 720) != 0) return 1;

    while (1) {
        nbPollEvent();
        if (!nbWindowIsUp()) break;

        // Graphics example: rendering a square
        nbClear();
        nbDrawRect(0, 0, 32, 32, 1.0f); 

        nbSwapBuffers();
    }

    nbQuit();
    return 0;
}
```
