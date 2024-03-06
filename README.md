# simpleboard

Configurable ncurses based TUI dashboard for systems like a power menu or controlling monitors.

![Screenshot of simpleboard utility](/simpleboard.jpg?raw=true)

## Installation

1. dependencies: `c standard library, ncurses, bash`
2. run `sudo make install`

## Updating

1. run `sudo make update`

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
title = simpleboard  (empty field will show no title)
titleColor = red     (title color)
border = 1           (window border)
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

## Building

`make` will output `./bin/simpleboard`, keep in mind behavior will be slightly different when running from a terminal that closes after execution or not.

## Testing

`make test` will output `./bin/test`, unit tests created using the [acutest](https://github.com/mity/acutest) framework.
