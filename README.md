# 六 Text Editor

Roku is a lightweight text editor, implementing everything you would want from one.
This software is not yet ready for daily use and may contain bugs. If you find some, please report them in the Issues tab.

## Features

- Lightweight (~1k LoC)
- Easy to use
- Searching in a file

## TODO

- Syntax highlighting
- Filetype detection
- Multiple buffers (open files) support
- Ability to open a shell from inside the editor
- Config file

## Installation

Use the supplied Makefile to compile and install Roku:

```bash
make
sudo make install
```

The default installation path is `/usr/local/bin`. If you want to change that, you can modify the *DEST* variable:

```bash
sudo make install DEST=/path/to/install
```

If you want to uninstall Roku, you can also set a custom *DEST* path. Please note it must be the same path as used to install Roku.

```bash
sudo make uninstall [DEST=/path/to/install]
```

## Usage

```bash
roku [file]
```

## Contributing

Pull requests are welcome. For major changes, please open an issue first
to discuss what you would like to change

## License

Sailfish is licensed under the MIT license, which you can find [here](LICENSE.md).