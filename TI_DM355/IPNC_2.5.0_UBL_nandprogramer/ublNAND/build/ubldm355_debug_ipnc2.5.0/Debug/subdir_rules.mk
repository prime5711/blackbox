################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
H:/Source_Debugging/IPNC_2.5.0_UBL_nandprogramer/ublNAND/build/UBL_OBJ/armboot_main.obj: H:/Source_Debugging/IPNC_2.5.0_UBL_nandprogramer/ublNAND/src/armboot_main.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv5/tools/compiler/arm_5.0.1/bin/armcl" --abi=ti_arm9_abi -me -g --include_path="C:/ti/ccsv5/tools/compiler/arm_5.0.1/include" --include_path="C:/ti/xdais_7_21_01_07/packages/ti/xdais" --include_path="H:/Source_Debugging/IPNC_2.5.0_UBL_nandprogramer/ublNAND/include" --define="_DEBUG" --define="QTMODE" --quiet --display_error_number --diag_wrap=off --obj_directory="H:/Source_Debugging/IPNC_2.5.0_UBL_nandprogramer/ublNAND/build/UBL_OBJ" --preproc_with_compile --preproc_dependency="armboot_main.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

H:/Source_Debugging/IPNC_2.5.0_UBL_nandprogramer/ublNAND/build/UBL_OBJ/dm355Init.obj: H:/Source_Debugging/IPNC_2.5.0_UBL_nandprogramer/ublNAND/src/dm355Init.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv5/tools/compiler/arm_5.0.1/bin/armcl" --abi=ti_arm9_abi -me -g --include_path="C:/ti/ccsv5/tools/compiler/arm_5.0.1/include" --include_path="C:/ti/xdais_7_21_01_07/packages/ti/xdais" --include_path="H:/Source_Debugging/IPNC_2.5.0_UBL_nandprogramer/ublNAND/include" --define="_DEBUG" --define="QTMODE" --quiet --display_error_number --diag_wrap=off --obj_directory="H:/Source_Debugging/IPNC_2.5.0_UBL_nandprogramer/ublNAND/build/UBL_OBJ" --preproc_with_compile --preproc_dependency="dm355Init.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

H:/Source_Debugging/IPNC_2.5.0_UBL_nandprogramer/ublNAND/build/UBL_OBJ/nand.obj: H:/Source_Debugging/IPNC_2.5.0_UBL_nandprogramer/ublNAND/src/nand.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv5/tools/compiler/arm_5.0.1/bin/armcl" --abi=ti_arm9_abi -me -g --include_path="C:/ti/ccsv5/tools/compiler/arm_5.0.1/include" --include_path="C:/ti/xdais_7_21_01_07/packages/ti/xdais" --include_path="H:/Source_Debugging/IPNC_2.5.0_UBL_nandprogramer/ublNAND/include" --define="_DEBUG" --define="QTMODE" --quiet --display_error_number --diag_wrap=off --obj_directory="H:/Source_Debugging/IPNC_2.5.0_UBL_nandprogramer/ublNAND/build/UBL_OBJ" --preproc_with_compile --preproc_dependency="nand.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

H:/Source_Debugging/IPNC_2.5.0_UBL_nandprogramer/ublNAND/build/UBL_OBJ/nandboot.obj: H:/Source_Debugging/IPNC_2.5.0_UBL_nandprogramer/ublNAND/src/nandboot.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv5/tools/compiler/arm_5.0.1/bin/armcl" --abi=ti_arm9_abi -me -g --include_path="C:/ti/ccsv5/tools/compiler/arm_5.0.1/include" --include_path="C:/ti/xdais_7_21_01_07/packages/ti/xdais" --include_path="H:/Source_Debugging/IPNC_2.5.0_UBL_nandprogramer/ublNAND/include" --define="_DEBUG" --define="QTMODE" --quiet --display_error_number --diag_wrap=off --obj_directory="H:/Source_Debugging/IPNC_2.5.0_UBL_nandprogramer/ublNAND/build/UBL_OBJ" --preproc_with_compile --preproc_dependency="nandboot.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

H:/Source_Debugging/IPNC_2.5.0_UBL_nandprogramer/ublNAND/build/UBL_OBJ/ublboot.obj: H:/Source_Debugging/IPNC_2.5.0_UBL_nandprogramer/ublNAND/src/ublboot.asm $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv5/tools/compiler/arm_5.0.1/bin/armcl" --abi=ti_arm9_abi -me -g --include_path="C:/ti/ccsv5/tools/compiler/arm_5.0.1/include" --include_path="C:/ti/xdais_7_21_01_07/packages/ti/xdais" --include_path="H:/Source_Debugging/IPNC_2.5.0_UBL_nandprogramer/ublNAND/include" --define="_DEBUG" --define="QTMODE" --quiet --display_error_number --diag_wrap=off --obj_directory="H:/Source_Debugging/IPNC_2.5.0_UBL_nandprogramer/ublNAND/build/UBL_OBJ" --preproc_with_compile --preproc_dependency="ublboot.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


