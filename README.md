# Gnuradio Tools


## Install to user local directory (~/.local)

### Build And Install

Define CMAKE_INSTALL_PREFIX variable when run cmake:

```sh
cmake -DCMAKE_INSTALL_PREFIX=~/.local ..
```

Then make and install as usual.

### Environment Variable

set LD_LIBRARY_PATH、PYTHONPATH environment variable, such as:

```sh
export LD_LIBRARY_PATH="$HOME/.local/lib:$LD_LIBRARY_PATH"
export PYTHONPATH="$HOME/.local/lib/python3.8/dist-packages"
```

LD_LIBRARY_PATH is used to locate .so file.
PYTHONPATH is used to locate python module.

### Gnuradio Companion config

Edit ~/.gnuradio/config.conf and add lines such as:

```ini
[grc]
local_blocks_path=/home/username/.local/share/gnuradio/grc/blocks
```

`local_blocks_path` is used to local grc block file, can contain multiple paths separated by colons, such as:

```ini
local_blocks_path=/path/to/blocks1:/path/to/blocks2
```
