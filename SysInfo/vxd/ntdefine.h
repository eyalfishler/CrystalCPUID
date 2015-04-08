/*************************************************

PCI BIOS, Memory , Interrupt handle VxD
							 for windows 95/98

  ver 1.0   kashiwano masahiro

  windows NT DDK compatible define file

*************************************************/

typedef DWORD NTSTATUS ;
typedef BYTE  BOOLEAN;           

#define STATUS_SUCCESS                          ((NTSTATUS)0x00000000L) // ntsubauth
#define STATUS_INVALID_PARAMETER         ((NTSTATUS)0xC000000DL)


#define CTL_CODE( DeviceType, Function, Method, Access ) (                 \
    ((DeviceType) << 16) | ((Access) << 14) | ((Function) << 2) | (Method) \
)

#define METHOD_BUFFERED                 0
#define METHOD_IN_DIRECT                1
#define METHOD_OUT_DIRECT               2
#define METHOD_NEITHER                  3

#define FILE_ANY_ACCESS                 0
#define FILE_READ_ACCESS          ( 0x0001 )    // file & pipe
#define FILE_WRITE_ACCESS         ( 0x0002 )    // file & pipe

