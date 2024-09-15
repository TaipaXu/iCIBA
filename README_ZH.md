# iCIBA

Linux下的iCIBA命令行工具。

[English](./README.md) | 中文

## 安装

```sh
wget https://github.com/TaipaXu/iCIBA/releases/download/v0.2.0/iciba_0.2.0_x64.deb
sudo apt-get install iciba_0.2.0_x64.deb
```

## 编译

在开始编译之前，请确保你已经安装了[vcpkg](https://github.com/microsoft/vcpkg)。设置`VCPKG_ROOT`环境变量，将其指向`vcpkg`的安装目录。将`vcpkg`的路径添加到系统的`PATH`环境变量中。

```sh
git clone https://github.com/TaipaXu/iCIBA.git && cd iCIBA
vcpkg install
cmake --preset=linux-release
cmake --build build/release -j $(nproc)
```

## 使用方法

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

# 许可证

[GPL-3.0](LICENSE)
