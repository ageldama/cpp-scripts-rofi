# c-scripts-rofi

Blazingly fast and easy way to launch your scripts with [Rofi](https://github.com/davatorium/rofi)

```
$ scripts-rofi -?
It asks to select a script within SCRIPT_DIRS and execute it.

(NO_DB_FLAG_FILE:  $HOME/.no-db-scripts-rofi)

-p : print selection
-s : save selection
-e : execute selection
-S SCRIPT_DIRS  (':'-separated list)
	$HOME/local/scripts
	$HOME/local/bin
	$HOME/.screenlayout
-D HIST_DB_FILE   : $HOME/.scripts-rofi.hist
-T XTERM_COMMAND  : x-terminal-emulator -e
-P : Dump stored history/freqs and exit
-W : execute wrapper (like 'wine')
-/ : filename matching regex
-i : ignorecase

Exiting.
```
