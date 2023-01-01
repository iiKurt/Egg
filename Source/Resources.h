// https://stackoverflow.com/a/2933357

#define RESOURCE_FONT_FILE       "zap-light16.bmp"
#define RESOURCE_HAPPYMAC_FILE   "happy-mac.bmp"
#define RESOURCE_SADMAC_FILE     "sad-mac.bmp"

// Only on Windows
#ifdef _WIN32

// Resource nameIDs
#define	RESOURCE_ICON       0
#define	RESOURCE_FONT       1
#define	RESOURCE_HAPPYMAC   2
#define	RESOURCE_SADMAC     3

// Resource typeIDs
#define FILE_BITMAP         256

// Not Windows
#else

#define RESOURCE_DIRECTORY  "./"

#define RESOURCE_FONT       RESOURCE_FONT_FILE
#define RESOURCE_HAPPYMAC   RESOURCE_HAPPYMAC_FILE
#define RESOURCE_SADMAC     RESOURCE_SADMAC_FILE

#endif
