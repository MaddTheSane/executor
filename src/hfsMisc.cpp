/* Copyright 1992 by Abacus Research and
 * Development, Inc.  All rights reserved.
 */

#if !defined (OMIT_RCSID_STRINGS)
char ROMlib_rcsid_hfsMisc[] =
	    "$Id: hfsMisc.c 63 2004-12-24 18:19:43Z ctm $";
#endif

#include "rsys/common.h"
#include "OSUtil.h"
#include "FileMgr.h"
#include "MemoryMgr.h"
#include "rsys/hfs.h"
#include "rsys/file.h"

using namespace Executor;

#if defined (TESTFCB)
PUBLIC void testfcb()
{
    short length;
    filecontrolblock *fp;
    INTEGER i;
    
    length =  CW(*(short *)CL(FCBSPtr));
    fp = (filecontrolblock *)((short *)CL(FCBSPtr) + 1);
    printf("length = %d, length / 94 = %d, length mod 94 = %d\n",
	length, length / 94, length % 94);
    for (i = 0; i < 40 && i < length / 94; i++, fp++) {
	printf("# %ld flags 0x%x vers %d sblk %d EOF %ld PLEN %ld mark %ld\n"
	       "vptr 0x%lx pbuffer 0x%lx FlPos %d clmpsiz %ld BTCBPtr 0x%lx\n"
	       "ext (%d %d) (%d %d) (%d %d) FNDR '%c%c%c%c' CatPos 0x%lx\n"
	       "parid %ld name %s\n", CL(fp->fcbFlNum), fp->fcbMdRByt,
	       fp->fcbTypByt, CW(fp->fcbSBlk), CL(fp->fcbEOF), CL(fp->fcbPLen), CL(fp->fcbCrPs),
	       CL(fp->fcbVPtr), CL(fp->fcbBfAdr), CW(fp->fcbFlPos), CL(fp->fcbClmpSize),
	       CL(fp->fcbBTCBPtr),
	       CW(fp->fcbExtRec[0].blockstart), CW(fp->fcbExtRec[0].blockcount),
	       CW(fp->fcbExtRec[1].blockstart), CW(fp->fcbExtRec[1].blockcount),
	       CW(fp->fcbExtRec[2].blockstart), CW(fp->fcbExtRec[2].blockcount),
	       (short) (CL(fp->fcbFType) >> 24), (short) (CL(fp->fcbFType) >> 16),
	       (short) CL(fp->fcbFType) >> 8,  (short) CL(fp->fcbFType),
	       CL(fp->fcbCatPos), CL(fp->fcbDirID), fp->fcbCName+1);
    }
}
#endif /* TESTFCB */

#if 0
PUBLIC void myFInitQueue( void )    /* IMIV-128 */
{
    /* When we support asynchronous stuff we'll have to do this */
}

PUBLIC QHdrPtr myGetFSQHdr( void )
{
    return &FSQHdr;
}

PUBLIC QHdrPtr myGetVCBQHdr( void )
{
    return &VCBQHdr;
}
#endif

/*
 * NOTE:  This is *the* PCBGetFCBInfo that we use whether
 *	  we're looking at hfs or ufs stuff.
 */

A2(PUBLIC trap, OSErrRET, PBGetFCBInfo, FCBPBPtr, pb, BOOLEAN, async)
{
    filecontrolblock *fcbp, *efcbp;
    INTEGER i;
    
    if ((i = CW(pb->ioFCBIndx)) > 0) {
	fcbp  = (filecontrolblock *) (MR(FCBSPtr) + sizeof(INTEGER));
	efcbp = (filecontrolblock *) (MR(FCBSPtr) + CW(*(INTEGER *)MR(FCBSPtr)));
	if (CW(pb->ioVRefNum) < 0) {
	    for (;fcbp != efcbp; fcbp++)
		if (fcbp->fcbFlNum &&
		        MR(fcbp->fcbVPtr)->vcbVRefNum == pb->ioVRefNum && --i <= 0)
		    break;
	} else if (pb->ioVRefNum == 0) {
	    for (;fcbp != efcbp && (fcbp->fcbFlNum == 0 || --i > 0); fcbp++)
		;
	} else /* if (CW(pb->ioVRefNum) > 0 */ {
	    for (;fcbp != efcbp; fcbp++)
		if (fcbp->fcbFlNum &&
		         MR(fcbp->fcbVPtr)->vcbDrvNum == pb->ioVRefNum && --i <= 0)
		    break;
	}
	if (fcbp == efcbp)
	    PBRETURN(pb, fnOpnErr);
	pb->ioRefNum = CW((char *) fcbp - (char *) MR(FCBSPtr));
    } else {
	fcbp = ROMlib_refnumtofcbp(CW(pb->ioRefNum));
	if (!fcbp)
	    PBRETURN(pb, rfNumErr);
    }
    if (pb->ioNamePtr)
	str255assign(MR(pb->ioNamePtr), fcbp->fcbCName);
    pb->ioFCBFlNm = fcbp->fcbFlNum;
    pb->ioFCBFlags = CW((fcbp->fcbMdRByt <<8) | (unsigned char) fcbp->fcbTypByt);
    pb->ioFCBStBlk = fcbp->fcbSBlk;
    pb->ioFCBEOF = fcbp->fcbEOF;
    if (MR(fcbp->fcbVPtr)->vcbCTRef) {
	pb->ioFCBCrPs = fcbp->fcbCrPs;	/* HFS */
	pb->ioFCBPLen = fcbp->fcbPLen;
    } else {
	pb->ioFCBCrPs = CL((ULONGINT)(lseek(((fcbrec *)fcbp)->fcfd, 0, SEEK_CUR) -    /* UFS */
						   FORKOFFSET((fcbrec *) fcbp)));
	pb->ioFCBPLen = fcbp->fcbEOF;
    }
    pb->ioFCBVRefNum = MR(fcbp->fcbVPtr)->vcbVRefNum;
    if (CW(pb->ioFCBIndx) <= 0 || pb->ioVRefNum == 0)
	pb->ioVRefNum = pb->ioFCBVRefNum;
    pb->ioFCBClpSiz = fcbp->fcbClmpSize;
    pb->ioFCBParID = fcbp->fcbDirID;
    PBRETURN(pb, noErr);
}

#if 0
PUBLIC QHdrPtr myGetDrvQHdr( void )
{
    return &DrvQHdr;
}
#endif

#if defined(MAC)
#define LINKHACK
#endif

#if defined (LINKHACK)
PUBLIC void bcopy(void *srcp, void *dstp, LONGINT length)
{
    BlockMove(srcp, dstp, length);
}

PUBLIC void bzero(void *dstp, LONGINT ntozero)
{
    char *dstcp;
    
    dstcp = dstp;
    while (--ntozero >= 0)
	*dstcp++ = 0;
}
#endif /* LINKHACK */
