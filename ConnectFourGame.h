#pragma once

BOOL CALLBACK AboutDlgProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam);

class CConnectFourGame :	IConnectFourGame,
							IConnectionPointContainer,
							IProvideClassInfo2
{
	//make our game window procedure a friend to this class so it can access
	//all the private data & methods
	friend LRESULT WINAPI GameWndProc(HWND hwnd, UINT msg, 
										WPARAM wParam, LPARAM lParam);
	friend LRESULT WINAPI SlotWndProc(HWND hwnd, UINT msg, 
										WPARAM wParam, LPARAM lParam);
	friend void CALLBACK ConnectFourProc(HWND hwnd, UINT uMsg, 
											UINT idEvent, DWORD dwTime);
public:
	//Constructor/Destructor
	CConnectFourGame();
	~CConnectFourGame();
	
	ULONG m_refCount;
    CComPtr<ITypeInfo> m_pTypeInfo;
	IConnectFourEvents *m_pConnectFourEvents;
	void *m_genericEventSink;

	//Nested class for IConnectionPoint
	class XCPConnectFourEvents : public IConnectionPoint
	{
	public:
		//CConnectFourEvents;
		inline CConnectFourGame* This(void);

		//IUnknown implementation
		STDMETHOD(QueryInterface)(REFIID riid, void** ppv);
		STDMETHOD_(ULONG, AddRef)(void);
		STDMETHOD_(ULONG, Release)(void);

		//IConnectionPoint
		STDMETHOD(Advise)(IUnknown *pUnkSink, DWORD *pdwCookie);
		STDMETHOD(Unadvise)(DWORD cookie);
		STDMETHOD(GetConnectionInterface)(IID *piid);
		STDMETHOD(GetConnectionPointContainer)(IConnectionPointContainer **ppcpc);
		STDMETHOD(EnumConnections)(IEnumConnections **ppEnum);

	} m_xcpConnectFourEvents;

	//IUnknown
	STDMETHOD_(ULONG, AddRef)();
    STDMETHOD_(ULONG, Release)();
    STDMETHOD(QueryInterface)(REFIID riid, void** ppv);

	//IDispatch
	STDMETHOD(GetIDsOfNames)(REFIID riid, OLECHAR** rgszNames, UINT cNames,
        LCID lcid, DISPID* rgDispId);
    STDMETHOD(GetTypeInfo)(UINT iTInfo, LCID lcid, ITypeInfo** ppTInfo);
    STDMETHOD(GetTypeInfoCount)(UINT* pctinfo);
	STDMETHOD(Invoke)(DISPID dispIdMember, REFIID riid, LCID lcid, WORD wFlags,
        DISPPARAMS* pDispParams, VARIANT* pVarResult, EXCEPINFO* pExcepInfo, UINT* puArgError);

	//IConnectionPointContainer implementation
	STDMETHOD(FindConnectionPoint)(REFIID riid, IConnectionPoint **ppcp);
	STDMETHOD(EnumConnectionPoints)(IEnumConnectionPoints **ppEnum);

	//IProvideClassInfo2
	STDMETHOD(GetClassInfo)(ITypeInfo **ppti);
	STDMETHOD(GetGUID)(DWORD dwGuidKind, GUID *pGUID);

	//vp game methods (IConnectFourGame interface)
	STDMETHOD_(void, EndGame)(void);
	STDMETHOD_(BSTR, GetClassID)(void);

	STDMETHOD_(void, GetGameDetails)(BSTR* Name, BSTR* Info, BSTR* SeatList, 
		BSTR* CanChangeSeat, BSTR* CanSelectSeat, SHORT* MinParticipants, 
		SHORT* MaxParticipants, BSTR* Avatar);

	STDMETHOD_(void, Send)(LONG Id, BSTR Name, SHORT Type, LONG ToId, SHORT SendType, 
		BSTR Message, BSTR* Data);
	STDMETHOD_(void, EnumMembers)(LONG Current, SHORT Level, LONG Id, BSTR Name, SHORT Type, 
		LONG X, LONG Y);
	STDMETHOD_(void, JoinGroup)(LONG Id, BSTR Name, LONG X, LONG Y, LONG GroupId, 
		BSTR GroupName);
	STDMETHOD_(void, LeaveGroup)(LONG Id, BSTR Name, LONG GroupId, BSTR GroupName);
	
	STDMETHOD_(void, StartGame)(LONG Id, BSTR Name, LONG GroupId, BSTR GroupName, 
		LONG LeaderId, SHORT Capacity, LONG Seat);

	STDMETHOD_(void, SendFromService)(LONG Id, SHORT Type, LONG SrvSendType, BSTR Message, 
		BSTR* Data, LONG ReqId);

	enum GameState
	{
		AuthStep1,
		AuthStep2,
		Started
	} con4State;

private:
	//event triggering methods
	HRESULT STDMETHODCALLTYPE Fire_LeaveGroup();
	HRESULT STDMETHODCALLTYPE Fire_GetPresenceInfo(long  Id, BSTR *  Nickname, BSTR *  Fullname, 
		BSTR *  Location, BSTR *  Email);
	HRESULT STDMETHODCALLTYPE Fire_GetParentWnd(long * hWnd);
	HRESULT STDMETHODCALLTYPE Fire_SetGameWnd(long  hWnd);
	HRESULT STDMETHODCALLTYPE Fire_DupAvatar(long  ParentWnd, long  Seat);
	HRESULT STDMETHODCALLTYPE Fire_SetAvatarPos(long  Seat, short  Left, short  Top, short  Width, 
		short  Height);
	HRESULT STDMETHODCALLTYPE Fire_EnumMembers(long  Id, short  MaxLevel);
	HRESULT STDMETHODCALLTYPE Fire_Send(long  Id, short  SendType, BSTR  Message, BSTR *  Data);
	HRESULT STDMETHODCALLTYPE Fire_QueryService(long *  Id, short *  Type);
	HRESULT STDMETHODCALLTYPE Fire_SendToService(long  ServiceId, long  SrvSendType, BSTR  Message, 
		BSTR *  Data, long  ReqId);
	
	//**** Worker Functions ****//
	void StartNewGame();
	void ProcessData(BSTR GameData);
	void ProcessData(BSTR GameData, LONG ReturnID);

	IID ConPntID;
	bool m_Con4DispSet;

	LONG m_GameSvrId;
	LONG m_GameId;
	SHORT m_GameType;
	LONG m_GameKey;
};

class CConnectFourFactory :	IClassFactory
{
	ULONG m_refCount;
public:
	//Constructors/Destructor
	CConnectFourFactory();
	~CConnectFourFactory();

	//IUnknow implementation
	STDMETHOD_(ULONG, AddRef)();
	STDMETHOD_(ULONG, Release)();
    STDMETHOD(QueryInterface)(REFIID riid, void** ppv);

	//IClassFactory
	STDMETHOD(CreateInstance)(IUnknown* pUnk, REFIID riid, void** ppv);
    STDMETHOD(LockServer)(BOOL lock);
};