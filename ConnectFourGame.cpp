#include "stdafx.h"

//****************** Global Data ************************//
CConnectFourGame* conFour;

bool g_IsPlacingChip = false;

LONG parenthWnd = 0;
HWND gameHwnd = NULL;
SLOTWNDINFO g_SlotWndInfo[GAME_COLS];

TCHAR g_szClassName[] = TEXT("Connect4WndClass");
TCHAR g_szSlotClsName[] = TEXT("SlotWndClass");

WNDCLASSEX gameWnd;
WNDCLASSEX slotWnd;

HBITMAP g_hbmGameBoard = NULL;
HBITMAP g_hbmSlot = NULL;
HBITMAP g_hbmRedChip = NULL;
HBITMAP g_hbmRedChipMask = NULL;
HBITMAP g_hbmBlueChip = NULL;
HBITMAP g_hbmBlueChipMask = NULL;
HBITMAP g_hbmGreenChip = NULL;
HBITMAP g_hbmGreenChipMask = NULL;

CHIPVECINFO *g_lpConFourVecs = NULL;
CHIPINFO chipInfo;

SEAT Seats[6];
ConnectFourPlayer ConFourPlayers[4];
ConnectFourPlayer RedPlayer;
ConnectFourPlayer BluePlayer;
ConnectFourPlayer *IPlayer;

CURRENTTURN g_PlayerTurn;
UINT g_GameState;
UINT g_GamesPlayed;

//***************** CConnectFourGame Class Definition ************************//

//***** Constructors/Destructor *****//

CConnectFourGame::CConnectFourGame() :	m_refCount(0),
										m_Con4DispSet(false),
										m_GameId(0),
										m_GameType(377),
										m_GameKey(33018),
										con4State(GameState::AuthStep1)
{
	//set our type info to point to the proper interface
	m_pTypeInfo = NULL;
	CComPtr<ITypeLib> pTypeLib;
	HRESULT hr;
	hr = LoadRegTypeLib(LIBID_ConnectFourLib, 1, 0, 0, &pTypeLib);
	CComPtr<ITypeInfo> pTypeInfo;
	hr = pTypeLib->GetTypeInfoOfGuid(IID_IConnectFourGame, &m_pTypeInfo);

	g_PlayerTurn = NOPLAYER;
	g_GameState = GAMEIDLE;
	g_GamesPlayed = 0;

	//set up the seats data
	//RedPlayers seat
	Seats[0].SeatID = 1;
	Seats[0].IsOccupied = false;
	Seats[0].Top = SEAT1_TOP;
	Seats[0].Left = SEAT1_LEFT;
	Seats[0].Width = SEAT1_WIDTH;
	Seats[0].Height = SEAT1_HEIGHT;

	//BluePlayers seat
	Seats[1].SeatID = 2;
	Seats[1].IsOccupied = false;
	Seats[1].Top = SEAT2_TOP;
	Seats[1].Left = SEAT2_LEFT;
	Seats[1].Width = SEAT2_WIDTH;
	Seats[1].Height = SEAT2_HEIGHT;

	for(SHORT i = 2; i < GAME_MAX; i++)
	{
		Seats[i].SeatID = i;
		Seats[i].IsOccupied = false;
		Seats[i].Top = 0;
		Seats[i].Left = 0;
		Seats[i].Width = 0;
		Seats[i].Width = 0;
	}

	//initialize our chip info struct
	chipInfo.dx = 0;
	chipInfo.dy = 7;
	chipInfo.height = CHIP_HEIGHT;
	chipInfo.width = CHIP_WIDTH;
	chipInfo.x = 0;
	chipInfo.y = -34;
}

CConnectFourGame::~CConnectFourGame()
{
}

//***** IUnknown Interface Method Implementation *****//

STDMETHODIMP_(ULONG) CConnectFourGame::AddRef() 
{
    return ++m_refCount;
}

STDMETHODIMP_(ULONG) CConnectFourGame::Release() 
{
    ULONG ret(--m_refCount);
    if(!ret) 
		delete this;
	
    return ret;
}

STDMETHODIMP CConnectFourGame::QueryInterface(REFIID riid, void** ppv) 
{
	USES_CONVERSION;
	//::MessageBox(NULL, "Queryinterface called", "", MB_OK);
	//look at what class and interface the client is asking for
	//LPOLESTR coclassidStr;
	//StringFromCLSID(riid, &coclassidStr);
	//::MessageBox(NULL, (LPCTSTR)OLE2T(coclassidStr), "QueryInterface", MB_OK);
	
	if(!ppv) 
	{
		//::MessageBox(NULL, "bad pointer error", "QueryInterface", MB_OK);
		return E_POINTER;
	}
    if(riid == IID_IUnknown || riid == IID_IConnectFourGame)
	{
		//::MessageBox(NULL, "IConnectFourGame pointer set", "QueryInterface", MB_OK);
		*ppv = static_cast<IConnectFourGame*>(this);
	}
	else if(riid == IID_IConnectionPointContainer)
	{
		//::MessageBox(NULL, "IConnectionPointCointainer pointer set", "QueryInterface", MB_OK);
		*ppv = static_cast<IConnectionPointContainer*>(this);
	}
	else if(riid == IID_IProvideClassInfo2)
	{
		//::MessageBox(NULL, "IProvideClassInfo2 pointer set", "QueryInterface", MB_OK);
		*ppv = static_cast<IProvideClassInfo2*>(this);
	}
	else if(riid == IID_IDispatch)
	{
		//::MessageBox(NULL, "IDispatch pointer set", "QueryInterface", MB_OK);
		//if(m_Con4DispSet == false)
		//{
			//*ppv = static_cast<IDispatch*>(m_pConnectFourEvents);
			//*ppv = static_cast<IDispatch*>(this);
			//m_Con4DispSet = true;
		//}
		//else
			*ppv = static_cast<IDispatch*>(this);
	}
	else if(riid == IID_IPersistStreamInit || riid == IID_IPersistPropertyBag)
	{
		//::MessageBox(NULL, "no IPersistStream or IPersistPropertyBag Interface", 
		///	"QueryInterface", MB_OK);
		return *ppv = NULL, E_NOINTERFACE;
	}
	else
	{
		//::MessageBox(NULL, "no matching interface found error", "QueryInterface", MB_OK);
		return *ppv = NULL, E_NOINTERFACE;
	}
	
	((IUnknown*)*ppv)->AddRef();
    return S_OK;
}

//***** IDispatch Interface Method Implementation *****//

STDMETHODIMP CConnectFourGame::GetIDsOfNames(REFIID riid, OLECHAR** rgszNames, UINT cNames,
    LCID lcid, DISPID* rgDispId) 
{
	USES_CONVERSION;
	//::MessageBox(NULL, "GetIdsOfNames called !", "IDispatch", MB_OK);
	//::MessageBox(NULL, OLE2T(rgszNames[0]), "IDispatch", MB_OK);
    //if(lcid == 0 || lcid == 9 || lcid == 0x409)
    HRESULT hr;
	hr = m_pTypeInfo->GetIDsOfNames(rgszNames, cNames, rgDispId);
	
	//if(SUCCEEDED(hr))
		//::MessageBox(NULL, "getidsofnames succeeded", "", MB_OK);

	return hr;
    //return DISP_E_UNKNOWNLCID;
}

STDMETHODIMP CConnectFourGame::GetTypeInfo(UINT iTInfo, LCID lcid, ITypeInfo** ppTInfo) 
{
	::MessageBox(NULL, "GetTypeInfo called !", "IDispatch", MB_OK);
    if(iTInfo) 
		return TYPE_E_ELEMENTNOTFOUND;
    if(!ppTInfo) 
		return E_POINTER;
    if(lcid == 0 || lcid == 9 || lcid == 0x409)
        return m_pTypeInfo.CopyTo(ppTInfo), S_OK;

    return *ppTInfo = NULL, DISP_E_UNKNOWNLCID;
}

STDMETHODIMP CConnectFourGame::GetTypeInfoCount(UINT* pctinfo) 
{
	::MessageBox(NULL, "GetTypeInfoCount called !", "IDispatch", MB_OK);
    if(!pctinfo) return E_POINTER;
    return *pctinfo = 1, S_OK;
}

STDMETHODIMP CConnectFourGame::Invoke(DISPID dispIdMember, REFIID riid, LCID lcid, WORD wFlags,
    DISPPARAMS* pDispParams, VARIANT* pVarResult, EXCEPINFO* pExcepInfo, UINT* puArgError) 
{
	//::MessageBox(NULL, "invoke called", "", MB_OK);
	//
	//we should do a LCID check here...
	//
		HRESULT hr;
		void *pvThis;
	
		pvThis = static_cast<IConnectFourGame*>(this);
		
		//if(dispIdMember == 4)
			//::MessageBox(NULL, "getgamedetails", "", MB_OK);
		
		hr = m_pTypeInfo->Invoke(pvThis, dispIdMember, wFlags, pDispParams, NULL,
        NULL, NULL);
        
		//if(SUCCEEDED(hr))
		//	::MessageBox(NULL, "invoke succeeded", "", MB_OK);

		return hr;
}

//********** Nested Class XCPConnectFourEvents Class Implementation **********//

//***** IUnknown Interface Method Implementation *****//

STDMETHODIMP CConnectFourGame::XCPConnectFourEvents::QueryInterface(REFIID riid, void **ppv)
{
	::MessageBox(NULL, "Connect4Events QueryInterface called", "_Iconnectfourevnets", MB_OK);
	if(riid == IID_IUnknown || riid == IID_IConnectionPoint)
		*ppv = static_cast<IConnectionPoint*>(this);
	else
		return *ppv = NULL, E_NOINTERFACE;
	
	((IUnknown*)*ppv)->AddRef();
	return S_OK;
}

STDMETHODIMP_(ULONG) CConnectFourGame::XCPConnectFourEvents::AddRef(void)
{
	return This()->AddRef();
}

STDMETHODIMP_(ULONG) CConnectFourGame::XCPConnectFourEvents::Release(void)
{
	return This()->Release();
}

//***** IConnectionPoint Interface Method Implementation *****//

STDMETHODIMP CConnectFourGame::XCPConnectFourEvents::Advise(IUnknown* pUnkSink, DWORD *pdwCookie)
{
	/*CoInitialize(0);
	HRESULT hr;
	//** instantiate an existing vp game first so we can grab its interface
	CComPtr<IDispatch> vpgDisp;
	CComPtr<IConnectionPointContainer> vpgConPntCntr;
	CComPtr<IConnectionPoint> vpgConPnt;

	USES_CONVERSION;
	CLSID vpgCLSID;
	LPOLESTR szvpgCLSID = L"{B87C4C58-51C9-11D0-9427-000021473605}";
	CLSIDFromString(szvpgCLSID, &vpgCLSID);

	hr = CoCreateInstance(	vpgCLSID,
							NULL,
							CLSCTX_INPROC,
							IID_IDispatch,
							(void**)&vpgDisp	);

	hr = vpgDisp->QueryInterface(	IID_IConnectionPointContainer, 
									(void**)&vpgConPntCntr);

	CLSID vpgConPntCLSID;
	LPOLESTR szvpgConPntCLSID = L"{B196B283-BAB4-101A-B69C-00AA00341D05}";
	CLSIDFromString(szvpgConPntCLSID, &vpgConPntCLSID);

	vpgConPntCntr->FindConnectionPoint(vpgConPntCLSID, &vpgConPnt);

	vpgConPnt->Advise((IUnknown*)&This()->m_ConPntProxy, &This()->tempCookie);*/
	
	HRESULT hr = S_OK;
	//::MessageBox(NULL, "Advise called", "", MB_OK);
	*pdwCookie = 0;
	//USES_CONVERSION;
	//LPOLESTR coclassidStr;
	//StringFromCLSID(This()->ConPntID, &coclassidStr);
	//::MessageBox(NULL, (LPCTSTR)OLE2T(coclassidStr), "ID being requested", MB_OK);
	//QueryInterface for correct callback type

	hr = pUnkSink->QueryInterface(/*This()->ConPntID*/IID_IDispatch,
									(void **)&(This()->m_pConnectFourEvents));

	//This()->m_pConnectFourEvents = static_cast<IConnectFourEvents*>(pUnkSink);
	//HRESULT hr = S_OK;

	//This()->m_pConnectFourEvents = static_cast<IConnectFourEvents*>(pUnkSink);

	//This()->m_pConnectFourEvents = static_cast<IDispatch*>(This()->m_genericEventSink);

	//CheckErr(hr);
	if(hr == E_NOINTERFACE)
	{
		::MessageBox(NULL, "Advise failed E_NOINTERFACE", "IConnectionPoint", MB_OK);
		hr = CONNECT_E_NOCONNECTION;
	}
	//CheckErr(hr);
	if(SUCCEEDED(hr)) // make up a meaningful cookie
	{
		//::MessageBox(NULL, "Advise succeeded", "IConnectionPoint", MB_OK);
		*pdwCookie = DWORD(This()->m_pConnectFourEvents);
		
	}
	
	//CoUninitialize();
	return hr;
}

STDMETHODIMP CConnectFourGame::XCPConnectFourEvents::Unadvise(DWORD cookie)
{
	//ensure that the cookie corresponds to a valid connection
	if(DWORD(This()->m_pConnectFourEvents) != cookie)
		return CONNECT_E_NOCONNECTION;
	//release the connection
	This()->m_pConnectFourEvents->Release();
	This()->m_pConnectFourEvents = 0;
	return S_OK;
}

STDMETHODIMP CConnectFourGame::XCPConnectFourEvents::GetConnectionInterface(IID *piid)
{
	::MessageBox(NULL, "GetConnectionInterface called", "spybot", MB_OK);
	//Assert(piid);
	//return IID of the interface managed by this subobject
	*piid = IID_IConnectFourEvents;
	return S_OK;
}

STDMETHODIMP CConnectFourGame::XCPConnectFourEvents::GetConnectionPointContainer(IConnectionPointContainer **ppcpc)
{
	::MessageBox(NULL, "GetConnectionPointContainer", "", MB_OK);

	//Assert(ppcpc);
	(*ppcpc = This())->AddRef(); //return containing object
	return S_OK;
}

STDMETHODIMP 
	CConnectFourGame::XCPConnectFourEvents::EnumConnections(
													IEnumConnections **ppEnum)
{
	::MessageBox(NULL, "EnumConnections called", "IConnectionPoint", MB_OK);
	return E_NOTIMPL;
}

inline CConnectFourGame* CConnectFourGame::XCPConnectFourEvents::This(void)
{
	return (CConnectFourGame*)((char*)this - offsetof(CConnectFourGame, 
															m_xcpConnectFourEvents));
}

//********** End of Nested Class XCPConnectFourEvents Implementation **********//

//***** IConnectionPointContainer Interface Method Implementation *****//

STDMETHODIMP CConnectFourGame::FindConnectionPoint(REFIID riid, IConnectionPoint **ppcp)
{
	USES_CONVERSION;
	
	//::MessageBox(NULL, "FindConnectionPoint Called", "IConnectionPointContainer", MB_OK);
	//LPOLESTR coclassidStr;
	//StringFromCLSID(riid, &coclassidStr);
	//::MessageBox(NULL, (LPCTSTR)OLE2T(coclassidStr), "ID being requested", MB_OK);
	
	ConPntID = riid;
	//if(riid == DIID__IConnectFourEvents)
		*ppcp = &m_xcpConnectFourEvents;
	/*else
	{
		LPOLESTR conpntid;
		StringFromCLSID(IID_IConnectionPoint, &conpntid);
		::MessageBox(NULL, (LPCTSTR)OLE2T(conpntid), "IConnectionPointID", MB_OK);

		::MessageBox(NULL, "FindConnectionPoint Failed", "=D", MB_OK);
		return *ppcp = NULL, CONNECT_E_NOCONNECTION;
	}*/

	(*ppcp)->AddRef();
	return S_OK;
}

STDMETHODIMP CConnectFourGame::EnumConnectionPoints(IEnumConnectionPoints **ppEnum)
{
	return E_NOTIMPL;
}

//***** IProvideClassInfo2 Interface Method Implementation *****//

STDMETHODIMP CConnectFourGame::GetClassInfo(ITypeInfo **ppti)
{
	::MessageBox(NULL, "GEtClassinfo", "", MB_OK);
	//Assert(ppti != 0);
	ITypeLib *ptl = 0;
	HRESULT hr = LoadRegTypeLib(LIBID_ConnectFourLib, 1, 0, 0, &ptl);
	if(SUCCEEDED(hr))
	{
		hr = ptl->GetTypeInfoOfGuid(CLSID_ConnectFourGame, ppti);
		ptl->Release();
	}
	return hr;
}
	
STDMETHODIMP CConnectFourGame::GetGUID(DWORD dwGuidKind, GUID *pGUID)
{
	if(dwGuidKind != GUIDKIND_DEFAULT_SOURCE_DISP_IID || !pGUID)
		return E_INVALIDARG;
	//IConnectFourEvents must be defined as a dispinterface
	*pGUID = IID_IConnectFourEvents;
	return S_OK;
}

//***** IConnectFourGame (vp game methods) Implementation *****//

STDMETHODIMP_(void) CConnectFourGame::EndGame(void)
{
	Fire_GetParentWnd(&parenthWnd);

	LeaveGroup(RedPlayer.PlayerID, RedPlayer.Name.m_str, 
				RedPlayer.GroupID, RedPlayer.GroupName.m_str);
	
	if(Seats[1].IsOccupied == true)
		LeaveGroup(BluePlayer.PlayerID, BluePlayer.Name.m_str, 
					BluePlayer.GroupID, BluePlayer.GroupName.m_str);

	for(int i = 0; i < MAX_VIEWERS; i++)
	{
		if(Seats[2+i].IsOccupied == true)
			LeaveGroup(	ConFourPlayers[i].PlayerID,
						ConFourPlayers[i].Name.m_str,
						ConFourPlayers[i].GroupID,
						ConFourPlayers[i].GroupName.m_str);
	}

	IPlayer = NULL;
	m_GameSvrId = 0;

	for(int x = 0; x < GAME_COLS; x++)
	{
		g_SlotWndInfo[x].hWnd = NULL;
		g_SlotWndInfo[x].chipCnt = 0;
		g_SlotWndInfo[x].Index = 0;
		g_SlotWndInfo[x].IsPlacingChip = false;
		
		for(int y = 0; y < GAME_ROWS; y++)
			g_SlotWndInfo[x].chipLocState[y] = NOCHIP;
	}

	g_PlayerTurn = NOPLAYER;
	g_GameState = GAMEIDLE;
	g_GamesPlayed = 0;

	if(g_lpConFourVecs != NULL)
	{
		delete g_lpConFourVecs;
		g_lpConFourVecs = NULL;
	}

	if(gameHwnd != NULL)
		DestroyWindow(gameHwnd);
}

STDMETHODIMP_(BSTR) CConnectFourGame::GetClassID()
{
	LPOLESTR oleClassID;
	StringFromCLSID(LIBID_ConnectFourLib, &oleClassID);
	return SysAllocString(oleClassID);
}

STDMETHODIMP_(void) CConnectFourGame::GetGameDetails(BSTR* Name, BSTR* Info, BSTR* SeatList, BSTR* CanChangeSeat, BSTR* CanSelectSeat, SHORT* MinParticipants, SHORT* MaxParticipants, BSTR* Avatar)
{
	USES_CONVERSION;

	*Name = SysAllocString(L"Connect Four Game Group\0");
	*Info = SysAllocString(L"http://www.xi0n.com\0");
	*SeatList = SysAllocString(L"2,3,24,32,Player 1;2,38,24,32,Player 2;2,95,18,24,Viewer 1;30,95,18,24,Viewer 2;59,95,18,24,Viewer 3;87,95,18,24,Viewer 4;\0");
	*CanChangeSeat = SysAllocString(L"N\0");
	*CanSelectSeat = SysAllocString(L"N\0");
	*MinParticipants = 1;
	*MaxParticipants = 6;

	HRSRC hResourceInfo;
	HGLOBAL hResource;

	//find our resource information
	hResourceInfo = FindResource(GetModuleHandle("Con4.dll"), 
		MAKEINTRESOURCE(IDG_GAMEGROUP), TEXT("GIFIMAGE"));
	
	if(hResourceInfo == NULL)
		ShowLastError("FindResource");
	
	//grab handle to our resource in global memory
	hResource = LoadResource(GetModuleHandle("Con4.dll"), hResourceInfo);
	
	if(hResource == NULL)
		ShowLastError("LoadResource");

	//determine size of the resource (av gif data)
	const DWORD resSize = SizeofResource(GetModuleHandle("Con4.dll"), hResourceInfo);
	//lock our resource data in memory, store it in a character array
	const char *szAvData = (char*)LockResource(hResource);
	
	//call method to convert ascii binary data to BSTR binary data
	BinAsciiToBinBSTR(szAvData, resSize, Avatar);

	//free resource memory block
	FreeResource(hResource);
}

STDMETHODIMP_(void) CConnectFourGame::Send(LONG Id, BSTR Name, SHORT Type, LONG ToId, SHORT SendType, BSTR Message, BSTR* Data)
{
	//is data being sent directly to me
	if(ToId != IPlayer->GroupID)
	{
		ProcessData(*Data, Id);
	}
	//or to my whole group
	else
	{
		ProcessData(*Data);
	}
}

STDMETHODIMP_(void) CConnectFourGame::EnumMembers(LONG Current, SHORT Level, LONG Id, BSTR Name, SHORT Type, LONG X, LONG Y)
{
	//we only need to fire the dup avatar/set avatar events for player slots
	//not viewer slots so check to make sure that is who they are enumerating
	if(X > 0 && X < 3)
	{
		if(Current == m_GameSvrId)
		{
			if(Id == IPlayer->PlayerID) // is it my id
			{
				Fire_DupAvatar((LONG)gameHwnd, X);
				Fire_SetAvatarPos(	X, 
									IPlayer->Seat->Left,
									IPlayer->Seat->Top,
									IPlayer->Seat->Width,
									IPlayer->Seat->Height);
			}
			else // or opponent
			{
				ConnectFourPlayer *newPlayer = (X == 1) ? &RedPlayer : &BluePlayer;
				newPlayer->PlayerID = Id;
				newPlayer->LeaderID = IPlayer->LeaderID;
				newPlayer->GroupID = m_GameSvrId;
				newPlayer->Name = Name;
				newPlayer->FullName = L"John Doe\0";
				newPlayer->Email = L"xi0n@xi0n.com\0";
				newPlayer->Location = L"xi0n Communities\0";
				newPlayer->Seat = &Seats[X-1];
				newPlayer->Seat->IsOccupied = true;
				newPlayer->IsPlaying = true;
				newPlayer->IsLeader = (Id == IPlayer->LeaderID) ? true : false;
				newPlayer->IsMyTurn = false;

				Fire_DupAvatar((LONG)gameHwnd, X);
				Fire_SetAvatarPos(	X,
									newPlayer->Seat->Left,
									newPlayer->Seat->Top,
									newPlayer->Seat->Width,
									newPlayer->Seat->Height);
			}
		}
	}
	else //we need to only grab the viewing player data if its not IPlayer
	{
		if(Id != IPlayer->PlayerID)
		{
			ConnectFourPlayer* newPlayer = &ConFourPlayers[X-3];
			newPlayer->PlayerID = Id;
			newPlayer->LeaderID = RedPlayer.LeaderID;
			newPlayer->GroupID = RedPlayer.GroupID;
			newPlayer->Name = Name;
			newPlayer->FullName = L"John Doe\0";
			newPlayer->Email = L"xi0n@xi0n.com\0";
			newPlayer->Location = L"xi0n Communities\0";
			newPlayer->Seat = &Seats[X-1];
			newPlayer->Seat->IsOccupied = true;
			newPlayer->IsPlaying = false;
			newPlayer->IsLeader = false;
			newPlayer->IsMyTurn = false;
		}
	}
}

STDMETHODIMP_(void) CConnectFourGame::JoinGroup(LONG Id, BSTR Name, LONG X, LONG Y, LONG GroupId, BSTR GroupName)
{
	//we know the leader of the group won't be joining the group
	//so do a quick check... also make sure they are joining a valid seat number
	if((Id != RedPlayer.PlayerID && X != 1) &&
		(X >= GAME_MIN && X <= GAME_MAX))
	{
		ConnectFourPlayer *newPlayer;
		//BluePlayer is joining
		if(X == 2)
		{
			if((g_GameState & BLUEPLAYERLEFT) == BLUEPLAYERLEFT)
				g_GameState ^= BLUEPLAYERLEFT;

			newPlayer = &BluePlayer;
			newPlayer->PlayerID = Id;
			newPlayer->LeaderID = RedPlayer.PlayerID; //redplayer is the leader
			newPlayer->GroupID = RedPlayer.GroupID;
			newPlayer->Name = Name;
			newPlayer->Seat = &Seats[X-1];
			newPlayer->Seat->IsOccupied = true;
			newPlayer->IsPlaying = true;
			newPlayer->IsLeader = false;
			newPlayer->IsMyTurn = false;
			newPlayer->PlayerType = BLUEPLAYER;
			GAMESTATS *gmStats = &newPlayer->GameStats;
			gmStats->Draws = 0;
			gmStats->Losses = 0;
			gmStats->Wins = 0;

			Fire_DupAvatar((LONG)gameHwnd, X);
			Fire_SetAvatarPos(	X,
								newPlayer->Seat->Left,
								newPlayer->Seat->Top,
								newPlayer->Seat->Width,
								newPlayer->Seat->Height);
			
			//if the current player is a viewer, and the game hasn't been started yet
			//when the blueplayer initially joins, go ahead, and initialize the game
			//now.
			if(IPlayer->IsPlaying == false)
				if(g_GameState == GAMEIDLE & g_PlayerTurn == NOPLAYER)
				{
					g_PlayerTurn = REDPLAYER;
					g_GameState = GAMEINPROGRESS;
					DrawTurnIndicator(gameHwnd, g_PlayerTurn, RGB(251, 206, 129));
				}
		}
		else //viewer is joining
		{
			newPlayer = &ConFourPlayers[X-3];
			newPlayer->PlayerID = Id;
			newPlayer->LeaderID = RedPlayer.PlayerID;
			newPlayer->GroupID = RedPlayer.GroupID;
			newPlayer->Name = Name;
			newPlayer->Seat = &Seats[X-1];
			newPlayer->Seat->IsOccupied = true;
			newPlayer->IsPlaying = false;
			newPlayer->IsLeader = false;
			newPlayer->IsMyTurn = false;
		}

		Fire_GetPresenceInfo(	newPlayer->PlayerID,
								&newPlayer->Name,
								&newPlayer->FullName,
								&newPlayer->Location,
								&newPlayer->Email);
	}
}

STDMETHODIMP_(void) CConnectFourGame::LeaveGroup(LONG Id, BSTR Name, LONG GroupId, BSTR GroupName)
{
	bool found = false;
	ConnectFourPlayer *p = NULL;

	if(Id == BluePlayer.PlayerID)
	{
		g_GameState |= BLUEPLAYERLEFT;
		p = &BluePlayer;
		found = true;
	}
	else if(Id == RedPlayer.PlayerID)
	{
		p = &RedPlayer;
		found = true;
	}
	else
		for(int i = 0; i < MAX_VIEWERS; i++)
			if(ConFourPlayers[i].PlayerID == Id)
			{
				p = &ConFourPlayers[i];
				found = true;
			}
	
	if(found == true)
	{
		p->FullName.Empty();
		p->Name.Empty();
		p->Email.Empty();
		p->Location.Empty();
		p->GroupName.Empty();
		p->PlayerID = 0;
		p->GroupID = 0;
		p->LeaderID = 0;
		p->IsLeader = false;
		p->IsMyTurn = false;
		p->IsPlaying = false;
		p->Seat->IsOccupied = false;
	}
}

STDMETHODIMP_(void) CConnectFourGame::StartGame(LONG Id, BSTR Name, LONG GroupId, BSTR GroupName, LONG LeaderId, SHORT Capacity, LONG Seat)
{
	//make sure they pass in valid seat index
	if(Seat < GAME_MIN || Seat > GAME_MAX)
	{
		::MessageBox(NULL, "Invalid Seat Index", "ERROR", MB_ICONERROR);
		return;
	}

	if(Seats[Seat-1].IsOccupied)
	{
		::MessageBox(NULL, "Seat is already taken", "ERROR", MB_ICONERROR);
		return;
	}

	HRESULT hr = S_OK;

	if(!SUCCEEDED(hr))
		::MessageBox(NULL, "Raising GetParentWnd Event Failed", "", MB_OK);
	
	//initialize, register, our main game window
	gameWnd.cbSize = sizeof(WNDCLASSEX);
	gameWnd.style = 0;
	gameWnd.lpfnWndProc = GameWndProc;
	gameWnd.cbClsExtra = 0;
	gameWnd.cbWndExtra = 0;
	gameWnd.hInstance = GetModuleHandle("Con4.dll");
	gameWnd.hIcon = NULL;
	gameWnd.hCursor = LoadCursor(NULL, IDC_ARROW);
	gameWnd.hbrBackground = HBRUSH(GetStockObject(LTGRAY_BRUSH));
	gameWnd.lpszClassName = g_szClassName;
	gameWnd.hIconSm = NULL;

	RegisterClassEx(&gameWnd);
	ShowLastError("registercls");

	//initialize and register our slot child window
	slotWnd.cbSize = sizeof(WNDCLASSEX);
	slotWnd.style = 0;
	slotWnd.lpfnWndProc = SlotWndProc;
	slotWnd.cbClsExtra = 0;
	slotWnd.cbWndExtra = 0;
	slotWnd.hInstance = GetModuleHandle("Con4.dll");
	slotWnd.hIcon = NULL;
	slotWnd.hCursor = LoadCursor(NULL, IDC_ARROW);
	slotWnd.hbrBackground = HBRUSH(GetStockObject(WHITE_BRUSH));
	slotWnd.lpszClassName = g_szSlotClsName;
	slotWnd.hIconSm = NULL;

	RegisterClassEx(&slotWnd);
	
	ConnectFourPlayer *newPlayer;
	
	if(Seat < 3)
	{
		if(Seat == 1)
		{
			newPlayer = &RedPlayer;
			newPlayer->PlayerType = REDPLAYER;
		}
		else if(Seat == 2)
		{
			newPlayer = &BluePlayer;
			newPlayer->PlayerType = BLUEPLAYER;
		}

		newPlayer->IsPlaying = true;
		newPlayer->IsMyTurn = false;
		newPlayer->IsLeader = (newPlayer == &RedPlayer) ? true : false;
		GAMESTATS *gmStats = &newPlayer->GameStats;
		gmStats->Draws = 0;
		gmStats->Losses = 0;
		gmStats->Wins = 0;
	}
	else
	{
		newPlayer = &ConFourPlayers[Seat-3];
		newPlayer->IsPlaying = false;
		newPlayer->IsLeader = false;
		newPlayer->IsMyTurn = false;
	}

	newPlayer->PlayerID = Id;
	newPlayer->LeaderID = LeaderId;
	newPlayer->GroupID = GroupId;
	newPlayer->Name = Name;
	newPlayer->FullName = L"John Doe\0";
	newPlayer->Email = L"xi0n@xi0n.com\0";
	newPlayer->Location = L"xi0n Communities\0";
	newPlayer->Seat = &Seats[Seat-1];
	newPlayer->Seat->IsOccupied = true;
	
	IPlayer = newPlayer; //set pointer to the Player instance of the game instance.
	m_GameSvrId = GroupId; //store the groupid as member data of the game instance.

	if(newPlayer->IsLeader == true) 
	{
		::MessageBox(NULL, "calling QueryService", "con4", MB_OK);

		hr = Fire_QueryService(&m_GameId, &m_GameType);
		
		if(SUCCEEDED(hr))
			::MessageBox(NULL, "QuerySevice called", "con4", MB_OK);
		else
			::MessageBox(NULL, "QueryService called", "con4", MB_OK);

		//verify that they set the proper Id here
		//if(m_GameId == Id)
		//{
			BSTR szSvcBytes = NULL;
			const char szSvcBytesA[4] = {(char)0, (char)0, (char)128, (char)250};
			BinAsciiToBinBSTR((LPCSTR)szSvcBytesA, 4, &szSvcBytes);

			hr = Fire_SendToService(3, 1, SysAllocString(L""), &szSvcBytes, 1);

			SysFreeString(szSvcBytes);
			if(SUCCEEDED(hr))
				::MessageBox(NULL, "Send to service called!", "con4", MB_OK);
			else
				::MessageBox(NULL, "send to service failed.", "con4", MB_OK);
		//}
		//else
		//	::MessageBox(NULL, "gameid didn't match", "con4", MB_OK);

		hr = Fire_GetParentWnd(&parenthWnd);

		if(SUCCEEDED(hr))
			::MessageBox(NULL, "getparentwnd called!", "con4", MB_OK);
		else
			::MessageBox(NULL, "getparentwnd failed", "con4", MB_OK);

		//create our main game window
		gameHwnd = CreateWindowEx(NULL, g_szClassName, "",
					WS_CHILD | WS_VISIBLE, 0, 0, 450, 300, 
					(HWND)parenthWnd, NULL,
					GetModuleHandle("Con4.dll"), NULL);

		if(gameHwnd == NULL)
			ShowLastError("CreateWindowEx");

		hr = Fire_SetGameWnd((LONG)gameHwnd);
		hr = Fire_DupAvatar((LONG)gameHwnd, Seat);
		hr = Fire_SetAvatarPos(	Seat, 
								newPlayer->Seat->Left,
								newPlayer->Seat->Top,
								newPlayer->Seat->Width,
								newPlayer->Seat->Height);
	} 
	else 
	{
		//compile our packet
		size_t PACKETINFOBUF = sizeof(PACKETINFO);
		size_t STATEINFORQBUF = sizeof(STATEINFOREQUEST);
		size_t PACKETBUF = PACKETINFOBUF+STATEINFORQBUF;

		PACKETINFO *lpPacketInfo = (PACKETINFO*)malloc(PACKETINFOBUF);
		STATEINFOREQUEST *lpStateInfoRq = (STATEINFOREQUEST*)malloc(STATEINFORQBUF);
		char *szStateInfoRqPcktA = new char[PACKETBUF];
		BSTR szStateInfoRqPckt = NULL;

		lpPacketInfo->GameKey = m_GameKey;
		lpPacketInfo->PacketType = 1;

		lpStateInfoRq->RequestType = 1;
		
		CopyMemory(	(LPVOID)szStateInfoRqPcktA, 
					(LPVOID)lpPacketInfo, 
					PACKETINFOBUF);

		CopyMemory(	(LPVOID)(szStateInfoRqPcktA+PACKETINFOBUF), 
					(LPVOID)lpStateInfoRq, 
					STATEINFORQBUF);

		BinAsciiToBinBSTR(szStateInfoRqPcktA, PACKETBUF, &szStateInfoRqPckt);

		Fire_Send(LeaderId, 0, SysAllocString(L""), &szStateInfoRqPckt);

		SysFreeString(szStateInfoRqPckt);
		free((LPVOID)lpPacketInfo);
		free((LPVOID)lpStateInfoRq);
		delete szStateInfoRqPcktA;
	}

}

STDMETHODIMP_(void) CConnectFourGame::SendFromService(LONG Id, SHORT Type, LONG SrvSendType, BSTR Message, BSTR* Data, LONG ReqId)
{
	if(con4State == GameState::AuthStep1)
	{
		const char szSvcBytesA[8] = {0, 0, 128, 250, 0, 0, 0, 0};
		BSTR szSvcBytes = NULL;
		BinAsciiToBinBSTR(szSvcBytesA, 8, &szSvcBytes);
		con4State = GameState::AuthStep2;
		Fire_SendToService(3, 6, SysAllocString(L""), &szSvcBytes, 1);
		SysFreeString(szSvcBytes);
	}
	else if(con4State == GameState::AuthStep2)
		con4State = GameState::Started;
}

//******* Event Firing Methods *******//

STDMETHODIMP CConnectFourGame::Fire_LeaveGroup()
{
	HRESULT hr;
	DISPPARAMS disp = {NULL, NULL, 0, 0};

	hr = m_pConnectFourEvents->Invoke(	1, IID_NULL, LOCALE_USER_DEFAULT, 
										DISPATCH_METHOD,
										&disp, NULL, NULL, NULL);
	return hr;
}

STDMETHODIMP CConnectFourGame::Fire_GetPresenceInfo(long Id, BSTR *Nickname, BSTR *Fullname, BSTR *Location, BSTR *Email)
{
	HRESULT hr;
	CComVariant avarParams[5];
	avarParams[4] = Id;
	avarParams[3] = Nickname;
	avarParams[2] = Fullname;
	avarParams[1] = Location;
	avarParams[0] = Email;
	DISPPARAMS params = { avarParams, NULL, 5, 0 };
	hr = m_pConnectFourEvents->Invoke(	2, IID_NULL, LOCALE_USER_DEFAULT, 
										DISPATCH_METHOD, &params, 
										NULL, NULL, NULL);
	return hr;
}

STDMETHODIMP CConnectFourGame::Fire_GetParentWnd(long *hWnd)
{
	HRESULT hr;

	CComVariant avarParams[1];
	avarParams[0] = hWnd;
	//VARIANTARG avarParams[1];
	//VariantInit(&avarParams[0]);
	//avarParams[0].vt = VT_BYREF | VT_I4;
	//avarParams[0].plVal = hWnd;

	//VARTYPE varType = VT_I4;

	EXCEPINFO excepInfo;
	ZeroMemory((void*)&excepInfo, sizeof(EXCEPINFO));
	UINT ArgNumb;
	DISPPARAMS params = { avarParams, NULL, 1, 0 };
	VARIANT varResults;
	VariantClear(&varResults);

	/*UINT tInfo = 2;
	hr = ((IDispatch*)m_pConnectFourEvents)->GetTypeInfoCount(&tInfo);

	if(tInfo == 1)
		::MessageBox(NULL, "type info is supported", "", MB_OK);
	if(tInfo == 0)
		::MessageBox(NULL, "type info is not supported", "", MB_OK);
	if(tInfo == 2)
		::MessageBox(NULL, "the dispatch method does nothing !", "", MB_OK);*/

	//DISPID pDispID;
	//LPOLESTR pNames = L"GetParentWnd";

	//hr = m_pConnectFourEvents->GetIDsOfNames(	IID_NULL,
	//											&pNames,
	//											1,
	//											0,
	//											&pDispID);
	//if(pDispID == 3)
	//	::MessageBox(NULL, "event dispids match", "????", MB_OK);
	////else
	//{
	//	char szdid[16];
	//	ltoa((long)pDispID, szdid, 10);

	//	::MessageBox(NULL, szdid, "????", MB_OK);
	//}
	//::MessageBox(NULL, "fire getparendwnd", "asdfasdf", MB_OK);

	//if(!m_pConnectFourEvents)
		//::MessageBox(NULL, "NULL interface =[", "", MB_OK);
	//VARIANT  varResult;
	//VariantInit(&varResult);
	//::MessageBox(NULL, "Attempting to invoke event", "", MB_OK);
	//CLSID conclsid;
	//CLSIDFromString(L"{B196B283-BAB4-101A-B69C-00AA00341D05}", &conclsid);
	//CComPtr<IUnknown> con4eventsUnk = reinterpret_cast<IUnknown*>(m_pConnectFourEvents);
	//CComPtr<IDispatch> con4eventDisp = reinterpret_cast<IDispatch*>(con4eventsUnk.p);

	//::MessageBox(NULL, "attempt to call evnet", "", MB_OK);
	
	
	//if(hWnd == NULL)
	//	::MessageBox(NULL, "hWnd is NULL", "", MB_OK);


	//ZeroMemory(szPnt, sizeof(szPnt));
	//ZeroMemory(szDeRef, sizeof(szDeRef));
	//sprintf(szPnt, "%d", (LONG_PTR)hWnd);
	//sprintf(szDeRef, "%d", (*hWnd));
	//::MessageBox(NULL, szPnt, "hWnd pointer val", MB_OK);
	//::MessageBox(NULL, szDeRef, "hWnd Val", MB_OK);

	if(m_pConnectFourEvents != NULL)
		hr = m_pConnectFourEvents->Invoke(	(DISPID)3,
											IID_NULL, 
											0,
											DISPATCH_METHOD,
											&params,
											&varResults,
											&excepInfo,
											&ArgNumb);

	//if(hWnd != NULL)
	///{
		char szPnt[10];
		char szDeRef[10];
		ZeroMemory(szPnt, sizeof(szPnt));
		ZeroMemory(szDeRef, sizeof(szDeRef));
		sprintf(szPnt, "%d", (LONG_PTR)hWnd);
		sprintf(szDeRef, "%d", (*hWnd));
		::MessageBox(NULL, szPnt, "hWnd pointer val", MB_OK);
		::MessageBox(NULL, szDeRef, "hWnd Val", MB_OK);

		//(*hWnd) = (LONG_PTR)hWnd;
	//}


	//CheckErr(hr);

	//char szHWND[sizeof(long)*8+1];
	//ltoa(*hWnd, szHWND, 10);
	//::MessageBox(NULL, (LPCTSTR)szHWND, "", MB_OK);

	/*if(&excepInfo != NULL && excepInfo.bstrDescription != NULL)
	{
		BSTR errDesc = excepInfo.bstrDescription;
		_bstr_t szErrDesc = errDesc;
		::MessageBox(NULL, (LPCTSTR)szErrDesc, "err info", MB_OK);
	}*/
	//m_pConnectFourEvents->GetParentWnd(hWnd);
	//char szHWND[16];
	//ltoa(*hWnd, szHWND, 10);
	//::MessageBox(NULL, (LPCTSTR)szHWND, "", MB_OK);

	//m_pConnectFourEvents->GetParentWnd(hWnd);
	//::MessageBox(NULL, "hmm looks like we made it", "", MB_OK);
	//CheckErr(hr);

	/*hr = DispCallFunc(	(void*)&m_pConnectFourEvents, 
						(ULONG_PTR)56, 
						CC_CDECL, 
						NULL, 
						1, 
						&varType, 
						(VARIANTARG**)&avarParams,
						NULL);*/

	//char szArgNumb[16];
	//ltoa((long)excepInfo.scode, szArgNumb, 10);
	//::MessageBox(NULL, (LPCTSTR)szArgNumb, "invoke error", MB_OK);
	//m_pConnectFourEvents->GetParentWnd(hWnd);
	//CheckErr(hr);
	
	/*gameWnd.cbSize = sizeof(WNDCLASSEX);
	gameWnd.style = 0;
	gameWnd.lpfnWndProc = GameWndProc;
	gameWnd.cbClsExtra = 0;
	gameWnd.cbWndExtra = 0;
	gameWnd.hInstance = NULL;
	gameWnd.hIcon = NULL;
	gameWnd.hCursor = LoadCursor(NULL, IDC_ARROW);
	gameWnd.hbrBackground = (HBRUSH)LTGRAY_BRUSH;
	gameWnd.lpszClassName = g_szClassName;
	gameWnd.hIconSm = NULL;

	if(!RegisterClassEx(&gameWnd))
		ShowLastError("RegisterClassEx");
	
	gameHwnd = CreateWindowEx(NULL, g_szClassName, "",
		WS_CHILD | WS_VISIBLE, 0, 0, 450, 300, (HWND)*hWnd, NULL,
		GetModuleHandle("Con4.dll"), NULL);
    
	if(gameHwnd == NULL)
       ShowLastError("CreateWindowEx");
    
	hr = Fire_SetGameWnd((LONG)gameHwnd);
    hr = Fire_DupAvatar((LONG)gameHwnd, 1);
	hr = Fire_SetAvatarPos(1, 0, 0, 48, 64);*/
	return hr;
}

STDMETHODIMP CConnectFourGame::Fire_SetGameWnd(long hWnd)
{
	HRESULT hr;
	CComVariant avarParams[1];
	avarParams[0] = hWnd;
	DISPPARAMS params = { avarParams, NULL, 1, 0 };
	hr = m_pConnectFourEvents->Invoke(	4, IID_NULL, LOCALE_USER_DEFAULT, 
										DISPATCH_METHOD, &params,
										NULL, NULL, NULL);
	return hr;	
}

STDMETHODIMP CConnectFourGame::Fire_DupAvatar(long ParentWnd, long Seat)
{
	HRESULT hr;
	CComVariant avarParams[2];
	avarParams[1] = ParentWnd;
	avarParams[0] = Seat;
	DISPPARAMS params = { avarParams, NULL, 2, 0 };
	hr = m_pConnectFourEvents->Invoke(	5, IID_NULL, LOCALE_USER_DEFAULT, 
										DISPATCH_METHOD, &params, 
										NULL, NULL, NULL);
	return hr;
}

STDMETHODIMP CConnectFourGame::Fire_SetAvatarPos(long Seat, short Left, short Top, short Width, short Height)
{
	HRESULT hr;
	CComVariant avarParams[5];
	avarParams[4] = Seat;
	avarParams[3] = Left;
	avarParams[2] = Top;
	avarParams[1] = Width;
	avarParams[0] = Height;
	DISPPARAMS params = { avarParams, NULL, 5, 0 };
	hr = m_pConnectFourEvents->Invoke(	6, IID_NULL, LOCALE_USER_DEFAULT, 
										DISPATCH_METHOD, &params, 
										NULL, NULL, NULL);
	return hr;
}

STDMETHODIMP CConnectFourGame::Fire_EnumMembers(long Id, short MaxLevel)
{
	HRESULT hr;
	CComVariant avarParams[2];
	avarParams[1] = Id;
	avarParams[0] = MaxLevel;
	DISPPARAMS params = { avarParams, NULL, 2, 0 };
	hr = m_pConnectFourEvents->Invoke(	7, IID_NULL, LOCALE_USER_DEFAULT, 
										DISPATCH_METHOD, &params, 
										NULL, NULL, NULL);
	return hr;
}

STDMETHODIMP CConnectFourGame::Fire_Send(long Id, short SendType, BSTR Message, BSTR *Data)
{
	HRESULT hr;
	CComVariant avarParams[4];
	avarParams[3] = Id;
	avarParams[2] = SendType;
	avarParams[1] = Message;
	avarParams[0].SetByRef(Data);
	DISPPARAMS params = { avarParams, NULL, 4, 0 };
	hr = m_pConnectFourEvents->Invoke(	8, IID_NULL, LOCALE_USER_DEFAULT, 
										DISPATCH_METHOD, 
										&params, NULL, NULL, NULL);
	return hr;
}

STDMETHODIMP CConnectFourGame::Fire_QueryService(long *Id, short *Type)
{
	HRESULT hr;
	CComVariant avarParams[2];
	avarParams[1] = Id;
	avarParams[0] = Type;
	DISPPARAMS params = { avarParams, NULL, 2, 0 };
	hr = m_pConnectFourEvents->Invoke(	9, IID_NULL, LOCALE_USER_DEFAULT, 
										DISPATCH_METHOD, &params, 
										NULL, NULL, NULL);
	return hr;
}

STDMETHODIMP CConnectFourGame::Fire_SendToService(long ServiceId, long SrvSendType, BSTR Message, BSTR *Data, long ReqId)
{
	HRESULT hr;
	CComVariant avarParams[5];
	avarParams[4] = ServiceId;
	avarParams[3] = SrvSendType;
	avarParams[2] = Message;
	avarParams[1].SetByRef(Data);
	avarParams[0] = ReqId;
	DISPPARAMS params = { avarParams, NULL, 5, 0 };
	hr = m_pConnectFourEvents->Invoke(	10, IID_NULL, LOCALE_USER_DEFAULT, 
										DISPATCH_METHOD, &params, 
										NULL, NULL, NULL);
	return hr;
}

//******************** Worker Functions *******************//

void CConnectFourGame::ProcessData(BSTR GameData)
{
	//first things first convert the binary BSTR data to its ascii representation
	LONG len = SysStringLen(GameData);
	char *szGameDataA = new char[len];

	INT lenConv = WideCharToMultiByte(	CP_ACP,
										0,
										GameData,
										len,
										szGameDataA,
										len,
										NULL,
										FALSE);

	//then grab the PACKETINFO struct off the top of the packet
	size_t PACKETINFOBUF = sizeof(PACKETINFO);

	PACKETINFO *lpPacketInfo = (PACKETINFO*)malloc(PACKETINFOBUF);

	CopyMemory((LPVOID)lpPacketInfo, (LPVOID)szGameDataA, PACKETINFOBUF);

	//check the packet type to distinguish what kind of data we have, and what
	//structure to deserialize the rest of the data into. Then perform the necassary
	//actions.
	switch(lpPacketInfo->PacketType)
	{
		//place chip packet
		case 3:
		{
			size_t PLACECHIPINFOBUF = sizeof(PLACECHIPINFO);
			PLACECHIPINFO *lpPlaceChip = (PLACECHIPINFO*)malloc(PLACECHIPINFOBUF);

			CopyMemory(	(LPVOID)lpPlaceChip,
						(LPVOID)(szGameDataA+PACKETINFOBUF),
						PLACECHIPINFOBUF);

			//verify the proper chip information was sent
			switch(lpPlaceChip->ChipType)
			{
				case REDCHIP: case BLUECHIP:
					if(lpPlaceChip->ChipType != (CHIPLOCSTATE)IPlayer->PlayerType)
						SendMessage(	g_SlotWndInfo[lpPlaceChip->SlotX].hWnd, 
										WM_LBUTTONDOWN, 
										255, 
										0);
				break;
			}

			free((LPVOID)lpPlaceChip);
		}
		break;

		//new game packet
		case 4:
			if(IPlayer->IsLeader == false)
				StartNewGame();
		break;
	}

	// free memory
	free((LPVOID)lpPacketInfo);
	delete szGameDataA;
}

void CConnectFourGame::ProcessData(BSTR GameData, LONG ReturnID)
{
	//first things first convert the binary BSTR data to its ascii representation
	LONG len = SysStringLen(GameData);
	char *szGameDataA = new char[len];

	INT lenConv = WideCharToMultiByte(	CP_ACP,
										0,
										GameData,
										len,
										szGameDataA,
										len,
										NULL,
										FALSE);

	//then grab the PACKETINFO struct off the top of the packet
	size_t PACKETINFOBUF = sizeof(PACKETINFO);

	PACKETINFO *lpPacketInfo = (PACKETINFO*)malloc(PACKETINFOBUF);

	CopyMemory((LPVOID)lpPacketInfo, (LPVOID)szGameDataA, PACKETINFOBUF);

	//check the packet type to distinguish what kind of data we have, and what
	//structure to deserialize the rest of the data into. Then perform the necassary
	//actions.
	switch(lpPacketInfo->PacketType)
	{
		//State Info Request
		case 1:
		{
			//initialize game
			if(g_GameState == GAMEIDLE & ReturnID == BluePlayer.PlayerID)
			{
				g_PlayerTurn = REDPLAYER;
				g_GameState = GAMEINPROGRESS;
				DrawTurnIndicator(gameHwnd, REDPLAYER, RGB(251, 206, 129));
			}

			size_t PACKETINFOBUF = sizeof(PACKETINFO);
			size_t GAMESTATEINFOBUF = sizeof(GAMESTATEINFO);
			size_t OVERALLBUF = PACKETINFOBUF+GAMESTATEINFOBUF;
			
			PACKETINFO *lpGmStatePcktInfo = (PACKETINFO*)malloc(PACKETINFOBUF);
			GAMESTATEINFO *lpGameStateInfo = (GAMESTATEINFO*)malloc(GAMESTATEINFOBUF);
			char *szGmStateInfoPcktA = new char[OVERALLBUF];
			BSTR szGmStateInfoPckt = NULL;

			//set the packet info
			lpGmStatePcktInfo->GameKey = m_GameKey;
			lpGmStatePcktInfo->PacketType = 2;
		
			//set some game state information
			lpGameStateInfo->CurrentGameState = g_GameState;
			lpGameStateInfo->CurrentTurn = g_PlayerTurn;
			lpGameStateInfo->ScoreBoard.GamesPlayed = g_GamesPlayed;
			
			//set red players stats in gamestateinfo structure
			GAMESTATS *lpGmStats = &lpGameStateInfo->ScoreBoard.PlayerStats[0];
			GAMESTATS *lpPlayerStats = &RedPlayer.GameStats;
			
			lpGmStats->Draws = lpPlayerStats->Draws;
			lpGmStats->Losses = lpPlayerStats->Losses;
			lpGmStats->Wins = lpPlayerStats->Wins;

			//set blue players stats in gamestateinfo structure
			lpGmStats = &lpGameStateInfo->ScoreBoard.PlayerStats[1];
			lpPlayerStats = &BluePlayer.GameStats;

			lpGmStats->Draws = lpPlayerStats->Draws;
			lpGmStats->Losses = lpPlayerStats->Losses;
			lpGmStats->Wins = lpPlayerStats->Wins;
			
			if((g_GameState & GAMEINTERMISSION) == GAMEINTERMISSION &
				g_lpConFourVecs != NULL)
			{
				CopyMemory(	(LPVOID)lpGameStateInfo->ConFourVecs,
							(LPVOID)g_lpConFourVecs, 
							(sizeof(CHIPVECINFO) * 4));
			}

			//set all the needed chip location information
			for(int x = 0; x < GAME_COLS; x++)
			{
				lpGameStateInfo->ChipLocInfo[x].ChipCount = g_SlotWndInfo[x].chipCnt;
				
				for(int y = 0; y < GAME_ROWS; y++)
					lpGameStateInfo->ChipLocInfo[x].ChipLocState[y] = 
													g_SlotWndInfo[x].chipLocState[y];
			}

			//serialize the data
			CopyMemory(	(LPVOID)szGmStateInfoPcktA, 
						(LPVOID)lpGmStatePcktInfo, 
						PACKETINFOBUF);

			CopyMemory(	(LPVOID)(szGmStateInfoPcktA+PACKETINFOBUF),
						(LPVOID)lpGameStateInfo,
						GAMESTATEINFOBUF);

			BinAsciiToBinBSTR(szGmStateInfoPcktA, OVERALLBUF, &szGmStateInfoPckt);

			//raise the event to have the data sent through the client.
			Fire_Send(ReturnID, 0, SysAllocString(L""), &szGmStateInfoPckt);

			//free up all our memory
			SysFreeString(szGmStateInfoPckt);
			free((LPVOID)lpGmStatePcktInfo);
			free((LPVOID)lpGameStateInfo);
			delete szGmStateInfoPcktA;
		}
		break;
		
		//Game State Info Packet
		case 2:
		{
			//deserialize the gamestateinfo data to its equivalent structure.
			size_t GMSTINFOBUF = sizeof(GAMESTATEINFO);
			GAMESTATEINFO *lpGameStateInfo = (GAMESTATEINFO*)malloc(GMSTINFOBUF);

			CopyMemory(	(LPVOID)lpGameStateInfo, 
						(LPVOID)(szGameDataA+PACKETINFOBUF),
						GMSTINFOBUF);

			g_PlayerTurn = lpGameStateInfo->CurrentTurn;
			g_GameState = lpGameStateInfo->CurrentGameState;
			g_GamesPlayed = lpGameStateInfo->ScoreBoard.GamesPlayed;

			RedPlayer.GameStats = lpGameStateInfo->ScoreBoard.PlayerStats[0];
			BluePlayer.GameStats = lpGameStateInfo->ScoreBoard.PlayerStats[1];

			for(int x = 0; x < GAME_COLS; x++)
			{
				g_SlotWndInfo[x].chipCnt = lpGameStateInfo->ChipLocInfo[x].ChipCount;
				
				for(int y = 0; y < GAME_ROWS; y++)
					g_SlotWndInfo[x].chipLocState[y] = 
										lpGameStateInfo->ChipLocInfo[x].ChipLocState[y];
			}
			
			if((g_GameState & GAMEINTERMISSION) == GAMEINTERMISSION)
			{
				g_lpConFourVecs = new CHIPVECINFO[4];

				CopyMemory(	(LPVOID)g_lpConFourVecs,
							(LPVOID)lpGameStateInfo->ConFourVecs,
							sizeof(CHIPVECINFO) * 4);
				// set timer with LPCHIPVECINFO as TimerId.
				SetTimer(gameHwnd, UINT_PTR(g_lpConFourVecs), 500, ConnectFourProc);
			}

			//create our main game window
			gameHwnd = CreateWindowEx(NULL, g_szClassName, "",
						WS_CHILD | WS_VISIBLE, 0, 0, 450, 300, 
						(HWND)parenthWnd, NULL,
						GetModuleHandle("Con4.dll"), NULL);

			if(gameHwnd == NULL)
				ShowLastError("CreateWindowEx");

			Fire_SetGameWnd(LONG(gameHwnd));
			Fire_EnumMembers(m_GameSvrId, 1);

		
			free((LPVOID)lpGameStateInfo);
		}
		break;
	}

	free((LPVOID)lpPacketInfo);
	delete szGameDataA;
}

void CConnectFourGame::StartNewGame()
{
	// if game hasn't started yet, return
	if((g_GameState & GAMEIDLE) == GAMEIDLE)
		return;

	INT response;

	// if game leader wants to start a new game, ask them first.
	if(IPlayer->IsLeader == true & (g_GameState & GAMEINPROGRESS) == GAMEINPROGRESS)
		if(::MessageBox(	NULL,
			"The game is in progress are you sure you want to start a new game ?",
			"Game Still In Progress", MB_YESNO) != IDYES)
				return;

	// if the blue|[2nd] player left then update the game state, and player turn values.
	if((g_GameState & BLUEPLAYERLEFT) == BLUEPLAYERLEFT)
	{
		g_GameState = GAMEIDLE;
		g_PlayerTurn = NOPLAYER;
	}
	else
	{
		g_GameState = GAMEINPROGRESS;
		g_PlayerTurn = REDPLAYER;
	}

	// clear chip vector memory @ g_lpConFourVecs[CHIPVECINFO*]
	if(g_lpConFourVecs != NULL)
	{
		delete g_lpConFourVecs;
		g_lpConFourVecs = NULL;
	}

	for(int x = 0; x < GAME_COLS; x++)
	{
		g_SlotWndInfo[x].chipCnt = 0;
		g_SlotWndInfo[x].IsPlacingChip = false;
		
		for(int y = 0; y < GAME_ROWS; y++)
			g_SlotWndInfo[x].chipLocState[y] = NOCHIP;

		InvalidateRect(g_SlotWndInfo[x].hWnd, NULL, FALSE);
	}

	RECT turnRect;
	SetRect(&turnRect, 300, 0, 500, 350);
	InvalidateRect(gameHwnd, &turnRect,  FALSE);

	if(IPlayer->IsLeader == true)
	{
		size_t PACKETINFOBUF = sizeof(PACKETINFO);
		PACKETINFO *lpPacketInfo = (PACKETINFO*)malloc(PACKETINFOBUF);
		char *szPacketInfoA = new char[PACKETINFOBUF];
		BSTR szPacketInfo = NULL;

		lpPacketInfo->GameKey = m_GameKey;
		lpPacketInfo->PacketType = 4;

		CopyMemory((LPVOID)szPacketInfoA, (LPVOID)lpPacketInfo, PACKETINFOBUF);
		BinAsciiToBinBSTR(szPacketInfoA, PACKETINFOBUF, &szPacketInfo);

		Fire_Send(m_GameSvrId, 0, SysAllocString(L""), &szPacketInfo);

		SysFreeString(szPacketInfo);
		delete szPacketInfoA;
		free((LPVOID)lpPacketInfo);
	}
}

//********************  Friend Functions  *****************//

//*************** Window Procedures *********************//
LRESULT CALLBACK GameWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static HMENU hMenu;

	switch(msg)
    {
		case WM_CREATE:
		{
			//load up our menu
			hMenu = LoadMenu((HINSTANCE)GetModuleHandle("Con4.dll"),
								MAKEINTRESOURCE(IDR_MNUOPTS));
			hMenu = GetSubMenu(hMenu, 0);

			if(IPlayer->IsLeader == false)
				EnableMenuItem(hMenu, ID_OPTIONS_NEWGAME, MF_GRAYED);

			//load up our bitmap resource
			g_hbmGameBoard = LoadBitmap(GetModuleHandle("Con4.dll"),
				MAKEINTRESOURCE(IDB_GAMEBOARD));
			if(g_hbmGameBoard == NULL)
				ShowLastError("LoadBitmap");
			
			int x = SLOT_STARTPNTX;
			int y = SLOT_STARTPNTY;
			int width = SLOT_WIDTH;
			int height = SLOT_HEIGHT;

			//create our slots table
			for(int i = 0; i < GAME_COLS; i++)
			{
				g_SlotWndInfo[i].hWnd = CreateWindowEx(NULL, g_szSlotClsName, "",
					WS_CHILDWINDOW | WS_VISIBLE, x, y, width, height,
					hwnd, NULL, HINSTANCE(GetWindowLong(hwnd, GWL_HINSTANCE)),
					NULL);

				if(g_SlotWndInfo[i].hWnd == NULL)
					ShowLastError("CreateWindowEx");
				
				//set the associatted struct with the window
				g_SlotWndInfo[i].Index = i;
				SetWindowLong(	g_SlotWndInfo[i].hWnd, 
								GWL_USERDATA, 
								(LONG_PTR)&g_SlotWndInfo[i]);

				x += (SLOT_WIDTH + SLOT_HSPACE);
			}
		}
		break;

		case WM_PAINT:
		{
			HDC hdc;
			PAINTSTRUCT ps;
			BITMAP bm;

			//grab the dc of our window
			hdc = BeginPaint(hwnd, &ps);

			//create a compatible memory dc from our windows dc
			HDC hdcMem = CreateCompatibleDC(hdc);
			//select our bitmap into the memory dc
			HBITMAP hbmOld = (HBITMAP)SelectObject(hdcMem, g_hbmGameBoard);
			//get object info for g_hbmGameBoard, and place it in our bm struct
			GetObject(g_hbmGameBoard, sizeof(bm), &bm);
			
			BitBlt(hdc, 0, 0, bm.bmWidth, bm.bmHeight, hdcMem, 0, 0, SRCCOPY);

			if((g_GameState & GAMEINPROGRESS) == GAMEINPROGRESS)
				DrawTurnIndicator(hdc, g_PlayerTurn, RGB(251, 206, 129));
			if((g_GameState & GAMEINTERMISSION) == GAMEINTERMISSION)
				DrawTurnIndicator(hdc, g_PlayerTurn, RGB(104, 224, 101));
			
			//Display all our scoreboard information.
			SetBkMode(hdc, TRANSPARENT);
			SetTextColor(hdc, RGB(255, 255, 255));
			
			//games played
			char szStat[10];

			itoa(g_GamesPlayed, szStat, 10);
			TextOut(hdc, 235, 25, (LPCTSTR)szStat, strlen(szStat));
			ZeroMemory((LPVOID)szStat, sizeof(szStat));

			//red player stats
			//wins
			itoa(RedPlayer.GameStats.Wins, szStat, 10);
			TextOut(hdc, 335, 235, (LPCTSTR)szStat, strlen(szStat));
			ZeroMemory((LPVOID)szStat, sizeof(szStat));

			//losses
			itoa(RedPlayer.GameStats.Losses, szStat, 10);
			TextOut(hdc, 335, 272, (LPCTSTR)szStat, strlen(szStat));
			ZeroMemory((LPVOID)szStat, sizeof(szStat));

			//blue player stats
			//wins
			itoa(BluePlayer.GameStats.Wins, szStat, 10);
			TextOut(hdc, 393, 235, (LPCTSTR)szStat, strlen(szStat));
			ZeroMemory((LPVOID)szStat, sizeof(szStat));

			//losses
			itoa(BluePlayer.GameStats.Losses, szStat, 10);
			TextOut(hdc, 393, 272, (LPCTSTR)szStat, strlen(szStat));
			ZeroMemory((LPVOID)szStat, sizeof(szStat));
			
			//clear out & delete our memory dc.
			SelectObject(hdcMem, hbmOld);
			DeleteDC(hdcMem);

			//release window dc.
			EndPaint(hwnd, &ps);
		}
		break;

		case WM_RBUTTONUP:
		{
			POINT pnt;
			pnt.x = LOWORD(lParam);
			pnt.y = HIWORD(lParam);
			ClientToScreen(hwnd, &pnt);

			TrackPopupMenu(hMenu, TPM_RIGHTBUTTON, pnt.x, pnt.y, 0, hwnd, NULL);

			return 0;
		}
		break;

		case WM_COMMAND:
			switch(LOWORD(wParam))
			{
				case ID_OPTIONS_NEWGAME:
					conFour->StartNewGame();
				break;

				case ID_OPTIONS_SOUNDFX:
				{
					static bool IsUnChecked;
					DWORD chkState;
					
					chkState = (IsUnChecked) ? MF_CHECKED : MF_UNCHECKED;
					IsUnChecked = !IsUnChecked;
					CheckMenuItem(hMenu, ID_OPTIONS_SOUNDFX, chkState);
				}
				break;

				case ID_OPTIONS_ABOUT:
					DialogBox(	HINSTANCE(GetModuleHandle("Con4.dll")),
								MAKEINTRESOURCE(IDD_DLGABOUT),
								hwnd,
								AboutDlgProc);
				break;

				case ID_OPTIONS_ENDGAME:
					conFour->Fire_LeaveGroup();
				break;
			}
		break;

		case WM_CLOSE:
			DestroyWindow(hwnd);
			return 0;
		break;

		case WM_DESTROY:
			gameHwnd = NULL;
			parenthWnd = NULL;
			DeleteObject(g_hbmGameBoard); g_hbmGameBoard = NULL;
			PostQuitMessage(0);
			return 0;
		break;

        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

LRESULT CALLBACK SlotWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static HRGN hRgnClip;
	static HRGN hRgnClip2;
	static HRGN hRgnClip3;
	static HBITMAP hbmChip;
	static HBITMAP hbmChipMask;

	switch(msg)
	{
		case WM_CREATE:
		{
			//**** Create our clipping Region ****//
			HRGN hTempRgns[6];
			HRGN hTempCmbRgns[4];

			int y = 0;
			int i = 0;

			for(i = 0; i < 6; i++)
			{
				hTempRgns[i] = CreateEllipticRgn(0, y, 34, y+34);
				y += 34 + SLOT_VSPACE;
			}

			hTempCmbRgns[0] = CreateRectRgn(0, 0, 1, 1);
			hTempCmbRgns[1] = CreateRectRgn(0, 0, 1, 1);
			hTempCmbRgns[2] = CreateRectRgn(0, 0, 1, 1);
			hTempCmbRgns[3] = CreateRectRgn(0, 0, 1, 1);
			hRgnClip = CreateRectRgn(0, 0, 1, 1);

			CombineRgn(hTempCmbRgns[0], hTempRgns[0], hTempRgns[1], RGN_OR);
			CombineRgn(hTempCmbRgns[1], hTempRgns[2], hTempRgns[3], RGN_OR);
			CombineRgn(hTempCmbRgns[2], hTempRgns[4], hTempRgns[5], RGN_OR);
			CombineRgn(hTempCmbRgns[3], hTempCmbRgns[0], hTempCmbRgns[1], RGN_OR);
			CombineRgn(hRgnClip, hTempCmbRgns[3], hTempCmbRgns[2], RGN_OR);

			for(i = 0; i < 6; i++)
				DeleteObject(hTempRgns[i]);
			for(i = 0; i < 4; i++)
				DeleteObject(hTempCmbRgns[i]);
			
			//**** grab handles to all our bitmap resources ****//
			if(g_hbmSlot == NULL)
			{
				g_hbmSlot = LoadBitmap(GetModuleHandle("Con4.dll"), 
					MAKEINTRESOURCE(IDB_SLOT));
				if(g_hbmSlot == NULL)
					ShowLastError("LoadBitmap Slot");
			}

			if(g_hbmRedChip == NULL)
			{
				g_hbmRedChip = LoadBitmap(GetModuleHandle("Con4.dll"), 
					MAKEINTRESOURCE(IDB_CHIP1));
				if(g_hbmRedChip == NULL)
					ShowLastError("LoadBitmap Chip1");
			}

			if(g_hbmRedChipMask == NULL)
			{
				g_hbmRedChipMask = CreateBitmapMask(g_hbmRedChip, RGB(214, 214, 0));
				if(g_hbmRedChipMask == NULL)
					ShowLastError("LoadBitmap chip1 mask");
			}

			if(g_hbmBlueChip == NULL)
			{
				g_hbmBlueChip = LoadBitmap(GetModuleHandle("Con4.dll"), 
					MAKEINTRESOURCE(IDB_CHIP2));
				if(g_hbmBlueChip == NULL)
					ShowLastError("Loadbitmap Chip2");
			}

			if(g_hbmBlueChipMask == NULL)
			{
				g_hbmBlueChipMask = CreateBitmapMask(g_hbmBlueChip, RGB(214, 214, 0));
				if(g_hbmBlueChipMask == NULL)
					ShowLastError("LoadBitmap chip2 mask");
			}

			if(g_hbmGreenChip == NULL)
			{
				g_hbmGreenChip = LoadBitmap(GetModuleHandle("Con4.dll"), 
					MAKEINTRESOURCE(IDB_GREENCHIP));
				if(g_hbmGreenChip == NULL)
					ShowLastError("Loadbitmap green chip");
			}

			if(g_hbmGreenChipMask == NULL)
			{
				g_hbmGreenChipMask = CreateBitmapMask(g_hbmGreenChip, RGB(214, 214, 0));
				if(g_hbmGreenChipMask == NULL)
					ShowLastError("LoadBitmap green chip mask");
			}

			return 0;
		}
		break;

		case WM_PAINT:
		{
			BITMAP bm;
			PAINTSTRUCT ps;
			HBITMAP hbmOld;

			RECT updateRgn;
			GetUpdateRect(hwnd, &updateRgn, FALSE);

			//grab the dc of our window
			HDC hdc = BeginPaint(hwnd, &ps);

			//create a compatible memory dc from our windows dc
			HDC hdcMem = CreateCompatibleDC(hdc);

			//select our bitmap into the memory dc
			hbmOld = (HBITMAP)SelectObject(hdcMem, g_hbmSlot);
			//get object info for g_hbmGameBoard, and place it in our bm struct
			GetObject(g_hbmSlot, sizeof(bm), &bm);
			//draw the bitmap to the window
			BitBlt(hdc, 0, 0, bm.bmWidth, bm.bmHeight, hdcMem, 0, 0, SRCCOPY);
			
			SLOTWNDINFO *lpSlotWndInfo = 
				(SLOTWNDINFO*)GetWindowLong(hwnd, GWL_USERDATA);
			SHORT chipCnt = lpSlotWndInfo->chipCnt;

			//if we are currently running the animation for the last set chip
			//decrement the chip count so that all chips previously added show only.
			if(lpSlotWndInfo->IsPlacingChip == true)
				chipCnt--;

			DrawAllChips(hdc, hdcMem, g_hbmRedChipMask, REDCHIP, lpSlotWndInfo, SRCAND);
			DrawAllChips(hdc, hdcMem, g_hbmRedChip, REDCHIP, lpSlotWndInfo, SRCPAINT);
			DrawAllChips(hdc, hdcMem, g_hbmBlueChipMask, BLUECHIP, lpSlotWndInfo, SRCAND);
			DrawAllChips(hdc, hdcMem, g_hbmBlueChip, BLUECHIP, lpSlotWndInfo, SRCPAINT);

			SelectObject(hdcMem, hbmOld);
			//clear memory dc
			DeleteDC(hdcMem);
			
			//release window dc.
			EndPaint(hwnd, &ps);
		}
		break;

		case WM_TIMER:
		{
			//grab a pointer to the window info struct associatted with the window
			SLOTWNDINFO *lpSlotWndInfo = 
				(SLOTWNDINFO*)GetWindowLong(hwnd, GWL_USERDATA);

			INT landingPos = (SLOT_HEIGHT - 34) - (36 * (lpSlotWndInfo->chipCnt -1));

			//check to see if the slot has reached its ending position
			if(chipInfo.y >= landingPos)
			{
				chipInfo.y = -34;	//reset chipinfo structure
				
				hbmChip = NULL;
				hbmChipMask = NULL;
				DeleteObject(hRgnClip2);
				DeleteObject(hRgnClip3);
				KillTimer(hwnd, lpSlotWndInfo->Index+1);
				lpSlotWndInfo->IsPlacingChip = false;
				g_IsPlacingChip = false;
				
				//only send the ChipPlacement packet if it is IPlayers chip that is
				//being placed, otherwise its the opponents chip.
				if((UINT)g_PlayerTurn == (UINT)IPlayer->PlayerType)
				{
					//initialize our structures
					size_t PACKETINFOBUF = sizeof(PACKETINFO);
					size_t PLACECHIPINFOBUF = sizeof(PLACECHIPINFO);
					size_t OVERALLBUF = PACKETINFOBUF+PLACECHIPINFOBUF;

					PACKETINFO *lpPacketInfo = (PACKETINFO*)malloc(PACKETINFOBUF);
					PLACECHIPINFO *lpPlaceChip = (PLACECHIPINFO*)malloc(PLACECHIPINFOBUF);
					char *szGameDataA = new char[OVERALLBUF];
					BSTR szGameData = NULL;

					lpPacketInfo->GameKey = conFour->m_GameKey;
					lpPacketInfo->PacketType = 3;

					SHORT Y = (GAME_ROWS-1) - (lpSlotWndInfo->chipCnt-1);

					lpPlaceChip->ChipType = lpSlotWndInfo->chipLocState[Y];
					lpPlaceChip->SlotX = lpSlotWndInfo->Index; 

					//serialize the structures to ascii string
					CopyMemory(	(LPVOID)szGameDataA,
								(LPVOID)lpPacketInfo,
								PACKETINFOBUF);

					CopyMemory(	(LPVOID)(szGameDataA+PACKETINFOBUF),
								(LPVOID)lpPlaceChip,
								PLACECHIPINFOBUF);
					
					//convert the binary ascii string to binary BSTR
					BinAsciiToBinBSTR(szGameDataA, OVERALLBUF, &szGameData);

					//issue the data to be sent
					conFour->Fire_Send(	conFour->m_GameSvrId, 
										0, 
										SysAllocString(L""), 
										&szGameData);

					//FREE MEMORY !!
					SysFreeString(szGameData);
					free((LPVOID)lpPacketInfo);
					free((LPVOID)lpPlaceChip);
					delete szGameDataA;
				}

				//check for a connect four.
				g_lpConFourVecs = CheckConnect4(lpSlotWndInfo->Index,
															g_SlotWndInfo);
				if(g_lpConFourVecs != NULL)
				{
					if(g_PlayerTurn == REDPLAYER)
					{
						g_GameState = (GAMEINTERMISSION | REDCONFOUR);
						RedPlayer.GameStats.Wins++;
						BluePlayer.GameStats.Losses++;
					}
					else if(g_PlayerTurn == BLUEPLAYER)
					{
						g_GameState = (GAMEINTERMISSION | BLUECONFOUR);
						BluePlayer.GameStats.Wins++;
						RedPlayer.GameStats.Losses++;
					}
					
					g_GamesPlayed++;

					RECT turnRect;
					SetRect(&turnRect, 200, 0, 500, 350);
					InvalidateRect(gameHwnd, &turnRect,  FALSE);
					//DrawTurnIndicator(gameHwnd, g_PlayerTurn, RGB(104, 224, 101));

					SetTimer(hwnd, lpSlotWndInfo->Index+10, 500, ConnectFourProc);
				}
				else
				{
					//switch turns and redraw the turn indicator
					g_PlayerTurn = (g_PlayerTurn == REDPLAYER) ? BLUEPLAYER : REDPLAYER;
					RECT turnRect;
					SetRect(&turnRect, 300, 0, 500, 350);
					InvalidateRect(gameHwnd, &turnRect,  FALSE);
				}
			}
			else
			{
				BITMAP bm;
				HBITMAP hbmOld;

				HDC hdc = GetDC(hwnd);
				HDC hdcMem = CreateCompatibleDC(hdc);

				//update sliding chips coordinates accordingly (make sure it doesn't
				//go over the postion its suppose to land on.
				if((chipInfo.y + chipInfo.dy) >= landingPos)
				{
					chipInfo.y += chipInfo.dy - ((chipInfo.y + chipInfo.dy) - landingPos);
					
					//reset hRgnClip3
					DeleteObject(hRgnClip3);

					hRgnClip3 = CreateRectRgn(	0, 
												landingPos, 
												SLOT_WIDTH, 
												landingPos + 36);
					SelectClipRgn(hdc, hRgnClip3);
				}
				else
				{
					chipInfo.y += chipInfo.dy;
					//if we have previously set chips there regions will be clipped from
					//the painting area so we don't have to repaint them. this will set
					//our update area accordingly
					if(lpSlotWndInfo->chipCnt > 1)
						SelectClipRgn(hdc, hRgnClip2);
					else
						SelectClipRgn(hdc, hRgnClip);
				}

				hbmOld = (HBITMAP)SelectObject(hdcMem, g_hbmSlot);
				GetObject(g_hbmSlot, sizeof(bm), &bm);
				BitBlt(hdc, 0, 0, bm.bmWidth, bm.bmHeight, hdcMem, 0, 0, SRCCOPY);

				SelectObject(hdcMem, hbmChipMask);
				GetObject(hbmChipMask, sizeof(bm), &bm);
				BitBlt(hdc, 0, chipInfo.y, bm.bmWidth, bm.bmHeight, hdcMem, 0, 0, SRCAND);

				SelectObject(hdcMem, hbmChip);
				GetObject(hbmChip, sizeof(bm), &bm);
				BitBlt(hdc, 0, chipInfo.y, bm.bmWidth, bm.bmHeight, hdcMem, 0, 0, SRCPAINT);

				SelectObject(hdcMem, hbmOld);
				DeleteDC(hdcMem);
				
				ReleaseDC(hwnd, hdc);
			}
		}
		break;

		case WM_LBUTTONDOWN:
		{
			SLOTWNDINFO *lpSlotWndInfo = 
				(SLOTWNDINFO*)GetWindowLong(hwnd, GWL_USERDATA);
			
			//first check if the game is in progress.
			if(g_GameState != GAMEINPROGRESS)
				break;
			
			//now make sure it is the players turn
			//or look for the special signal in wParam
			//also to be safe make sure the sliding chip ani. isn't in progress atm.
			if(((UINT)g_PlayerTurn == (UINT)IPlayer->PlayerType && 
										g_IsPlacingChip == false) |
										wParam == 255)
			{
				//make sure the slot isn't already filled up
				if(lpSlotWndInfo->chipCnt < GAME_ROWS)
				{
					SHORT chipCnt = lpSlotWndInfo->chipCnt++;
					SHORT Y = (GAME_ROWS-1) - chipCnt;
					
					if(g_PlayerTurn == REDPLAYER)
					{
						lpSlotWndInfo->chipLocState[Y] = REDCHIP;
						hbmChip = g_hbmRedChip;
						hbmChipMask = g_hbmRedChipMask;
					}
					else if(g_PlayerTurn == BLUEPLAYER)
					{
						lpSlotWndInfo->chipLocState[Y] = BLUECHIP;
						hbmChip = g_hbmBlueChip;
						hbmChipMask = g_hbmBlueChipMask;
					}
		
					//if we are going to run an animation, and we have previously
					//stacked up chips set our regions accordingly so our already
					//set chips don't get painted over.
					if(lpSlotWndInfo->chipCnt > 1)
					{
						hRgnClip3 = CreateRectRgn(	0, 
												(SLOT_HEIGHT - 34) - (36 * (lpSlotWndInfo->chipCnt -2)), 
												SLOT_WIDTH, 
												SLOT_HEIGHT);

						//reset hRgnClip2
						if(hRgnClip2)
							DeleteObject(hRgnClip2);

						hRgnClip2 = CreateRectRgn(0, 0, 1, 1);
						CombineRgn(hRgnClip2, hRgnClip, hRgnClip3, RGN_DIFF);
					}
					
					UINT ret = SetTimer(hwnd, lpSlotWndInfo->Index+1, 10, NULL);
					
					if(ret == 0)
						ShowLastError("SetTimer");
					else
					{
						lpSlotWndInfo->IsPlacingChip = true;
						g_IsPlacingChip = true;
					}
				}
			}
		}
		break;

		case WM_CLOSE:
			DestroyWindow(hwnd);
		break;

		case WM_DESTROY:
			DeleteObject(g_hbmSlot); g_hbmSlot = NULL;
			DeleteObject(g_hbmRedChip); g_hbmRedChip = NULL;
			DeleteObject(g_hbmRedChipMask); g_hbmRedChipMask = NULL;
			DeleteObject(g_hbmBlueChip); g_hbmBlueChip = NULL;
			DeleteObject(g_hbmBlueChipMask); g_hbmBlueChipMask = NULL;
			DeleteObject(g_hbmGreenChip); g_hbmGreenChip = NULL;
			DeleteObject(g_hbmGreenChipMask); g_hbmGreenChipMask = NULL;
			PostQuitMessage(0);
		break;

		default:
			return DefWindowProc(hwnd, msg, wParam, lParam);
	}
	return 0;
}

void CALLBACK ConnectFourProc(HWND hwnd, UINT uMsg, UINT uptrVecs, DWORD dwTime)
{
	static CHIPTYPE lastChip;

	if((g_GameState & GAMEINTERMISSION) == GAMEINTERMISSION)
	{
		HBITMAP hbmChip = NULL;
		HBITMAP hbmChipMask = NULL;

		if(lastChip == GREENCHIP)
		{	
			if((g_GameState & REDCONFOUR) == REDCONFOUR)
			{
				lastChip = REDCHIP;
				hbmChip = g_hbmRedChip;
				hbmChipMask = g_hbmRedChipMask;
			}
			else if((g_GameState & BLUECONFOUR) == BLUECONFOUR)
			{
				lastChip = BLUECHIP;
				hbmChip = g_hbmBlueChip;
				hbmChipMask = g_hbmBlueChipMask;
			}
		}
		else
		{
			lastChip = GREENCHIP;
			hbmChip = g_hbmGreenChip;
			hbmChipMask = g_hbmGreenChipMask;
		}

		HighLightConnectFour(	hbmChip, 
								hbmChipMask,
								g_lpConFourVecs, 
								g_SlotWndInfo);
	}
	else
	{
		lastChip = NOCHIP;
		KillTimer(hwnd, uptrVecs);

		if(g_lpConFourVecs != NULL)
		{
			delete g_lpConFourVecs;
			g_lpConFourVecs = NULL;
		}
	}
}

BOOL CALLBACK AboutDlgProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{
		case WM_INITDIALOG:
			return TRUE;
		break;

		case WM_COMMAND:
			switch(LOWORD(wParam))
			{
				case IDOK:
					EndDialog(hDlg, 0);
					return TRUE;
				break;
			}
		break;

		case WM_CLOSE:
			EndDialog(hDlg, 0);
			return TRUE;
		break;
	}

	return FALSE;
}

//********* End of CConnectFourGame Class Implementation **********//

//********* CConnectFourFactory Class Implementation **********//

//Constructors/Destructor

CConnectFourFactory::CConnectFourFactory() : m_refCount(0)
{
}

CConnectFourFactory::~CConnectFourFactory()
{
}

//**** IUnknown Interface Method Implementation *****//
 
STDMETHODIMP_(ULONG) CConnectFourFactory::AddRef(void)
{
	//++globalCount;
    return ++m_refCount;
}

STDMETHODIMP_(ULONG) CConnectFourFactory::Release(void)
{
	//--globalCount;
    ULONG ret(--m_refCount);
    if(!ret) delete this;
    return ret;
}

STDMETHODIMP CConnectFourFactory::QueryInterface(REFIID riid, void **ppv)
{
	if(!ppv) 
		return E_POINTER;
    if(riid == IID_IUnknown) 
		*ppv = static_cast<IUnknown*>(this);
    else if(riid == IID_IClassFactory) 
		*ppv = static_cast<IClassFactory*>(this);
    else 
		return *ppv = NULL, E_NOINTERFACE;

	((IUnknown*)*ppv)->AddRef();

    return S_OK;
}

//***** IClassFactory Interface Method Implementation *****//

STDMETHODIMP CConnectFourFactory::CreateInstance(IUnknown* pUnk, REFIID riid, void** ppv) 
{
    if(pUnk) 
		return CLASS_E_NOAGGREGATION;

    conFour = new CConnectFourGame;
	if(conFour == NULL)
		return E_OUTOFMEMORY;

    HRESULT hr = conFour->QueryInterface(riid, ppv);
    
	if(FAILED(hr))
		delete conFour;
	
    return hr;
}

STDMETHODIMP CConnectFourFactory::LockServer(BOOL lock) 
{ 
	return S_OK; 
}