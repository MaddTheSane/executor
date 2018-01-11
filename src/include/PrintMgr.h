#if !defined(__PRINTING__)
#define __PRINTING__

#include "DialogMgr.h"

/*
 * Copyright 1986, 1989, 1990 by Abacus Research and Development, Inc.
 * All rights reserved.
 *

 */

namespace Executor
{
enum
{
    bDraftLoop = 0,
    bSpoolLoop = 1,
};

enum
{
    bDevCItoh = 1,
    bDevLaser = 3,
};

enum
{
    iPFMaxPgs = 128,
};

enum
{
    iPrSavPFil = (-1),
    iIOAbort = (-27),
    MemFullErr = (-108),
    iPrAbort = 128,
};

enum
{
    iPrDevCtl = 7,
    lPrReset = 0x10000,
    lPrLineFeed = 0x30000,
    lPrLFSixth = 0x3FFFF,
    lPrPageEnd = 0x20000,
    iPrBitsCtl = 4,
    lScreenBits = 0,
    lPaintBits = 1,
    iPrIOCtl = 5,
};

const char *const sPrDrvr = ".Print";
enum
{
    iPrDrvrRef = (-3),
};

struct TPrPort
{
    GUEST_STRUCT;
    GUEST<GrafPort> gPort;
    GUEST<QDProcs> saveprocs;
    GUEST<LONGINT[4]> spare;
    GUEST<BOOLEAN> fOurPtr;
    GUEST<BOOLEAN> fOurBits;
};
typedef TPrPort *TPPrPort;

struct TPrInfo
{
    GUEST_STRUCT;
    GUEST<INTEGER> iDev;
    GUEST<INTEGER> iVRes;
    GUEST<INTEGER> iHRes;
    GUEST<Rect> rPage;
};

typedef enum { feedCut,
               feedFanFold,
               feedMechCut,
               feedOther } TFeed;

struct TPrStl
{
    GUEST_STRUCT;
    GUEST<INTEGER> wDev;
    GUEST<INTEGER> iPageV;
    GUEST<INTEGER> iPageH;
    GUEST<SignedByte> bPort;
    GUEST<char> feed;
};

typedef enum { scanTB,
               scanBL,
               scanLR,
               scanRL } TScan;
struct TPrXInfo
{
    GUEST_STRUCT;
    GUEST<INTEGER> iRowBytes;
    GUEST<INTEGER> iBandV;
    GUEST<INTEGER> iBandH;
    GUEST<INTEGER> iDevBytes;
    GUEST<INTEGER> iBands;
    GUEST<SignedByte> bPatScale;
    GUEST<SignedByte> bULThick;
    GUEST<SignedByte> bULOffset;
    GUEST<SignedByte> bULShadow;
    GUEST<char> scan;
    GUEST<SignedByte> bXInfoX;
};

struct TPrJob
{
    GUEST_STRUCT;
    GUEST<INTEGER> iFstPage;
    GUEST<INTEGER> iLstPage;
    GUEST<INTEGER> iCopies;
    GUEST<SignedByte> bJDocLoop;
    GUEST<BOOLEAN> fFromUsr;
    GUEST<ProcPtr> pIdleProc;
    GUEST<StringPtr> pFileName;
    GUEST<INTEGER> iFileVol;
    GUEST<SignedByte> bFileVers;
    GUEST<SignedByte> bJobX;
};

struct TPrint
{
    GUEST_STRUCT;
    GUEST<INTEGER> iPrVersion;
    GUEST<TPrInfo> prInfo;
    GUEST<Rect> rPaper;
    GUEST<TPrStl> prStl;
    GUEST<TPrInfo> prInfoPT;
    GUEST<TPrXInfo> prXInfo;
    GUEST<TPrJob> prJob;
    GUEST<INTEGER[19]> printX;
};
typedef TPrint *TPPrint;

typedef GUEST<TPPrint> *THPrint;

typedef Rect *TPRect;

struct TPrStatus
{
    GUEST_STRUCT;
    GUEST<INTEGER> iTotPages;
    GUEST<INTEGER> iCurPage;
    GUEST<INTEGER> iTotCopies;
    GUEST<INTEGER> iCurCopy;
    GUEST<INTEGER> iTotBands;
    GUEST<INTEGER> iCurBand;
    GUEST<BOOLEAN> fPgDirty;
    GUEST<BOOLEAN> fImaging;
    GUEST<THPrint> hPrint;
    GUEST<TPPrPort> pPRPort;
    GUEST<PicHandle> hPic;
};

/* From Technote 095 */
/* more stuff may be here */
typedef struct TPrDlg
{
    GUEST_STRUCT;
    GUEST<DialogRecord> dlg;
    GUEST<ProcPtr> pFltrProc;
    GUEST<ProcPtr> pItemProc;
    GUEST<THPrint> hPrintUsr;
    GUEST<BOOLEAN> fDoIt;
    GUEST<BOOLEAN> fDone;
    GUEST<LONGINT> lUser1;
    GUEST<LONGINT> lUser2;
    GUEST<LONGINT> lUser3;
    GUEST<LONGINT> lUser4;
    GUEST<INTEGER> iNumFst;
    GUEST<INTEGER> iNumLst;
} * TPPrDlg;

const LowMemGlobal<INTEGER> PrintErr { 0x944 }; // PrintMgr IMII-161 (true-b);

extern INTEGER C_PrError(void);
PASCAL_FUNCTION(PrError);
extern void C_PrSetError(INTEGER iErr);
PASCAL_FUNCTION(PrSetError);
extern void C_PrOpen(void);
PASCAL_FUNCTION(PrOpen);
extern void C_PrClose(void);
PASCAL_FUNCTION(PrClose);
extern void C_PrDrvrOpen(void);
PASCAL_FUNCTION(PrDrvrOpen);
extern void C_PrDrvrClose(void);
PASCAL_FUNCTION(PrDrvrClose);
extern void C_PrCtlCall(INTEGER iWhichCtl, LONGINT lParam1,
                                    LONGINT lParam2, LONGINT lParam3);
PASCAL_FUNCTION(PrCtlCall);
extern Handle C_PrDrvrDCE(void);
PASCAL_FUNCTION(PrDrvrDCE);
extern INTEGER C_PrDrvrVers(void);
PASCAL_FUNCTION(PrDrvrVers);
extern void C_ROMlib_myjobproc(DialogPtr dp, INTEGER itemno);
extern BOOLEAN C_ROMlib_stlfilterproc(DialogPeek dp,
                                             EventRecord *evt, GUEST<INTEGER> *ith);

extern BOOLEAN C_ROMlib_numsonlyfilterproc(DialogPeek dp,
                                                  EventRecord *evt,
                                                  GUEST<INTEGER> *ith);

extern void C_ROMlib_mystlproc(DialogPtr dp, INTEGER itemno);
extern TPPrDlg C_PrJobInit(THPrint hPrint);
PASCAL_FUNCTION(PrJobInit);
extern TPPrDlg C_PrStlInit(THPrint hPrint);
PASCAL_FUNCTION(PrStlInit);
extern BOOLEAN C_PrDlgMain(THPrint hPrint, ProcPtr initfptr);
PASCAL_FUNCTION(PrDlgMain);
extern void C_PrGeneral(Ptr pData);
PASCAL_FUNCTION(PrGeneral);
extern void C_donotPrArc(GrafVerb verb, Rect *r,
                                     INTEGER starta, INTEGER arca);
PASCAL_FUNCTION(donotPrArc);
extern void C_PrArc(GrafVerb verb, Rect *r, INTEGER starta,
                                INTEGER arca);
PASCAL_FUNCTION(PrArc);
extern void C_donotPrBits(BitMap *srcbmp, Rect *srcrp,
                                      Rect *dstrp, INTEGER mode,
                                      RgnHandle mask);
PASCAL_FUNCTION(donotPrBits);
extern void C_PrBits(BitMap *srcbmp, Rect *srcrp,
                                 Rect *dstrp, INTEGER mode, RgnHandle mask);
PASCAL_FUNCTION(PrBits);
extern void C_donotPrLine(Point p);
PASCAL_FUNCTION(donotPrLine);
extern void C_PrLine(Point p);
PASCAL_FUNCTION(PrLine);
extern void C_donotPrOval(GrafVerb v, Rect *rp);
PASCAL_FUNCTION(donotPrOval);
extern void C_PrOval(GrafVerb v, Rect *rp);
PASCAL_FUNCTION(PrOval);
extern void C_textasPS(INTEGER n, Ptr textbufp,
                                   Point num, Point den);
PASCAL_FUNCTION(textasPS);
extern void C_donotPrGetPic(Ptr dp, INTEGER bc);
PASCAL_FUNCTION(donotPrGetPic);
extern void C_PrGetPic(Ptr dp, INTEGER bc);
PASCAL_FUNCTION(PrGetPic);
extern void C_donotPrPutPic(Ptr sp, INTEGER bc);
PASCAL_FUNCTION(donotPrPutPic);
extern void C_PrPutPic(Ptr sp, INTEGER bc);
PASCAL_FUNCTION(PrPutPic);
extern void C_donotPrPoly(GrafVerb verb, PolyHandle ph);
PASCAL_FUNCTION(donotPrPoly);
extern void C_PrPoly(GrafVerb verb, PolyHandle ph);
PASCAL_FUNCTION(PrPoly);
extern void C_donotPrRRect(GrafVerb verb, Rect *r,
                                       INTEGER width, INTEGER height);
PASCAL_FUNCTION(donotPrRRect);
extern void C_PrRRect(GrafVerb verb, Rect *r, INTEGER width,
                                  INTEGER height);
PASCAL_FUNCTION(PrRRect);

extern void C_donotPrRect(GrafVerb v, Rect *rp);
PASCAL_FUNCTION(donotPrRect);
extern void C_PrRect(GrafVerb v, Rect *rp);
PASCAL_FUNCTION(PrRect);
extern void C_donotPrRgn(GrafVerb verb, RgnHandle rgn);
PASCAL_FUNCTION(donotPrRgn);
extern void C_PrRgn(GrafVerb verb, RgnHandle rgn);
PASCAL_FUNCTION(PrRgn);
extern INTEGER C_PrTxMeas(INTEGER n, Ptr p, GUEST<Point> *nump,
                                      GUEST<Point> *denp, FontInfo *finfop);
PASCAL_FUNCTION(PrTxMeas);
extern void C_donotPrText(INTEGER n, Ptr textbufp, Point num,
                                      Point den);
PASCAL_FUNCTION(donotPrText);
extern void C_PrText(INTEGER n, Ptr textbufp, Point num,
                                 Point den);
PASCAL_FUNCTION(PrText);
extern void C_PrComment(INTEGER kind, INTEGER size, Handle hand);
PASCAL_FUNCTION(PrComment);
extern TPPrPort C_PrOpenDoc(THPrint hPrint, TPPrPort port,
                                        Ptr pIOBuf);
PASCAL_FUNCTION(PrOpenDoc);
extern void C_PrOpenPage(TPPrPort port, TPRect pPageFrame);
PASCAL_FUNCTION(PrOpenPage);
extern void C_PrClosePage(TPPrPort pPrPort);
PASCAL_FUNCTION(PrClosePage);
extern void C_PrCloseDoc(TPPrPort port);
PASCAL_FUNCTION(PrCloseDoc);
extern void C_PrPicFile(THPrint hPrint, TPPrPort pPrPort,
                                    Ptr pIOBuf, Ptr pDevBuf,
                                    TPrStatus *prStatus);
PASCAL_FUNCTION(PrPicFile);
extern void C_PrintDefault(THPrint hPrint);
PASCAL_FUNCTION(PrintDefault);
extern BOOLEAN C_PrValidate(THPrint hPrint);
PASCAL_FUNCTION(PrValidate);
extern BOOLEAN C_PrStlDialog(THPrint hPrint);
PASCAL_FUNCTION(PrStlDialog);
extern BOOLEAN C_PrJobDialog(THPrint hPrint);
PASCAL_FUNCTION(PrJobDialog);
extern void C_PrJobMerge(THPrint hPrintSrc, THPrint hPrintDst);
PASCAL_FUNCTION(PrJobMerge);
}
#endif /* __PRINTING__ */
