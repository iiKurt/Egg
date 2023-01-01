// https://stackoverflow.com/a/2933357

// Only on Windows
#ifdef _WIN32

// Resource nameIDs
#define	RESOURCE_ICON		0
#define	RESOURCE_FONT		1
#define	RESOURCE_HAPPYMAC	2
#define	RESOURCE_SADMAC		3

// Resource typeIDs
#define FILE_PSF			256
#define FILE_BITMAP			257

#define RESOURCE_DIRECTORY	"./Resources/"
#define RESOURCE_ICON_PATH	"AppIcon.ico"

#endif

// Not Windows
#ifndef _WIN32

#define RESOURCE_DIRECTORY	"./"

#endif

#define RESOURCE_FONT_PATH		RESOURCE_DIRECTORY "zap-light16.psf"
#define RESOURCE_HAPPYMAC_PATH	RESOURCE_DIRECTORY "happy-mac.bmp"
#define RESOURCE_SADMAC_PATH	RESOURCE_DIRECTORY "sad-mac.bmp"
