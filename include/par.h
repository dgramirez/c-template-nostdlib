#ifndef INCLUDE_PLATFORM_APP_RELATIONSHIP_H
#define INCLUDE_PLATFORM_APP_RELATIONSHIP_H

#include "define.h"
#include "platform.h"
#include "mem/s8.h"

// Note: To future self, this is just an example. This **should** be
//       expanded to solve "Platform-To-App" problems.
//
//       For Instance: A "very simple chat application" can use the buffer
//       provided by the platform layer to setup a send network buffer.
//       Then the app can use a "Platform Function" to give its buffer's
//       pointer to the platform. The platform will check to see if the
//       buffer "len" property is above 0, and if so it will attempt to
//       send that many bytes through the network.
//
//       On that note, the same setup can be used for a recv network buffer.
//       Follows the same procedure up to the platform function. This could
//       be a bit more complex, but the premise remains the same: The app
//       can check to see both how many messages sent from previous check
//       and parse through them (either all, bulk, or single) to update the
//       client.
typedef struct {
	b8 bufapp;
	i32 (*os_write)(fb8 *b);
	b8  (*get_cpu_vendor)(u8*, usz);
	void *std_out;
	i32 run_app;
} PlatformData;

#endif // INCLUDE_PLATFORM_APP_RELATIONSHIP_H

