/* =========================================================================================

   This is an auto-generated file: Any edits you make may be overwritten!

*/

#pragma once

namespace BinaryData
{
    extern const char*   EuphemiaCAS_ttf;
    const int            EuphemiaCAS_ttfSize = 137976;

    extern const char*   Hamburger_icon_svg_png;
    const int            Hamburger_icon_svg_pngSize = 241;

    extern const char*   Icon_icns;
    const int            Icon_icnsSize = 20724;

    extern const char*   White_Circle_svg_png;
    const int            White_Circle_svg_pngSize = 1634;

    extern const char*   closeicon_svg;
    const int            closeicon_svgSize = 2167;

    extern const char*   gear_svg;
    const int            gear_svgSize = 811;

    extern const char*   logo_large_svg;
    const int            logo_large_svgSize = 5256;

    extern const char*   mappingsourceicon_svg;
    const int            mappingsourceicon_svgSize = 20066;

    extern const char*   mappingtargeticon_svg;
    const int            mappingtargeticon_svgSize = 12967;

    extern const char*   panel_svg;
    const int            panel_svgSize = 51239;

    extern const char*   snyderphonicswhitelogo_svg;
    const int            snyderphonicswhitelogo_svgSize = 9291;

    extern const char*   snyderphonics_logot_svg;
    const int            snyderphonics_logot_svgSize = 2030;

    // Number of elements in the namedResourceList and originalFileNames arrays.
    const int namedResourceListSize = 12;

    // Points to the start of a list of resource names.
    extern const char* namedResourceList[];

    // Points to the start of a list of resource filenames.
    extern const char* originalFilenames[];

    // If you provide the name of one of the binary resource variables above, this function will
    // return the corresponding data and its size (or a null pointer if the name isn't found).
    const char* getNamedResource (const char* resourceNameUTF8, int& dataSizeInBytes);

    // If you provide the name of one of the binary resource variables above, this function will
    // return the corresponding original, non-mangled filename (or a null pointer if the name isn't found).
    const char* getNamedResourceOriginalFilename (const char* resourceNameUTF8);
}
