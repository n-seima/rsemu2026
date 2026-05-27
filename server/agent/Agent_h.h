

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 8.01.0622 */
/* at Tue Jan 19 12:14:07 2038
 */
/* Compiler settings for Agent.odl:
    Oicf, W1, Zp8, env=Win32 (32b run), target_arch=X86 8.01.0622 
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */



/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 500
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif /* __RPCNDR_H_VERSION__ */


#ifndef __Agent_h_h__
#define __Agent_h_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IAgent_FWD_DEFINED__
#define __IAgent_FWD_DEFINED__
typedef interface IAgent IAgent;

#endif 	/* __IAgent_FWD_DEFINED__ */


#ifndef __Agent_FWD_DEFINED__
#define __Agent_FWD_DEFINED__

#ifdef __cplusplus
typedef class Agent Agent;
#else
typedef struct Agent Agent;
#endif /* __cplusplus */

#endif 	/* __Agent_FWD_DEFINED__ */


#ifdef __cplusplus
extern "C"{
#endif 



#ifndef __Agent_LIBRARY_DEFINED__
#define __Agent_LIBRARY_DEFINED__

/* library Agent */
/* [version][uuid] */ 


DEFINE_GUID(LIBID_Agent,0xDDA1672F,0x66C9,0x42FF,0x9E,0x8A,0x3C,0x73,0x9A,0xFA,0x2A,0x07);

#ifndef __IAgent_DISPINTERFACE_DEFINED__
#define __IAgent_DISPINTERFACE_DEFINED__

/* dispinterface IAgent */
/* [uuid] */ 


DEFINE_GUID(DIID_IAgent,0xD439BFFB,0x26FE,0x44BB,0xB0,0x34,0x51,0x22,0xAC,0xF6,0x33,0x53);

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("D439BFFB-26FE-44BB-B034-5122ACF63353")
    IAgent : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct IAgentVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IAgent * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IAgent * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IAgent * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IAgent * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IAgent * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IAgent * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IAgent * This,
            /* [annotation][in] */ 
            _In_  DISPID dispIdMember,
            /* [annotation][in] */ 
            _In_  REFIID riid,
            /* [annotation][in] */ 
            _In_  LCID lcid,
            /* [annotation][in] */ 
            _In_  WORD wFlags,
            /* [annotation][out][in] */ 
            _In_  DISPPARAMS *pDispParams,
            /* [annotation][out] */ 
            _Out_opt_  VARIANT *pVarResult,
            /* [annotation][out] */ 
            _Out_opt_  EXCEPINFO *pExcepInfo,
            /* [annotation][out] */ 
            _Out_opt_  UINT *puArgErr);
        
        END_INTERFACE
    } IAgentVtbl;

    interface IAgent
    {
        CONST_VTBL struct IAgentVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IAgent_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IAgent_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IAgent_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IAgent_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IAgent_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IAgent_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IAgent_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* __IAgent_DISPINTERFACE_DEFINED__ */


DEFINE_GUID(CLSID_Agent,0xE4FF91DC,0xE699,0x4724,0x92,0x58,0xBA,0x07,0x90,0x34,0x10,0x34);

#ifdef __cplusplus

class DECLSPEC_UUID("E4FF91DC-E699-4724-9258-BA0790341034")
Agent;
#endif
#endif /* __Agent_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


