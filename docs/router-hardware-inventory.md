# 仓库涉及机型硬件配置汇总

> 更新时间：2026-09-26
>
> 本文根据本仓库的提交记录、配置文件和公开资料整理。公开资料之间可能存在地区版、批次版差异；`待确认` 表示尚未找到足够可靠的公开来源，不能用于直接选择刷机镜像或修改 DDR/NAND 参数。

## 机型总表

| 仓库名称 | 厂商/零售型号 | SoC/CPU | RAM | 闪存 | 有线网络 | 无线 | 启动/恢复相关 |
|---|---|---|---:|---:|---|---|---|
| G-AX1800 | 型号归属待确认 | MT7621 双核 880 MHz（仓库平台） | 待确认 | 待确认 | MT7621/MT7530 千兆交换机（平台级） | AX1800，芯片待确认 | MT7621 NAND U-Boot，仓库提交明确支持 |
| ZTT RX6000 | ZTT RX6000 | MT7621 双核 880 MHz（高度可能，待实机确认） | 待确认 | 待确认 | 千兆交换机，端口数待确认 | AC/AX 规格待确认 | MT7621 NAND U-Boot，仓库提交明确支持 |
| 小米 R3P | Xiaomi Mi Router 3 Pro | MT7621A，双核 880 MHz | 512 MB（按用户提供规格配置，待实机核对） | NAND，常见 128 MB | 1×WAN + 3×LAN，千兆 | MT7615，2.4/5 GHz 802.11ac | NAND 启动，OpenWrt 有独立设备支持 |
| 网件 R6800 | NETGEAR AC1900 R6800 | MT7621AT，双核 880 MHz | 256 MB | 128 MB（厂商数据表） | 千兆以太网交换机 | 双频 AC1900，3×3 2.4 GHz + 3×3 5 GHz | U-Boot；OpenWrt 有独立设备支持 |
| RM2100 | Xiaomi Redmi Router AC2100 | MT7621，双核 880 MHz | 128 MB（常见公开资料） | 128 MB ESMT NAND | 1×WAN + 3×LAN，千兆 | 2.4/5 GHz 802.11ac，4×4 规格（区域资料有差异） | NAND + NMBM；仓库配置为 MT7621 NAND |
| 小娱 C3 NAND 版 | 小娱 C3（NAND 版） | MT7621（仓库平台，待实机确认） | 512 MB（按用户提供规格配置，待实机核对） | NAND，容量待确认 | 千兆端口布局待确认 | 双频无线，芯片待确认 | 仓库提交明确区分 NAND 版 |
| CMCC A9 | 中国移动 A9 | MT7621（仓库平台，待实机确认） | 待确认 | 待确认 | 待确认 | 双频无线，芯片待确认 | MT7621 NAND U-Boot |
| HiWiFi 4 | 极路由 4 增强版 / HC5962 / B70 | MT7621AT，双核 880 MHz | 256 MB DDR3 | 128 MB NAND | 4×千兆 | MT7603EN + MT7612EN，N300 + AC867 | NAND U-Boot；OpenWrt 官方支持 |
| A-040W-Q | Nokia A-040W-Q / RAISECOM MSG1500 X.00 同硬件型号 | MT7621A，双核 880 MHz | 256 MB DDR3 | 128 MB NAND | 千兆以太网 | MT7615D 双频 802.11ac | NAND U-Boot；本仓库使用统一 NMBM 布局 |
| MSG1500 X.00 | RAISECOM MSG1500 X.00 | MT7621A，双核 880 MHz | 256 MB DDR3 | 128 MB NAND | 千兆以太网 | MT7615D 双频 802.11ac | NAND U-Boot；独立构建配置沿用 A-040W-Q 硬件定义 |
| ASUS RT-AX53U | ASUS RT-AX53U / RT-AX1800U | MT7621AT，双核 880 MHz | 256 MB | 128 MB NAND | 4×千兆 | MT7975 + MT7905，2.4/5 GHz Wi‑Fi 6 | U-Boot，支持 TFTP 恢复 |
| ASUS RT-AX54 | ASUS RT-AX54 / AX1800 系列 | MT7621AT，双核 880 MHz | 256 MB | 128 MB NAND | 1×WAN + 4×LAN（公开设备资料） | 双频 Wi‑Fi 6，具体射频因变体而异 | U-Boot，OpenWrt 有独立设备支持 |
| ASUS ZenWiFi XD4S | ASUS XD4S | MT7621（仓库配置命名推断，待实机确认） | 待确认 | 待确认 | 千兆端口，具体数量待确认 | AX1800 Mesh，射频芯片待确认 | 仓库提供 `config_xd4s` |
| ASUS 4G-AX56 | ASUS 4G-AX56 | MT7621（仓库配置命名推断，待实机确认） | 待确认 | 待确认 | 千兆端口，具体数量待确认 | AX1800 级双频，射频芯片待确认 | 仓库提供 `config_4gax56` |
| ASUS RT-AX54/RT-AX53U 变体 | 可能包括 RT-AX1800S、RT-AX1800HP | MT7621AT | 256 MB（公开 OpenWrt 资料） | 128 MB NAND | 千兆 | Wi‑Fi 6 | 不同硬件变体不可直接共用镜像 |

## 已确认机型

### 小米 Mi Router 3 Pro（R3P）

- SoC：MediaTek MT7621A，双核 MIPS，880 MHz。
- 内存：本仓库按用户提供规格配置 512 MB DDR3；公开资料列为 256 MB，需确认设备批次与实机容量。
- 闪存：NAND；公开启动日志显示常见容量为 128 MB，NAND 页 2048 B、擦除块 128 KiB。
- 网络：MT7530 交换芯片，1 个 WAN、3 个 LAN，均为千兆。
- 无线：MediaTek MT7615，双频 802.11ac，4 根外置天线。
- 备注：OpenWrt 页面显示该设备使用 NMBM/UBI 类 NAND 分区；仓库的 `config_r3p` 采用 `mt7621_nand_rfb` 板级配置。

来源：

- https://openwrt.org/toh/xiaomi/mi_router_3_pro
- https://55bf4b76-a5b9-4058-a97d-c72d8ac6922e.filesusr.com/ugd/7e125d_5bdd4847d907460d94fe24a9d134e061.pdf

### NETGEAR R6800

- SoC：MediaTek MT7621AT，双核 880 MHz。
- 内存：256 MB RAM。
- 闪存：128 MB flash。
- 无线：AC1900，2.4 GHz 和 5 GHz 均为 3×3，支持波束成形。
- 外设：USB 存储访问功能；具体 USB 版本应以拆机或设备树为准。
- 备注：厂商数据表确认内存/闪存，但没有给出完整 NAND 型号；OpenWrt/DeviWiki 可用于补充芯片级资料。

来源：

- https://www.netgear.com/images/datasheet/networking/wifirouter/R6800.pdf
- https://deviwiki.com/wiki/Netgear_R6800
- https://openwrt.org/toh/netgear/r6800

### 小米 Redmi Router AC2100（RM2100）

- SoC：MediaTek MT7621。
- 闪存：公开 OpenWrt 资料通常标为 128 MB ESMT NAND。
- 网络：1 个 WAN + 3 个 LAN，千兆。
- 无线：双频 802.11ac，公开资料对内存和射频配置存在版本差异；仓库设备配置使用 MT7621 NAND 启动链。
- 备注：不要将 Redmi AC2100 与黑色圆柱形 Xiaomi Mi Router AC2100 混为同一硬件；两者刷机镜像和分区布局不同。

来源：

- https://openwrt.org/toh/xiaomi/xiaomi_redmi_router_ac2100
- https://openwrt.org/toh/hwdata/xiaomi/xiaomi_redmi_router_ac2100
- https://git.laboratoryb.org/hurricos/openwrt/commits/commit/6ea87d35c671f50d86cc7f2969e3930e03e3ea31

### ASUS RT-AX53U

- SoC：MediaTek MT7621AT，双核 880 MHz。
- 内存：256 MB。
- 闪存：128 MB NAND。
- 网络：4 个千兆以太网端口（OpenWrt Techdata 记录）。
- 无线：MT7975 + MT7905，2.4/5 GHz 802.11ax，4 根不可拆天线。
- USB：1×USB 2.0。
- 串口：3.3 V，115200/8N1；U-Boot TFTP 恢复。

来源：

- https://openwrt.org/toh/hwdata/asus/asus_rt-ax53u
- https://www.asus.com/uk/networking-iot-servers/wifi-routers/asus-wifi-routers/rt-ax53u/techspec/

### ASUS RT-AX54

- SoC：MT7621AT，双核 880 MHz。
- 内存/闪存：OpenWrt 设备资料标为 256 MB RAM、128 MB NAND。
- 无线：双频 Wi‑Fi 6；RT-AX54、RT-AX54HP、RT-AX1800HP、RT-AX1800S 共享部分软件支持，但不能据此认定硬件完全相同。
- 备注：公开启动日志显示 NAND 为 128 MiB，且使用 NMBM；该信息与仓库的 NAND failsafe 设计相符。

来源：

- https://openwrt.org/toh/asus/rt-ax54
- https://openwrt.org/toh/asus/rt-ax54?s%5B%5D=op
- https://firmware-selector.openwrt.org/?version=23.05.0&target=ramips%2Fmt7621&id=asus_rt-ax54

### HiWiFi 4（极路由 4 增强版 / HC5962 / B70）

- 型号关系：公开 OpenWrt 和社区资料将 HiWiFi 4、HC5962 与 B70 对应为同一设备系列；购买或刷写前仍应核对底部铭牌的 `HC5962`。
- SoC：MediaTek MT7621AT，双核 MIPS 1004Kc，880 MHz。
- 内存：256 MB DDR3；第三方 Bootloader 的 HC5962 专用构建也使用 256 MB DDR3 初始化参数。
- 闪存：128 MB NAND，页大小 2048 B、擦除块 128 KiB。
- 交换机：MT7621 内置 MT7530 千兆交换机；公开硬件数据库记录 4 个千兆网口，通常为 1×WAN + 3×LAN。
- 2.4 GHz：MediaTek MT7603EN，2×2 802.11b/g/n，最高 300 Mbit/s。
- 5 GHz：MediaTek MT7612EN，2×2 802.11a/n/ac，最高 867 Mbit/s。
- USB：1×USB 2.0、1×USB 3.0。
- 人机接口：公开硬件数据库记录 2 个 LED、1 个按键；HC5962 专用 Breed 资料将复位键标为 GPIO 18。
- 串口：具备串口；HC5962 专用 Breed 使用 115200 波特率。电平和引脚顺序应在连接前用万用表确认，不要直接接入 RS-232 电平。
- Bootloader：原机使用 U-Boot；OpenWrt 自 18.06 起正式支持，目标为 `ramips/mt7621`，设备 ID 为 `hiwifi_hc5962`。
- OpenWrt 旧版镜像定义：NAND block size 128 KiB、page size 2048 B、kernel 区 2 MiB，并生成独立的 factory、sysupgrade 和 initramfs 镜像。
- 镜像兼容性：HC5961、HC5962 名称相近但硬件并不相同；本机必须使用 `hiwifi_hc5962` 镜像。第三方 Breed 也必须选择 `breed-mt7621-hiwifi-hc5962.bin`。
- 校准数据：刷写或擦除前应完整备份 Factory/EEPROM、MAC 地址及原始 NAND；不要用其他 MT7621 设备的 Factory 分区替换。

来源：

- https://openwrt.org/toh/hwdata/hiwifi_gee/hiwifi_gee_hc5962
- https://firmware-selector.openwrt.org/?id=hiwifi_hc5962&target=ramips%2Fmt7621&version=23.05.5
- https://github.com/usnistgov/pscr-openwrt-fork/blob/master/target/linux/ramips/image/mt7621.mk
- https://lists.infradead.org/pipermail/lede-commits/2025-June/025789.html
- https://www.anywlan.com/xiazai_3352.html
- https://github.com/wc7086/breed

## 公开资料不足的机型

### G-AX1800

仓库提交明确添加了该型号，但没有在仓库中提供独立 DTS 或完整板级目录；目前只能确认它被纳入 MT7621 NAND U-Boot 体系。建议通过串口启动日志确认：CPU 频率、DDR 容量、NAND ID、交换机端口映射、无线 PCIe 芯片和 GPIO LED 定义。

### ZTT RX6000

公开网页中存在多个同名或相近型号，尚未找到能同时确认 SoC、内存、闪存和 PCB 版本的可靠一手资料。仓库提交说明它是 MT7621 目标设备之一，但不应仅凭型号名选择 R3P、R6800 或 RM2100 镜像。

### 小娱 C3 NAND 版

仓库提交明确称为“C3 nand版”，说明至少存在 NOR/NAND 硬件差异。当前公开资料不足以确认 NAND 厂商、容量、坏块管理区域和 DDR 颗粒。刷写前必须读取 NAND ID，并核对 U-Boot 环境偏移、NMBM 起始块和 kernel/UBI 分区边界。

### CMCC A9

中国移动 A9 存在运营商定制批次，公开资料容易把不同代际硬件混在一起。仓库仅能确认其适配 MT7621 NAND 方案；RAM、NAND 容量、无线芯片和网口布局应以拆机照片、串口日志和原厂设备树为准。

### ASUS ZenWiFi XD4S 与 4G-AX56

仓库中的 `config_xd4s`、`config_4gax56` 使用 MT7621 NAND 板级配置，但这只能说明启动代码复用，不能证明两者硬件完全相同。尤其 4G-AX56 还可能包含蜂窝基带、SIM 接口、电源和 USB/PCIe 资源差异；XD4S 也可能有不同区域包和 Mesh 节点版本。

## 与本仓库刷机相关的结论

1. 多数目标共享 MT7621/MIPS32r2，但 **SoC 相同不等于镜像兼容**；GPIO、DDR 参数、NAND ID、分区布局和无线校准区都可能不同。
2. 仓库的 failsafe 代码会提供 NVRAM、Factory、Factory2 擦除入口。Factory/Factory2 往往包含 MAC 地址、无线校准和地区参数，擦除前必须有备份。
3. 对 NAND 机型，应优先记录 `nand info`、NMBM 状态和 MTD 分区；不要直接套用 NOR 机型的偏移地址。
4. 对资料不足的机型，建议把实机采集结果补充到本文：串口日志、`bdinfo`、NAND ID、DDR 容量、以太网 PHY 地址、PCIe 无线设备 ID 和完整分区表。

## 仓库依据

- `config_r3p`
- `config_rm2100`
- `config_rtax53u`
- `config_rtax54`
- `config_xd4s`
- `config_4gax56`
- Git 提交：`添加G-AX1800`、`添加ZTT RX6000`、`添加小米R3P`、`添加网件R6800`、`添加RM2100`、`添加小娱C3nand版`、`添加CMCCA9`

HiWiFi 4 已加入 `CONFIG_HIWIFI4` 和 `config_hiwifi4` 构建配置。该适配按项目约定使用统一的 NMBM 分区布局和 failsafe 文件类型，不兼容 HC5962 原厂分区表；首次写入前必须完整备份原厂 NAND、Factory 与 bdinfo。


## ZTE E8820S 构建配置

新增 `CONFIG_E8820S` 和 `config_e8820s`，适用于 MT7621 NAND 版 E8820S。
CPU 880 MHz，DDR3 采用与 `config_rtax53u` 相同的 256 MB、1200 MHz 初始化参数。

- 电源灯：GPIO16，低电平点亮；WPS：GPIO8，低电平触发。
- 复位键：GPIO18，按用户选择采用低电平触发，按住复位键上电进入网页救援。
  本地 ImmortalWrt 和 Padavan 均定义为低电平，LEDE 定义为高电平，仍需实机验证。
- 闪存相关配置全部沿用 RT-AX53U，包括 NAND 驱动、NMBM、环境变量、Factory、固件偏移和升级路径；不采用 E8820S 原厂分区或原厂 MAC 偏移。
- 项目分区：`896k(u-boot),1024k(u-boot-env),1024k(factory),1024k(factory2),-(firmware)`，固件起始地址 `0x3e0000`。
- 网页救援沿用项目现有实现，地址 `192.168.1.1`。此镜像要求配套的 RT-AX53U 风格闪存布局，不能按 E8820S 原厂分区直接使用。

在仓库目录构建：

```sh
cp config_e8820s .config
make olddefconfig
make -j8 spl/u-boot-spl
make -j8
```

输出为 `u-boot-mt7621.bin`。首次干净构建显式生成 SPL，以满足本仓库镜像打包依赖。
GPIO 参考：[LEDE E8820S 设备树](https://github.com/coolsnowwolf/lede/blob/master/target/linux/ramips/dts/mt7621_zte_e8820s.dts)，并与本地 ImmortalWrt、Padavan 的 E8820S 定义交叉核对。
编译通过不代表硬件验证；DDR 初始化、按键、灯和网络救援仍需实机测试。


## Nokia A-040W-Q

OpenWrt 将 A-040W-Q 列为 RAISECOM MSG1500 X.00 的同硬件型号。设备树定义了 GPIO15 低电平复位键、GPIO18 低电平 WPS 键、GPIO13 低电平 USB 灯，以及 GPIO4/GPIO3 无线灯。项目中的 `CONFIG_A040WQ` 使用 USB 灯作为 U-Boot 状态灯。

`config_a040wq` 使用 256 MB DDR3、128 MB NAND 的 MT7621 NAND 配置。闪存沿用仓库统一的 NMBM 分区，固件偏移为 `0x3e0000`；原厂设备树的 `0x140000` kernel 分区不适用于此镜像。配套系统镜像必须采用相同布局。当前仅完成编译验证，按键和灯需实机确认。

来源：[OpenWrt A-040W-Q 同硬件型号提交](https://git.openwrt.org/?p=openwrt/openwrt.git;a=commit;h=4f9b360f0b9a85202422ef07ee573eeca06d11ab)，[OpenWrt MSG1500 设备树](https://github.com/openwrt/openwrt/blob/main/target/linux/ramips/dts/mt7621_raisecom_msg1500-x-00.dts)。


## RAISECOM MSG1500 X.00

新增 `CONFIG_MSG1500X00` 和 `config_msg1500x00`。依据 OpenWrt 的 `mt7621_raisecom_msg1500-x-00.dts`，它与 A-040W-Q 使用相同的 GPIO15 复位键、GPIO18 WPS 键、GPIO13 USB 灯定义。构建配置沿用项目 NMBM 分区与 `0x3e0000` 固件偏移，并启用网页救援、HTTP 和 DHCP。镜像须搭配相同分区布局的系统固件；GPIO 和启动流程仍需实机验证。

来源：[OpenWrt MSG1500 X.00 设备树](https://github.com/openwrt/openwrt/blob/main/target/linux/ramips/dts/mt7621_raisecom_msg1500-x-00.dts)。

### R6800 项目配置

新增 `CONFIG_R6800`、`config_r6800` 和自动编译入口。复位 GPIO12、WPS GPIO18，GPIO5 WLAN 灯用作状态灯，均低电平有效；SX1503 扩展器上的电源灯未接入 U-Boot。采用统一 NMBM 分区与 `0x3e0000` 固件偏移，不使用原厂 Sercomm 分区。硬件定义参考本地 OpenWrt/ImmortalWrt 的 `mt7621_netgear_r6800.dts` 及 `mt7621_netgear_sercomm_bzv.dtsi`。尚未实机验证。

## 自动编译配置补充（2026-09-27）

新增 `config_h3ctx180x`、`config_xg1`、`config_ax18t`、`config_q20`、`config_cr660x`、`config_bzv`、`config_rx6000` 和 `config_gax1800`，并加入 GitHub Actions。XG1、C3N、R3P 按用户提供规格选用 512 MB DDR3 初始化；其余新增配置暂沿用 256 MB DDR3 和项目 NMBM 布局。这些配置用于自动编译，硬件参数和原厂分区尚需实机核对。
