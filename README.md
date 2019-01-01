# Autoly

Autoly is a floating window manager based on user-defined layouts for positioning, sizing, and launching windows in exactly The Right Place™.

Autoly is built on the source code and lessons learnt from Polychrome<sup>\*</sup> after coming to the realisation that:
1. The majority of time spent interacting with the window manager is spent rearranging windows, not swapping between them
2. The majority of time spent on the computer is spent using the same few sets of applications in (roughly) the same layouts to perform the same tasks

Autoly kills these two birds with one stone by allowing users to define layouts. Simply set up any applications as desired for a particular task (e.g. for developing, place a browser on the left and 4 terminals on the right) and press `mod+s` to save the layout followed by the desired hotkey to bind the layout to (e.g. `mod+d`, for develop). Now, when `mod+d` is pressed, all the applications needed for developing will be shown in exactly The Right Place™. Swapping to another layout (by pressing e.g. `mod+g` for games, another user-defined layout) will hide all development windows and show all windows desired for gaming, again in The Right Place™.

If some or all of the applications required by a layout are not running, Autoly can launch the missing applications with a single keystroke (by default `mod+l` for load/launch), which will launch, resize, and position the applications as specified by the layout.

In essence, Autoly combines (and even improves upon) the effortlessness of tiling window managers' window placement with the aesthetics and practicality of a regular floating window manager (i.e. not forcing applications to contort to certain sizes and shapes that won't play nice, as tiling window managers require) 

<sup>\*</sup> Polychrome is my first window manager, a (somewhat) failed experiment in dynamic window management and selection of window by border colour.

## Why should I try yet another window manager?

Advantages:
* Time is not wasted manually launching the same applications for the same tasks
* Time is not wasted constantly manually rearranging windows for the same tasks
* Clear separation of applications used for different tasks - both mental and practical separation. No more wondering which workspace that one window was left on!

There are, however, a few drawbacks:
* An inevitable first-time-setup cost when all initial layouts and other hotkeys need to be defined (generally takes around 5-10 minutes)
* Can be disheartening to realise so much time is spent on the same few activities (No known fix for this one yet)

## Default hotkeys

note: `mod` refers to the modifier key of choice - `alt` or `win` (default `win`)

| key combo                              | action                                                                 |
| -------------------------------------- | ---------------------------------------------------------------------- |
| `mod + s` followed by `mod + <HOTKEY>` | save current layout to `<HOTKEY>`                                      |
| `mod + <HOTKEY>`                       | switch to layout `<HOTKEY>` (if `<HOTKEY>` has been assigned a layout) |
| `mod + l`                              | launch any unlaunched applications required for the current layout     |
| `mod + [0-9]`                          | focus window [0-9]<sup>†</sup>                                         |
| `shift + mod + <HOTKEY>`               | move focused window to layout `<HOTKEY>`                               |
| `mod + f`                              | toggle fullscreen of window                                            |

<sup>†</sup> the "number" of a window is determined by its X coordinate - the window furthest to the left is window 1, the window 2nd furthest to the left is window 2, etc. The Y coordinate is used for two windows with the same X coordinate (window higher on the screen will come before the window lower on the screen)
