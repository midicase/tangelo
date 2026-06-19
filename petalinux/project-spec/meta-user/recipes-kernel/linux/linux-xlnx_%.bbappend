FILESEXTRAPATHS:prepend := "${THISDIR}/${PN}:"

SRC_URI:append = " file://bsp.cfg \
   file://0001-parallel-memories-do-not-use-size-property.patch"
KERNEL_FEATURES:append = " bsp.cfg"
SRC_URI += "file://user_2026-05-27-17-33-00.cfg \
            file://user_2026-06-19-21-01-00.cfg \
            "

