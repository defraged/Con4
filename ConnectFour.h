/* this ALWAYS GENERATED file contains the definitions for the interfaces */


/* File created by MIDL compiler version 5.01.0164 */
/* at Mon Sep 26 00:00:12 2005
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

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__

#ifndef COM_NO_WINDOWS_H
#include "windows.h"
#include "ole2.h"
#endif /*COM_NO_WINDOWS_H*/

#ifndef __ConnectFour_h__
#define __ConnectFour_h__

#ifdef __cplusplus
extern "C"{
#endif 

/* Forward Declarations */ 

#ifndef __IConnectFourGame_FWD_DEFINED__
#define __IConnectFourGame_FWD_DEFINED__
typedef interface IConnectFourGame IConnectFourGame;
#endif 	/* __IConnectFourGame_FWD_DEFINED__ */


#ifndef __IConnectFourEvents_FWD_DEFINED__
#define __IConnectFourEvents_FWD_DEFINED__
typedef interface IConnectFourEvents IConnectFourEvents;
#endif 	/* __IConnectFourEvents_FWD_DEFINED__ */


#ifndef __ConnectFourGame_FWD_DEFINED__
#define __ConnectFourGame_FWD_DEFINED__

#ifdef __cplusplus
typedef class ConnectFourGame ConnectFourGame;
#else
typedef struct ConnectFourGame ConnectFourGame;
#endif /* __cplusplus */

#endif 	/* __ConnectFourGame_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

void __RPC_FAR * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void __RPC_FAR * ); 

#ifndef __IConnectFourGame_INTERFACE_DEFINED__
#define __IConnectFourGame_INTERFACE_DEFINED__

/* interface IConnectFourGame */
/* [unique][helpstring][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_IConnectFourGame;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("A1241FE6-B51F-42aa-877B-BFA47BEF3FC1")
    IConnectFourGame : public IDispatch
    {
    public:
        virtual /* [helpstring][local][id] */ void STDMETHODCALLTYPE EndGame( void) = 0;
		
        virtual /* [local][id] */ BSTR STDMETHODCALLTYPE GetClassID( void) = 0;
        
        virtual /* [helpstring][local][id] */ void STDMETHODCALLTYPE GetGameDetails( 
            /* [out] */ BSTR __RPC_FAR *Name,
            /* [out] */ BSTR __RPC_FAR *Info,
            /* [out] */ BSTR __RPC_FAR *SeatList,
            /* [out] */ BSTR __RPC_FAR *CanChangeSeat,
            /* [out] */ BSTR __RPC_FAR *CanSelectSeat,
            /* [out] */ SHORT __RPC_FAR *MinParticipants,
            /* [out] */ SHORT __RPC_FAR *MaxParticipants,
            /* [out] */ BSTR __RPC_FAR *Avatar) = 0;
        
        virtual /* [helpstring][local][id] */ void STDMETHODCALLTYPE Send( 
            /* [in] */ LONG Id,
            /* [in] */ BSTR Name,
            /* [in] */ SHORT Type,
            /* [in] */ LONG ToId,
            /* [in] */ SHORT SendType,
            /* [in] */ BSTR Message,
            /* [out][in] */ BSTR __RPC_FAR *Data) = 0;
        
        virtual /* [helpstring][local][id] */ void STDMETHODCALLTYPE EnumMembers( 
            /* [in] */ LONG Current,
            /* [in] */ SHORT Level,
            /* [in] */ LONG Id,
            /* [in] */ BSTR Name,
            /* [in] */ SHORT Type,
            /* [in] */ LONG X,
            /* [in] */ LONG Y) = 0;
        
        virtual /* [helpstring][local][id] */ void STDMETHODCALLTYPE JoinGroup( 
            /* [in] */ LONG Id,
            /* [in] */ BSTR Name,
            /* [in] */ LONG X,
            /* [in] */ LONG Y,
            /* [in] */ LONG GroupId,
            /* [in] */ BSTR GroupName) = 0;
        
        virtual /* [helpstring][local][id] */ void STDMETHODCALLTYPE LeaveGroup( 
            /* [in] */ LONG Id,
            /* [in] */ BSTR Name,
            /* [in] */ LONG GroupId,
            /* [in] */ BSTR GroupName) = 0;
        
        virtual /* [helpstring][local][id] */ void STDMETHODCALLTYPE StartGame( 
            /* [in] */ LONG Id,
            /* [in] */ BSTR Name,
            /* [in] */ LONG GroupId,
            /* [in] */ BSTR GroupName,
            /* [in] */ LONG LeaderId,
            /* [in] */ SHORT Capacity,
            /* [in] */ LONG Seat) = 0;
        
        virtual /* [helpstring][local][id] */ void STDMETHODCALLTYPE SendFromService( 
            /* [in] */ LONG Id,
            /* [in] */ SHORT Type,
            /* [in] */ LONG SrvSendType,
            /* [in] */ BSTR Message,
            /* [out][in] */ BSTR __RPC_FAR *Data,
            /* [in] */ LONG ReqId) = 0;
        
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
        
        /* [helpstring][local][id] */ void ( STDMETHODCALLTYPE __RPC_FAR *EndGame )( 
            IConnectFourGame __RPC_FAR * This);
        
        /* [local][id] */ BSTR ( STDMETHODCALLTYPE __RPC_FAR *GetClassID )( 
            IConnectFourGame __RPC_FAR * This);
        
        /* [helpstring][local][id] */ void ( STDMETHODCALLTYPE __RPC_FAR *GetGameDetails )( 
            IConnectFourGame __RPC_FAR * This,
            /* [out] */ BSTR __RPC_FAR *Name,
            /* [out] */ BSTR __RPC_FAR *Info,
            /* [out] */ BSTR __RPC_FAR *SeatList,
            /* [out] */ BSTR __RPC_FAR *CanChangeSeat,
            /* [out] */ BSTR __RPC_FAR *CanSelectSeat,
            /* [out] */ SHORT __RPC_FAR *MinParticipants,
            /* [out] */ SHORT __RPC_FAR *MaxParticipants,
            /* [out] */ BSTR __RPC_FAR *Avatar);
        
        /* [helpstring][local][id] */ void ( STDMETHODCALLTYPE __RPC_FAR *Send )( 
            IConnectFourGame __RPC_FAR * This,
            /* [in] */ LONG Id,
            /* [in] */ BSTR Name,
            /* [in] */ SHORT Type,
            /* [in] */ LONG ToId,
            /* [in] */ SHORT SendType,
            /* [in] */ BSTR Message,
            /* [out][in] */ BSTR __RPC_FAR *Data);
        
        /* [helpstring][local][id] */ void ( STDMETHODCALLTYPE __RPC_FAR *EnumMembers )( 
            IConnectFourGame __RPC_FAR * This,
            /* [in] */ LONG Current,
            /* [in] */ SHORT Level,
            /* [in] */ LONG Id,
            /* [in] */ BSTR Name,
            /* [in] */ SHORT Type,
            /* [in] */ LONG X,
            /* [in] */ LONG Y);
        
        /* [helpstring][local][id] */ void ( STDMETHODCALLTYPE __RPC_FAR *JoinGroup )( 
            IConnectFourGame __RPC_FAR * This,
            /* [in] */ LONG Id,
            /* [in] */ BSTR Name,
            /* [in] */ LONG X,
            /* [in] */ LONG Y,
            /* [in] */ LONG GroupId,
            /* [in] */ BSTR GroupName);
        
        /* [helpstring][local][id] */ void ( STDMETHODCALLTYPE __RPC_FAR *LeaveGroup )( 
            IConnectFourGame __RPC_FAR * This,
            /* [in] */ LONG Id,
            /* [in] */ BSTR Name,
            /* [in] */ LONG GroupId,
            /* [in] */ BSTR GroupName);
        
        /* [helpstring][local][id] */ void ( STDMETHODCALLTYPE __RPC_FAR *StartGame )( 
            IConnectFourGame __RPC_FAR * This,
            /* [in] */ LONG Id,
            /* [in] */ BSTR Name,
            /* [in] */ LONG GroupId,
            /* [in] */ BSTR GroupName,
            /* [in] */ LONG LeaderId,
            /* [in] */ SHORT Capacity,
            /* [in] */ LONG Seat);
        
        /* [helpstring][local][id] */ void ( STDMETHODCALLTYPE __RPC_FAR *SendFromService )( 
            IConnectFourGame __RPC_FAR * This,
            /* [in] */ LONG Id,
            /* [in] */ SHORT Type,
            /* [in] */ LONG SrvSendType,
            /* [in] */ BSTR Message,
            /* [out][in] */ BSTR __RPC_FAR *Data,
            /* [in] */ LONG ReqId);
        
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


#define IConnectFourGame_EndGame(This)	\
    (This)->lpVtbl -> EndGame(This)

#define IConnectFourGame_GetClassID(This)	\
    (This)->lpVtbl -> GetClassID(This)

#define IConnectFourGame_GetGameDetails(This,Name,Info,SeatList,CanChangeSeat,CanSelectSeat,MinParticipants,MaxParticipants,Avatar)	\
    (This)->lpVtbl -> GetGameDetails(This,Name,Info,SeatList,CanChangeSeat,CanSelectSeat,MinParticipants,MaxParticipants,Avatar)

#define IConnectFourGame_Send(This,Id,Name,Type,ToId,SendType,Message,Data)	\
    (This)->lpVtbl -> Send(This,Id,Name,Type,ToId,SendType,Message,Data)

#define IConnectFourGame_EnumMembers(This,Current,Level,Id,Name,Type,X,Y)	\
    (This)->lpVtbl -> EnumMembers(This,Current,Level,Id,Name,Type,X,Y)

#define IConnectFourGame_JoinGroup(This,Id,Name,X,Y,GroupId,GroupName)	\
    (This)->lpVtbl -> JoinGroup(This,Id,Name,X,Y,GroupId,GroupName)

#define IConnectFourGame_LeaveGroup(This,Id,Name,GroupId,GroupName)	\
    (This)->lpVtbl -> LeaveGroup(This,Id,Name,GroupId,GroupName)

#define IConnectFourGame_StartGame(This,Id,Name,GroupId,GroupName,LeaderId,Capacity,Seat)	\
    (This)->lpVtbl -> StartGame(This,Id,Name,GroupId,GroupName,LeaderId,Capacity,Seat)

#define IConnectFourGame_SendFromService(This,Id,Type,SrvSendType,Message,Data,ReqId)	\
    (This)->lpVtbl -> SendFromService(This,Id,Type,SrvSendType,Message,Data,ReqId)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][local][id] */ void STDMETHODCALLTYPE IConnectFourGame_EndGame_Proxy( 
    IConnectFourGame __RPC_FAR * This);


void __RPC_STUB IConnectFourGame_EndGame_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [local][id] */ BSTR STDMETHODCALLTYPE IConnectFourGame_GetClassID_Proxy( 
    IConnectFourGame __RPC_FAR * This);


void __RPC_STUB IConnectFourGame_GetClassID_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][local][id] */ void STDMETHODCALLTYPE IConnectFourGame_GetGameDetails_Proxy( 
    IConnectFourGame __RPC_FAR * This,
    /* [out] */ BSTR __RPC_FAR *Name,
    /* [out] */ BSTR __RPC_FAR *Info,
    /* [out] */ BSTR __RPC_FAR *SeatList,
    /* [out] */ BSTR __RPC_FAR *CanChangeSeat,
    /* [out] */ BSTR __RPC_FAR *CanSelectSeat,
    /* [out] */ SHORT __RPC_FAR *MinParticipants,
    /* [out] */ SHORT __RPC_FAR *MaxParticipants,
    /* [out] */ BSTR __RPC_FAR *Avatar);


void __RPC_STUB IConnectFourGame_GetGameDetails_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][local][id] */ void STDMETHODCALLTYPE IConnectFourGame_Send_Proxy( 
    IConnectFourGame __RPC_FAR * This,
    /* [in] */ LONG Id,
    /* [in] */ BSTR Name,
    /* [in] */ SHORT Type,
    /* [in] */ LONG ToId,
    /* [in] */ SHORT SendType,
    /* [in] */ BSTR Message,
    /* [out][in] */ BSTR __RPC_FAR *Data);


void __RPC_STUB IConnectFourGame_Send_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][local][id] */ void STDMETHODCALLTYPE IConnectFourGame_EnumMembers_Proxy( 
    IConnectFourGame __RPC_FAR * This,
    /* [in] */ LONG Current,
    /* [in] */ SHORT Level,
    /* [in] */ LONG Id,
    /* [in] */ BSTR Name,
    /* [in] */ SHORT Type,
    /* [in] */ LONG X,
    /* [in] */ LONG Y);


void __RPC_STUB IConnectFourGame_EnumMembers_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][local][id] */ void STDMETHODCALLTYPE IConnectFourGame_JoinGroup_Proxy( 
    IConnectFourGame __RPC_FAR * This,
    /* [in] */ LONG Id,
    /* [in] */ BSTR Name,
    /* [in] */ LONG X,
    /* [in] */ LONG Y,
    /* [in] */ LONG GroupId,
    /* [in] */ BSTR GroupName);


void __RPC_STUB IConnectFourGame_JoinGroup_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][local][id] */ void STDMETHODCALLTYPE IConnectFourGame_LeaveGroup_Proxy( 
    IConnectFourGame __RPC_FAR * This,
    /* [in] */ LONG Id,
    /* [in] */ BSTR Name,
    /* [in] */ LONG GroupId,
    /* [in] */ BSTR GroupName);


void __RPC_STUB IConnectFourGame_LeaveGroup_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][local][id] */ void STDMETHODCALLTYPE IConnectFourGame_StartGame_Proxy( 
    IConnectFourGame __RPC_FAR * This,
    /* [in] */ LONG Id,
    /* [in] */ BSTR Name,
    /* [in] */ LONG GroupId,
    /* [in] */ BSTR GroupName,
    /* [in] */ LONG LeaderId,
    /* [in] */ SHORT Capacity,
    /* [in] */ LONG Seat);


void __RPC_STUB IConnectFourGame_StartGame_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][local][id] */ void STDMETHODCALLTYPE IConnectFourGame_SendFromService_Proxy( 
    IConnectFourGame __RPC_FAR * This,
    /* [in] */ LONG Id,
    /* [in] */ SHORT Type,
    /* [in] */ LONG SrvSendType,
    /* [in] */ BSTR Message,
    /* [out][in] */ BSTR __RPC_FAR *Data,
    /* [in] */ LONG ReqId);


void __RPC_STUB IConnectFourGame_SendFromService_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IConnectFourGame_INTERFACE_DEFINED__ */


#ifndef __IConnectFourEvents_INTERFACE_DEFINED__
#define __IConnectFourEvents_INTERFACE_DEFINED__

/* interface IConnectFourEvents */
/* [unique][helpstring][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_IConnectFourEvents;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("A1241FE7-B51F-42aa-877B-BFA47BEF3FC1")
    IConnectFourEvents : public IDispatch
    {
    public:
        virtual /* [hidden][local][id] */ void STDMETHODCALLTYPE LeaveGroup( void) = 0;
        
        virtual /* [hidden][local][id] */ void STDMETHODCALLTYPE GetPresenceInfo( 
            long Id,
            BSTR __RPC_FAR *Nickname,
            BSTR __RPC_FAR *Fullname,
            BSTR __RPC_FAR *Location,
            BSTR __RPC_FAR *Email) = 0;
        
        virtual /* [hidden][local][id] */ void STDMETHODCALLTYPE GetParentWnd( 
            long __RPC_FAR *hWnd) = 0;
        
        virtual /* [hidden][local][id] */ void STDMETHODCALLTYPE SetGameWnd( 
            long hWnd) = 0;
        
        virtual /* [hidden][local][id] */ void STDMETHODCALLTYPE DupAvatar( 
            long ParentWnd,
            long Seat) = 0;
        
        virtual /* [hidden][local][id] */ void STDMETHODCALLTYPE SetAvatarPos( 
            long Seat,
            short Left,
            short Top,
            short Width,
            short Height) = 0;
        
        virtual /* [hidden][local][id] */ void STDMETHODCALLTYPE EnumMembers( 
            long Id,
            short MaxLevel) = 0;
        
        virtual /* [hidden][local][id] */ void STDMETHODCALLTYPE Send( 
            long Id,
            short SendType,
            BSTR Message,
            BSTR __RPC_FAR *Data) = 0;
        
        virtual /* [hidden][local][id] */ void STDMETHODCALLTYPE QueryService( 
            long __RPC_FAR *Id,
            short __RPC_FAR *Type) = 0;
        
        virtual /* [hidden][local][id] */ void STDMETHODCALLTYPE SendToService( 
            long ServiceId,
            long SrvSendType,
            BSTR Message,
            BSTR __RPC_FAR *Data,
            long ReqId) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IConnectFourEventsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IConnectFourEvents __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IConnectFourEvents __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IConnectFourEvents __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IConnectFourEvents __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IConnectFourEvents __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IConnectFourEvents __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IConnectFourEvents __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [hidden][local][id] */ void ( STDMETHODCALLTYPE __RPC_FAR *LeaveGroup )( 
            IConnectFourEvents __RPC_FAR * This);
        
        /* [hidden][local][id] */ void ( STDMETHODCALLTYPE __RPC_FAR *GetPresenceInfo )( 
            IConnectFourEvents __RPC_FAR * This,
            long Id,
            BSTR __RPC_FAR *Nickname,
            BSTR __RPC_FAR *Fullname,
            BSTR __RPC_FAR *Location,
            BSTR __RPC_FAR *Email);
        
        /* [hidden][local][id] */ void ( STDMETHODCALLTYPE __RPC_FAR *GetParentWnd )( 
            IConnectFourEvents __RPC_FAR * This,
            long __RPC_FAR *hWnd);
        
        /* [hidden][local][id] */ void ( STDMETHODCALLTYPE __RPC_FAR *SetGameWnd )( 
            IConnectFourEvents __RPC_FAR * This,
            long hWnd);
        
        /* [hidden][local][id] */ void ( STDMETHODCALLTYPE __RPC_FAR *DupAvatar )( 
            IConnectFourEvents __RPC_FAR * This,
            long ParentWnd,
            long Seat);
        
        /* [hidden][local][id] */ void ( STDMETHODCALLTYPE __RPC_FAR *SetAvatarPos )( 
            IConnectFourEvents __RPC_FAR * This,
            long Seat,
            short Left,
            short Top,
            short Width,
            short Height);
        
        /* [hidden][local][id] */ void ( STDMETHODCALLTYPE __RPC_FAR *EnumMembers )( 
            IConnectFourEvents __RPC_FAR * This,
            long Id,
            short MaxLevel);
        
        /* [hidden][local][id] */ void ( STDMETHODCALLTYPE __RPC_FAR *Send )( 
            IConnectFourEvents __RPC_FAR * This,
            long Id,
            short SendType,
            BSTR Message,
            BSTR __RPC_FAR *Data);
        
        /* [hidden][local][id] */ void ( STDMETHODCALLTYPE __RPC_FAR *QueryService )( 
            IConnectFourEvents __RPC_FAR * This,
            long __RPC_FAR *Id,
            short __RPC_FAR *Type);
        
        /* [hidden][local][id] */ void ( STDMETHODCALLTYPE __RPC_FAR *SendToService )( 
            IConnectFourEvents __RPC_FAR * This,
            long ServiceId,
            long SrvSendType,
            BSTR Message,
            BSTR __RPC_FAR *Data,
            long ReqId);
        
        END_INTERFACE
    } IConnectFourEventsVtbl;

    interface IConnectFourEvents
    {
        CONST_VTBL struct IConnectFourEventsVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IConnectFourEvents_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IConnectFourEvents_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IConnectFourEvents_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IConnectFourEvents_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IConnectFourEvents_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IConnectFourEvents_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IConnectFourEvents_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IConnectFourEvents_LeaveGroup(This)	\
    (This)->lpVtbl -> LeaveGroup(This)

#define IConnectFourEvents_GetPresenceInfo(This,Id,Nickname,Fullname,Location,Email)	\
    (This)->lpVtbl -> GetPresenceInfo(This,Id,Nickname,Fullname,Location,Email)

#define IConnectFourEvents_GetParentWnd(This,hWnd)	\
    (This)->lpVtbl -> GetParentWnd(This,hWnd)

#define IConnectFourEvents_SetGameWnd(This,hWnd)	\
    (This)->lpVtbl -> SetGameWnd(This,hWnd)

#define IConnectFourEvents_DupAvatar(This,ParentWnd,Seat)	\
    (This)->lpVtbl -> DupAvatar(This,ParentWnd,Seat)

#define IConnectFourEvents_SetAvatarPos(This,Seat,Left,Top,Width,Height)	\
    (This)->lpVtbl -> SetAvatarPos(This,Seat,Left,Top,Width,Height)

#define IConnectFourEvents_EnumMembers(This,Id,MaxLevel)	\
    (This)->lpVtbl -> EnumMembers(This,Id,MaxLevel)

#define IConnectFourEvents_Send(This,Id,SendType,Message,Data)	\
    (This)->lpVtbl -> Send(This,Id,SendType,Message,Data)

#define IConnectFourEvents_QueryService(This,Id,Type)	\
    (This)->lpVtbl -> QueryService(This,Id,Type)

#define IConnectFourEvents_SendToService(This,ServiceId,SrvSendType,Message,Data,ReqId)	\
    (This)->lpVtbl -> SendToService(This,ServiceId,SrvSendType,Message,Data,ReqId)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [hidden][local][id] */ void STDMETHODCALLTYPE IConnectFourEvents_LeaveGroup_Proxy( 
    IConnectFourEvents __RPC_FAR * This);


void __RPC_STUB IConnectFourEvents_LeaveGroup_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [hidden][local][id] */ void STDMETHODCALLTYPE IConnectFourEvents_GetPresenceInfo_Proxy( 
    IConnectFourEvents __RPC_FAR * This,
    long Id,
    BSTR __RPC_FAR *Nickname,
    BSTR __RPC_FAR *Fullname,
    BSTR __RPC_FAR *Location,
    BSTR __RPC_FAR *Email);


void __RPC_STUB IConnectFourEvents_GetPresenceInfo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [hidden][local][id] */ void STDMETHODCALLTYPE IConnectFourEvents_GetParentWnd_Proxy( 
    IConnectFourEvents __RPC_FAR * This,
    long __RPC_FAR *hWnd);


void __RPC_STUB IConnectFourEvents_GetParentWnd_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [hidden][local][id] */ void STDMETHODCALLTYPE IConnectFourEvents_SetGameWnd_Proxy( 
    IConnectFourEvents __RPC_FAR * This,
    long hWnd);


void __RPC_STUB IConnectFourEvents_SetGameWnd_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [hidden][local][id] */ void STDMETHODCALLTYPE IConnectFourEvents_DupAvatar_Proxy( 
    IConnectFourEvents __RPC_FAR * This,
    long ParentWnd,
    long Seat);


void __RPC_STUB IConnectFourEvents_DupAvatar_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [hidden][local][id] */ void STDMETHODCALLTYPE IConnectFourEvents_SetAvatarPos_Proxy( 
    IConnectFourEvents __RPC_FAR * This,
    long Seat,
    short Left,
    short Top,
    short Width,
    short Height);


void __RPC_STUB IConnectFourEvents_SetAvatarPos_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [hidden][local][id] */ void STDMETHODCALLTYPE IConnectFourEvents_EnumMembers_Proxy( 
    IConnectFourEvents __RPC_FAR * This,
    long Id,
    short MaxLevel);


void __RPC_STUB IConnectFourEvents_EnumMembers_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [hidden][local][id] */ void STDMETHODCALLTYPE IConnectFourEvents_Send_Proxy( 
    IConnectFourEvents __RPC_FAR * This,
    long Id,
    short SendType,
    BSTR Message,
    BSTR __RPC_FAR *Data);


void __RPC_STUB IConnectFourEvents_Send_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [hidden][local][id] */ void STDMETHODCALLTYPE IConnectFourEvents_QueryService_Proxy( 
    IConnectFourEvents __RPC_FAR * This,
    long __RPC_FAR *Id,
    short __RPC_FAR *Type);


void __RPC_STUB IConnectFourEvents_QueryService_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [hidden][local][id] */ void STDMETHODCALLTYPE IConnectFourEvents_SendToService_Proxy( 
    IConnectFourEvents __RPC_FAR * This,
    long ServiceId,
    long SrvSendType,
    BSTR Message,
    BSTR __RPC_FAR *Data,
    long ReqId);


void __RPC_STUB IConnectFourEvents_SendToService_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IConnectFourEvents_INTERFACE_DEFINED__ */



#ifndef __ConnectFourLib_LIBRARY_DEFINED__
#define __ConnectFourLib_LIBRARY_DEFINED__

/* library ConnectFourLib */
/* [helpstring][version][uuid] */ 


EXTERN_C const IID LIBID_ConnectFourLib;

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
