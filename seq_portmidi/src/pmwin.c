/*
 *  This file is part of sequencer64, adapted from the PortMIDI project.
 *
 *  sequencer64 is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  sequencer64 is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with seq24; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/**
 * \file        pmwin.c
 *
 *      PortMidi os-dependent code for Windows.
 *
 * \library     sequencer64 application
 * \author      PortMIDI team; modifications by Chris Ahlstrom
 * \date        2017-08-21
 * \updates     2018-04-11
 * \license     GNU GPLv2 or above

 *  This file needs to implement:
 *
 *      -   pm_init(), which calls various routines to register the available
 *          MIDI devices.
 *      -   Pm_GetDefaultInputDeviceID().
 *      -   Pm_GetDefaultOutputDeviceID().
 *
 *  This file must be separate from the main portmidi.c file because it is
 *  system dependent, and it is separate from, say, pmwinmm.c, because it might
 *  need to register devices for WinMM, DirectX, and others.
 */

#include "stdlib.h"
#include "portmidi.h"
#include "pmutil.h"
#include "pminternal.h"
#include "pmwinmm.h"

#ifdef PLATFORM_DEBUG
#include "stdio.h"
#endif

#include <windows.h>

/*
 * #include <tchar.h>
 */

#define _T(x)       ((const char *)(x))

#define PATTERN_MAX 256

/**
 *  pm_exit() is called when the program exits.  It calls pm_term() to make
 *  sure PortMidi is properly closed.  If PLATFORM_DEBUG is on, we prompt for
 *  input to avoid losing error messages.  Without this prompting, client
 *  console application cannot see one of its errors before closing.
 */

static void
pm_exit(void)
{
    pm_term();

#ifdef PLATFORM_DEBUG
#define STRING_MAX 80
    {
        char line[STRING_MAX];
        printf("Type Enter to exit...\n");
        fgets(line, STRING_MAX, stdin);
    }
#endif
}

/*
 *  pm_init() provides the windows-dependent initialization.  It also sets the
 *  atexit() callback to pm_exit().
 */

void pm_init(void)
{
    atexit(pm_exit);

#ifdef PLATFORM_DEBUG
    printf("registered pm_exit with atexit()\n");
#endif

    pm_winmm_init();

    /*
     * Initialize other APIs (DirectX?) here.
     */
}

/**
 *
 */

void
pm_term (void)
{
    pm_winmm_term();
}

/**
 *
 */

static PmDeviceID
pm_get_default_device_id (int is_input, TCHAR * key)
{
    HKEY hkey;
    BYTE pattern[PATTERN_MAX];
    ULONG pattern_max = PATTERN_MAX;
    DWORD dwType;

    /* Find first input or device -- this is the default. */

    PmDeviceID id = pmNoDevice;
    int i, j;
    Pm_Initialize();                      /* make sure descriptors exist! */
    for (i = 0; i < pm_descriptor_index; ++i)
    {
        if (descriptors[i].pub.input == is_input)
        {
            id = i;
            break;
        }
    }

    /* Look in registry for a default device name pattern. */

    if (RegOpenKeyEx(HKEY_CURRENT_USER, _T("Software"), 0, KEY_READ, &hkey) !=
            ERROR_SUCCESS)
    {
        return id;
    }
    if (RegOpenKeyEx(hkey, _T("JavaSoft"), 0, KEY_READ, &hkey) != ERROR_SUCCESS)
    {
        return id;
    }
    if (RegOpenKeyEx(hkey, _T("Prefs"), 0, KEY_READ, &hkey) != ERROR_SUCCESS)
    {
        return id;
    }
    if (RegOpenKeyEx(hkey, _T("/Port/Midi"), 0, KEY_READ, &hkey) != ERROR_SUCCESS)
    {
        return id;
    }
    if (RegQueryValueEx(hkey, key, NULL, &dwType, pattern, &pattern_max) !=
            ERROR_SUCCESS)
    {
        return id;
    }

    i = j = 0;          /* decode pattern: upper case encoded with "/" prefix */
    while (pattern[i])
    {
        if (pattern[i] == '/' && pattern[i + 1])
        {
            pattern[j++] = toupper(pattern[++i]);
        }
        else
        {
            pattern[j++] = tolower(pattern[i]);
        }
        i++;
    }
    pattern[j] = 0;                         /* end of string */

    /*
     * Now pattern is the string from the Registry; search for match.
     * We may need to use typedefs to properly switch between Linux and
     * Windows.
     */

    i = pm_find_default_device((char *) pattern, is_input);
    if (i != pmNoDevice)
        id = i;

    return id;
}

/**
 * \tricky
 *      The string is meant to be compared to a UTF-16 string from the
 *      Registry.  Do not use _T() here.
 */

PmDeviceID
Pm_GetDefaultInputDeviceID ()
{
    return pm_get_default_device_id
    (
         TRUE, "/P/M_/R/E/C/O/M/M/E/N/D/E/D_/I/N/P/U/T_/D/E/V/I/C/E"
    );
}

/**
 * \tricky
 *      The string is meant to be compared to a UTF-16 string from the
 *      Registry.  Do not use _T() here.
 */

PmDeviceID
Pm_GetDefaultOutputDeviceID ()
{
    return pm_get_default_device_id
    (
         FALSE, "/P/M_/R/E/C/O/M/M/E/N/D/E/D_/O/U/T/P/U/T_/D/E/V/I/C/E"
    );
}

/**
 *
 */

void *
pm_alloc (size_t s)
{
    return malloc(s);
}

/**
 *
 */

void
pm_free (void * ptr)
{
    if (not_nullptr(ptr))
        free(ptr);
}

/*
 * pmwin.c
 *
 * vim: sw=4 ts=4 wm=4 et ft=c
 */

