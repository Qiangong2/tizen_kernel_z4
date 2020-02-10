#!/bin/bash
#
ENABLE_CONFIGS=(
CONFIG_DEVTMPFS
CONFIG_DEVTMPFS_MOUNT
CONFIG_FHANDLE
CONFIG_AUTOFS4_FS
CONFIG_EXT4_FS_XATTR
CONFIG_EXT4_FS_POSIX_ACL
CONFIG_SQUASHFS
CONFIG_SQUASHFS_XATTR
CONFIG_SQUASHFS_ZLIB
CONFIG_SQUASHFS_LZO
CONFIG_SQUASHFS_4K_DEVBLK_SIZE
CONFIG_SQUASHFS_FILE_CACHE
CONFIG_SQUASHFS_DECOMP_MULTI_PERCPU
CONFIG_CGROUPS
CONFIG_CGROUP_DEBUG
CONFIG_CGROUP_FREEZER
CONFIG_CGROUP_CPUACCT
CONFIG_RESOURCE_COUNTERS
CONFIG_CGROUP_MEM_RES_CTLR
CONFIG_CGROUP_SCHED
CONFIG_MEMCG_SWAP
CONFIG_MEMCG_SWAP_ENABLED
CONFIG_FAIR_GROUP_SCHED
CONFIG_RT_GROUP_SCHED
CONFIG_BLK_DEV_INITRD
CONFIG_BLK_DEV_RAM
CONFIG_BLK_DEV_CRYPTOLOOP
CONFIG_TMPFS
CONFIG_TMPFS_POSIX_ACL
CONFIG_TMPFS_XATTR
CONFIG_FANOTIFY
CONFIG_GENERIC_ACL
CONFIG_DRM
CONFIG_DRM_KMS_HELPER
CONFIG_SLP_GLOBAL_LOCK
CONFIG_VT
CONFIG_CONSOLE_TRANSLATIONS
CONFIG_HW_CONSOLE
CONFIG_USB_G_SLP
CONFIG_SECURITYFS
CONFIG_SECURITY_SMACK
CONFIG_CMA
CONFIG_CMA_AREAS
CONFIG_CPU_FREQ_DEFAULT_GOV_ONDEMAND
CONFIG_CPU_FREQ_GOV_PERFORMANCE
CONFIG_CPU_FREQ_GOV_ONDEMAND
CONFIG_SYSTEM_LOAD_ANALYZER
CONFIG_PM_AUTOSLEEP
CONFIG_TIZEN_SEC_KERNEL_ENG
CONFIG_PM_SLEEP_HISTORY
CONFIG_PM_SLEEP_MONITOR
CONFIG_SHRINK_MEMORY
CONFIG_ECRYPT_FS
CONFIG_SHRINK_MEMORY
CONFIG_SECURITYFS
CONFIG_SECURITY_SMACK
CONFIG_DEFAULT_SECURITY_SMACK
CONFIG_BLK_DEV_CRYPTOLOOP
CONFIG_NET_CLS_CGROUP
CONFIG_NETFILTER_NETLINK_ACCT
CONFIG_NETFILTER_XT_MATCH_CGROUP
CONFIG_NETFILTER_XT_MATCH_NFACCT
CONFIG_BLK_DEV_CRYPTOLOOP
)

ENABLE_CONFIGS2=(
CONFIG_CMA_ALIGNMENT=4
CONFIG_SQUASHFS_FRAGMENT_CACHE_SIZE=3
CONFIG_BLK_DEV_RAM_SIZE=8192
CONFIG_DEFAULT_SECURITY=\"smack\"
)

ACL_SUPPORT_CONFIGS=(
CONFIG_ANDROID_BINDER_IPC
)

IPSEC_SUPPORT_CONFIGS=(
CONFIG_INET_AH
CONFIG_INET_IPCOMP
CONFIG_INET_XFRM_TUNNEL
CONFIG_CRYPTO_NULL
)

DISABLE_CONFIGS=(
CONFIG_ANDROID_PARANOID_NETWORK
CONFIG_ANDROID_PERSISTENT_RAM
CONFIG_ANDROID_RAM_CONSOLE
CONFIG_ANDROID_TIMED_GPIO
CONFIG_ANDROID_LOW_MEMORY_KILLER
CONFIG_ANDROID_LOW_MEMORY_KILLER_AUTODETECT_OOM_ADJ_VALUES
CONFIG_REED_SOLOMON
CONFIG_REED_SOLOMON_ENC8
CONFIG_REED_SOLOMON_DEC8
CONFIG_CRAMFS
CONFIG_USB_G_ANDROID
CONFIG_USB_ANDROID_SAMSUNG_COMPOSITE
CONFIG_USB_ANDROID_SAMSUNG_MTP
CONFIG_USB_ANDROID_SAMSUNG_SIDESYNC
CONFIG_INITRAMFS_SOURCE
CONFIG_RD_GZIP
CONFIG_DEFAULT_SECURITY_SELINUX
CONFIG_SECURITY_SELINUX
CONFIG_SECURITY_SELINUX_DEVELOP
CONFIG_SECURITY_SELINUX_AVC_STATS
CONFIG_SECURITY_SELINUX_CHECKREQPROT_VALUE
CONFIG_CPU_FREQ_DEFAULT_GOV_INTERACTIVE
CONFIG_HAS_EARLYSUSPEND
CONFIG_EARLYSUSPEND
CONFIG_FB_EARLYSUSPEND
CONFIG_HAS_WAKELOCK
CONFIG_HAS_WAKELOCK_STAT
CONFIG_WAKELOCK
CONFIG_EXT2_FS
CONFIG_EXT3_FS
CONFIG_KSWAPD_NOSWAP
CONFIG_RUNTIME_COMPCACHE
CONFIG_SDCARD_FS
CONFIG_SDCARD_FS_CI_SEARCH
CONFIG_FAT_VIRTUAL_XATTR_SELINUX_LABEL
CONFIG_EXFAT_VIRTUAL_XATTR_SELINUX_LABEL
CONFIG_SDFAT_VIRTUAL_XATTR_SELINUX_LABEL
CONFIG_BENCHMARK
CONFIG_BOOT_PERF
)

DISABLE_CONFIGS2=(
CONFIG_DEFAULT_SECURITY=\"selinux\"
)

ARM=arm
CHIPSET=sc9830
MODEL=${1}
if [ "${MODEL}" == "" ]; then
	DEF_CONFIG=".config"
	echo -e "Config analysis for latest built\n"
else
	DEF_CONFIG="arch/${ARM}/configs/tizen_${MODEL}_defconfig"
	echo -e "Config analysis for ${MODEL}_defconfig\n"
fi

echo -e "Required configs\n"
for CONFIG in "${ENABLE_CONFIGS[@]}"
do
	RESULT=`sed -n "/${CONFIG}=y/p" ${DEF_CONFIG}`
	if [ "${RESULT}" == "${CONFIG}=y" ]; then
		CHECK_RESULT="\033[00;36mOK\033[0m"
	else
		CHECK_RESULT="\033[1;31mNeeded but not found\033[0m"
	fi
	echo -e checking ${CONFIG} ... [${CHECK_RESULT}]
done

for CONFIG in "${ENABLE_CONFIGS2[@]}"
do
	RESULT=`sed -n "/${CONFIG}/p" ${DEF_CONFIG}`
	if [ "${RESULT}" == "${CONFIG}" ]; then
		CHECK_RESULT="\033[00;36mOK\033[0m"
	else
		CHECK_RESULT="\033[1;31mNeeded but not found\033[0m"
	fi
	echo -e checking ${CONFIG} ... [${CHECK_RESULT}]
done

echo -e "\n\n"
echo -e "ACL support configs\n"
for CONFIG in "${ACL_SUPPORT_CONFIGS[@]}"
do
	RESULT=`sed -n "/${CONFIG}=y/p" ${DEF_CONFIG}`
	if [ "${RESULT}" == "${CONFIG}=y" ]; then
		CHECK_RESULT="\033[00;36mOK\033[0m"
	else
		CHECK_RESULT="\033[1;31mNeeded but not found\033[0m"
	fi
	echo -e checking ${CONFIG} ... [${CHECK_RESULT}]
done

echo -e "\n\n"
echo -e "IPSEC support configs\n"
for CONFIG in "${IPSEC_SUPPORT_CONFIGS[@]}"
do
	RESULT=`sed -n "/${CONFIG}=y/p" ${DEF_CONFIG}`
	if [ "${RESULT}" == "${CONFIG}=y" ]; then
		CHECK_RESULT="\033[00;36mOK\033[0m"
	else
		CHECK_RESULT="\033[1;31mNeeded but not found\033[0m"
	fi
	echo -e checking ${CONFIG} ... [${CHECK_RESULT}]
done


echo -e "\n\n"
echo -e "Conflict configs\n"
for CONFIG in "${DISABLE_CONFIGS[@]}"
do
	RESULT=`sed -n "/${CONFIG}=y/p" ${DEF_CONFIG}`
	if [ "${RESULT}" == "${CONFIG}=y" ]; then
		CHECK_RESULT="\033[1;31mShould be turned off\033[0m"
	else
		CHECK_RESULT="\033[00;36mNot found\033[0m"
	fi
	echo -e checking ${CONFIG} ... [${CHECK_RESULT}]
done
for CONFIG in "${DISABLE_CONFIGS2[@]}"
do
	RESULT=`sed -n "/${CONFIG}/p" ${DEF_CONFIG}`
	if [ "${RESULT}" == "${CONFIG}" ]; then
		CHECK_RESULT="\033[1;31mShould be turned off\033[0m"
	else
		CHECK_RESULT="\033[00;36mNot found\033[0m"
	fi
	echo -e checking ${CONFIG} ... [${CHECK_RESULT}]
done