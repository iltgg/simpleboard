# simpleboard

Configurable ncurses based tui dashboard for systems like a power menu or controlling monitors.

![simpleboard](/simpleboard.jpg?raw=true)

## Installation

1. dependencies: `c standard library, ncurses, bash`
2. run `sudo make install`

## Uninstallation

1. run `sudo make uninstall`

## Configuration

Default config is in `simpleboard.conf` (copied when installed)

Config file location: `~/.config/gusboard/simpleboard.conf`

### Config Options

```
# Default values provided
# 0 = false, 1 = true

:[appearance]
color = blue         (text color)
height = 10          (box height)
width = 20           (box width)
dynamicHeight = 0    (determined from # commands)
dynamicWidth = 0     (determined from max command name len)
dynamicWidthMin = 0  (min width if dynamicWidth = 1)
title = 1            (show title 0 = false)
titleColor = red     (title color)
border = 0           (window border)
preCommand =         (system command to run before showing anything)

:[commands]
{command display name} = {system command}, {hotkey}, {misc}

# Comments with #, must be newline
```

### Swaylock

If `daemonize` is in swaylock config or `-f` argument is passed, swaylock will red screen if terminal is closed while swaylock is running (do not know why).

Pass `stay_alive` misc argument in `simpleboard.conf` to keep terminal open after executing command.

### Hyprland Window Rules

Will make dashboard take up the entire screen when called with `exec kitty -e simpleboard` (or equivalent)

```
windowrule=nomaximizerequest,title:^(simpleboard)$
windowrule=nofullscreenrequest,title:^(simpleboard)$
windowrule=size 100% 100%,title:^(simpleboard)$
windowrule=float,title:^(simpleboard)$
windowrule=move 0% 0%,title:^(simpleboard)$
windowrule=animation slide,title:^(simpleboard)$

```
