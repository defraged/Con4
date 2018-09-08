/* this ALWAYS GENERATED file contains the definitions for the interfaces */


/* File created by MIDL compiler version 5.01.0164 */
/* at Sun Sep 25 18:08:56 2005
 */
/* Compiler settings for ConnectFour.idl:
    Os (OptLev=s), W1, Zp8, env=Win32, ms_ext, c_ext
    error checks: allocation ref bounds_check enum stub_data 
*/
//@@MIDL_FILE_HEADING(  )


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 440
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __ConFour_h__
#define __ConFour_h__

#ifdef __cplusplus
extern "C"{
#endif 

/* Forward Declarations */ 

#ifndef __IConnectFourGame_FWD_DEFINED__
#define __IConnectFourGame_FWD_DEFINED__
typedef interface IConnectFourGame IConnectFourGame;
#endif 	/* __IConnectFourGame_FWD_DEFINED__ */


#ifndef ___IConnectFourEvents_FWD_DEFINED__
#define ___IConnectFourEvents_FWD_DEFINED__
typedef interface _IConnectFourEvents _IConnectFourEvents;
#endif 	/* ___IConnectFourEvents_FWD_DEFINED__ */


#ifndef __ConnectFourGame_FWD_DEFINED__
#define __ConnectFourGame_FWD_DEFINED__

#ifdef __cplusplus
typedef class ConnectFourGame ConnectFourGame;
#else
typedef struct ConnectFourGame ConnectFourGame;
#endif /* __cplusplus */

#endif 	/* __ConnectFourGame_FWD_DEFINED__ */


void __RPC_FAR * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void __RPC_FAR * ); 


#ifndef __ConnectFourLib_LIBRARY_DEFINED__
#define __ConnectFourLib_LIBRARY_DEFINED__

/* library ConnectFourLib */
/* [helpcontext][helpfile][helpstring][version][uuid] */ 




EXTERN_C const IID LIBID_ConnectFourLib;

#ifndef __IConnectFourGame_DISPINTERFACE_DEFINED__
#define __IConnectFourGame_DISPINTERFACE_DEFINED__

/* dispinterface IConnectFourGame */
/* [hidden][helpstring][uuid] */ 


EXTERN_C const IID DIID_IConnectFourGame;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("A1241FE6-B51F-42aa-877B-BFA47BEF3FC1")
    IConnectFourGame : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct IConnectFourGameVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IConnectFourGame __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IConnectFourGame __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IConnectFourGame __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IConnectFourGame __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IConnectFourGame __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IConnectFourGame __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IConnectFourGame __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        END_INTERFACE
    } IConnectFourGameVtbl;

    interface IConnectFourGame
    {
        CONST_VTBL struct IConnectFourGameVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IConnectFourGame_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IConnectFourGame_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IConnectFourGame_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IConnectFourGame_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IConnectFourGame_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IConnectFourGame_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IConnectFourGame_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* __IConnectFourGame_DISPINTERFACE_DEFINED__ */


#ifndef ___IConnectFourEvents_DISPINTERFACE_DEFINED__
#define ___IConnectFourEvents_DISPINTERFACE_DEFINED__

/* dispinterface _IConnectFourEvents */
/* [helpstring][uuid] */ 


EXTERN_C const IID DIID__IConnectFourEvents;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("A1241FE7-B51F-42aa-877B-BFA47BEF3FC1")
    _IConnectFourEvents : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _IConnectFourEventsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            _IConnectFourEvents __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            _IConnectFourEvents __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            _IConnectFourEvents __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            _IConnectFourEvents __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            _IConnectFourEvents __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            _IConnectFourEvents __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            _IConnectFourEvents __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        END_INTERFACE
    } _IConnectFourEventsVtbl;

    interface _IConnectFourEvents
    {
        CONST_VTBL struct _IConnectFourEventsVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _IConnectFourEvents_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define _IConnectFourEvents_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define _IConnectFourEvents_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define _IConnectFourEvents_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define _IConnectFourEvents_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define _IConnectFourEvents_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define _IConnectFourEvents_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___IConnectFourEvents_DISPINTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_ConnectFourGame;

#ifdef __cplusplus

class DECLSPEC_UUID("A1241FE8-B51F-42aa-877B-BFA47BEF3FC1")
ConnectFourGame;
#endif
#endif /* __ConnectFourLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif
