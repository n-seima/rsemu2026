#include <string.h>
#include <stdio.h>
#include <time.h>
#include <ddraw.h>
#include <dsound.h>
#include <d3drm.h>
#include "cMESSAGE.H"
#define	dDEBUG_MODE

char		currentDir[1024];		//	현재 디렉토리
char		buffer[1024];
va_list		argptr;
char		reportDate[128];
char		reportTime[128];
FILE		*fp=NULL;

char		ReportFile[20]		=	"Result.txt";
int			ErrorCount	=	0;

cMSG		temp;

BOOL
Error()
{	if (ErrorCount > 0)
	{	cMSG::Error(_ms("%d Errors occured!!",ErrorCount),"Please Check \"%s\" File",ReportFile);

		return TRUE;
	}

	return FALSE;
}

cMSG::cMSG()
{
#ifdef dDEBUG_MODE

	_strdate( reportDate );
	_strtime( reportTime );

	fp	=	fopen("result.txt", "wb" );

	if (fp)
	{	fprintf( fp, "화일 생성 시간 : date[%s] time[%s] \n", reportDate, reportTime);
		fclose(fp);
	}

#endif
}

void
cMSG::Test(int num)
{	static int count=0;

	if (num==-1) count++;
	else count=num;

	Put("Message from Message Hadler","Test!! It's Check Point No.%d!!",count);
}

int
cMSG::YESNO(char *headmsg,char *mainmsg,...)
{	if (!mainmsg) mainmsg="(null)";

	va_start(argptr, mainmsg);
	vsprintf(buffer,mainmsg,argptr);
	va_end(argptr);

	return	MessageBox(GetActiveWindow(),buffer,headmsg,MB_ICONQUESTION|MB_YESNO|MB_TOPMOST);
}

int
cMSG::YESNOCANCEL(char *headmsg,char *mainmsg,...)
{	if (!mainmsg) mainmsg="(null)";

	va_start(argptr, mainmsg);
	vsprintf(buffer,mainmsg,argptr);
	va_end(argptr);

	return MessageBox(GetActiveWindow(),buffer,headmsg,MB_ICONQUESTION|MB_YESNOCANCEL|MB_TOPMOST);
}

BOOL
cMSG::Output(char *mainmsg,...)
{
#ifdef dDEBUG_MODE

	GetCurrentDirectory(sizeof(currentDir),currentDir);
	SetCurrentDirectory(_logFolder);

	if (!mainmsg) mainmsg="(null)";

	va_start(argptr, mainmsg);
	vsprintf(buffer,mainmsg,argptr);
	va_end	(argptr);

	_strtime( reportTime );
	fp = fopen(ReportFile, "a+" );

	if (fp)
	{	fprintf( fp, "[%s] Message : %s\n",reportTime,buffer );
		fclose( fp );
	}

	SetCurrentDirectory(currentDir);

#endif

	return FALSE;
}

BOOL
cMSG::Put(char *mainmsg,...)
{
#ifdef dDEBUG_MODE

	GetCurrentDirectory(sizeof(currentDir),currentDir);
	SetCurrentDirectory(_logFolder);

	if (!mainmsg) mainmsg="(null)";

	va_start(argptr, mainmsg);
	vsprintf(buffer,mainmsg,argptr);
	va_end	(argptr);

	_strtime( reportTime );
	fp = fopen(ReportFile, "a+" );

	if (fp)
	{	fprintf( fp, "[%s] Message : %s\n",reportTime,buffer );
		fclose( fp );
	}

	SetCurrentDirectory(currentDir);

#endif

	MessageBox(GetActiveWindow(),buffer,"Message from Message Handler",MB_OK|MB_TOPMOST|MB_ICONINFORMATION);

	return FALSE;
}

BOOL
cMSG::Put(char *headmsg,char *mainmsg,...)
{	if (!mainmsg) mainmsg="(null)";

	va_start(argptr, mainmsg);
	vsprintf(buffer,mainmsg,argptr);
	va_end(argptr);

	MessageBox(GetActiveWindow(),buffer,headmsg,MB_OK|MB_TOPMOST|MB_ICONINFORMATION);

#ifdef dDEBUG_MODE
	GetCurrentDirectory(sizeof(currentDir),currentDir);
	SetCurrentDirectory(_logFolder);

	_strtime( reportTime );
	fp = fopen(ReportFile, "a+" );

	if (fp)
	{	fprintf( fp, "[%s] Message : %s - %s\n",reportTime,headmsg,buffer );
		fclose( fp );
	}

	SetCurrentDirectory(currentDir);
#endif

	return FALSE;
}

BOOL
cMSG::Error(char *mainmsg,...)
{	
#ifdef dDEBUG_MODE
	if (!mainmsg) mainmsg="(null)";

	GetCurrentDirectory(sizeof(currentDir),currentDir);
	SetCurrentDirectory(_logFolder);

	va_start(argptr, mainmsg);
	vsprintf(buffer,mainmsg,argptr);
	va_end	(argptr);

	_strtime( reportTime );
	fp = fopen(ReportFile, "a+" );

	if (fp)
	{	fprintf( fp, "[%s] Message : %s\n",reportTime,buffer );
		fclose( fp );
	}

	SetCurrentDirectory(currentDir);
#endif

	ErrorCount++;

	return FALSE;
}

BOOL
cMSG::Error(char *headmsg,char *mainmsg,...)
{	if (!mainmsg) mainmsg="(null)";

	va_start(argptr, mainmsg);
	vsprintf(buffer,mainmsg,argptr);
	va_end(argptr);

	MessageBox(GetActiveWindow(),buffer,headmsg,MB_ICONERROR|MB_TOPMOST);

	ErrorCount++;

#ifdef dDEBUG_MODE
	GetCurrentDirectory(sizeof(currentDir),currentDir);
	SetCurrentDirectory(_logFolder);

	_strtime( reportTime );
	fp = fopen(ReportFile, "a+" );

	if (fp)
	{	fprintf( fp, "[%s] Message : %s - %s\n",reportTime,headmsg,buffer );
		fclose( fp );
	}
	SetCurrentDirectory(currentDir);
#endif

	return FALSE;
}

BOOL
cMSG::Put(HWND hwnd,char *msg)
{	MessageBox(hwnd,msg,"Message Handler Message!!",MB_OK|MB_TOPMOST);

	return FALSE;
}

char*
DDErrorToString(HRESULT hErr)
{	switch (hErr)
    {	case DDERR_ALREADYINITIALIZED : return("DDERR_ALREADYINITIALIZED");
        case DDERR_CANNOTATTACHSURFACE : return("DDERR_CANNOTATTACHSURFACE"); 
        case DDERR_CANNOTDETACHSURFACE : return("DDERR_CANNOTDETACHSURFACE"); 
        case DDERR_CURRENTLYNOTAVAIL : return("DDERR_CURRENTLYNOTAVAIL"); 
        case DDERR_EXCEPTION : return("DDERR_EXCEPTION"); 
        case DDERR_GENERIC : return("DDERR_GENERIC"); 
        case DDERR_HEIGHTALIGN : return("DDERR_HEIGHTALIGN"); 
        case DDERR_INCOMPATIBLEPRIMARY : return("DDERR_INCOMPATIBLEPRIMARY"); 
        case DDERR_INVALIDCAPS : return("DDERR_INVALIDCAPS"); 
        case DDERR_INVALIDCLIPLIST : return("DDERR_INVALIDCLIPLIST"); 
        case DDERR_INVALIDMODE : return("DDERR_INVALIDMODE"); 
        case DDERR_INVALIDOBJECT : return("DDERR_INVALIDOBJECT"); 
        case DDERR_INVALIDPARAMS : return("DDERR_INVALIDPARAMS"); 
        case DDERR_INVALIDPIXELFORMAT : return("DDERR_INVALIDPIXELFORMAT"); 
        case DDERR_INVALIDRECT : return("DDERR_INVALIDRECT"); 
        case DDERR_LOCKEDSURFACES : return("DDERR_LOCKEDSURFACES"); 
        case DDERR_NO3D : return("DDERR_NO3D"); 
        case DDERR_NOALPHAHW : return("DDERR_NOALPHAHW"); 
        case DDERR_NOCLIPLIST : return("DDERR_NOCLIPLIST"); 
        case DDERR_NOCOLORCONVHW : return("DDERR_NOCOLORCONVHW"); 
        case DDERR_NOCOOPERATIVELEVELSET : return("DDERR_NOCOOPERATIVELEVELSET"); 
        case DDERR_NOCOLORKEY : return("DDERR_NOCOLORKEY"); 
        case DDERR_NOCOLORKEYHW : return("DDERR_NOCOLORKEYHW"); 
        case DDERR_NODIRECTDRAWSUPPORT : return("DDERR_NODIRECTDRAWSUPPORT"); 
        case DDERR_NOEXCLUSIVEMODE : return("DDERR_NOEXCLUSIVEMODE"); 
        case DDERR_NOFLIPHW : return("DDERR_NOFLIPHW"); 
        case DDERR_NOGDI : return("DDERR_NOGDI"); 
        case DDERR_NOMIRRORHW : return("DDERR_NOMIRRORHW"); 
        case DDERR_NOTFOUND : return("DDERR_NOTFOUND"); 
        case DDERR_NOOVERLAYHW : return("DDERR_NOOVERLAYHW"); 
        case DDERR_NORASTEROPHW : return("DDERR_NORASTEROPHW"); 
        case DDERR_NOROTATIONHW : return("DDERR_NOROTATIONHW"); 
        case DDERR_NOSTRETCHHW : return("DDERR_NOSTRETCHHW"); 
        case DDERR_NOT4BITCOLOR : return("DDERR_NOT4BITCOLOR"); 
        case DDERR_NOT4BITCOLORINDEX : return("DDERR_NOT4BITCOLORINDEX"); 
        case DDERR_NOT8BITCOLOR : return("DDERR_NOT8BITCOLOR"); 
        case DDERR_NOTEXTUREHW : return("DDERR_NOTEXTUREHW"); 
        case DDERR_NOVSYNCHW : return("DDERR_NOVSYNCHW"); 
        case DDERR_NOZBUFFERHW : return("DDERR_NOZBUFFERHW"); 
        case DDERR_NOZOVERLAYHW : return("DDERR_NOZOVERLAYHW"); 
        case DDERR_OUTOFCAPS : return("DDERR_OUTOFCAPS"); 
        case DDERR_OUTOFMEMORY : return("DDERR_OUTOFMEMORY"); 
        case DDERR_OUTOFVIDEOMEMORY : return("DDERR_OUTOFVIDEOMEMORY"); 
        case DDERR_OVERLAYCANTCLIP : return("DDERR_OVERLAYCANTCLIP"); 
        case DDERR_OVERLAYCOLORKEYONLYONEACTIVE : return("DDERR_OVERLAYCOLORKEYONLYONEACTIVE"); 
        case DDERR_PALETTEBUSY : return("DDERR_PALETTEBUSY"); 
        case DDERR_COLORKEYNOTSET : return("DDERR_COLORKEYNOTSET"); 
        case DDERR_SURFACEALREADYATTACHED : return("DDERR_SURFACEALREADYATTACHED"); 
        case DDERR_SURFACEALREADYDEPENDENT : return("DDERR_SURFACEALREADYDEPENDENT"); 
        case DDERR_SURFACEBUSY : return("DDERR_SURFACEBUSY"); 
        case DDERR_CANTLOCKSURFACE : return("DDERR_CANTLOCKSURFACE"); 
        case DDERR_SURFACEISOBSCURED : return("DDERR_SURFACEISOBSCURED"); 
        case DDERR_SURFACELOST : return("DDERR_SURFACELOST"); 
        case DDERR_SURFACENOTATTACHED : return("DDERR_SURFACENOTATTACHED"); 
        case DDERR_TOOBIGHEIGHT : return("DDERR_TOOBIGHEIGHT"); 
        case DDERR_TOOBIGSIZE : return("DDERR_TOOBIGSIZE"); 
        case DDERR_TOOBIGWIDTH : return("DDERR_TOOBIGWIDTH"); 
        case DDERR_UNSUPPORTED : return("DDERR_UNSUPPORTED"); 
        case DDERR_UNSUPPORTEDFORMAT : return("DDERR_UNSUPPORTEDFORMAT"); 
        case DDERR_UNSUPPORTEDMASK : return("DDERR_UNSUPPORTEDMASK"); 
        case DDERR_VERTICALBLANKINPROGRESS : return("DDERR_VERTICALBLANKINPROGRESS"); 
        case DDERR_WASSTILLDRAWING : return("DDERR_WASSTILLDRAWING"); 
        case DDERR_XALIGN : return("DDERR_XALIGN"); 
        case DDERR_INVALIDDIRECTDRAWGUID : return("DDERR_INVALIDDIRECTDRAWGUID"); 
        case DDERR_DIRECTDRAWALREADYCREATED : return("DDERR_DIRECTDRAWALREADYCREATED"); 
        case DDERR_NODIRECTDRAWHW : return("DDERR_NODIRECTDRAWHW"); 
        case DDERR_PRIMARYSURFACEALREADYEXISTS : return("DDERR_PRIMARYSURFACEALREADYEXISTS"); 
        case DDERR_NOEMULATION : return("DDERR_NOEMULATION"); 
        case DDERR_REGIONTOOSMALL : return("DDERR_REGIONTOOSMALL"); 
        case DDERR_CLIPPERISUSINGHWND : return("DDERR_CLIPPERISUSINGHWND"); 
        case DDERR_NOCLIPPERATTACHED : return("DDERR_NOCLIPPERATTACHED"); 
        case DDERR_NOHWND : return("DDERR_NOHWND"); 
        case DDERR_HWNDSUBCLASSED : return("DDERR_HWNDSUBCLASSED"); 
        case DDERR_HWNDALREADYSET : return("DDERR_HWNDALREADYSET"); 
        case DDERR_NOPALETTEATTACHED : return("DDERR_NOPALETTEATTACHED"); 
        case DDERR_NOPALETTEHW : return("DDERR_NOPALETTEHW"); 
        case DDERR_BLTFASTCANTCLIP : return("DDERR_BLTFASTCANTCLIP"); 
        case DDERR_NOBLTHW : return("DDERR_NOBLTHW"); 
        case DDERR_NODDROPSHW : return("DDERR_NODDROPSHW"); 
        case DDERR_OVERLAYNOTVISIBLE : return("DDERR_OVERLAYNOTVISIBLE"); 
        case DDERR_NOOVERLAYDEST : return("DDERR_NOOVERLAYDEST"); 
        case DDERR_INVALIDPOSITION : return("DDERR_INVALIDPOSITION"); 
        case DDERR_NOTAOVERLAYSURFACE : return("DDERR_NOTAOVERLAYSURFACE"); 
        case DDERR_EXCLUSIVEMODEALREADYSET : return("DDERR_EXCLUSIVEMODEALREADYSET"); 
        case DDERR_NOTFLIPPABLE : return("DDERR_NOTFLIPPABLE"); 
        case DDERR_CANTDUPLICATE : return("DDERR_CANTDUPLICATE"); 
        case DDERR_NOTLOCKED : return("DDERR_NOTLOCKED"); 
        case DDERR_CANTCREATEDC : return("DDERR_CANTCREATEDC"); 
        case DDERR_NODC : return("DDERR_NODC"); 
        case DDERR_WRONGMODE : return("DDERR_WRONGMODE"); 
        case DDERR_IMPLICITLYCREATED : return("DDERR_IMPLICITLYCREATED"); 
        case DDERR_NOTPALETTIZED : return("DDERR_NOTPALETTIZED"); 
        case DDERR_UNSUPPORTEDMODE : return("DDERR_UNSUPPORTEDMODE"); 
        case DDERR_NOMIPMAPHW : return("DDERR_NOMIPMAPHW"); 
        case DDERR_INVALIDSURFACETYPE : return("DDERR_INVALIDSURFACETYPE"); 
        case DDERR_DCALREADYCREATED : return("DDERR_DCALREADYCREATED"); 
        case DDERR_CANTPAGELOCK : return("DDERR_CANTPAGELOCK"); 
        case DDERR_CANTPAGEUNLOCK : return("DDERR_CANTPAGEUNLOCK"); 
        case DDERR_NOTPAGELOCKED : return("DDERR_NOTPAGELOCKED"); 
        case DDERR_NOTINITIALIZED : return("DDERR_NOTINITIALIZED"); 

        default : return("Unknown Error"); 
    }
}

//----------------------------------------------------------------------
// 
// Function     : DSErrorToString(HRESULT hErr)
//
// Purpose      : Traces an error (DirectSound)
//
//----------------------------------------------------------------------

char*
DSErrorToString(HRESULT hErr)
{	switch (hErr)
    {	case DSERR_ALLOCATED : return("DSERR_ALLOCATED"); 
        case DSERR_CONTROLUNAVAIL : return("DSERR_CONTROLUNAVAIL"); 
        case DSERR_INVALIDPARAM : return("DSERR_INVALIDPARAM"); 
        case DSERR_INVALIDCALL : return("DSERR_INVALIDCALL"); 
        case DSERR_GENERIC : return("DSERR_GENERIC"); 
        case DSERR_PRIOLEVELNEEDED : return("DSERR_PRIOLEVELNEEDED"); 
        case DSERR_OUTOFMEMORY : return("DSERR_OUTOFMEMORY"); 
        case DSERR_BADFORMAT : return("DSERR_BADFORMAT"); 
        case DSERR_UNSUPPORTED : return("DSERR_UNSUPPORTED"); 
        case DSERR_NODRIVER : return("DSERR_NODRIVER"); 
        case DSERR_ALREADYINITIALIZED : return("DSERR_ALREADYINITIALIZED"); 
        case DSERR_NOAGGREGATION : return("DSERR_NOAGGREGATION"); 
        case DSERR_BUFFERLOST : return("DSERR_BUFFERLOST"); 
        case DSERR_OTHERAPPHASPRIO : return("DSERR_OTHERAPPHASPRIO"); 
        case DSERR_UNINITIALIZED : return("DSERR_UNINITIALIZED"); 

        default : return("Unknown Error"); 
    }
}

//----------------------------------------------------------------------
// 
// Function     : D3DErrorToString(HRESULT hErr)
//
// Purpose      : Traces an error (Direct3D)
//
//----------------------------------------------------------------------

char*
D3DErrorToString(HRESULT hErr)
{	switch (hErr)
    {	case D3DERR_BADMAJORVERSION : return("D3DERR_BADMAJORVERSION"); 
        case D3DERR_BADMINORVERSION : return("D3DERR_BADMINORVERSION"); 
        case D3DERR_EXECUTE_CREATE_FAILED : return("D3DERR_EXECUTE_CREATE_FAILED"); 
        case D3DERR_EXECUTE_DESTROY_FAILED : return("D3DERR_EXECUTE_DESTROY_FAILED"); 
        case D3DERR_EXECUTE_LOCK_FAILED : return("D3DERR_EXECUTE_LOCK_FAILED"); 
        case D3DERR_EXECUTE_UNLOCK_FAILED : return("D3DERR_EXECUTE_UNLOCK_FAILED"); 
        case D3DERR_EXECUTE_LOCKED : return("D3DERR_EXECUTE_LOCKED"); 
        case D3DERR_EXECUTE_NOT_LOCKED : return("D3DERR_EXECUTE_NOT_LOCKED"); 
        case D3DERR_EXECUTE_FAILED : return("D3DERR_EXECUTE_FAILED"); 
        case D3DERR_EXECUTE_CLIPPED_FAILED : return("D3DERR_EXECUTE_CLIPPED_FAILED"); 
        case D3DERR_TEXTURE_NO_SUPPORT : return("D3DERR_TEXTURE_NO_SUPPORT"); 
        case D3DERR_TEXTURE_CREATE_FAILED : return("D3DERR_TEXTURE_CREATE_FAILED"); 
        case D3DERR_TEXTURE_DESTROY_FAILED : return("D3DERR_TEXTURE_DESTROY_FAILED"); 
        case D3DERR_TEXTURE_LOCK_FAILED : return("D3DERR_TEXTURE_LOCK_FAILED"); 
        case D3DERR_TEXTURE_UNLOCK_FAILED : return("D3DERR_TEXTURE_UNLOCK_FAILED"); 
        case D3DERR_TEXTURE_LOAD_FAILED : return("D3DERR_TEXTURE_LOAD_FAILED"); 
        case D3DERR_TEXTURE_SWAP_FAILED : return("D3DERR_TEXTURE_SWAP_FAILED"); 
        case D3DERR_TEXTURE_LOCKED : return("D3DERR_TEXTURE_LOCKED"); 
        case D3DERR_TEXTURE_NOT_LOCKED : return("D3DERR_TEXTURE_NOT_LOCKED"); 
        case D3DERR_TEXTURE_GETSURF_FAILED : return("D3DERR_TEXTURE_GETSURF_FAILED"); 
        case D3DERR_MATRIX_CREATE_FAILED : return("D3DERR_MATRIX_CREATE_FAILED"); 
        case D3DERR_MATRIX_DESTROY_FAILED : return("D3DERR_MATRIX_DESTROY_FAILED"); 
        case D3DERR_MATRIX_SETDATA_FAILED : return("D3DERR_MATRIX_SETDATA_FAILED"); 
        case D3DERR_MATRIX_GETDATA_FAILED : return("D3DERR_MATRIX_GETDATA_FAILED"); 
        case D3DERR_SETVIEWPORTDATA_FAILED : return("D3DERR_SETVIEWPORTDATA_FAILED"); 
        case D3DERR_MATERIAL_CREATE_FAILED : return("D3DERR_MATERIAL_CREATE_FAILED"); 
        case D3DERR_MATERIAL_DESTROY_FAILED : return("D3DERR_MATERIAL_DESTROY_FAILED"); 
        case D3DERR_MATERIAL_SETDATA_FAILED : return("D3DERR_MATERIAL_SETDATA_FAILED"); 
        case D3DERR_MATERIAL_GETDATA_FAILED : return("D3DERR_MATERIAL_GETDATA_FAILED"); 
        case D3DERR_LIGHT_SET_FAILED : return("D3DERR_LIGHT_SET_FAILED"); 
        case D3DERR_SCENE_IN_SCENE : return("D3DERR_SCENE_IN_SCENE"); 
        case D3DERR_SCENE_NOT_IN_SCENE : return("D3DERR_SCENE_NOT_IN_SCENE"); 
        case D3DERR_SCENE_BEGIN_FAILED : return("D3DERR_SCENE_BEGIN_FAILED"); 
        case D3DERR_SCENE_END_FAILED : return("D3DERR_SCENE_END_FAILED"); 

        default : return("Unknown Error"); 
    }
}

//----------------------------------------------------------------------
// 
// Function     : D3DRMErrorToString(HRESULT hErr)
//
// Purpose      : Traces an error (Direct3D retained mode)
//
//----------------------------------------------------------------------

char*
D3DRMErrorToString(HRESULT hErr)
{	switch (hErr)
    {	case D3DRMERR_BADOBJECT : return("D3DRMERR_BADOBJECT"); 
        case D3DRMERR_BADTYPE : return("D3DRMERR_BADTYPE"); 
        case D3DRMERR_BADALLOC : return("D3DRMERR_BADALLOC"); 
        case D3DRMERR_FACEUSED : return("D3DRMERR_FACEUSED"); 
        case D3DRMERR_NOTFOUND : return("D3DRMERR_NOTFOUND"); 
        case D3DRMERR_NOTDONEYET : return("D3DRMERR_NOTDONEYET"); 
        case D3DRMERR_FILENOTFOUND : return("D3DRMERR_FILENOTFOUND"); 
        case D3DRMERR_BADFILE : return("D3DRMERR_BADFILE"); 
        case D3DRMERR_BADDEVICE : return("D3DRMERR_BADDEVICE"); 
        case D3DRMERR_BADVALUE : return("D3DRMERR_BADVALUE"); 
        case D3DRMERR_BADMAJORVERSION : return("D3DRMERR_BADMAJORVERSION"); 
        case D3DRMERR_BADMINORVERSION : return("D3DRMERR_BADMINORVERSION"); 
        case D3DRMERR_UNABLETOEXECUTE : return("D3DRMERR_UNABLETOEXECUTE"); 

        default : return("Unknown Error"); 
    }
}

char*
DXErrorToString(HRESULT hErr)
{	switch (hErr)
    {	//DirectDraw
		case DD_OK:
		/* Also includes D3D_OK and D3DRM_OK */
			return "No error.\0";
		case DDERR_ALREADYINITIALIZED:
			return "This object is already initialized.\0";
		case DDERR_BLTFASTCANTCLIP:
			return "Return if a clipper object is attached to the source surface passed into a BltFast call.\0";
		case DDERR_CANNOTATTACHSURFACE:
			return "This surface can not be attached to the requested surface.\0";
		case DDERR_CANNOTDETACHSURFACE:
			return "This surface can not be detached from the requested surface.\0";
		case DDERR_CANTCREATEDC:
			return "Windows can not create any more DCs.\0";
		case DDERR_CANTDUPLICATE:
			return "Can't duplicate primary & 3D surfaces, or surfaces that are implicitly created.\0";
		case DDERR_CLIPPERISUSINGHWND:
			return "An attempt was made to set a cliplist for a clipper object that is already monitoring an hwnd.\0";
		case DDERR_COLORKEYNOTSET:
			return "No src color key specified for this operation.\0";
		case DDERR_CURRENTLYNOTAVAIL:
			return "Support is currently not available.\0";
		case DDERR_DIRECTDRAWALREADYCREATED:
			return "A DirectDraw object representing this driver has already been created for this process.\0";
		case DDERR_EXCEPTION:
			return "An exception was encountered while performing the requested operation.\0";
		case DDERR_EXCLUSIVEMODEALREADYSET:
			return "An attempt was made to set the cooperative level when it was already set to exclusive.\0";
		case DDERR_GENERIC:
			return "Generic failure.\0";
		case DDERR_HEIGHTALIGN:
			return "Height of rectangle provided is not a multiple of reqd alignment.\0";
		case DDERR_HWNDALREADYSET:
			return "The CooperativeLevel HWND has already been set. It can not be reset while the process has surfaces or palettes created.\0";
		case DDERR_HWNDSUBCLASSED:
			return "HWND used by DirectDraw CooperativeLevel has been subclassed, this prevents DirectDraw from restoring state.\0";
		case DDERR_IMPLICITLYCREATED:
			return "This surface can not be restored because it is an implicitly created surface.\0";
		case DDERR_INCOMPATIBLEPRIMARY:
			return "Unable to match primary surface creation request with existing primary surface.\0";
		case DDERR_INVALIDCAPS:
			return "One or more of the caps bits passed to the callback are incorrect.\0";
		case DDERR_INVALIDCLIPLIST:
			return "DirectDraw does not support the provided cliplist.\0";
		case DDERR_INVALIDDIRECTDRAWGUID:
			return "The GUID passed to DirectDrawCreate is not a valid DirectDraw driver identifier.\0";
		case DDERR_INVALIDMODE:
			return "DirectDraw does not support the requested mode.\0";
		case DDERR_INVALIDOBJECT:
			return "DirectDraw received a pointer that was an invalid DIRECTDRAW object.\0";
		case DDERR_INVALIDPARAMS:
			return "One or more of the parameters passed to the function are incorrect.\0";
		case DDERR_INVALIDPIXELFORMAT:
			return "The pixel format was invalid as specified.\0";
		case DDERR_INVALIDPOSITION:
			return "Returned when the position of the overlay on the destination is no longer legal for that destination.\0";
		case DDERR_INVALIDRECT:
			return "Rectangle provided was invalid.\0";
		case DDERR_LOCKEDSURFACES:
			return "Operation could not be carried out because one or more surfaces are locked.\0";
		case DDERR_NO3D:
			return "There is no 3D present.\0";
		case DDERR_NOALPHAHW:
			return "Operation could not be carried out because there is no alpha accleration hardware present or available.\0";
		case DDERR_NOBLTHW:
			return "No blitter hardware present.\0";
		case DDERR_NOCLIPLIST:
			return "No cliplist available.\0";
		case DDERR_NOCLIPPERATTACHED:
			return "No clipper object attached to surface object.\0";
		case DDERR_NOCOLORCONVHW:
			return "Operation could not be carried out because there is no color conversion hardware present or available.\0";
		case DDERR_NOCOLORKEY:
			return "Surface doesn't currently have a color key\0";
		case DDERR_NOCOLORKEYHW:
			return "Operation could not be carried out because there is no hardware support of the destination color key.\0";
		case DDERR_NOCOOPERATIVELEVELSET:
			return "Create function called without DirectDraw object method SetCooperativeLevel being called.\0";
		case DDERR_NODC:
			return "No DC was ever created for this surface.\0";
		case DDERR_NODDROPSHW:
			return "No DirectDraw ROP hardware.\0";
		case DDERR_NODIRECTDRAWHW:
			return "A hardware-only DirectDraw object creation was attempted but the driver did not support any hardware.\0";
		case DDERR_NOEMULATION:
			return "Software emulation not available.\0";
		case DDERR_NOEXCLUSIVEMODE:
			return "Operation requires the application to have exclusive mode but the application does not have exclusive mode.\0";
		case DDERR_NOFLIPHW:
			return "Flipping visible surfaces is not supported.\0";
		case DDERR_NOGDI:
			return "There is no GDI present.\0";
		case DDERR_NOHWND:
			return "Clipper notification requires an HWND or no HWND has previously been set as the CooperativeLevel HWND.\0";
		case DDERR_NOMIRRORHW:
			return "Operation could not be carried out because there is no hardware present or available.\0";
		case DDERR_NOOVERLAYDEST:
			return "Returned when GetOverlayPosition is called on an overlay that UpdateOverlay has never been called on to establish a destination.\0";
		case DDERR_NOOVERLAYHW:
			return "Operation could not be carried out because there is no overlay hardware present or available.\0";
		case DDERR_NOPALETTEATTACHED:
			return "No palette object attached to this surface.\0";
		case DDERR_NOPALETTEHW:
			return "No hardware support for 16 or 256 color palettes.\0";
		case DDERR_NORASTEROPHW:
			return "Operation could not be carried out because there is no appropriate raster op hardware present or available.\0";
		case DDERR_NOROTATIONHW:
			return "Operation could not be carried out because there is no rotation hardware present or available.\0";
		case DDERR_NOSTRETCHHW:
			return "Operation could not be carried out because there is no hardware support for stretching.\0";
		case DDERR_NOT4BITCOLOR:
			return "DirectDrawSurface is not in 4 bit color palette and the requested operation requires 4 bit color palette.\0";
		case DDERR_NOT4BITCOLORINDEX:
			return "DirectDrawSurface is not in 4 bit color index palette and the requested operation requires 4 bit color index palette.\0";
		case DDERR_NOT8BITCOLOR:
			return "DirectDrawSurface is not in 8 bit color mode and the requested operation requires 8 bit color.\0";
		case DDERR_NOTAOVERLAYSURFACE:
			return "Returned when an overlay member is called for a non-overlay surface.\0";
		case DDERR_NOTEXTUREHW:
			return "Operation could not be carried out because there is no texture mapping hardware present or available.\0";
		case DDERR_NOTFLIPPABLE:
			return "An attempt has been made to flip a surface that is not flippable.\0";
		case DDERR_NOTFOUND:
			return "Requested item was not found.\0";
		case DDERR_NOTLOCKED:
			return "Surface was not locked.  An attempt to unlock a surface that was not locked at all, or by this process, has been attempted.\0";
		case DDERR_NOTPALETTIZED:
			return "The surface being used is not a palette-based surface.\0";
		case DDERR_NOVSYNCHW:
			return "Operation could not be carried out because there is no hardware support for vertical blank synchronized operations.\0";
		case DDERR_NOZBUFFERHW:
			return "Operation could not be carried out because there is no hardware support for zbuffer blitting.\0";
		case DDERR_NOZOVERLAYHW:
			return "Overlay surfaces could not be z layered based on their BltOrder because the hardware does not support z layering of overlays.\0";
		case DDERR_OUTOFCAPS:
			return "The hardware needed for the requested operation has already been allocated.\0";
		case DDERR_OUTOFMEMORY:
			return "DirectDraw does not have enough memory to perform the operation.\0";
		case DDERR_OUTOFVIDEOMEMORY:
			return "DirectDraw does not have enough memory to perform the operation.\0";
		case DDERR_OVERLAYCANTCLIP:
			return "The hardware does not support clipped overlays.\0";
		case DDERR_OVERLAYCOLORKEYONLYONEACTIVE:
			return "Can only have ony color key active at one time for overlays.\0";
		case DDERR_OVERLAYNOTVISIBLE:
			return "Returned when GetOverlayPosition is called on a hidden overlay.\0";
		case DDERR_PALETTEBUSY:
			return "Access to this palette is being refused because the palette is already locked by another thread.\0";
		case DDERR_PRIMARYSURFACEALREADYEXISTS:
			return "This process already has created a primary surface.\0";
		case DDERR_REGIONTOOSMALL:
			return "Region passed to Clipper::GetClipList is too small.\0";
		case DDERR_SURFACEALREADYATTACHED:
			return "This surface is already attached to the surface it is being attached to.\0";
		case DDERR_SURFACEALREADYDEPENDENT:
			return "This surface is already a dependency of the surface it is being made a dependency of.\0";
		case DDERR_SURFACEBUSY:
			return "Access to this surface is being refused because the surface is already locked by another thread.\0";
		case DDERR_SURFACEISOBSCURED:
			return "Access to surface refused because the surface is obscured.\0";
		case DDERR_SURFACELOST:
			return "Access to this surface is being refused because the surface memory is gone. The DirectDrawSurface object representing this surface should have Restore called on it.\0";
		case DDERR_SURFACENOTATTACHED:
			return "The requested surface is not attached.\0";
		case DDERR_TOOBIGHEIGHT:
			return "Height requested by DirectDraw is too large.\0";
		case DDERR_TOOBIGSIZE:
			return "Size requested by DirectDraw is too large, but the individual height and width are OK.\0";
		case DDERR_TOOBIGWIDTH:
			return "Width requested by DirectDraw is too large.\0";
		case DDERR_UNSUPPORTED:
			return "Action not supported.\0";
		case DDERR_UNSUPPORTEDFORMAT:
			return "FOURCC format requested is unsupported by DirectDraw.\0";
		case DDERR_UNSUPPORTEDMASK:
			return "Bitmask in the pixel format requested is unsupported by DirectDraw.\0";
		case DDERR_VERTICALBLANKINPROGRESS:
			return "Vertical blank is in progress.\0";
		case DDERR_WASSTILLDRAWING:
			return "Informs DirectDraw that the previous Blt which is transfering information to or from this Surface is incomplete.\0";
		case DDERR_WRONGMODE:
			return "This surface can not be restored because it was created in a different mode.\0";
		case DDERR_XALIGN:
			return "Rectangle provided was not horizontally aligned on required boundary.\0";
		case D3DERR_BADMAJORVERSION:
			return "D3DERR_BADMAJORVERSION\0";
		case D3DERR_BADMINORVERSION:
			return "D3DERR_BADMINORVERSION\0";
		case D3DERR_EXECUTE_LOCKED:
			return "D3DERR_EXECUTE_LOCKED\0";
		case D3DERR_EXECUTE_NOT_LOCKED:
			return "D3DERR_EXECUTE_NOT_LOCKED\0";
		case D3DERR_EXECUTE_CREATE_FAILED:
			return "D3DERR_EXECUTE_CREATE_FAILED\0";
		case D3DERR_EXECUTE_DESTROY_FAILED:
			return "D3DERR_EXECUTE_DESTROY_FAILED\0";
		case D3DERR_EXECUTE_LOCK_FAILED:
			return "D3DERR_EXECUTE_LOCK_FAILED\0";
		case D3DERR_EXECUTE_UNLOCK_FAILED:
			return "D3DERR_EXECUTE_UNLOCK_FAILED\0";
		case D3DERR_EXECUTE_FAILED:
			return "D3DERR_EXECUTE_FAILED\0";
		case D3DERR_EXECUTE_CLIPPED_FAILED:
			return "D3DERR_EXECUTE_CLIPPED_FAILED\0";
		case D3DERR_TEXTURE_NO_SUPPORT:
			return "D3DERR_TEXTURE_NO_SUPPORT\0";
		case D3DERR_TEXTURE_NOT_LOCKED:
			return "D3DERR_TEXTURE_NOT_LOCKED\0";
		case D3DERR_TEXTURE_LOCKED:
			return "D3DERR_TEXTURELOCKED\0";
		case D3DERR_TEXTURE_CREATE_FAILED:
			return "D3DERR_TEXTURE_CREATE_FAILED\0";
		case D3DERR_TEXTURE_DESTROY_FAILED:
			return "D3DERR_TEXTURE_DESTROY_FAILED\0";
		case D3DERR_TEXTURE_LOCK_FAILED:
			return "D3DERR_TEXTURE_LOCK_FAILED\0";
		case D3DERR_TEXTURE_UNLOCK_FAILED:
			return "D3DERR_TEXTURE_UNLOCK_FAILED\0";
		case D3DERR_TEXTURE_LOAD_FAILED:
			return "D3DERR_TEXTURE_LOAD_FAILED\0";
		case D3DERR_MATRIX_CREATE_FAILED:
			return "D3DERR_MATRIX_CREATE_FAILED\0";
		case D3DERR_MATRIX_DESTROY_FAILED:
			return "D3DERR_MATRIX_DESTROY_FAILED\0";
		case D3DERR_MATRIX_SETDATA_FAILED:
			return "D3DERR_MATRIX_SETDATA_FAILED\0";
		case D3DERR_SETVIEWPORTDATA_FAILED:
			return "D3DERR_SETVIEWPORTDATA_FAILED\0";
		case D3DERR_MATERIAL_CREATE_FAILED:
			return "D3DERR_MATERIAL_CREATE_FAILED\0";
		case D3DERR_MATERIAL_DESTROY_FAILED:
			return "D3DERR_MATERIAL_DESTROY_FAILED\0";
		case D3DERR_MATERIAL_SETDATA_FAILED:
			return "D3DERR_MATERIAL_SETDATA_FAILED\0";
		case D3DERR_LIGHT_SET_FAILED:
			return "D3DERR_LIGHT_SET_FAILED\0";
		case D3DRMERR_BADOBJECT:
			return "D3DRMERR_BADOBJECT\0";
		case D3DRMERR_BADTYPE:
			return "D3DRMERR_BADTYPE\0";
		case D3DRMERR_BADALLOC:
			return "D3DRMERR_BADALLOC\0";
		case D3DRMERR_FACEUSED:
			return "D3DRMERR_FACEUSED\0";
		case D3DRMERR_NOTFOUND:
			return "D3DRMERR_NOTFOUND\0";
		case D3DRMERR_NOTDONEYET:
			return "D3DRMERR_NOTDONEYET\0";
		case D3DRMERR_FILENOTFOUND:
			return "The file was not found.\0";
		case D3DRMERR_BADFILE:
			return "D3DRMERR_BADFILE\0";
		case D3DRMERR_BADDEVICE:
			return "D3DRMERR_BADDEVICE\0";
		case D3DRMERR_BADVALUE:
			return "D3DRMERR_BADVALUE\0";
		case D3DRMERR_BADMAJORVERSION:
			return "D3DRMERR_BADMAJORVERSION\0";
		case D3DRMERR_BADMINORVERSION:
			return "D3DRMERR_BADMINORVERSION\0";
		case D3DRMERR_UNABLETOEXECUTE:
			return "D3DRMERR_UNABLETOEXECUTE\0";
        case DSERR_ALLOCATED : return("DSERR_ALLOCATED"); 
        case DSERR_CONTROLUNAVAIL : return("DSERR_CONTROLUNAVAIL"); 
//        case DSERR_INVALIDPARAM : return("DSERR_INVALIDPARAM"); // 이것들은 값이 겹쳐서 빼놓은 거야.
        case DSERR_INVALIDCALL : return("DSERR_INVALIDCALL"); 
//        case DSERR_GENERIC : return("DSERR_GENERIC"); 
        case DSERR_PRIOLEVELNEEDED : return("DSERR_PRIOLEVELNEEDED"); 
//        case DSERR_OUTOFMEMORY : return("DSERR_OUTOFMEMORY"); 
        case DSERR_BADFORMAT : return("DSERR_BADFORMAT"); 
//        case DSERR_UNSUPPORTED : return("DSERR_UNSUPPORTED"); 
        case DSERR_NODRIVER : return("DSERR_NODRIVER"); 
        case DSERR_ALREADYINITIALIZED : return("DSERR_ALREADYINITIALIZED"); 
        case DSERR_NOAGGREGATION : return("DSERR_NOAGGREGATION"); 
        case DSERR_BUFFERLOST : return("DSERR_BUFFERLOST"); 
        case DSERR_OTHERAPPHASPRIO : return("DSERR_OTHERAPPHASPRIO"); 
        case DSERR_UNINITIALIZED : return("DSERR_UNINITIALIZED"); 

        default : return("Unknown Error"); 
    }
}

BOOL
cMSG::DXError(char *msg,HRESULT index)
{	ErrorCount++;

	return Error(msg,DXErrorToString(index));
}

BOOL
cMSG::DDError(char *msg,HRESULT index)
{	ErrorCount++;

	return Error(msg,DDErrorToString(index));
}

BOOL
cMSG::DSError(char *msg,HRESULT index)
{	ErrorCount++;

	return Error(msg,DSErrorToString(index));
}

BOOL
cMSG::D3DError(char *msg,HRESULT index)
{	ErrorCount++;

	return Error(msg,D3DErrorToString(index));
}

BOOL
cMSG::D3DRMError(char *msg,HRESULT index)
{	ErrorCount++;

	return Error(msg,D3DRMErrorToString(index));
}