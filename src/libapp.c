#include "define.h"
#include "platform.h"
#include "par.h"
global PlatformData *g_platform;

#if EXE_ARCH == 32
	#define APP_USZ_VALUE 1234567890
	#define APP_ISZ_VALUE -1876543290
#else
	#define APP_USZ_VALUE 12345678901234567
	#define APP_ISZ_VALUE -9876543210987654
#endif

export i32
app_init(PlatformData *pd)
{
	g_platform = pd;
	os_write = g_platform->os_write;

	// Returning a positive or negative number acts as an error value.
	return 0;
}

export i32
app_update()
{
	s8  str  =  s8("Hello, World!\n");
	fb8 fb   =  {0};
	f32 pi   =  3.14159265;
	f32 npi  = -3.14159265;
	usz uval =  APP_USZ_VALUE;
	isz ival =  APP_ISZ_VALUE;

	u8  cpuid_vendor[16];
	u8  randbuf[256];

	fb.data = g_platform->bufapp.data;
	fb.cap  = g_platform->bufapp.len;
	fb.fd   = g_platform->std_out;

	// Regular str -> fb
	fb8_append(&fb, str);
	fb8_append_lf(&fb);
	fb8_append_lf(&fb);
	fb8_flush(&fb);

	// Convert float into ascii inside fb
	str = s8("Pi: ");
	fb8_append(&fb, str);
	fb8_append_f64(&fb, pi);
	fb8_append_lf(&fb);
	fb8_flush(&fb);

	str = s8("Negative Pi: ");
	fb8_append(&fb, str);
	fb8_append_f64(&fb, npi);
	fb8_append_lf(&fb);
	fb8_flush(&fb);

	// Convert size_t types into ascii (decimal) inside fb
	str = s8("Positive size_t Value: ");
	fb8_append(&fb, str);
	fb8_append_usz(&fb, uval);
	fb8_append_lf(&fb);
	fb8_flush(&fb);

	str = s8("Negative size_t Value: ");
	fb8_append(&fb, str);
	fb8_append_isz(&fb, ival);
	fb8_append_lf(&fb);
	fb8_flush(&fb);

	// Convert size_t types into ascii (hex) inside fb
	str = s8("Hex Value (lower): ");
	usz hval = 0xCAFEBABE;
	fb8_append(&fb, str);
	fb8_append_hex(&fb, hval);
	fb8_append_lf(&fb);
	fb8_flush(&fb);

	str = s8("Hex Value (capital): ");
	usz hcval = 0xCAFED00D;
	fb8_append(&fb, str);
	fb8_append_hex_cap(&fb, hcval);
	fb8_append_lf(&fb);
	fb8_append_lf(&fb);
	fb8_flush(&fb);

	// CPUID (Provided by the Platform Layer.)
	str = s8("CPUID Vendor: ");
	g_platform->get_cpu_vendor(&cpuid_vendor[0], 16);
	fb8_append_cstr(&fb, &cpuid_vendor[0], 12);
	fb8_append_lf(&fb);
	fb8_append_lf(&fb);
	fb8_flush(&fb);

	// Checking for Alignment Values
	str = s8("Current Pointer: ");
	fb8_append(&fb, str);
	fb8_append_hex(&fb, (usz)(&randbuf[1]));
	fb8_append_lf(&fb);

	str = s8("In-Alignment Pointer for page alignment: ");
	fb8_append(&fb, str);
	fb8_append_hex(&fb, align_addr(&randbuf[1], page_size));
	fb8_append_lf(&fb);

	str = s8("Next-Alignment Pointer for page alignment: ");
	fb8_append(&fb, str);
	fb8_append_hex(&fb, align_next(&randbuf[1], page_size));
	fb8_append_lf(&fb);

	str = s8("Pointer value over for page alignment: ");
	fb8_append(&fb, str);
	fb8_append_usz(&fb, align_over(&randbuf[1], page_size));
	fb8_append_lf(&fb);

	str = s8("Padding value for page alignment: ");
	fb8_append(&fb, str);
	fb8_append_usz(&fb, align_pad(&randbuf[1], page_size));
	fb8_append_lf(&fb);
	fb8_append_lf(&fb);

	fb8_flush(&fb);

	// Memmove Tests
	for (int i = 0; i < 256; ++i)
		randbuf[i] = (i % 94) + 32;

	randbuf[0] = '>';
	randbuf[1] = ' ';
	randbuf[128] = '>';
	randbuf[129] = ' ';
	memmoveu_usz(&randbuf[2], &randbuf[130], 126);
	fb8_append_cstr(&fb, (u8 *)randbuf, 256);
	fb8_append_lf(&fb);
	fb8_flush(&fb);

	for (int i = 0; i < 256; ++i)
		randbuf[i] = (i % 94) + 32;

	randbuf[0] = '>';
	randbuf[1] = ' ';
	randbuf[128] = '>';
	randbuf[129] = ' ';
	memmoveu_usz(&randbuf[130], &randbuf[2], 126);
	fb8_append_cstr(&fb, (u8 *)randbuf, 256);
	fb8_append_lf(&fb);
	fb8_flush(&fb);

	// Finish with the application (Not doing this will have the app run
	// Continually, which may be ideal for a simple protocol client.)
	g_platform->run_app = 0;

	// Returning a positive or negative number acts as an error value.
	return 0;
}

export void
app_close()
{
	g_platform->run_app = 0;
	// Going to be a stub for the meantime.
}

