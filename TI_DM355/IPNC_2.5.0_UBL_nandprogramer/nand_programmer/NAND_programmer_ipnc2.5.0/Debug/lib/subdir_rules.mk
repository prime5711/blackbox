################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
lib/rts16e.exe: H:/Source_Debugging/IPNC_2.5.0_UBL_nandprogramer/nand_programmer/lib/rts16e.lib $(GEN_CMDS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Linker'
	"C:/ti/ccsv5/tools/compiler/arm_5.0.1/bin/armcl" --abi=ti_arm9_abi -me -g --define="_DEBUG" --quiet --display_error_number --diag_wrap=off --signed_chars --obj_directory="D:/Ti_platform/dm355/src/IPNC/nand_programmer/Debug" -z -m"H:/Source_Debugging/IPNC_2.5.0_UBL_nandprogramer/nand_programmer/lib/Debug/NAND_programmer.map" -i"C:/ti/ccsv5/tools/compiler/arm_5.0.1/lib" -i"C:/ti/ccsv5/tools/compiler/arm_5.0.1/include" -i"H:/Source_Debugging/IPNC_2.5.0_UBL_nandprogramer/nand_programmer/NAND_programmer_ipnc2.5.0" -i"H:/Source_Debugging/IPNC_2.5.0_UBL_nandprogramer/nand_programmer" --reread_libs --warn_sections --display_error_number --diag_wrap=off --rom_model -o "$@" "$<" "../DM355_emif.cmd"
	@echo 'Finished building: $<'
	@echo ' '


