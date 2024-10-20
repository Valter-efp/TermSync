# TermSync
This program runs 2 terminals, one with your custom version and another one virtualized with bash. It will replicate the inputs on the custom one to the virtual one.

The TermSync program is a terminal synchronization tool for inter-process communication. It sets up a custom terminal and an xterm session, allowing keystrokes to be sent to both terminals simultaneously. The program uses a FIFO (named pipe) for data exchange and manages terminal modes to facilitate real-time input and output handling. It is useful for scenarios where synchronized terminal interactions are required, such as debugging or monitoring multiple terminal sessions.

Thought this would be usefull to test Minishell.
There are still some bugs, like if you press "backspace" on the custom terminal it will mess the input of the Xterm that is running bash (just make sure to not make any typos and it should work fine ;)). If you would like to recommend some fix, feel free.
Be sure to change the "custom_argv[0] = "minishell";" in the function "start_minishell" to the path of your minishell program.
