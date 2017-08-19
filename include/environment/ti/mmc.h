/*
 * Copyright (C) 2017 Texas Instruments Incorporated - http://www.ti.com
 *
 * Environment variable definitions for MMC/SD on TI boards.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef __TI_MMC_H
#define __TI_MMC_H

#define DEFAULT_MMC_TI_ARGS \
	"mmcdev=0\0" \
	"mmcrootfstype=ext4 rootwait\0" \
	"finduuid=part uuid mmc ${bootpart} uuid\0" \
	"args_mmc=run finduuid;setenv bootargs console=${console} " \
		"${optargs} " \
		"root=PARTUUID=${uuid} rw " \
		"rootfstype=${mmcrootfstype}\0" \
	"loadbootscript=load mmc ${mmcdev} ${loadaddr} boot.scr\0" \
	"bootscript=echo Running bootscript from mmc${mmcdev} ...; " \
		"source ${loadaddr}\0" \
	"bootenvfile=uEnv.txt\0" \
	"importbootenv=echo Importing environment from mmc${mmcdev} ...; " \
		"env import -t ${loadaddr} ${filesize}\0" \
	"loadbootenv=if fatload mmc ${mmcdev} ${loadaddr} ${bootdir}/${bootenvfile}; then echo Found ${bootdir}/${bootenvfile} in FAT partition; else load mmc ${mmcdev} ${loadaddr} ${bootdir}/${bootenvfile}; fi\0" \
	"loadimage=if test -n ${uname_r}; then load ${devtype} ${bootpart} ${loadaddr} ${bootdir}/vmlinuz-${uname_r}; run loadrd; else load ${devtype} ${bootpart} ${loadaddr} ${bootdir}/${bootfile}\0" \
	"loadrd=load ${devtype} ${bootpart} ${rdaddr} ${bootdir}/initrd.img-${uname_r}; setenv rdsize ${filesize}\0" \
	"loadfdt=if test -n ${uname_r}; load ${devtype} ${bootpart} ${fdtaddr} ${bootdir}/dtbs/${uname_r}/${fdtfile}; else load ${devtype} ${bootpart} ${fdtaddr} ${bootdir}/${fdtfile}; fi;\0" \
	"envboot=mmc dev ${mmcdev}; " \
		"if mmc rescan; then " \
			"echo SD/MMC found on device ${mmcdev};" \
			"if run loadbootscript; then " \
				"run bootscript;" \
			"else " \
				"if run loadbootenv; then " \
					"echo Loaded env from ${bootenvfile};" \
					"run importbootenv;" \
				"fi;" \
				"if test -n $uenvcmd; then " \
					"echo Running uenvcmd ...;" \
					"run uenvcmd;" \
				"fi;" \
			"fi;" \
		"fi;\0" \
	"mmcloados=run args_mmc; " \
		"if test ${boot_fdt} = yes || test ${boot_fdt} = try; then " \
			"if run loadfdt; then " \
				"if test -n ${uname_r}; then " \
					"bootz ${loadaddr} ${rdaddr}:${rdsize} ${fdtaddr}; " \
				"else " \
					"bootz ${loadaddr} - ${fdtaddr}; " \
				"fi; " \
			"else " \
				"if test ${boot_fdt} = try; then " \
					"bootz; " \
				"else " \
					"echo WARN: Cannot load the DT; " \
				"fi; " \
			"fi; " \
		"else " \
			"bootz; " \
		"fi;\0" \
	"mmcboot=mmc dev ${mmcdev}; " \
		"setenv devnum ${mmcdev}; " \
		"setenv devtype mmc; " \
		"if mmc rescan; then " \
			"echo SD/MMC found on device ${mmcdev};" \
			"if run loadimage; then " \
				"if test ${boot_fit} -eq 1; then " \
					"run loadfit; " \
				"else " \
					"run mmcloados;" \
				"fi;" \
			"fi;" \
		"fi;\0"

#endif /* __TI_MMC_H */
