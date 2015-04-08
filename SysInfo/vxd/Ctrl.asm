;============================================================================
;
;PCI BIOS, Memory , Interrupt handle VxD
;							 for windows 95/98
;
;  ver 1.0   kashiwano masahiro
;============================================================================

	WIN40COMPAT EQU 1

	PAGE 58,132

    .386p


    .xlist
    include vmm.inc
    .list


;============================================================================
;        V I R T U A L   D E V I C E   D E C L A R A T I O N
;============================================================================

DECLARE_VIRTUAL_DEVICE	SysInfo, 1, 0, PCIDBG_Control, Undefined_Device_ID, UNDEFINED_INIT_ORDER, 0 ,0

;============================================================================
;        PCIDBG_Control
;============================================================================

VxD_LOCKED_CODE_SEG

BeginProc PCIDBG_Control
    Control_Dispatch SYS_DYNAMIC_DEVICE_INIT, Dynamic_Init, sCall
    Control_Dispatch SYS_DYNAMIC_DEVICE_EXIT, Dynamic_Exit, sCall
    Control_Dispatch W32_DEVICEIOCONTROL,     W32_DeviceIOControl, sCall, <ecx, ebx, edx, esi>
    clc
    ret
EndProc PCIDBG_Control


public C Exec_VxD_Int_rap
Exec_VxD_Int_rap proc
	push	dword ptr 1ah
	VmmCall	Exec_VxD_Int
	ret
Exec_VxD_Int_rap endp

VxD_LOCKED_CODE_ENDS

end
