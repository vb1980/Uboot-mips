# MT7621 U-Boot

基于 U-Boot 2018.09 的 MediaTek MT7621 路由器引导程序，包含 NAND/NMBM、机型 GPIO 配置和网页救援功能。上游通用说明见 [README](README)。

## 机型配置

仓库根目录提供以下构建配置。配置文件存在不代表所有机型均已完成实机验证。

| 配置文件 | 机型 |
| --- | --- |
| `config_rtax53u` | ASUS RT-AX53U |
| `config_rtax54` | ASUS RT-AX54 |
| `config_xd4s` | ASUS ZenWiFi XD4S |
| `config_4gax56` | ASUS 4G-AX56 |
| `config_r3p` | 小米 R3P |
| `config_r6800` | NETGEAR R6800，使用项目 NMBM 闪存布局 |
| `config_rm2100` | Redmi AC2100 / RM2100 |
| `config_hiwifi4` | 极路由 4 增强版 / HC5962 / B70 |
| `config_e8820s` | ZTE E8820S，使用 RT-AX53U 闪存布局 |
| `config_a040wq` | Nokia A-040W-Q，使用项目 NMBM 闪存布局 |
| `config_msg1500x00` | RAISECOM MSG1500 X.00，使用项目 NMBM 闪存布局 |
| `config_a9` | CMCC A9，使用项目 NMBM 闪存布局 |
| `config_c3n` | 小娱 C3 NAND，使用项目 NMBM 闪存布局 |
| `config_h3ctx180x` | H3C TX180X |
| `config_xg1` | XG1，512 MB DDR3 配置 |
| `config_ax18t` | SIM AX18T |
| `config_q20` | JCG Q20 |
| `config_cr660x` | 小米 CR660X |
| `config_bzv` | NETGEAR BZV |
| `config_rx6000` | ZTT RX6000 |
| `config_gax1800` | G-AX1800 |

其他机型选项见根目录 `Kconfig`，硬件资料与适配说明见 [机型硬件配置汇总](docs/router-hardware-inventory.md)。

## 编译

使用 Linux 构建环境，需要主机 C/C++ 编译工具、GNU Make、Python 2.7（本版本镜像打包工具使用）、LZMA 工具和 MIPS 小端交叉工具链。使用 `menuconfig` 时还需要 ncurses 开发库。

当前 `Makefile` 默认交叉编译器为：

```text
/opt/buildroot-gcc492_mips_glibc/usr/bin/mipsel-linux-gcc
```

以 E8820S 为例，在仓库根目录执行：

```sh
cp config_e8820s .config
make olddefconfig
make -j8 spl/u-boot-spl
make -j8
```

先构建 SPL，以满足本仓库干净构建时的 NAND 镜像打包依赖。其他机型将第一条命令中的配置文件替换为对应文件即可；切换机型前可运行 `make clean` 清理旧产物。

如需指定其他工具链，所有 `make` 命令均传入相同的 `CROSS_COMPILE`，例如：

```sh
make CROSS_COMPILE=/path/to/toolchain/bin/mipsel-linux- olddefconfig
make CROSS_COMPILE=/path/to/toolchain/bin/mipsel-linux- -j8 spl/u-boot-spl
make CROSS_COMPILE=/path/to/toolchain/bin/mipsel-linux- -j8
```

最终打包镜像为根目录下的 `u-boot-mt7621.bin`。

## GitHub 自动编译

仓库的 [GitHub Actions 工作流](.github/workflows/build.yml) 在 `master` 分支推送、面向 `master` 的 Pull Request，以及手动启动时编译上表全部机型。每个机型独立构建并上传一个名为 `u-boot-mt7621-机型名` 的 Artifact，内含 `u-boot-mt7621.bin`、构建生成的 `config` 和 `SHA256SUMS`。Artifact 保留 14 天。

启用方法：将工作流提交并推送至 GitHub 的默认分支；在仓库 **Actions** 页面确认 Actions 已启用。推送到 `master` 会自动开始编译；手动编译可进入 **Actions → Build MT7621 U-Boot → Run workflow**。运行结束后，在对应运行页面的 **Artifacts** 下载所需机型的镜像。工作流使用 Ubuntu 22.04，校验并解压仓库中 `toolchains/` 的 Buildroot MIPS 小端工具链，随后用它编译全部机型；GitHub 运行器无需另行下载交叉编译器。

自动编译只验证源码可以生成镜像。刷写前仍须核对机型、NAND 布局和配套系统固件。

## E8820S 适配约定

- CPU：MT7621，880 MHz；DDR3 使用与 RT-AX53U 配置相同的 256 MB、1200 MHz 初始化参数。
- 复位键：GPIO18，低电平触发；WPS：GPIO8，低电平触发。
- 电源灯：GPIO16，低电平点亮。
- 闪存相关配置全部与 RT-AX53U 一致，包括 NAND 驱动、NMBM、环境变量、Factory、固件偏移和升级流程，不使用 E8820S 原厂闪存布局。

采用的项目分区布局：

| 分区 | 起始偏移 | 大小 |
| --- | --- | --- |
| `u-boot` | `0x000000` | 896 KiB |
| `u-boot-env` | `0x0e0000` | 1024 KiB |
| `factory` | `0x1e0000` | 1024 KiB |
| `factory2` | `0x2e0000` | 1024 KiB |
| `firmware` | `0x3e0000` | 剩余可用空间 |

上表为 `nmbm0` 分区布局；U-Boot 自身的环境存储参数沿用 RT-AX53U 的 `CONFIG_ENV_OFFSET=0x80000`、`CONFIG_ENV_SIZE=0x20000`，与表中的 `u-boot-env` 分区不是同一概念。

配套固件及 Factory 数据必须适配上述布局，不能将该镜像视为 E8820S 原厂分区的直接替代品。

## A-040W-Q 适配约定

- CPU：MT7621，DDR3 256 MB；NAND 容量为 128 MB。
- 复位键：GPIO15，低电平触发；WPS：GPIO18，低电平触发。
- 本机无可控电源灯；U-Boot 将 GPIO13 的低电平有效 USB 灯用作启动和救援状态灯。无线灯 GPIO4/GPIO3 不由 U-Boot 控制。
- 闪存使用本项目统一 NMBM 分区和 `0x3e0000` 固件偏移，配置参数与 `config_e8820s` 相同。该布局与原厂分区不同，配套固件必须使用项目布局。

硬件定义参考 OpenWrt 的 `raisecom,msg1500-x-00` 设备树；OpenWrt 将 Nokia A-040W-Q 列为该机型的同硬件名称。本配置尚未经过 A-040W-Q 实机验证。

## MSG1500 X.00 适配约定

`config_msg1500x00` 是 MSG1500 X.00 的独立构建配置，沿用 A-040W-Q 的 MT7621、256 MB DDR3、128 MB NAND 参数及 GPIO 定义：复位键 GPIO15、WPS 键 GPIO18，均为低电平触发；GPIO13 低电平点亮的 USB 灯用作 U-Boot 状态灯。镜像使用本项目统一的 NMBM 分区和 `0x3e0000` 固件偏移，不兼容原厂设备树中的 `0x140000` kernel 分区。尚未完成实机验证。

## 网页救援

上表 21 份机型配置均已启用 `WEBUI_FAILSAFE`、HTTP 服务和救援 DHCP 服务。按住复位键上电可进入网页救援；也可在串口 U-Boot 命令行执行 `httpd`。

1. 电脑通过网线连接路由器 LAN 口，并设置为 `192.168.1.x/24` 网段内的可用地址，例如 `192.168.1.2`。
2. 按住复位键上电，进入网页救援；也可在串口 U-Boot 命令行执行 `httpd`。
3. 浏览器访问 `http://192.168.1.1`，按页面选择对应文件类型并上传与项目布局匹配的镜像。

串口默认参数为 115200 波特率。网页救援包含固件、Bootloader 和 Factory 写入入口，文件类型应与目标分区对应。

## 验证状态

E8820S 已完成交叉编译和镜像打包验证，并核对闪存、DDR 参数与 RT-AX53U 配置一致；尚未完成实机验证。

复位键极性在不同开源设备树中存在差异，目前按低电平实现。DDR 初始化、复位键、指示灯、以太网和网页救援仍需实机确认。

## R6800 适配约定

`config_r6800` 使用 MT7621 880 MHz、256 MB DDR3、128 MB NAND，启用网页救援、HTTP 和 DHCP。复位键 GPIO12、WPS GPIO18 均低电平触发；GPIO5 的低电平 WLAN 灯用作 U-Boot 状态灯。电源灯位于 SX1503 扩展器，本配置不控制该灯。

闪存采用项目统一 NMBM 布局，固件偏移 `0x3e0000`，与原厂 Sercomm 分区不同；配套固件及 Factory 数据须按项目布局准备。编译验证不能代替实机验证。

## CMCC A9 与小娱 C3 NAND

`config_a9` 和 `config_c3n` 已加入 GitHub Actions 自动编译。A9 暂沿用 256 MB DDR3 初始化参数；C3N 按用户提供的规格使用 512 MB DDR3 初始化参数。两者均使用项目 NMBM 闪存布局。闪存布局及启动流程仍需实机核对；编译产物不应在核对前直接刷入设备。

## 其余新增自动编译机型

H3C TX180X、SIM AX18T、JCG Q20、小米 CR660X、NETGEAR BZV、ZTT RX6000 与 G-AX1800 暂沿用 256 MB DDR3 和项目 NMBM 闪存布局；XG1 按用户提供的规格使用 512 MB DDR3。R3P 原有配置也已选用 512 MB DDR3。各机型的实际 DDR、NAND 容量及原厂分区仍需实机核对；自动编译成功不代表镜像可直接刷写。
