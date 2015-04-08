////////////////////////////////////////////////////////
//  pcidebug.dll
//  pcidegug.sys windows NT driver part
//                        Aug 20 1999 kashiwano masahiro
//
////////////////////////////////////////////////////////

#include "stddef.h"
#include "pcidebug.h"

//#define _AMD64
//#define _NT4
//
// ISR
//
BOOLEAN
isr_prototype(
    IN PKINTERRUPT Interrupt,
    IN OUT PVOID Context,
	IN int index
    )
{
    PDEVICE_OBJECT DeviceObject;
	PLOCAL_DEVICE_INFO pLDI;
	BOOLEAN ret;

    DeviceObject = Context;
    pLDI = DeviceObject->DeviceExtension;

    disconnectinterrupt(pLDI, index);

    IoRequestDpc(DeviceObject,
		DeviceObject->CurrentIrp,
//		(PVOID) index
		(PVOID)(INT_PTR) index
		);

    return TRUE;
}

BOOLEAN isr0(IN PKINTERRUPT Interrupt, IN OUT PVOID Context)
{return isr_prototype(Interrupt, Context, 0);}
BOOLEAN isr1(IN PKINTERRUPT Interrupt, IN OUT PVOID Context)
{return isr_prototype(Interrupt, Context, 1);}
BOOLEAN isr2(IN PKINTERRUPT Interrupt, IN OUT PVOID Context)
{return isr_prototype(Interrupt, Context, 2);}
BOOLEAN isr3(IN PKINTERRUPT Interrupt, IN OUT PVOID Context)
{return isr_prototype(Interrupt, Context, 3);}
BOOLEAN isr4(IN PKINTERRUPT Interrupt, IN OUT PVOID Context)
{return isr_prototype(Interrupt, Context, 4);}
BOOLEAN isr5(IN PKINTERRUPT Interrupt, IN OUT PVOID Context)
{return isr_prototype(Interrupt, Context, 5);}
BOOLEAN isr6(IN PKINTERRUPT Interrupt, IN OUT PVOID Context)
{return isr_prototype(Interrupt, Context, 6);}
BOOLEAN isr7(IN PKINTERRUPT Interrupt, IN OUT PVOID Context)
{return isr_prototype(Interrupt, Context, 7);}
BOOLEAN isr8(IN PKINTERRUPT Interrupt, IN OUT PVOID Context)
{return isr_prototype(Interrupt, Context, 8);}
BOOLEAN isr9(IN PKINTERRUPT Interrupt, IN OUT PVOID Context)
{return isr_prototype(Interrupt, Context, 9);}
BOOLEAN isr10(IN PKINTERRUPT Interrupt, IN OUT PVOID Context)
{return isr_prototype(Interrupt, Context, 10);}
BOOLEAN isr11(IN PKINTERRUPT Interrupt, IN OUT PVOID Context)
{return isr_prototype(Interrupt, Context, 11);}
BOOLEAN isr12(IN PKINTERRUPT Interrupt, IN OUT PVOID Context)
{return isr_prototype(Interrupt, Context, 12);}
BOOLEAN isr13(IN PKINTERRUPT Interrupt, IN OUT PVOID Context)
{return isr_prototype(Interrupt, Context, 13);}
BOOLEAN isr14(IN PKINTERRUPT Interrupt, IN OUT PVOID Context)
{return isr_prototype(Interrupt, Context, 14);}
BOOLEAN isr15(IN PKINTERRUPT Interrupt, IN OUT PVOID Context)
{return isr_prototype(Interrupt, Context, 15);}

PKSERVICE_ROUTINE isr_table[] = {
	isr0, isr1, isr2, isr3, isr4, isr5, isr6, isr7,
	isr8, isr9, isr10, isr11, isr12, isr13, isr14, isr15
};

static ULONG dwRefCount;

NTSTATUS
DriverEntry(
    IN PDRIVER_OBJECT  DriverObject,
    IN PUNICODE_STRING RegistryPath
    )

/*

Return Value:

	STATUS_SUCCESS if the driver initialized correctly, otherwise an error
    indicating the reason for failure.
*/

{
/*
ULONG dwEAX = 0;
ULONG dwREGs[4];
__temp_cpuid(&dwREGs[0], dwEAX);
*/
    PLOCAL_DEVICE_INFO pLocalInfo;  // Device extension:
                                    //      local information for each device.
    NTSTATUS Status;
    PDEVICE_OBJECT DeviceObject;
	int i;



#ifdef ENABLEINT3
	_asm {
		INT 3;
	}
#endif

    DriverObject->MajorFunction[IRP_MJ_CREATE]          = PciDispatch;
    DriverObject->MajorFunction[IRP_MJ_CLOSE]           = PciDispatch;
    DriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL]  = PciDispatch;
    DriverObject->DriverUnload                          = PciUnload;
	DriverObject->DriverStartIo = NULL;

    // Create our device.
    Status = PciCreateDevice(
                    PCI_DEVICE_NAME,
                    PCI_TYPE,
                    DriverObject,
                    &DeviceObject
                    );
	dwRefCount = NT_SUCCESS(Status) ? 0 : (ULONG)-1;


	pLocalInfo = (PLOCAL_DEVICE_INFO)DeviceObject->DeviceExtension;
    pLocalInfo->DeviceObject    = DeviceObject;
    pLocalInfo->DeviceType      = PCI_TYPE;
	for(i=0;i<N_ISR;i++){
		pLocalInfo->table[i].SetupOK = FALSE;
		pLocalInfo->table[i].Handle = NULL;
	}
	IoInitializeDpcRequest(DeviceObject, statDpcRoutine);


    return Status;
}

NTSTATUS
PciCreateDevice(
    IN   PWSTR              PrototypeName,
    IN   DEVICE_TYPE        DeviceType,
    IN   PDRIVER_OBJECT     DriverObject,
    OUT  PDEVICE_OBJECT     *ppDevObj
    )


{
    NTSTATUS Status;                        // Status of utility calls
    UNICODE_STRING NtDeviceName;
    UNICODE_STRING Win32DeviceName;

    // Get UNICODE name for device.

    RtlInitUnicodeString(&NtDeviceName, PrototypeName);

    Status = IoCreateDevice(                             // Create it.
                    DriverObject,
                    sizeof(LOCAL_DEVICE_INFO),
                    &NtDeviceName,
                    DeviceType,
                    0,
//                  TRUE,                      // This is    an exclusive device
                    FALSE,                     // This isn't an exclusive device
                    ppDevObj
                    );

    if (!NT_SUCCESS(Status))
        return Status;             // Give up if create failed.

    // Clear local device info memory
    RtlZeroMemory((*ppDevObj)->DeviceExtension, sizeof(LOCAL_DEVICE_INFO));

    RtlInitUnicodeString(&Win32DeviceName, DOS_DEVICE_NAME);

    Status = IoCreateSymbolicLink( &Win32DeviceName, &NtDeviceName );

    if (!NT_SUCCESS(Status))    // If we we couldn't create the link then
    {                           //  abort installation.
        IoDeleteDevice(*ppDevObj);
    }

    return Status;
}

NTSTATUS
PciDispatch(
    IN    PDEVICE_OBJECT pDO,
    IN    PIRP pIrp             
    )

/*++

Routine Description:
    This routine is the dispatch handler for the driver.  It is responsible
    for processing the IRPs.

Arguments:
    
    pDO - Pointer to device object.

    pIrp - Pointer to the current IRP.

Return Value:

    STATUS_SUCCESS if the IRP was processed successfully, otherwise an error
    indicating the reason for failure.

--*/

{
    PLOCAL_DEVICE_INFO pLDI;
    PIO_STACK_LOCATION pIrpStack;
    NTSTATUS Status;
	int index;
    //  Initialize the irp info field.
    //      This is used to return the number of bytes transfered.

    pIrp->IoStatus.Information = 0;
    pLDI = (PLOCAL_DEVICE_INFO)pDO->DeviceExtension;    // Get local info struct

    pIrpStack = IoGetCurrentIrpStackLocation(pIrp);

    //  Set default return status
    Status = STATUS_NOT_IMPLEMENTED;

    // Dispatch based on major fcn code.

    switch (pIrpStack->MajorFunction)
    {
        case IRP_MJ_CREATE:
			if(dwRefCount!=(ULONG)-1) dwRefCount++;
            Status = STATUS_SUCCESS;
            break;

        case IRP_MJ_CLOSE:
			if(dwRefCount!=(ULONG)-1) dwRefCount--;
            Status = STATUS_SUCCESS;
            break;

        case IRP_MJ_DEVICE_CONTROL:
            //  Dispatch on IOCTL
            switch (pIrpStack->Parameters.DeviceIoControl.IoControlCode)
            {
			case IOCTL_GET_REFCONT:
	            *(PULONG)pIrp->AssociatedIrp.SystemBuffer = dwRefCount;
				pIrp->IoStatus.Information = sizeof(dwRefCount);
				Status = STATUS_SUCCESS;
		        break;

			case IOCTL_READ_MSR:
				Status = ReadMSR(
						pIrpStack->Parameters.DeviceIoControl.IoControlCode,
						pIrp->AssociatedIrp.SystemBuffer,
						pIrpStack->Parameters.DeviceIoControl.InputBufferLength,
						pIrp->AssociatedIrp.SystemBuffer,
						pIrpStack->Parameters.DeviceIoControl.OutputBufferLength,
						(ULONG*)&pIrp->IoStatus.Information);
						//memcpy( pIrp->AssociatedIrp.SystemBuffer ,test,4);
				break;

			case IOCTL_WRITE_MSR:
				Status = WriteMSR(
						pIrpStack->Parameters.DeviceIoControl.IoControlCode,
						pIrp->AssociatedIrp.SystemBuffer,
						pIrpStack->Parameters.DeviceIoControl.InputBufferLength,
						pIrp->AssociatedIrp.SystemBuffer,
						pIrpStack->Parameters.DeviceIoControl.OutputBufferLength,
						(ULONG*)&pIrp->IoStatus.Information);
						//memcpy( pIrp->AssociatedIrp.SystemBuffer ,test,4);
				break;
			case IOCTL_HLT:
#ifndef _AMD64
				_asm { hlt }
#endif
				break;

			case IOCTL_PCI_READ_PORT_UCHAR:
			case IOCTL_PCI_READ_PORT_USHORT:
			case IOCTL_PCI_READ_PORT_ULONG:
                Status = PciIoctlReadPort(
                            pIrpStack->Parameters.DeviceIoControl.IoControlCode,
                            pIrp->AssociatedIrp.SystemBuffer,
                            pIrpStack->Parameters.DeviceIoControl.InputBufferLength,
                            pIrp->AssociatedIrp.SystemBuffer,
							pIrpStack->Parameters.DeviceIoControl.OutputBufferLength,
                            (ULONG*)&pIrp->IoStatus.Information);
                break;

            case IOCTL_PCI_WRITE_PORT_UCHAR:
            case IOCTL_PCI_WRITE_PORT_USHORT:
            case IOCTL_PCI_WRITE_PORT_ULONG:
                Status = PciIoctlWritePort(
                            pIrpStack->Parameters.DeviceIoControl.IoControlCode,
                            pIrp->AssociatedIrp.SystemBuffer,
                            pIrpStack->Parameters.DeviceIoControl.InputBufferLength,
                            pIrp->AssociatedIrp.SystemBuffer,
							pIrpStack->Parameters.DeviceIoControl.OutputBufferLength,
                            (ULONG*)&pIrp->IoStatus.Information);
                break;

            case IOCTL_PCI_READ_MEM:
                Status = PciIoctlReadMem(
                            pIrp->AssociatedIrp.SystemBuffer,
                            pIrpStack->Parameters.DeviceIoControl.InputBufferLength,
                            pIrp->AssociatedIrp.SystemBuffer,
							pIrpStack->Parameters.DeviceIoControl.OutputBufferLength,
                            (ULONG*)&pIrp->IoStatus.Information);
                break;

            case IOCTL_PCI_WRITE_MEM:
                Status = PciIoctlWriteMem(
                            pIrp->AssociatedIrp.SystemBuffer,
                            pIrpStack->Parameters.DeviceIoControl.InputBufferLength,
                            pIrp->AssociatedIrp.SystemBuffer,
							pIrpStack->Parameters.DeviceIoControl.OutputBufferLength,
                            (ULONG*)&pIrp->IoStatus.Information);
                break;

            case IOCTL_PCI_COPY_MEM:
                Status = PciIoctlCopyMem(
                            pIrp->AssociatedIrp.SystemBuffer,
                            pIrpStack->Parameters.DeviceIoControl.InputBufferLength,
                            pIrp->AssociatedIrp.SystemBuffer,
							pIrpStack->Parameters.DeviceIoControl.OutputBufferLength,
                            (ULONG*)&pIrp->IoStatus.Information);
                break;

            case IOCTL_PCI_FILL_MEM:
                Status = PciIoctlFillMem(
                            pIrp->AssociatedIrp.SystemBuffer,
                            pIrpStack->Parameters.DeviceIoControl.InputBufferLength,
                            pIrp->AssociatedIrp.SystemBuffer,
							pIrpStack->Parameters.DeviceIoControl.OutputBufferLength,
                            (ULONG*)&pIrp->IoStatus.Information);
                break;

            case IOCTL_PCI_READ_CONF:
                Status = PciIoctlReadConf(
                            pIrp->AssociatedIrp.SystemBuffer,
                            pIrpStack->Parameters.DeviceIoControl.InputBufferLength,
                            pIrp->AssociatedIrp.SystemBuffer,
							pIrpStack->Parameters.DeviceIoControl.OutputBufferLength,
                            (ULONG*)&pIrp->IoStatus.Information);
                break;

            case IOCTL_PCI_WRITE_CONF:
                Status = PciIoctlWriteConf(
                            pIrp->AssociatedIrp.SystemBuffer,
                            pIrpStack->Parameters.DeviceIoControl.InputBufferLength,
                            pIrp->AssociatedIrp.SystemBuffer,
							pIrpStack->Parameters.DeviceIoControl.OutputBufferLength,
                            (ULONG*)&pIrp->IoStatus.Information);
                break;

			case IOCTL_PCI_ENABLEISR:
				if(pIrpStack->Parameters.DeviceIoControl.InputBufferLength != sizeof(int)){
					Status = STATUS_INVALID_PARAMETER;
				} else {
					index = *(int*)pIrp->AssociatedIrp.SystemBuffer; 
					Status = ConnectInterrupt(pLDI, index);
				    pIrp->IoStatus.Information = 0;
				}
				break;

            case IOCTL_PCI_SETUPISR:
				Status = SetupInterrupt(pLDI, pIrp, pIrpStack);
				break;
            }
            break;
    }

    // We're done with I/O request.  Record the status of the I/O action.
    pIrp->IoStatus.Status = Status;

    // Don't boost priority when returning since this took little time.
    IoCompleteRequest(pIrp, IO_NO_INCREMENT );

    return Status;
}

NTSTATUS
ReadMSR(	ULONG IoctlCode,
			void *lpInBuffer, 
			ULONG nInBufferSize, 
			void *lpOutBuffer, 
			ULONG nOutBufferSize, 
			ULONG* lpBytesReturned)
{
	ULONG ulECX = *(ULONG*)lpInBuffer;
	ULONG ulEAX = 0;
	ULONG ulEDX = 0;

#ifndef _AMD64
#ifdef _NT4
	_asm{
		mov ecx,ulECX
		rdmsr
		mov ulEAX,eax
		mov ulEDX,edx
	}
#else
try{
	_asm{
		mov ecx,ulECX
		rdmsr
		mov ulEAX,eax
		mov ulEDX,edx
	}
} except (EXCEPTION_EXECUTE_HANDLER) {
		*lpBytesReturned = 0;
		return STATUS_UNSUCCESSFUL;
}
#endif // _NT4
	memcpy((PULONG)lpOutBuffer,&ulEAX,4);
	memcpy((PULONG)lpOutBuffer+1,&ulEDX,4);
#else
	ULONG64 data;
try{
	data = __readmsr(ulECX);
} except (EXCEPTION_EXECUTE_HANDLER) {
	*lpBytesReturned = 0;
	return STATUS_UNSUCCESSFUL;
}
	memcpy((PULONG)lpOutBuffer,&data,8);
#endif

	*lpBytesReturned = 8;
	return STATUS_SUCCESS;
}

NTSTATUS
WriteMSR(	ULONG IoctlCode,
			void *lpInBuffer, 
			ULONG nInBufferSize, 
			void *lpOutBuffer, 
			ULONG nOutBufferSize, 
			ULONG* lpBytesReturned)
{

	ULONG ulECX = *(ULONG*)(PULONG)lpInBuffer;

#ifndef _AMD64
	ULONG ulEAX = *(ULONG*)((PULONG)lpInBuffer + 1);
	ULONG ulEDX = *(ULONG*)((PULONG)lpInBuffer + 2);

#ifdef _NT4
	_asm{
		mov eax,ulEAX
		mov edx,ulEDX
		mov ecx,ulECX
		wrmsr
	}
#else
try{
	_asm{
		mov eax,ulEAX
		mov edx,ulEDX
		mov ecx,ulECX
		wrmsr
	}
} except (EXCEPTION_EXECUTE_HANDLER) {
	*lpBytesReturned = 0;
	return STATUS_UNSUCCESSFUL;
}
#endif
#else
	ULONG64 ulEAX = *(ULONG*)((PULONG)lpInBuffer + 1);
	ULONG64 ulEDX = *(ULONG*)((PULONG)lpInBuffer + 2);
	ULONG64 data = ( ulEDX << 32 ) + ulEAX;
try{
	__writemsr(ulECX, data);
} except (EXCEPTION_EXECUTE_HANDLER) {
	*lpBytesReturned = 0;
	return STATUS_UNSUCCESSFUL;
}
#endif

	*lpBytesReturned = 0;
	return STATUS_SUCCESS;
}


NTSTATUS
PciIoctlReadPort(ULONG IoctlCode,
				void *lpInBuffer, 
			    ULONG nInBufferSize, 
				void *lpOutBuffer, 
				ULONG nOutBufferSize, 
				ULONG *lpBytesReturned)

{
    ULONG DataBufferSize;
    ULONG nPort; 
    switch (IoctlCode)
    {
    default:                    // There isn't really any default but 
	/* FALL THRU */         // this will quiet the compiler.
    case IOCTL_PCI_READ_PORT_UCHAR:
        DataBufferSize = sizeof(UCHAR);
        break;
    case IOCTL_PCI_READ_PORT_USHORT:
        DataBufferSize = sizeof(USHORT);
        break;
    case IOCTL_PCI_READ_PORT_ULONG:
        DataBufferSize = sizeof(ULONG);
        break;
    }

    if ( nInBufferSize != sizeof(ULONG) || nOutBufferSize < DataBufferSize )
    {
        return STATUS_INVALID_PARAMETER;
    }

    // Buffers are big enough.

    nPort = *(ULONG*)lpInBuffer;             // Get the I/O port number from the buffer.

    switch (IoctlCode){
        case IOCTL_PCI_READ_PORT_UCHAR:
            *(PUCHAR)lpOutBuffer = READ_PORT_UCHAR((PUCHAR)(ULONG_PTR)(nPort) );
            break;
        case IOCTL_PCI_READ_PORT_USHORT:
            *(PUSHORT)lpOutBuffer = READ_PORT_USHORT((PUSHORT)(ULONG_PTR)(nPort) );
            break;
        case IOCTL_PCI_READ_PORT_ULONG:
            *(PULONG)lpOutBuffer = READ_PORT_ULONG((PULONG)(ULONG_PTR)(nPort) );
            break;
    }
    
    *lpBytesReturned = DataBufferSize;

    return STATUS_SUCCESS;
}

NTSTATUS
PciIoctlWritePort(ULONG IoctlCode,
				void *lpInBuffer, 
			    ULONG nInBufferSize, 
				void *lpOutBuffer, 
				ULONG nOutBufferSize, 
				ULONG *lpBytesReturned)
{
    ULONG DataBufferSize;
    ULONG nPort; 
	PCIDEBUG_WRITE_INPUT* InBuffer;
	InBuffer = (PCIDEBUG_WRITE_INPUT*) lpInBuffer;

    switch (IoctlCode)
    {
    default:                    // There isn't really any default but 
	/* FALL THRU */         // this will quiet the compiler.
    case IOCTL_PCI_WRITE_PORT_UCHAR:
        DataBufferSize = sizeof(UCHAR);
        break;
    case IOCTL_PCI_WRITE_PORT_USHORT:
        DataBufferSize = sizeof(USHORT);
        break;
    case IOCTL_PCI_WRITE_PORT_ULONG:
        DataBufferSize = sizeof(ULONG);
        break;
    }

    if ( nInBufferSize < (sizeof(ULONG) + DataBufferSize) )
    {
        return STATUS_INVALID_PARAMETER;
    }

    nPort = InBuffer->PortNumber;

    switch (IoctlCode){
        case IOCTL_PCI_WRITE_PORT_UCHAR:
            WRITE_PORT_UCHAR((PUCHAR)(ULONG_PTR)(nPort), InBuffer->CharData );
            break;
        case IOCTL_PCI_WRITE_PORT_USHORT:
            WRITE_PORT_USHORT((PUSHORT)(ULONG_PTR)(nPort), InBuffer->ShortData );
            break;
        case IOCTL_PCI_WRITE_PORT_ULONG:
            WRITE_PORT_ULONG((PULONG)(ULONG_PTR)(nPort), InBuffer->LongData );
            break;
    }
    return STATUS_SUCCESS;
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
	PHYSICAL_ADDRESS address;
	PVOID	maped;
	BOOLEAN	err;
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

	address.HighPart = 0;
	address.LowPart = read_param->address;

    maped = MmMapIoSpace(address, size, FALSE);

	err = FALSE;
	switch(read_param->unitsize){
		case 1:
		READ_REGISTER_BUFFER_UCHAR(maped, lpOutBuffer, read_param->count);
		break;
		case 2:
		READ_REGISTER_BUFFER_USHORT(maped, lpOutBuffer, read_param->count);
		break;
		case 4:
		READ_REGISTER_BUFFER_ULONG(maped, lpOutBuffer, read_param->count);
		break;
	default:
		err = TRUE;
	}

	MmUnmapIoSpace(maped, size);

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
	PHYSICAL_ADDRESS address;
	PVOID	maped;
	BOOLEAN	err;

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

	address.HighPart = 0;
	address.LowPart = write_param->address;

    maped = MmMapIoSpace(address, size, FALSE);

	err = FALSE;
	switch(write_param->unitsize){
		case 1:
		WRITE_REGISTER_BUFFER_UCHAR(maped, (UCHAR*)&write_param->data, write_param->count);
		break;
		case 2:
		WRITE_REGISTER_BUFFER_USHORT(maped, (USHORT*)&write_param->data, write_param->count);
		break;
		case 4:
		WRITE_REGISTER_BUFFER_ULONG(maped, (ULONG*)&write_param->data, write_param->count);
		break;
		default:
		err = TRUE;
	}

	MmUnmapIoSpace(maped, size);

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
	ULONG i;
	PHYSICAL_ADDRESS src_address;
	PHYSICAL_ADDRESS dest_address;
	PVOID	src_maped, dest_maped;
	BOOLEAN	err;

    if ( nInBufferSize < sizeof(PCIDEBUG_MEMCOPY_INPUT) )
    {
        return STATUS_INVALID_PARAMETER;
    }

    copy_param = (PCIDEBUG_MEMCOPY_INPUT *)lpInBuffer;
	size = copy_param->unitsize * copy_param->count;

	src_address.HighPart = 0;
	src_address.LowPart = copy_param->src_address;
    src_maped = MmMapIoSpace(src_address, size, FALSE);

	dest_address.HighPart = 0;
	dest_address.LowPart = copy_param->dest_address;
    dest_maped = MmMapIoSpace(dest_address, size, FALSE);

	err = FALSE;
	switch(copy_param->unitsize){
		case 1:
			for(i=0;i<copy_param->count;i++){
					WRITE_REGISTER_UCHAR(((PUCHAR)dest_maped) + i, 
						READ_REGISTER_UCHAR(((PUCHAR)src_maped) + i) );
			}
		break;
		case 2:
			for(i=0;i<copy_param->count;i++){
					WRITE_REGISTER_USHORT(((PUSHORT)dest_maped) + i, 
						READ_REGISTER_USHORT(((PUSHORT)src_maped) + i) );
			}
		break;
		case 4:
			for(i=0;i<copy_param->count;i++){
					WRITE_REGISTER_ULONG(((PULONG)dest_maped) + i, 
						READ_REGISTER_ULONG(((PULONG)src_maped) + i) );
			}
		break;
		default:
		err = TRUE;
	}

	MmUnmapIoSpace(dest_maped, size);
	MmUnmapIoSpace(src_maped, size);

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
	ULONG i;
	PHYSICAL_ADDRESS address;
	PVOID	maped;
	BOOLEAN	err;

    if ( nInBufferSize < sizeof(PCIDEBUG_MEMFILL_INPUT) )
    {
        return STATUS_INVALID_PARAMETER;
    }

    fill_param = (PCIDEBUG_MEMFILL_INPUT *)lpInBuffer;
	size = fill_param->unitsize * fill_param->count;

	address.HighPart = 0;
	address.LowPart = fill_param->address;
    maped = MmMapIoSpace(address, size, FALSE);

	err = FALSE;
	switch(fill_param->unitsize){
		case 1:
			for(i=0;i<fill_param->count;i++){
				WRITE_REGISTER_UCHAR(((PUCHAR)maped)+i, (UCHAR)fill_param->data);
			}
		break;
		case 2:
			for(i=0;i<fill_param->count;i++){
				WRITE_REGISTER_USHORT(((PUSHORT)maped)+i, (USHORT)fill_param->data);
			}
		break;
		case 4:
			for(i=0;i<fill_param->count;i++){
				WRITE_REGISTER_ULONG(((PULONG)maped)+i, (ULONG)fill_param->data);
			}
		break;
		default:
		err = TRUE;
	}

	MmUnmapIoSpace(maped, size);

	if(err) return STATUS_INVALID_PARAMETER;

    *lpBytesReturned = 0;

    return STATUS_SUCCESS;
}

NTSTATUS
PciIoctlReadConf(
				void *lpInBuffer, 
			    ULONG nInBufferSize, 
				void *lpOutBuffer, 
				ULONG nOutBufferSize, 
				ULONG *lpBytesReturned)
{
	PCIDEBUG_CONFREAD_INPUT *read_param;
	NTSTATUS status;

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
		&write_param->data,
		writesize);

}

NTSTATUS pciConfigRead(ULONG pci_address, ULONG offset, void *data, int len)
{
	PCI_SLOT_NUMBER slot;
	int err;
	ULONG BusNumber;

	BusNumber = pciGetBus(pci_address);
    slot.u.AsULONG = 0;
	slot.u.bits.DeviceNumber = pciGetDev(pci_address);
	slot.u.bits.FunctionNumber = pciGetFunc(pci_address);
	err = 	HalGetBusDataByOffset(PCIConfiguration,
			BusNumber,
			slot.u.AsULONG,
			data,
			offset,
			len);
	if(err == 0) return PCI_ERR_BUSNOTEXIST;
	if(len != 2 && err == 2)return PCI_ERR_NODEVICE;
	if(len != err)return PCI_ERR_CONFREAD;
	return STATUS_SUCCESS;
}

NTSTATUS pciConfigWrite(ULONG pci_address, ULONG offset, void *data, int len)
{
	PCI_SLOT_NUMBER slot;
	int err;
	ULONG BusNumber;

	BusNumber = pciGetBus(pci_address);

    slot.u.AsULONG = 0;
	slot.u.bits.DeviceNumber = pciGetDev(pci_address);
	slot.u.bits.FunctionNumber = pciGetFunc(pci_address);
	err = HalSetBusDataByOffset(PCIConfiguration,
			BusNumber,
			slot.u.AsULONG,
			data,
			offset,
			len);
	if(err != len )return PCI_ERR_CONFWRITE;
	return STATUS_SUCCESS;
}

VOID
PciUnload(
    PDRIVER_OBJECT DriverObject
    )

/*++

Routine Description:
    This routine prepares our driver to be unloaded.  It is responsible
    for freeing all resources allocated by DriverEntry as well as any 
    allocated while the driver was running.  The symbolic link must be
    deleted as well.

Arguments:
    
    DriverObject - Pointer to driver object created by the system.

Return Value:

    None

--*/

{
    PLOCAL_DEVICE_INFO pLDI;
    CM_RESOURCE_LIST NullResourceList;
    BOOLEAN ResourceConflict;
    UNICODE_STRING Win32DeviceName;
	int i;

    // Find our global data
    pLDI = (PLOCAL_DEVICE_INFO)DriverObject->DeviceObject->DeviceExtension;


    RtlInitUnicodeString(&Win32DeviceName, DOS_DEVICE_NAME);

    IoDeleteSymbolicLink(&Win32DeviceName);
	for(i=0;i<N_ISR;i++){
		disconnectinterrupt(pLDI, i);
		if(pLDI->table[i].Handle) {
			ZwClose (pLDI->table[i].Handle);
			pLDI->table[i].Handle = NULL;
		}
	}
    IoDeleteDevice(pLDI->DeviceObject);
}

VOID
disconnectinterrupt(
    PLOCAL_DEVICE_INFO pLDI, int index
    )
{
    if(pLDI->table[index].InterruptObject) {
		IoDisconnectInterrupt(pLDI->table[index].InterruptObject);
		pLDI->table[index].InterruptObject = NULL;	
	}
}


NTSTATUS SetupInterrupt(
    IN PLOCAL_DEVICE_INFO pLDI,
    IN PIRP pIrp,
    IN PIO_STACK_LOCATION IrpStack)
{
#ifndef _AMD64
	ULONG InBufferSize;
	PCIDEBUG_ISRINFO* isrinfo;
	UNICODE_STRING name ;
	UNICODE_STRING eventbase ;
	UNICODE_STRING eventname ;
    STRING eventnameString;
	int index;
    pIrp->IoStatus.Information = 0;

    InBufferSize  = IrpStack->Parameters.DeviceIoControl.InputBufferLength;
    isrinfo     = (PCIDEBUG_ISRINFO*)pIrp->AssociatedIrp.SystemBuffer;

	if(InBufferSize != sizeof(PCIDEBUG_ISRINFO) || 
		isrinfo->type > MaximumInterfaceType) return STATUS_INVALID_PARAMETER;

	//以前SetupInterruptが呼ばれていた場合、それをクリアーする
	index = isrinfo->index;
	disconnectinterrupt(pLDI, index); //割り込みの切り離し
	if(pLDI->table[index].Handle) {
		ZwClose (pLDI->table[index].Handle) ; //イベントのClose
		pLDI->table[index].Handle = NULL;
	}
	pLDI->table[index].SetupOK = FALSE; // flagをさげる

	if(isrinfo->eventname[0] != 0){
		// Event nameを作る
		// name = L"\\BaseNamedObjects\\" + isrinfo->eventname
		RtlInitUnicodeString (&eventbase, L"\\BaseNamedObjects\\") ;

		name.MaximumLength = EVENTNAMEMAXLEN + eventbase.Length;
		name.Length = 0 ;
		name.Buffer = ExAllocatePool (NonPagedPool, name.MaximumLength) ;
		RtlZeroMemory (name.Buffer, name.MaximumLength) ;

		RtlInitString(&eventnameString, isrinfo->eventname);
		RtlAnsiStringToUnicodeString(&eventname, &eventnameString, TRUE);

		RtlCopyUnicodeString (&name, &eventbase) ;
		RtlAppendUnicodeStringToString (&name, &eventname) ;
		RtlFreeUnicodeString(&eventname);

		pLDI->table[index].Event = IoCreateNotificationEvent (&name, &pLDI->table[index].Handle) ;
	    ExFreePool (name.Buffer) ;
		if (!pLDI->table[index].Event) {
			return PCI_ERR_EVENTCREATE;
		}
		KeClearEvent (pLDI->table[index].Event) ;

		pLDI->table[index].MappedVector = HalGetInterruptVector(
			 isrinfo->type,	    // Interface type
			 isrinfo->busnumber,	    // Bus number
			 isrinfo->ilevel, // Bus interrupt level
			 isrinfo->ivector, // Bus interrupt vector
			 &pLDI->table[index].irql,     // IRQ level
			 &pLDI->table[index].Affinity  // Affinity mask
			 );
		pLDI->table[index].InterruptMode = isrinfo->InterruptMode;
		pLDI->table[index].ShareVector = isrinfo->ShareVector;

		pLDI->table[index].SetupOK = TRUE;	// すべての割り込みパラメータがセットされていることをあらわす
	}
#else
#endif
	return STATUS_SUCCESS;
}

NTSTATUS ConnectInterrupt(PLOCAL_DEVICE_INFO pLDI, int index)
{
    NTSTATUS Status;
	if(!pLDI->table[index].SetupOK) return STATUS_ACCESS_VIOLATION; 
	if(pLDI->table[index].InterruptObject) return STATUS_ACCESS_VIOLATION;
	KeClearEvent (pLDI->table[index].Event) ;
	Status = IoConnectInterrupt(
				&pLDI->table[index].InterruptObject,
				isr_table[index],
				pLDI->DeviceObject,
				NULL,
				pLDI->table[index].MappedVector,
				pLDI->table[index].irql,
				pLDI->table[index].irql,
				pLDI->table[index].InterruptMode,
				pLDI->table[index].ShareVector,
				pLDI->table[index].Affinity,
				FALSE
				);

	if ( !NT_SUCCESS (Status) )
	{
	    KdPrint( ("STAT: couldn't connect interrupt\n" ) );
	}
    return Status;
}


//
// This is the deferred procedure call that gets queued by the ISR to
// finish any interrupt relate processing
// 
VOID
statDpcRoutine(
    IN PKDPC Dpc,
    IN PVOID DeferredContext,
    IN PVOID SystemArgument1,
    IN PVOID SystemArgument2
    )
{
#ifndef _AMD64
    PDEVICE_OBJECT DeviceObject;
    PLOCAL_DEVICE_INFO extension;
    PIRP Irp;
	int index;

    DeviceObject = DeferredContext;
    Irp = DeviceObject->CurrentIrp;
    extension = DeviceObject->DeviceExtension;
	index = (int)SystemArgument2;

    if (Irp) {
        Irp->IoStatus.Information = 0;
        Irp->IoStatus.Status = STATUS_SUCCESS;


        IoStartNextPacket(DeviceObject, FALSE);
        IoCompleteRequest(Irp, IO_NO_INCREMENT);
    }

	KeSetEvent (extension->table[index].Event, 0, FALSE) ; //イベントをシグナル状態にする
#else
#endif
    return;
}
  