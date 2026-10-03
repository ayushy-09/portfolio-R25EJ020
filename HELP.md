# Simple Line Editor — Command Reference

| Command | Usage | Description |
| :--- | :--- | :--- |
| `H` | `H` | Displays the help menu with available commands. |
| `I <text>` | `I Hello World` | Inserts a new line containing `<text>` after the current cursor position. |
| `P` | `P` | Prints all buffer lines. The current cursor line is marked with `>`. |
| `D` | `D` | Deletes the line at the current cursor position and updates neighbor pointers. |
| `Q` | `Q` | Frees all dynamically allocated memory and quits the application. |
