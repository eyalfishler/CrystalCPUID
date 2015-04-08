/*************************************************

PCI BIOS, Memory , Interrupt handle VxD
							 for windows 95/98

  ver 1.0   kashiwano masahiro
  ver 1.1   ï°êîÇÃäÑÇËçûÇ›ÉnÉìÉhÉãÇâ¬î\Ç…Ç∑ÇÈ
*************************************************/

#define WIN40COMPAT   1
#define WANTVXDWRAPS

#include <basedef.h>
#include <vmm.h>

#pragma VxD_LOCKED_CODE_SEG
#pragma VxD_LOCKED_DATA_SEG

#include <vwin32.h>
#include <vpicd.h>
#include <vxdwraps.h>
#include <winerror.h>
#include <stddef.h>


#include "ntdefine.h"
#include "..\pcidef.h"
#include "..\pciioctl.h"

typedef DIOCPARAMETERS *LPDIOC;

#define pciGetBus(BXreg)		((BXreg>>8)&0xff)

#define N_ISR	16
HIRQ	irqHandle[N_ISR] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int		isr_flag[N_ISR] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0} ;


DWORD _stdcall W32_DeviceIOControl(DWORD, DWORD, DWORD, LPDIOC);

DWORD _stdcall Dynamic_Init(void)
{
    return(VXD_SUCCESS);
}

DWORD _stdcall Dynamic_Exit(void)
{
    return(VXD_SUCCESS);
}

DWORD _stdcall CleanUp(void)
{
    return(VXD_SUCCESS);
}

extern void Exec_VxD_Int_rap(void);
#define CallPciBios() Exec_VxD_Int_rap()

/*
void CallPciBios()
{
	Exec_VxD_Int_rap();
//	_asm push dword ptr 1ah;
//	VMMCall(Exec_VxD_Int);
}
*/

void isrprototype(x) 
{
	VPICD_Phys_EOI(irqHandle[x]);
	VPICD_Physically_Mask(irqHandle[x]);
	isr_flag[x] = 1;
	_asm clc
}

void isr0(void) {isrprototype(0);}
void isr1(void) {isrprototype(1);}
void isr2(void) {isrprototype(2);}
void isr3(void) {isrprototype(3);}
void isr4(void) {isrprototype(4);}
void isr5(void) {isrprototype(5);}
void isr6(void) {isrprototype(6);}
void isr7(void) {isrprototype(7);}
void isr8(void) {isrprototype(8);}
void isr9(void) {isrprototype(9);}
void isr10(void) {isrprototype(10);}
void isr11(void) {isrprototype(11);}
void isr12(void) {isrprototype(12);}
void isr13(void) {isrprototype(13);}
void isr14(void) {isrprototype(14);}
void isr15(void) {isrprototype(15);}

ULONG isr_table[] = {
	(ULONG)isr0,
	(ULONG)isr1,
	(ULONG)isr2,
	(ULONG)isr3,
	(ULONG)isr4,
	(ULONG)isr5,
	(ULONG)isr6,
	(ULONG)isr7,
	(ULONG)isr8,
	(ULONG)isr9,
	(ULONG)isr10,
	(ULONG)isr11,
	(ULONG)isr12,
	(ULONG)isr13,
	(ULONG)isr14,
	(ULONG)isr15
};

NTSTATUS
pciConfigRead(ULONG pci_address, ULONG offset, char *data, int len);

NTSTATUS
pciConfigWrite(ULONG pci_address, ULONG offset, char *data, int len);

NTSTATUS
PciIoctlBiosStatus(	PCIDEBUG_PCIBIOSSTATUS *buf, int size, ULONG *lpcbBytesReturned );

NTSTATUS
PciIoctlReadConf(
				void *lpInBuffer, 
			    ULONG nInBufferSize, 
				void *lpOutBuffer, 
				ULONG nOutBufferSize, 
				ULONG *lpBytesReturned);
NTSTATUS
PciIoctlWriteConf(
				void *lpInBuffer, 
			    ULONG nInBufferSize, 
				void *lpOutBuffer, 
				ULONG nOutBufferSize, 
				ULONG *lpBytesReturned);

NTSTATUS
PciIoctlReadMem(
				void *lpInBuffer, 
			    ULONG nInBufferSize, 
				void *lpOutBuffer, 
				ULONG nOutBufferSize, 
				ULONG *lpBytesReturned);
NTSTATUS
PciIoctlWriteMem(
				void *lpInBuffer, 
			    ULONG nInBufferSize, 
				void *lpOutBuffer, 
				ULONG nOutBufferSize, 
				ULONG *lpBytesReturned);

NTSTATUS
PciIoctlCopyMem(
				void *lpInBuffer, 
			    ULONG nInBufferSize, 
				void *lpOutBuffer, 
				ULONG nOutBufferSize, 
				ULONG *lpBytesReturned);

NTSTATUS
PciIoctlFillMem(
				void *lpInBuffer, 
			    ULONG nInBufferSize, 
				void *lpOutBuffer, 
				ULONG nOutBufferSize, 
				ULONG *lpBytesReturned);


/****************************************************************************
                  CVXD_W32_DeviceIOControl
****************************************************************************/
DWORD _stdcall W32_DeviceIOControl(DWORD  dwService,
                                        DWORD  dwDDB,
                                        DWORD  hDevice,
                                        LPDIOC lpDIOCParms)
{
    DWORD dwRetVal = 0;
	PCIDEBUG_ISRINFO* isrinfo;
	int		count;
	int		index;
	VID InterDesc;

	switch( dwService) {
	case DIOC_OPEN:
        dwRetVal = 0;
		break;
    case DIOC_CLOSEHANDLE:
        dwRetVal = CleanUp();
		break;
	case IOCTL_PCI_READ_CONF:
        dwRetVal = PciIoctlReadConf(
			(void *)lpDIOCParms->lpvInBuffer,
			lpDIOCParms->cbInBuffer,
			(void *)lpDIOCParms->lpvOutBuffer,
			lpDIOCParms->cbOutBuffer,
			&lpDIOCParms->lpcbBytesReturned
			);
		break;
	case IOCTL_PCI_WRITE_CONF:
        dwRetVal = PciIoctlWriteConf(
			(void *)lpDIOCParms->lpvInBuffer,
			lpDIOCParms->cbInBuffer,
			(void *)lpDIOCParms->lpvOutBuffer,
			lpDIOCParms->cbOutBuffer,
			&lpDIOCParms->lpcbBytesReturned
			);
		break;
	case IOCTL_PCI_PCIBIOSSTATUS:
        dwRetVal = PciIoctlBiosStatus(
			(PCIDEBUG_PCIBIOSSTATUS *)lpDIOCParms->lpvOutBuffer,
			lpDIOCParms->cbOutBuffer,
			&lpDIOCParms->lpcbBytesReturned
			);
		break;
	case IOCTL_PCI_ENABLEISR:
		index = *(int*)lpDIOCParms->lpvInBuffer;
		if(index <0 || index >=N_ISR){
	        dwRetVal = STATUS_INVALID_PARAMETER;
			break;
		}
		if(irqHandle[index]){
			VPICD_Physically_Unmask(irqHandle[index]);
		}
		break;
	case IOCTL_PCI_SETUPISR:
		count = 0;
		isrinfo = (PCIDEBUG_ISRINFO*)lpDIOCParms->lpvInBuffer;
		if(isrinfo->index <0 || isrinfo->index >=N_ISR){
	        dwRetVal = STATUS_INVALID_PARAMETER;
			break;
		}
		if(irqHandle[isrinfo->index]){
			VPICD_Force_Default_Behavior(irqHandle[isrinfo->index]);
		}
		if(isrinfo->eventname[0] == 0) break; // only remove irq handler if no event name

		InterDesc.VID_IRQ_Number = isrinfo->ilevel;
	 	InterDesc.VID_Options = VPICD_OPT_READ_HW_IRR |VPICD_OPT_CAN_SHARE;                 // INIT<0>
		InterDesc.VID_Hw_Int_Proc = isr_table[isrinfo->index];
		InterDesc.VID_Virt_Int_Proc = 0;            // INIT<0>
		InterDesc.VID_EOI_Proc = 0;                 // INIT<0>
		InterDesc.VID_Mask_Change_Proc = 0;         // INIT<0>
		InterDesc.VID_IRET_Proc = 0;                // INIT<0>
		InterDesc.VID_IRET_Time_Out = 500;            // INIT<500>
		InterDesc.VID_Hw_Int_Ref = 0;               // new for 4.0
		isr_flag[isrinfo->index] = 0;
		irqHandle[isrinfo->index] = VPICD_Virtualize_IRQ(&InterDesc);
		if(irqHandle[isrinfo->index]){
			VPICD_Physically_Unmask(irqHandle[isrinfo->index]);
		}

		break;

	case IOCTL_PCI_READ_MEM:
        dwRetVal = PciIoctlReadMem(
			(void *)lpDIOCParms->lpvInBuffer,
			lpDIOCParms->cbInBuffer,
			(void *)lpDIOCParms->lpvOutBuffer,
			lpDIOCParms->cbOutBuffer,
			&lpDIOCParms->lpcbBytesReturned
			);
		break;

	case IOCTL_PCI_WRITE_MEM:
        dwRetVal = PciIoctlWriteMem(
			(void *)lpDIOCParms->lpvInBuffer,
			lpDIOCParms->cbInBuffer,
			(void *)lpDIOCParms->lpvOutBuffer,
			lpDIOCParms->cbOutBuffer,
			&lpDIOCParms->lpcbBytesReturned
			);
		break;

	case IOCTL_PCI_COPY_MEM:
        dwRetVal = PciIoctlCopyMem(
			(void *)lpDIOCParms->lpvInBuffer,
			lpDIOCParms->cbInBuffer,
			(void *)lpDIOCParms->lpvOutBuffer,
			lpDIOCParms->cbOutBuffer,
			&lpDIOCParms->lpcbBytesReturned
			);
		break;

	case IOCTL_PCI_FILL_MEM:
        dwRetVal = PciIoctlFillMem(
			(void *)lpDIOCParms->lpvInBuffer,
			lpDIOCParms->cbInBuffer,
			(void *)lpDIOCParms->lpvOutBuffer,
			lpDIOCParms->cbOutBuffer,
			&lpDIOCParms->lpcbBytesReturned
			);
		break;

	case IOCTL_WAIT_INTERRUPT:
		index = *(int*)lpDIOCParms->lpvInBuffer;
		if(index <0 || index >=N_ISR){
	        dwRetVal = STATUS_INVALID_PARAMETER;
			break;
		}
		count = 0;
		while(1){
			if( isr_flag[index]) {
				isr_flag[index] = 0;
				*((DWORD*)lpDIOCParms->lpvOutBuffer) = 1;
				break;
			}
			_asm mov eax, dword ptr 1
			VMMCall(Time_Slice_Sleep);
			count++;
			if(count>100) {
				*((DWORD*)lpDIOCParms->lpvOutBuffer) = 0; // time out 100ms
				break;
			}
		}
		break;


	case IOCTL_PCI_READ_PORT_UCHAR:
    case IOCTL_PCI_READ_PORT_USHORT:
    case IOCTL_PCI_READ_PORT_ULONG:
	case IOCTL_PCI_WRITE_PORT_UCHAR:
	case IOCTL_PCI_WRITE_PORT_USHORT:
	case IOCTL_PCI_WRITE_PORT_ULONG:
	default:
        dwRetVal = ERROR_NOT_SUPPORTED;
    }
    return(dwRetVal);
}

NTSTATUS
PciIoctlReadMem(
				void *lpInBuffer, 
			    ULONG nInBufferSize, 
				void *lpOutBuffer, 
				ULONG nOutBufferSize, 
				ULONG *lpBytesReturned)
{
	PCIDEBUG_MEMREAD_INPUT *read_param;
	ULONG	size;
	PVOID	maped;
	BOOLEAN	err;
	int i;
    if ( nInBufferSize != sizeof(PCIDEBUG_MEMREAD_INPUT) )
    {
        return STATUS_INVALID_PARAMETER;
    }
	read_param	= (PCIDEBUG_MEMREAD_INPUT *) lpInBuffer;
	size = read_param->unitsize * read_param->count;

    if ( nOutBufferSize < size )
    {
        return STATUS_INVALID_PARAMETER;
    }
    maped = _MapPhysToLinear(read_param->address, size, 0);
	err = FALSE;
	switch(read_param->unitsize){
		case 1:
			for(i=0;i<read_param->count;i++){
				 ((UCHAR*)lpOutBuffer)[i] = ((UCHAR*)maped)[i] ;
			}
		break;
		case 2:
			for(i=0;i<read_param->count;i++){
				((USHORT*)lpOutBuffer)[i] = ((USHORT*)maped)[i];
			}
		break;
		case 4:
			for(i=0;i<read_param->count;i++){
				((ULONG*)lpOutBuffer)[i] = ((ULONG*)maped)[i];
			}
		break;
	default:
		err = TRUE;
	}


	if(err) return STATUS_INVALID_PARAMETER;

    *lpBytesReturned = nOutBufferSize;

    return STATUS_SUCCESS;
}

NTSTATUS
PciIoctlWriteMem(
				void *lpInBuffer, 
			    ULONG nInBufferSize, 
				void *lpOutBuffer, 
				ULONG nOutBufferSize, 
				ULONG *lpBytesReturned)
{
	PCIDEBUG_MEMWRITE_INPUT *write_param;
	ULONG size;
	PVOID	maped;
	BOOLEAN	err;
	int i;

    if ( nInBufferSize < offsetof(PCIDEBUG_MEMWRITE_INPUT,data) )
    {
        return STATUS_INVALID_PARAMETER;
    }

    write_param = (PCIDEBUG_MEMWRITE_INPUT *)lpInBuffer;

	size = write_param->unitsize * write_param->count;
	if ( nInBufferSize < size + offsetof(PCIDEBUG_MEMWRITE_INPUT, data))
	{
        return STATUS_INVALID_PARAMETER;
    }

    maped = _MapPhysToLinear(write_param->address, size, 0);

	err = FALSE;
	switch(write_param->unitsize){
		case 1:
			for(i=0;i<write_param->count;i++){
				 ((UCHAR*)maped)[i] = ((UCHAR*)write_param->data)[i];
			}
		break;
		case 2:
			for(i=0;i<write_param->count;i++){
				 ((USHORT*)maped)[i] = ((USHORT*)write_param->data)[i];
			}
		break;
		case 4:
			for(i=0;i<write_param->count;i++){
				 ((ULONG*)maped)[i] = ((ULONG*)write_param->data)[i];
			}
		break;
		default:
		err = TRUE;
	}

	if(err) return STATUS_INVALID_PARAMETER;

    *lpBytesReturned = 0;

    return STATUS_SUCCESS;
}

NTSTATUS
PciIoctlCopyMem(
				void *lpInBuffer, 
			    ULONG nInBufferSize, 
				void *lpOutBuffer, 
				ULONG nOutBufferSize, 
				ULONG *lpBytesReturned)

{
	PCIDEBUG_MEMCOPY_INPUT *copy_param;
	ULONG size;
	int i;
	PVOID	src_maped, dest_maped;
	BOOLEAN	err;

    if ( nInBufferSize < sizeof(PCIDEBUG_MEMCOPY_INPUT) )
    {
        return STATUS_INVALID_PARAMETER;
    }

    copy_param = (PCIDEBUG_MEMCOPY_INPUT *)lpInBuffer;
	size = copy_param->unitsize * copy_param->count;

    src_maped = _MapPhysToLinear(copy_param->src_address, size, 0);
    dest_maped = _MapPhysToLinear(copy_param->dest_address, size, 0);

	err = FALSE;
	switch(copy_param->unitsize){
		case 1:
			for(i=0;i<copy_param->count;i++){
				((UCHAR*)dest_maped)[i] = ((UCHAR*)src_maped)[i];
			}
		break;
		case 2:
			for(i=0;i<copy_param->count;i++){
				((USHORT*)dest_maped)[i] = ((USHORT*)src_maped)[i];
			}
		break;
		case 4:
			for(i=0;i<copy_param->count;i++){
				((ULONG*)dest_maped)[i] = ((ULONG*)src_maped)[i];
			}
		break;
		default:
		err = TRUE;
	}

	if(err) return STATUS_INVALID_PARAMETER;

    *lpBytesReturned = 0;

    return STATUS_SUCCESS;
}

NTSTATUS
PciIoctlFillMem(
				void *lpInBuffer, 
			    ULONG nInBufferSize, 
				void *lpOutBuffer, 
				ULONG nOutBufferSize, 
				ULONG *lpBytesReturned)

{
	PCIDEBUG_MEMFILL_INPUT *fill_param;
	ULONG size;
	int i;
	PVOID	maped;
	BOOLEAN	err;

    if ( nInBufferSize < sizeof(PCIDEBUG_MEMFILL_INPUT) )
    {
        return STATUS_INVALID_PARAMETER;
    }

    fill_param = (PCIDEBUG_MEMFILL_INPUT *)lpInBuffer;
	size = fill_param->unitsize * fill_param->count;

    maped = _MapPhysToLinear(fill_param->address, size, 0);

	err = FALSE;
	switch(fill_param->unitsize){
		case 1:
			for(i=0;i<fill_param->count;i++){
				((UCHAR*)maped)[i] = (UCHAR)fill_param->data;
			}
		break;
		case 2:
			for(i=0;i<fill_param->count;i++){
				((USHORT*)maped)[i] = (USHORT)fill_param->data;
			}
		break;
		case 4:
			for(i=0;i<fill_param->count;i++){
				((ULONG*)maped)[i] = (ULONG)fill_param->data;
			}
		break;
		default:
		err = TRUE;
	}

	if(err) return STATUS_INVALID_PARAMETER;

    *lpBytesReturned = 0;

    return STATUS_SUCCESS;
}

DWORD
PciIoctlReadConf(
				void *lpInBuffer, 
			    ULONG nInBufferSize, 
				void *lpOutBuffer, 
				ULONG nOutBufferSize, 
				ULONG *lpBytesReturned)
{
	PCIDEBUG_CONFREAD_INPUT *read_param;
	DWORD status;

    if ( nInBufferSize != sizeof(PCIDEBUG_CONFREAD_INPUT) )
    {
        return STATUS_INVALID_PARAMETER;
    }
	read_param	= (PCIDEBUG_CONFREAD_INPUT *) lpInBuffer;


	status = pciConfigRead(read_param->pci_address, read_param->pci_offset,
						lpOutBuffer, nOutBufferSize);

	if(status == STATUS_SUCCESS) {
	    *lpBytesReturned = nOutBufferSize;
    } else {
	    *lpBytesReturned = 0;
	}

    return status;
}

NTSTATUS
PciIoctlWriteConf(
				void *lpInBuffer, 
			    ULONG nInBufferSize, 
				void *lpOutBuffer, 
				ULONG nOutBufferSize, 
				ULONG *lpBytesReturned)

{
	PCIDEBUG_CONFWRITE_INPUT *write_param;
	ULONG writesize;
	NTSTATUS status;

    if ( nInBufferSize < offsetof(PCIDEBUG_CONFWRITE_INPUT,data))
    {
        return STATUS_INVALID_PARAMETER;
    }

    write_param = (PCIDEBUG_CONFWRITE_INPUT *)lpInBuffer;
	writesize = nInBufferSize - offsetof(PCIDEBUG_CONFWRITE_INPUT, data);
	
    *lpBytesReturned = 0;

	return pciConfigWrite(write_param->pci_address,
		write_param->pci_offset,
		(char*)&write_param->data,
		writesize);

}

NTSTATUS
PciIoctlBiosStatus(	PCIDEBUG_PCIBIOSSTATUS *buf, int size, ULONG *lpcbBytesReturned )
{
    BYTE	returncode;
	BYTE	accessway;
	BYTE	version_measure;
	BYTE	version_minor;
	BYTE	maxbusnumber;
	BYTE	c;
	DWORD	sig;
	if(size != sizeof(PCIDEBUG_PCIBIOSSTATUS)) return STATUS_INVALID_PARAMETER;

	__asm mov  ax, word ptr 0xb101 ;  
	CallPciBios();
	__asm {
		mov returncode, ah;
		mov accessway, al;
		mov version_measure, bh;
		mov version_minor, bl;
		mov maxbusnumber, cl;
		mov sig, edx;
		pushf;
		pop ax;
		mov c, al;
	}
	buf->returncode = returncode;
	buf->accessway = accessway;
	buf->version_measure = version_measure;
	buf->version_minor = version_minor;
	buf->maxbusnumber = maxbusnumber;
	buf->sig = sig;
	buf->cf = c & 1;
	*lpcbBytesReturned = sizeof(PCIDEBUG_PCIBIOSSTATUS);
	return STATUS_SUCCESS;
}

NTSTATUS
pciConfigRead(ULONG pci_address, ULONG loffsetadd, char *data, int len)
{
	BYTE status, maxbusnumber;
	WORD offsetadd = loffsetadd;
	__asm mov  ax, word ptr 0xb101 ;  
	CallPciBios();
	__asm mov maxbusnumber, cl;
	if(pciGetBus(pci_address) > maxbusnumber) return PCI_ERR_BUSNOTEXIST;

	while(len >0){
		if(len >=4 && (offsetadd & 3) ==0) {
			DWORD d;
			__asm mov  ax, word ptr 0xb10a ;  
			__asm mov  bx, word ptr pci_address ;
			__asm mov  di, offsetadd
			CallPciBios();
			__asm mov  d, ecx ;
			__asm mov status, ah ;
			*(DWORD*)data = d;
			data += 4;
			len -= 4;
			offsetadd += 4;
		} else if(len >=2 && (offsetadd & 1) ==0) {
			WORD d;
			__asm mov  ax, word ptr 0xb109 ;  
			__asm mov  bx, word ptr pci_address ;
			__asm mov  di, offsetadd;
			CallPciBios();
			__asm mov  d, cx ;
			__asm mov status, ah;
			*(WORD*)data = d;
			data += 2;
			len -= 2;
			offsetadd += 2;
		} else {
			BYTE d;
			__asm mov  ax, word ptr 0xb108 ;  
			__asm mov  bx, word ptr pci_address ;
			__asm mov  di, offsetadd;
			CallPciBios();
			__asm mov  d, cl ;
			__asm mov status, ah ;
			*(WORD*)data = d;
			data += 1;
			len -= 1;
			offsetadd += 1;
		}
		if (status != 0) return PCI_ERR_CONFREAD;
	}
	return STATUS_SUCCESS;
}

NTSTATUS
pciConfigWrite(ULONG pci_address, ULONG loffsetadd, char *data, int len)
{
	BYTE status, maxbusnumber;
	WORD offsetadd = loffsetadd;
	__asm mov  ax, word ptr 0xb101 ;  
	CallPciBios();
	__asm mov maxbusnumber, cl;
	if(pciGetBus(pci_address) > maxbusnumber) return PCI_ERR_BUSNOTEXIST;
	while(len >0){
		if(len >=4 && (offsetadd & 3) ==0) {
			DWORD d = *(DWORD*)data;
			__asm mov  ax, word ptr 0xb10d ;  
			__asm mov  bx, word ptr pci_address ;
			__asm mov  di, offsetadd;
			__asm mov  ecx, d;
			CallPciBios();
			__asm mov status, ah
			data += 4;
			len -= 4;
			offsetadd += 4;
		} else if(len >=2 && (offsetadd & 1) ==0) {
			WORD d = *(DWORD*)data;
			__asm mov  ax, word ptr 0xb10c ;  
			__asm mov  bx, word ptr pci_address ;
			__asm mov  di, offsetadd;
			__asm mov  cx, d;
			CallPciBios();
			__asm mov status, ah
			data += 2;
			len -= 2;
			offsetadd += 2;
		} else {
			BYTE d = *(DWORD*)data;
			__asm mov  ax, word ptr 0xb10b ;  
			__asm mov  bx, word ptr pci_address ;
			__asm mov  di, offsetadd;
			__asm mov  cl, d ;
			CallPciBios();
			__asm mov status, ah
			data += 1;
			len -= 1;
			offsetadd += 1;
		}
		if (status != 0) return PCI_ERR_CONFWRITE;
	}
	return STATUS_SUCCESS;
}	
