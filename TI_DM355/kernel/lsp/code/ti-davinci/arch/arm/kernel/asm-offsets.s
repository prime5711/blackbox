	.file	"asm-offsets.c"
	.text
	.align	2
	.global	main
	.type	main, %function
main:
.LFB410:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	mov	ip, sp
.LCFI0:
	stmfd	sp!, {fp, ip, lr, pc}
.LCFI1:
	sub	fp, ip, #4
.LCFI2:
#APP
	
->TSK_ACTIVE_MM #112 offsetof(struct task_struct, active_mm)
	
->
	
->TI_FLAGS #0 offsetof(struct thread_info, flags)
	
->TI_PREEMPT #4 offsetof(struct thread_info, preempt_count)
	
->TI_ADDR_LIMIT #8 offsetof(struct thread_info, addr_limit)
	
->TI_TASK #12 offsetof(struct thread_info, task)
	
->TI_EXEC_DOMAIN #16 offsetof(struct thread_info, exec_domain)
	
->TI_CPU #20 offsetof(struct thread_info, cpu)
	
->TI_CPU_DOMAIN #24 offsetof(struct thread_info, cpu_domain)
	
->TI_CPU_SAVE #28 offsetof(struct thread_info, cpu_context)
	
->TI_USED_CP #76 offsetof(struct thread_info, used_cp)
	
->TI_TP_VALUE #92 offsetof(struct thread_info, tp_value)
	
->TI_FPSTATE #96 offsetof(struct thread_info, fpstate)
	
->TI_VFPSTATE #240 offsetof(struct thread_info, vfpstate)
	
->TI_IWMMXT_STATE #96 (offsetof(struct thread_info, fpstate)+4)&~7
	
->
	
->VMA_VM_MM #0 offsetof(struct vm_area_struct, vm_mm)
	
->VMA_VM_FLAGS #20 offsetof(struct vm_area_struct, vm_flags)
	
->
	
->VM_EXEC #4 VM_EXEC
	
->
	
->PAGE_SZ #4096 PAGE_SIZE
	
->VIRT_OFFSET #-1073741824 PAGE_OFFSET
	
->
	
->SYS_ERROR0 #10420224 0x9f0000
	
->
	
->SIZEOF_MACHINE_DESC #56 sizeof(struct machine_desc)
	mov	r0, #0
	ldmfd	sp, {fp, sp, pc}
.LFE410:
	.size	main, .-main
	.ident	"GCC: (GNU) 3.4.3 (MontaVista 3.4.3-25.0.104.0600975 2006-07-06)"
