# osfetch

osfetch is a simple system information fetching tool desinged to be minimal, might add custom ascii art, but that can wait

note, the system information realted to Memory and Available Memory accounts for Cached memory as well, so do not be alarmed to see less available memory, in any case, it makes it more accurate in my opinion

install gcc

fedora

```bash
sudo dnf install gcc
```

debian

```bash
sudo apt install gcc

```

to compile run

```bash
chmod +x build.sh
./build.sh
```

then run

```bash
./osfetch
```
