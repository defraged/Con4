#include "stdafx.h"
#include "ConnectFour_i.c"

HINSTANCE hInstance;

void RegisterServer(wchar_t* widePath, bool reg) 
{
    ATL::CRegObject ro;
    ro.AddReplacement(L"Module", widePath);
    
	if(reg == true)
		ro.ResourceRegister(widePath, IDR_CONFOUR, L"REGISTRY");
	else
        ro.ResourceUnregister(widePath, IDR_CONFOUR, L"REGISTRY");
}

void GetPathName(wchar_t* widePath) 
{
    char ansiPath[MAX_PATH];
    GetModuleFileName(hInstance, ansiPath, MAX_PATH);
    MultiByteToWideChar(CP_ACP, 0, ansiPath, lstrlen(ansiPath) + 1, widePath, MAX_PATH);
}

STDMETHODIMP_(INT) DllMain(HINSTANCE hInstance, DWORD reason, void*)
{
	::hInstance = hInstance;
	return 1;
}

extern "C" STDMETHODIMP DllRegisterServer()
{
	wchar_t widePath[MAX_PATH];
    GetPathName(widePath);
    RegisterServer(widePath, true);
    CComPtr<ITypeLib> pTypeLib;
    HRESULT hr(LoadTypeLib(widePath, &pTypeLib));
    
	if(hr) 
		return SELFREG_E_TYPELIB;

    hr = RegisterTypeLib(pTypeLib, widePath, 0);

    if(hr) 
		return SELFREG_E_TYPELIB;

	return S_OK;
}

extern "C" STDMETHODIMP DllUnregisterServer()
{
	wchar_t widePath[MAX_PATH];
    GetPathName(widePath);
    RegisterServer(widePath, false);
    HRESULT hr(UnRegisterTypeLib(LIBID_ConnectFourLib, 1, 0, 0, SYS_WIN32));
    if(hr) return SELFREG_E_TYPELIB;
	return S_OK;
}

extern "C" STDMETHODIMP DllGetClassObject(REFCLSID rclsid, REFIID riid, void** ppv)
{
	if(rclsid != CLSID_ConnectFourGame)
		return *ppv = NULL, CLASS_E_CLASSNOTAVAILABLE;
	
	CConnectFourFactory* ConFourCLSFactory = new CConnectFourFactory;
	HRESULT hr = ConFourCLSFactory->QueryInterface(riid, ppv);

	if(hr) delete ConFourCLSFactory;
	return hr;
}

extern "C" STDMETHODIMP DllCanUnloadNow() 
{
	return S_FALSE;
}