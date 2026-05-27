FILESEXTRAPATHS:prepend := "${THISDIR}/${PN}:"

#mki: this override only works if meta-user layer has higher priority than meta-xilinx-tools
SRC_URI += " file://0001-te-install-general-hooks-zynqmp.patch \
             file://git/lib/sw_apps/zynqmp_fsbl/src"

# SOURCE_DATE_EPOCH is injected by base.bbclass from the pinned embeddedsw git
# commit timestamp (Nov 5 2024), causing GCC to freeze __DATE__/__TIME__ in the
# FSBL banner regardless of when the build actually runs.  Unset it here so the
# banner reflects the real build date.
do_compile:prepend() {
    unset SOURCE_DATE_EPOCH
}
            