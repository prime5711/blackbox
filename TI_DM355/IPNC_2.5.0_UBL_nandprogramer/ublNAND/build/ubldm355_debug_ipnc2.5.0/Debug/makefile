################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

CG_TOOL_ROOT := C:/ti/ccsv5/tools/compiler/arm_5.0.1

ORDERED_OBJS += \
$(GEN_CMDS__FLAG) \
"H:/Source_Debugging/IPNC_2.5.0_UBL_nandprogramer/ublNAND/build/ublDM355.cmd" \
"H:/Source_Debugging/IPNC_2.5.0_UBL_nandprogramer/ublNAND/build/UBL_OBJ/ublboot.obj" \
"H:/Source_Debugging/IPNC_2.5.0_UBL_nandprogramer/ublNAND/build/UBL_OBJ/nandboot.obj" \
"H:/Source_Debugging/IPNC_2.5.0_UBL_nandprogramer/ublNAND/build/UBL_OBJ/nand.obj" \
"H:/Source_Debugging/IPNC_2.5.0_UBL_nandprogramer/ublNAND/build/UBL_OBJ/dm355Init.obj" \
"H:/Source_Debugging/IPNC_2.5.0_UBL_nandprogramer/ublNAND/build/UBL_OBJ/armboot_main.obj" \

-include ../makefile.init

RM := DEL /F
RMDIR := RMDIR /S/Q

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir_vars.mk
-include subdir_rules.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(S62_DEPS)),)
-include $(S62_DEPS)
endif
ifneq ($(strip $(C64_DEPS)),)
-include $(C64_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(S55_DEPS)),)
-include $(S55_DEPS)
endif
ifneq ($(strip $(C67_DEPS)),)
-include $(C67_DEPS)
endif
ifneq ($(strip $(C??_DEPS)),)
-include $(C??_DEPS)
endif
ifneq ($(strip $(CLA_DEPS)),)
-include $(CLA_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(S??_DEPS)),)
-include $(S??_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(C62_DEPS)),)
-include $(C62_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(K_DEPS)),)
-include $(K_DEPS)
endif
ifneq ($(strip $(C43_DEPS)),)
-include $(C43_DEPS)
endif
ifneq ($(strip $(S67_DEPS)),)
-include $(S67_DEPS)
endif
ifneq ($(strip $(SA_DEPS)),)
-include $(SA_DEPS)
endif
ifneq ($(strip $(S43_DEPS)),)
-include $(S43_DEPS)
endif
ifneq ($(strip $(OPT_DEPS)),)
-include $(OPT_DEPS)
endif
ifneq ($(strip $(S64_DEPS)),)
-include $(S64_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(C55_DEPS)),)
-include $(C55_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: H:/Source_Debugging/IPNC_2.5.0_UBL_nandprogramer/ublNAND/build/ubldm355_debug_ipnc2.5.0/../../build/UBL_OBJ/ublDM350.out

# Tool invocations
H:/Source_Debugging/IPNC_2.5.0_UBL_nandprogramer/ublNAND/build/ubldm355_debug_ipnc2.5.0/../../build/UBL_OBJ/ublDM350.out: $(OBJS) $(CMD_SRCS) $(GEN_CMDS)
	@echo 'Building target: $@'
	@echo 'Invoking: ARM Linker'
	"C:/ti/ccsv5/tools/compiler/arm_5.0.1/bin/armcl" --abi=ti_arm9_abi -me -g --define="_DEBUG" --define="QTMODE" --quiet --display_error_number --diag_wrap=off --obj_directory="H:/Source_Debugging/IPNC_2.5.0_UBL_nandprogramer/ublNAND/build/UBL_OBJ" -z -m"H:/Source_Debugging/IPNC_2.5.0_UBL_nandprogramer/ublNAND/build/UBL_OBJ/ublDM350.map" -i"C:/ti/ccsv5/tools/compiler/arm_5.0.1/lib" -i"C:/ti/ccsv5/tools/compiler/arm_5.0.1/include" -i"H:/Source_Debugging/IPNC_2.5.0_UBL_nandprogramer/ublNAND/build/ubldm355_debug_ipnc2.5.0" -i"H:/Source_Debugging/IPNC_2.5.0_UBL_nandprogramer/ublNAND/build" --reread_libs --warn_sections --display_error_number --diag_wrap=off --entry_point=_c_intDM355 --rom_model -o "H:/Source_Debugging/IPNC_2.5.0_UBL_nandprogramer/ublNAND/build/ubldm355_debug_ipnc2.5.0/../../build/UBL_OBJ/ublDM350.out" $(ORDERED_OBJS)
	@echo 'Finished building target: $@'
	@echo ' '
	$(MAKE) --no-print-directory post-build

# Other Targets
clean:
	-$(RM) $(TMS470_EXECUTABLE_OUTPUTS__QUOTED) "H:\Source_Debugging\IPNC_2.5.0_UBL_nandprogramer\ublNAND\build\UBL_OBJ\ublDM350.out"
	-$(RM) "armboot_main.pp" "dm355Init.pp" "nand.pp" "nandboot.pp" 
	-$(RM) "H:\Source_Debugging\IPNC_2.5.0_UBL_nandprogramer\ublNAND\build\UBL_OBJ\armboot_main.obj" "H:\Source_Debugging\IPNC_2.5.0_UBL_nandprogramer\ublNAND\build\UBL_OBJ\dm355Init.obj" "H:\Source_Debugging\IPNC_2.5.0_UBL_nandprogramer\ublNAND\build\UBL_OBJ\nand.obj" "H:\Source_Debugging\IPNC_2.5.0_UBL_nandprogramer\ublNAND\build\UBL_OBJ\nandboot.obj" "H:\Source_Debugging\IPNC_2.5.0_UBL_nandprogramer\ublNAND\build\UBL_OBJ\ublboot.obj" 
	-$(RM) "ublboot.pp" 
	-@echo 'Finished clean'
	-@echo ' '

post-build:
	-"H:/Source_Debugging/IPNC_2.5.0_UBL_nandprogramer/ublNAND/build/ubldm355_debug_ipnc2.5.0/postBuildStep_Debug.bat"
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY: post-build

-include ../makefile.targets
