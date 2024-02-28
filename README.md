# simpleboard

Configurable ncurses based tui dashboard for systems like a power menu or controlling monitors.

![simpleboard](/simpleboard.jpg?raw=true)

## Installation

1. ncurses is a required system dependency
2. run `sudo make install`

## Uninstallation

1. run `sudo make uninstall`

## Configuration

Default config is in `simpleboard.conf` (copied when installed)

Config file location: `~/.config/gusboard/simpleboard.conf`

### Config Options

```
# Default values provided

:[appearance]
color = blue       (text color)
height = 10        (box height)
width = 20         (box widht)
title = 1          (show title 0 = false)
titleColor = red   (title color)
preCommnad =       (system command to run before showing anything)

:[commands]
{command display name} = {system command}, {hotkey}
```

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
