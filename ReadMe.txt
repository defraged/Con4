********* TODOs ************

-	watch for a draw/all slots are full.

-	when chips are blinking for connect four, set there chip state so if it has
	to repaint in the middle of that, it will repaint the chips 
	according to its current color state.
	
-	load/unload all of our bitmaps in gamewndproc, instead of in slotwndproc


**** Things todo to get game working in vp client ****
-	*learn some ASM and read the debug information you lamer !!!!!
-	implement the event interface
-	see if you can somehow get around using the clients IDispatch::Invoke method
	by passing in a void* to hold there event sink interface pointer, then using
	that as the instance with ITypeInfo::Invoke for a direct call.

X-	check excepinfo struct after invoke see if you can find any errors
X-	try your own idispatch interface with your own calling convetion
X-	serialize the interface pointer you get frome existing game
X-	possibly the vp client asks for IDispatch twice once for method interface
	once for event interface

******** Problem/Solution *******
-

******* code snippets *************
- for the CheckConnect4 function
	//char infoStr[300];
	//sprintf(infoStr, "overallrect.top: %d\n overallrect.left: %d\n overallrect.bottom: %d\n overallrect.right: %d\n\nposrect.top: %d\n posrect.left: %d\n posrect.bottom: %d\n posrect.right: %d\n\ncon4rect.top: %d\n con4rect.left: %d\n con4rect.bottom: %d\n con4rect.right: %d", 
	//	SlotsRect.top, SlotsRect.left, SlotsRect.bottom, SlotsRect.right, Con4RectBounds.top, Con4RectBounds.left, Con4RectBounds.bottom, Con4RectBounds.right, Con4Rect.top, Con4Rect.left, Con4Rect.bottom, Con4Rect.right);
	//::MessageBox(NULL, (LPCTSTR)infoStr, "", MB_OK);
	
- for StartGame method
	//::MessageBox(NULL, "back in startgame after getparentwnd event", "", MB_OK);
	//char szParentWnd[16];
	//ltoa(parenthWnd, szParentWnd, 10);
	//::MessageBox(NULL, szParentWnd, "hmmm", MB_OK);
	
- nested test class for connectfourgame class
	/*class ConnectionPointProxy : IUnknown
	{
		inline CConnectFourGame* This(void);

		//IUnknown implementation
		STDMETHOD(QueryInterface)(REFIID riid, void** ppv);
		STDMETHOD_(ULONG, AddRef)(void);
		STDMETHOD_(ULONG, Release)(void);
	} m_ConPntProxy;*/
	
//********* Temporary Event Sink Proxy Class ***************//
/*STDMETHODIMP_(ULONG) CConnectFourGame::ConnectionPointProxy::AddRef()
{
	return This()->AddRef();
}

STDMETHODIMP_(ULONG) CConnectFourGame::ConnectionPointProxy::Release()
{
	return This()->Release();
}

STDMETHODIMP CConnectFourGame::ConnectionPointProxy::QueryInterface(REFIID riid, void** ppv)
{
	::MessageBox(NULL, "interface set", "EventSnk Proxy", MB_OK);
	UINT typeInfoCnt;
	((IDispatch*)*ppv)->GetTypeInfoCount(&typeInfoCnt);
	if(typeInfoCnt == 1)
	::MessageBox(NULL, "Type info is available", "", MB_OK);
	
	return S_OK;
}

inline CConnectFourGame* CConnectFourGame::ConnectionPointProxy::This(void)
{
	return (CConnectFourGame*)((char*)this - offsetof(CConnectFourGame, m_ConPntProxy));
}*/
	
- drag drop stuff

		case WM_DROPFILES:
		{
			CHAR fileName[255];
			UINT fileNameLen = strlen(fileName);
			HDROP hfileDrop = (HDROP)wParam;
			
			//grab the number of files dropped onto the window
			UINT fileCnt = DragQueryFile(hfileDrop, 0xFFFFFFFF, NULL, 0);

			//display the name(s) of the file(s) dropped onto the window
			for(UINT i = 0; i < fileCnt; i++)
			{
				DragQueryFile(hfileDrop, i, (LPSTR)&fileName, fileNameLen);
				::MessageBox(NULL, (LPCTSTR)fileName, "", MB_OK);
			}

			//clear resources
			DragFinish(hfileDrop);
		}
		break;
		
************ Design Ideas ****************