# simpleboard

Configurable ncurses based TUI dashboard for systems like a power menu or controlling monitors.

![Screenshot of simpleboard utility](/simpleboard.jpg?raw=true)

Features:
- Customizable appearance
- Bind any system command to a hotkey
- Mouse support

## Installation

1. dependencies: `c standard library, ncurses, bash`
2. run `sudo make && sudo make install`

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

### Hyprland Window Rules

To launch `simpleboard` as full screen in `Hyprland` the following command can be used:

```sh
hyprctl dispatch exec "[size 100% 100%; float; move 0% 0%; animation slide]" "kitty -e simpleboard"
```

In my testing terminal based applications seem to not work with static window rules. As terminal apps set window titles after process creation, `Hyprland` will not apply window rules to them.

## Building

`make` will output `./bin/simpleboard`, keep in mind behavior will be slightly different when running from a terminal that closes after execution or not.

## Testing and Debugging

`make debug` will output `./bin/simpleboard_debug`, which is compiled with the `-g`flag

`make test` will output `./bin/readConfig_test` and run automatically on compile success, unit tests created using the [acutest](https://github.com/mity/acutest) framework.
