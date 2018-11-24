# CAbGC
Everything In Its Right Place

CAbGC (named after the opening 4 notes of Everything In Its Right Place) is a floating window manager based on user-defined presets for positioning and sizing windows in exactly The Right Place™.

CAbGC is built on the source code and lessons learnt from polychrome[1] after coming to the realisation that:
	1. The majority of time spent interacting with the window manager is spent rearranging windows, not swapping between them
	2. The majority of time spent on the computer is spent using the same few sets of applications in (roughly) the same layouts to perform the same tasks

CAbGC kills these two birds with one stone by allowing users to define presets. Simply set up any applications as desired for a particular activity (e.g. for developing, place a browser on the left and 4 terminals on the right) and press `mod+s` to save the layout followed by the desired hotkey to bind the layout to (e.g. `mod+d`, for develop). Now, when `mod+d` is pressed, all the applications needed for developing will be shown in exactly The Right Place™. Swapping to another workspace (by pressing e.g. `mod+g` for games, another user-defined preset) will hide all development windows and show all windows desired for gaming, again in The Right Place™.

If some or all of the applications required by an activity are not running, CAbGC can launch the missing applications by pressing `mod+l` (for load/launch), which will launch, resize, and position the applications as specified by the preset.

In essence, CAbGC combines (and even improves upon) the effortlessness of tiling window managers' window placement with the aesthetics and practicality of a regular floating window manager (i.e. not forcing applications to contort to certain sizes and shapes that won't play nice, as tiling window managers require)

## Why should I try yet another window manager?

Advantages:
	* Time is not wasted manually launching the same applications for the same tasks
	* Time is not wasted constantly manually rearranging windows for the same tasks
	* Clear separation of applications used for different tasks - both mental and practical separation. No more wondering which workspace that one window was left on!

There are, however, a few drawbacks:
	* An inevitable first-time-setup cost when all initial presets and other hotkeys need to be defined (generally takes around 10-15 minutes)
	* Can be distressing to realise so much time is spent on the same few activities (No known fix for this one yet)


[1] polychrome is my first window manager, a (somewhat) failed experiment in dynamic window management and selection of window by border colour.
