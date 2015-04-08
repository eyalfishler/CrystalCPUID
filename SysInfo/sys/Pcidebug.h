////////////////////////////////////////////////////////
//  pcidebug.dll
//  pcidegug.sys windows NT driver part
//                        Aug 20 1999 kashiwano masahiro
//
////////////////////////////////////////////////////////

#include <ntddk.h>
#include <string.h>
#include <devioctl.h>
#include "pciioctl.h"        // Get IOCTL interface definitions


// NT device name
#define PCI_DEVICE_NAME L"\\Device\\CrystalSysInfo"
#define DOS_DEVICE_NAME L"\\DosDevices\\CrystalSysInfo"

#define N_ISR	16

// driver local data structure specific to each device object
typedef struct _LOCAL_DEVICE_INFO {
    ULONG               DeviceType;     // Our private Device Type
    PDEVICE_OBJECT      DeviceObject;   // The Gpd device object.
	struct {
	    PKINTERRUPT	InterruptObject;
		BOOLEAN				SetupOK;		// after call setup interrupt set TRUE
	    KAFFINITY			Affinity;
	    KIRQL				irql;
		ULONG				MappedVector;
		KINTERRUPT_MODE		InterruptMode;
		BOOLEAN				ShareVector;

		HANDLE Handle ;
		PKEVENT Event ;
	} table[N_ISR];

} LOCAL_DEVICE_INFO, *PLOCAL_DEVICE_INFO;

/********************* function prototypes ***********************************/
//

NTSTATUS    DriverEntry(       IN  PDRIVER_OBJECT DriverObject,
                               IN  PUNICODE_STRING RegistryPath );

NTSTATUS    PciCreateDevice(   IN  PWSTR szPrototypeName,
                               IN  DEVICE_TYPE DeviceType,
                               IN  PDRIVER_OBJECT DriverObject,
                               OUT PDEVICE_OBJECT *ppDevObj     );

NTSTATUS    PciDispatch(       IN  PDEVICE_OBJECT pDO,
                               IN  PIRP pIrp                    );

NTSTATUS	PciIoctlReadPort(
				ULONG IoctlCode,
				void *lpInBuffer, 
			    ULONG nInBufferSize, 
				void *lpOutBuffer, 
				ULONG nOutBufferSize, 
				ULONG *lpBytesReturned);

NTSTATUS	PciIoctlWritePort(
				ULONG IoctlCode,
				void *lpInBuffer, 
			    ULONG nInBufferSize, 
				void *lpOutBuffer, 
				ULONG nOutBufferSize, 
				ULONG *lpBytesReturned);

NTSTATUS    PciIoctlReadMem(
				void *lpInBuffer, 
			    ULONG nInBufferSize, 
				void *lpOutBuffer, 
				ULONG nOutBufferSize, 
				ULONG *lpBytesReturned);

NTSTATUS    PciIoctlWriteMem(
				void *lpInBuffer, 
			    ULONG nInBufferSize, 
				void *lpOutBuffer, 
				ULONG nOutBufferSize, 
				ULONG *lpBytesReturned);

NTSTATUS	PciIoctlCopyMem(
				void *lpInBuffer, 
			    ULONG nInBufferSize, 
				void *lpOutBuffer, 
				ULONG nOutBufferSize, 
				ULONG *lpBytesReturned);

NTSTATUS	PciIoctlFillMem(
				void *lpInBuffer, 
			    ULONG nInBufferSize, 
				void *lpOutBuffer, 
				ULONG nOutBufferSize, 
				ULONG *lpBytesReturned);

NTSTATUS    PciIoctlReadConf(
				void *lpInBuffer, 
			    ULONG nInBufferSize, 
				void *lpOutBuffer, 
				ULONG nOutBufferSize, 
				ULONG *lpBytesReturned);

NTSTATUS    PciIoctlWriteConf(
				void *lpInBuffer, 
			    ULONG nInBufferSize, 
				void *lpOutBuffer, 
				ULONG nOutBufferSize, 
				ULONG *lpBytesReturned);

NTSTATUS    ReadMSR(
				ULONG IoctlCode,
				void *lpInBuffer, 
			 	ULONG nInBufferSize, 
				void *lpOutBuffer, 
				ULONG nOutBufferSize, 
				ULONG *lpBytesReturned);

NTSTATUS    WriteMSR(
				ULONG IoctlCode,
				void *lpInBuffer, 
			 	ULONG nInBufferSize, 
				void *lpOutBuffer, 
				ULONG nOutBufferSize, 
				ULONG *lpBytesReturned);


VOID        PciUnload(         IN  PDRIVER_OBJECT DriverObject );

NTSTATUS    pciConfigRead(ULONG pci_address, ULONG offset, void *data, int len);
NTSTATUS    pciConfigWrite(ULONG pci_address, ULONG offset, void *data, int len);

NTSTATUS    SetupInterrupt(
    IN PLOCAL_DEVICE_INFO pLDI,
    IN PIRP pIrp,
    IN PIO_STACK_LOCATION IrpStack);

NTSTATUS    ConnectInterrupt(PLOCAL_DEVICE_INFO pLDI, int index);

VOID   disconnectinterrupt(
    PLOCAL_DEVICE_INFO pLDI,
	int index);

BOOLEAN statInterruptServiceRoutine(
    IN PKINTERRUPT Interrupt,
    IN OUT PVOID Context
    );

VOID statDpcRoutine(
    IN PKDPC Dpc,
    IN PVOID DeferredContext,
    IN PVOID SystemArgument1,
    IN PVOID SystemArgument2
    );

/* バス番号,デバイス番号,ファンクション番号 から PCIデバイスアドレスに変換 */
#define pciBusDevFunc(Bus,Dev,Func)		((Bus&0xff)<<8)|((Dev&0x1f)<<3)|(Func&7)
/* PCIデバイスアドレス から バス番号取得 */
#define pciGetBus(BXreg)		((BXreg>>8)&0xff)
/* PCIデバイスアドレス から デバイス番号取得 */
#define pciGetDev(BXreg)		((BXreg>>3)&0x1f)
/* PCIデバイスアドレス から ファンクション番号取得 */
#define pciGetFunc(BXreg)		(BXreg&7)

