## About 

"fave" is a command line tool that lets you save directories and each directory's commands as easy access bookmarks.
*(Note: I am aware of convenient solutions such as `reverse-i-search` and aliases. This project was a way of learning some C)*

Feature ideas for the future:
    - option to provide title or note for directory/command
    - automatically create faved folder if `fave ac` was ran in a non-faved folder

### Usage

`fave ad` - **a**dds current working **d**irectory
`fave rd` - **r**emoves current working **d**irectory
`fave sd` - **s**hows a list of saved (faved) **d**irectories and prompts you to enter a number to jump to that directory

`fave ac [command]` - **a**dds a **c**ommand to the current directory (if it is saved)
`fave rc` - shows a list of saved commands and prompts you to enter a number to **r**emove a **c**ommand
`fave sc` - **s**hows a list of saved **c**ommands and prompts you to enter a number to execute a command

Example:
```sh
$ fave sd
Faved directories:
1) /home/user/directory1/
2) /home/user/directory2/

Select number: 1
$ fave sc
Faved commands:
1) command1
2) command2
3) command3

Select number: 1

$ executes command 1...
```

### Prerequisites

- gcc
- make

#### .bashrc 

Since a C program cannot change the directory of the parent shell the program has ran in, there is a slight workaround:

```sh
fave() {
    local result=$(/path/to/binary/fave "$@")
    
    if [ -n "$result" ]; then
        eval "$result"
    fi
}
```

For changes to take effect, run `source ~/.bashrc`.
If this step is skipped, `fave` won't run commands or change directories, it will simply print the result

### Installation

```bash
$ git clone https://github.com/MicasiO/fave.git
$ cd fave
$ sudo make install 
```

