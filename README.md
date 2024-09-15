# iCIBA

A command-line tool of iCIBA for Linux.

English | [中文](./README_ZH.md)

## Installation

```sh
wget https://github.com/TaipaXu/iCIBA/releases/download/v0.2.0/iciba_0.2.0_x64.deb
sudo apt-get install iciba_0.2.0_x64.deb
```

## Compiling

Before starting the compilation, please make sure you have installed [vcpkg](https://github.com/microsoft/vcpkg). Set the `VCPKG_ROOT` environment variable to point to the directory where `vcpkg` is installed. Add the `vcpkg` path to the system's `PATH` environment variable.

```sh
git clone https://github.com/TaipaXu/iCIBA.git && cd iCIBA
vcpkg install
cmake --preset=linux-release
cmake --build build/release -j $(nproc)
```

## Usage

```sh
$ iciba miss
miss
英[mɪs]  美[mɪs]
n. （用于姓名或姓之前，对未婚女子的称呼）小姐; 女士; 失误
v. 漏掉; 错过（机会）; 思念; 没遇到
复数: misses  第三人称: misses  过去式: missed  过去分词: missed  现在分词: missing
```

```sh
$ iciba -q miss
miss
英[mɪs]  美[mɪs]
n. （用于姓名或姓之前，对未婚女子的称呼）小姐; 女士; 失误
v. 漏掉; 错过（机会）; 思念; 没遇到
复数: misses  第三人称: misses  过去式: missed  过去分词: missed  现在分词: missing
```

![word](./word.gif)

```sh
$ iciba How I miss you!
How I miss you!
我多么想念你！
```

```sh
$ iciba -q How I miss you!
How I miss you!
我多么想念你！
```

![sentence](./sentence.gif)

# License

[GPL-3.0](LICENSE)
