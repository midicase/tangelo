Updated BSP build steps:

extract TE0820-te0820-rd-vivado_2024.2-build_1_20250214171345.zip

`tar -xvf FPGAs_AdaptiveSoCs_Unified_2024.2_1113_1001.tar.gz`

`cd FPGAs_AdaptiveSoCs_Unified_2024.2_1113_1001`

`./xsetup`

Pick the petalinux install.

Install to /opt/xilinx

`source /opt/xilinx/PetaLinux/2024.2/tool/settings.sh`

In some working folder:

`petalinux-create --type project --template zynqMP --name te0820`

Copy this project's petalinux/project-spec folder over te0820/project-spec created from above

`cd te0820`
`petalinux-config --get-hw-description=/path/to/supplied.xsa`   

Exit since already configured

Note: The default is te0820-rd/prebuilt/hardware/2eg_1i_2gb/te0820-rd_2eg_1i_2gb.xsa in the zip above if want to test with reference board.


`petalinux-build`

Generate BOOT.BIN

`petalinux-package boot --pmufw ./images/linux/pmufw.elf --atf images/linux/bl31.elf --fsbl ./images/linux/zynqmp_fsbl.elf --u-boot images/linux/u-boot.elf --fpga images/linux/system.bit --force`


The assets needed from this are in images/linux.  Make note of this folder location.



This project can be updated with: 

`source /opt/xilinx/PetaLinux/2024.2/tool/settings.sh
petalinux-config --get-hw-description=/path/to/supplied.xsa
petalinux-build
petalinux-package boot --pmufw ./images/linux/pmufw.elf --atf images/linux/bl31.elf --fsbl ./images/linux/zynqmp_fsbl.elf --u-boot images/linux/u-boot.elf --fpga images/linux/system.bit --force`
























