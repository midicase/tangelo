# BSP Build Steps

## Prerequisites

Extract the BSP archive:

```
TE0820-te0820-rd-vivado_2024.2-build_1_20250214171345.zip
```

## Install PetaLinux

```bash
tar -xvf FPGAs_AdaptiveSoCs_Unified_2024.2_1113_1001.tar.gz
cd FPGAs_AdaptiveSoCs_Unified_2024.2_1113_1001
./xsetup
```

Select the PetaLinux install and install to `/opt/xilinx`.

## First-time Setup

After cloning, tell git to ignore local changes to the PetaLinux metadata file. `petalinux-config` overwrites `HARDWARE_PATH` on every run with the local machine path, which would otherwise appear as a dirty file on every build.

```bash
git update-index --skip-worktree petalinux/.petalinux/metadata
```

## Build

The `petalinux/` directory in this repo is the PetaLinux project. No scaffolding step is needed.

```bash
source /opt/xilinx/PetaLinux/2024.2/tool/settings.sh
cd petalinux
petalinux-config --get-hw-description=/path/to/supplied.xsa --silentconfig
petalinux-build
```

> **Note:** The default XSA for the reference board is `te0820-rd/prebuilt/hardware/2eg_1i_2gb/te0820-rd_2eg_1i_2gb.xsa` in the zip above.

### Generate BOOT.BIN

```bash
petalinux-package boot \
  --pmufw ./images/linux/pmufw.elf \
  --atf images/linux/bl31.elf \
  --fsbl ./images/linux/zynqmp_fsbl.elf \
  --u-boot images/linux/u-boot.elf \
  --fpga images/linux/system.bit \
  --force
```

The required assets are in `petalinux/images/linux/`.

## Updating the Project

```bash
source /opt/xilinx/PetaLinux/2024.2/tool/settings.sh
cd petalinux
petalinux-config --get-hw-description=/path/to/supplied.xsa
petalinux-build
petalinux-package boot \
  --pmufw ./images/linux/pmufw.elf \
  --atf images/linux/bl31.elf \
  --fsbl ./images/linux/zynqmp_fsbl.elf \
  --u-boot images/linux/u-boot.elf \
  --fpga images/linux/system.bit \
  --force
```

> **PL dependency:** `project-spec/meta-user/recipes-bsp/device-tree/files/system-user.dtsi` references PL peripherals `axi_dma_0` and `PL0_REF` (fclk). If the new XSA removes or renames either, the device tree will fail to compile. Review `system-user.dtsi` whenever the PL design changes.

> **Clean build:** For significant hardware changes (new PL IPs, changed memory map), run a full clean before building to avoid stale Yocto artifacts:
> ```bash
> petalinux-build -x mrproper
> petalinux-build
> ```
