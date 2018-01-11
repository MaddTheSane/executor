#if !defined(_ICON_UTIL_H_)
#define _ICON_UTIL_H_

/* Copyright 1986-1996 by Abacus Research and Development, Inc.
 * All rights reserved.
 *

 */

#define large1BitMask (FOURCC('I', 'C', 'N', '#'))
#define large4BitData (FOURCC('i', 'c', 'l', '4'))
#define large8BitData (FOURCC('i', 'c', 'l', '8'))
#define small1BitMask (FOURCC('i', 'c', 's', '#'))
#define small4BitData (FOURCC('i', 'c', 's', '4'))
#define small8BitData (FOURCC('i', 'c', 's', '8'))
#define mini1BitMask (FOURCC('i', 'c', 'm', '#'))
#define mini4BitData (FOURCC('i', 'c', 'm', '4'))
#define mini8BitData (FOURCC('i', 'c', 'm', '8'))

namespace Executor
{
/* IconAlignmentType values */
enum
{
    atNone = 0,
    atVerticalCenter = 1,
    atTop = 2,
    atBottom = 3,
    atHorizontalCenter = 4,
    atAbsoluteCenter = (atVerticalCenter | atHorizontalCenter), /* 5 */
    atCenterTop = (atTop | atHorizontalCenter), /* 6 */
    atCenterBottom = (atBottom | atHorizontalCenter), /* 7 */
    atLeft = 8,
    atCenterLeft = (atVerticalCenter | atLeft), /* 9 */
    atTopLeft = (atTop | atLeft), /* 10 */
    atBottomLeft = (atBottom | atLeft), /* 11 */
    atRight = 12,
    atCenterRight = (atVerticalCenter | atRight), /* 13 */
    atTopRight = (atTop | atRight), /* 14 */
    atBottomRight = (atBottom | atRight), /* 15 */
};

/* IconTranformType values */
enum
{
    ttNone = 0,
    ttDisabled = 1,
    ttOffline = 2,
    ttOpen = 3,
    ttLabel1 = 0x0100,
    ttLabel2 = 0x0200,
    ttLabel3 = 0x0300,
    ttLabel4 = 0x0400,
    ttLabel5 = 0x0500,
    ttLabel6 = 0x0600,
    ttLabel7 = 0x0700,
    ttSelected = 0x4000,
    ttSelectedDisabled = (ttSelected | ttDisabled),
    ttSelectedOffline = (ttSelected | ttOffline),
    ttSelectedOpen = (ttSelected | ttOpen),
};

/* IconSelectorValue values */
/* #### what kind of eediot at apple named all the other icon flag
   types *Type, except this one? */
enum
{
    svLarge1Bit = 0x01,
    svLarge4Bit = 0x02,
    svLarge8Bit = 0x04,
    svSmall1Bit = 0x0100,
    svSmall4Bit = 0x0200,
    svSmall8Bit = 0x0400,
    svMini1Bit = 0x010000,
    svMini4Bit = 0x020000,
    svMini8Bit = 0x040000,

    svAllLargeData = 0xFF,
    svAllSmallData = 0xFF00,
    svAllMiniData = 0xFF0000,

    svAll1BitData = 0x010101,
    svAll4BitData = 0x020202,
    svAll8BitData = 0x040404,

    svAllAvailableData = 0xFFFFFF,
};

enum
{
    noMaskFoundErr = (-1000),
};

typedef ProcPtr IconActionProcPtr;
typedef ProcPtr IconGetterProcPtr;

typedef uint32_t IconSelectorValue;
typedef int16_t IconAlignmentType;
typedef int16_t IconTransformType;

typedef struct CIcon
{
    GUEST_STRUCT;
    GUEST<PixMap> iconPMap;
    GUEST<BitMap> iconMask;
    GUEST<BitMap> iconBMap;
    GUEST<Handle> iconData;
    GUEST<int16_t[1]> iconMaskData;
} * CIconPtr;

typedef GUEST<CIconPtr> *CIconHandle;

/* icon utility function prototypes */

extern OSErr C_PlotIconID(const Rect *rect,
                                      IconAlignmentType align,
                                      IconTransformType tranform,
                                      short res_id);
PASCAL_FUNCTION(PlotIconID);

extern OSErr C_PlotIconMethod(const Rect *rect,
                                          IconAlignmentType align,
                                          IconTransformType transform,
                                          IconGetterProcPtr method,
                                          void *data);
PASCAL_FUNCTION(PlotIconMethod);
extern void C_PlotCIcon(const Rect *rect, CIconHandle icon);
PASCAL_TRAP(PlotCIcon, 0xAA1F);
extern void C_PlotIcon(const Rect *rect, Handle icon);
PASCAL_TRAP(PlotIcon, 0xA94B);
extern OSErr C_PlotIconHandle(const Rect *rect,
                                          IconAlignmentType align,
                                          IconTransformType transform,
                                          Handle icon);
PASCAL_FUNCTION(PlotIconHandle);
extern OSErr C_PlotCIconHandle(const Rect *rect,
                                           IconAlignmentType align,
                                           IconTransformType transform,
                                           CIconHandle icon);
PASCAL_FUNCTION(PlotCIconHandle);
extern OSErr C_PlotSICNHandle(const Rect *rect,
                                          IconAlignmentType align,
                                          IconTransformType transform,
                                          Handle icon);
PASCAL_FUNCTION(PlotSICNHandle);

extern Handle C_GetIcon(short icon_id);
PASCAL_TRAP(GetIcon, 0xA9BB);
extern CIconHandle C_GetCIcon(short icon_id);
PASCAL_TRAP(GetCIcon, 0xAA1E);

extern void C_DisposeCIcon(CIconHandle icon);
PASCAL_TRAP(DisposeCIcon, 0xAA25);

extern OSErr C_GetIconSuite(GUEST<Handle> *suite, short res_id,
                                        IconSelectorValue selector);
PASCAL_FUNCTION(GetIconSuite);
extern OSErr C_NewIconSuite(GUEST<Handle> *suite);
PASCAL_FUNCTION(NewIconSuite);

extern OSErr C_AddIconToSuite(Handle icon_data, Handle suite,
                                          ResType type);
PASCAL_FUNCTION(AddIconToSuite);
extern OSErr C_GetIconFromSuite(GUEST<Handle> *icon_data,
                                            Handle suite, ResType type);
PASCAL_FUNCTION(GetIconFromSuite);
extern OSErr C_PlotIconSuite(const Rect *rect,
                                         IconAlignmentType align,
                                         IconTransformType transform,
                                         Handle suite);
PASCAL_FUNCTION(PlotIconSuite);
extern OSErr C_ForEachIconDo(Handle suite,
                                         IconSelectorValue selector,
                                         IconActionProcPtr action,
                                         void *data);
PASCAL_FUNCTION(ForEachIconDo);
extern short C_GetSuiteLabel(Handle suite);
PASCAL_FUNCTION(GetSuiteLabel);
extern OSErr C_SetSuiteLabel(Handle suite, short label);
PASCAL_FUNCTION(SetSuiteLabel);
extern OSErr C_GetLabel(short label, RGBColor *label_color,
                                    Str255 label_string);
PASCAL_FUNCTION(GetLabel);
extern OSErr C_DisposeIconSuite(Handle suite,
                                            Boolean dispose_data_p);
PASCAL_FUNCTION(DisposeIconSuite);

extern OSErr C_IconSuiteToRgn(RgnHandle rgn, const Rect *rect,
                                          IconAlignmentType align,
                                          Handle suite);
PASCAL_FUNCTION(IconSuiteToRgn);
extern OSErr C_IconIDToRgn(RgnHandle rgn, const Rect *rect,
                                       IconAlignmentType align,
                                       short icon_id);
PASCAL_FUNCTION(IconIDToRgn);
extern OSErr C_IconMethodToRgn(RgnHandle rgn, const Rect *rect,
                                           IconAlignmentType align,
                                           IconGetterProcPtr method,
                                           void *data);
PASCAL_FUNCTION(IconMethodToRgn);

extern Boolean C_PtInIconSuite(Point test_pt,
                                           const Rect *rect,
                                           IconAlignmentType align,
                                           Handle suite);
PASCAL_FUNCTION(PtInIconSuite);
extern Boolean C_PtInIconID(Point test_pt, const Rect *rect,
                                        IconAlignmentType align,
                                        short icon_id);
PASCAL_FUNCTION(PtInIconID);
extern Boolean C_PtInIconMethod(Point test_pt, const Rect *rect,
                                            IconAlignmentType align,
                                            IconGetterProcPtr method,
                                            void *data);
PASCAL_FUNCTION(PtInIconMethod);
extern Boolean C_RectInIconSuite(const Rect *test_rect,
                                             const Rect *rect,
                                             IconAlignmentType align,
                                             Handle suite);
PASCAL_FUNCTION(RectInIconSuite);
extern Boolean C_RectInIconID(const Rect *test_rect,
                                          const Rect *rect,
                                          IconAlignmentType align,
                                          short icon_id);
PASCAL_FUNCTION(RectInIconID);
extern Boolean C_RectInIconMethod(const Rect *test_rect,
                                              const Rect *rect,
                                              IconAlignmentType align,
                                              IconGetterProcPtr method,
                                              void *data);
PASCAL_FUNCTION(RectInIconMethod);
extern OSErr C_MakeIconCache(Handle *cache,
                                         IconGetterProcPtr make_icon,
                                         void *data);
PASCAL_FUNCTION(MakeIconCache);
extern OSErr C_LoadIconCache(const Rect *rect,
                                         IconAlignmentType align,
                                         IconTransformType transform,
                                         Handle cache);
PASCAL_FUNCTION(LoadIconCache);
extern OSErr C_GetIconCacheData(Handle cache, void **data);
PASCAL_FUNCTION(GetIconCacheData);
extern OSErr C_SetIconCacheData(Handle cache, void *data);
PASCAL_FUNCTION(SetIconCacheData);
extern OSErr C_GetIconCacheProc(Handle cache,
                                            IconGetterProcPtr *proc);
PASCAL_FUNCTION(GetIconCacheProc);
extern OSErr C_SetIconCacheProc(Handle cache,
                                            IconGetterProcPtr proc);
PASCAL_FUNCTION(SetIconCacheProc);
}

#endif /* !_ICON_UTIL_H */
