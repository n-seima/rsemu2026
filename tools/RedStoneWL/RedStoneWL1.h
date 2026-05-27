// RedStoneWL1.h : Declaration of the CRedStoneWL

#ifndef __REDSTONEWL_H_
#define __REDSTONEWL_H_

#include "resource.h"       // main symbols
#include <atlctl.h>


/////////////////////////////////////////////////////////////////////////////
// CRedStoneWL
class ATL_NO_VTABLE CRedStoneWL : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public IDispatchImpl<IRedStoneWL, &IID_IRedStoneWL, &LIBID_REDSTONEWLLib>,
	public CComControl<CRedStoneWL>,
	public IPersistStreamInitImpl<CRedStoneWL>,
	public IOleControlImpl<CRedStoneWL>,
	public IOleObjectImpl<CRedStoneWL>,
	public IOleInPlaceActiveObjectImpl<CRedStoneWL>,
	public IViewObjectExImpl<CRedStoneWL>,
	public IOleInPlaceObjectWindowlessImpl<CRedStoneWL>,
	public ISupportErrorInfo,
	public IConnectionPointContainerImpl<CRedStoneWL>,
	public IPersistStorageImpl<CRedStoneWL>,
	public ISpecifyPropertyPagesImpl<CRedStoneWL>,
	public IQuickActivateImpl<CRedStoneWL>,
	public IDataObjectImpl<CRedStoneWL>,
	public IProvideClassInfo2Impl<&CLSID_RedStoneWL, &DIID__IRedStoneWLEvents, &LIBID_REDSTONEWLLib>,
	public IPropertyNotifySinkCP<CRedStoneWL>,
	public CComCoClass<CRedStoneWL, &CLSID_RedStoneWL>,
	public IObjectSafetyImpl<CRedStoneWL,INTERFACESAFE_FOR_UNTRUSTED_CALLER|INTERFACESAFE_FOR_UNTRUSTED_DATA>
{
public:
	CRedStoneWL()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_REDSTONEWL)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CRedStoneWL)
	COM_INTERFACE_ENTRY(IRedStoneWL)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IViewObjectEx)
	COM_INTERFACE_ENTRY(IViewObject2)
	COM_INTERFACE_ENTRY(IViewObject)
	COM_INTERFACE_ENTRY(IOleInPlaceObjectWindowless)
	COM_INTERFACE_ENTRY(IOleInPlaceObject)
	COM_INTERFACE_ENTRY2(IOleWindow, IOleInPlaceObjectWindowless)
	COM_INTERFACE_ENTRY(IOleInPlaceActiveObject)
	COM_INTERFACE_ENTRY(IOleControl)
	COM_INTERFACE_ENTRY(IOleObject)
	COM_INTERFACE_ENTRY(IPersistStreamInit)
	COM_INTERFACE_ENTRY2(IPersist, IPersistStreamInit)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY(IConnectionPointContainer)
	COM_INTERFACE_ENTRY(ISpecifyPropertyPages)
	COM_INTERFACE_ENTRY(IQuickActivate)
	COM_INTERFACE_ENTRY(IPersistStorage)
	COM_INTERFACE_ENTRY(IDataObject)
	COM_INTERFACE_ENTRY(IProvideClassInfo)
	COM_INTERFACE_ENTRY(IProvideClassInfo2)
	COM_INTERFACE_ENTRY(IObjectSafety)
END_COM_MAP()

BEGIN_PROP_MAP(CRedStoneWL)
	PROP_DATA_ENTRY("_cx", m_sizeExtent.cx, VT_UI4)
	PROP_DATA_ENTRY("_cy", m_sizeExtent.cy, VT_UI4)
	// Example entries
	// PROP_ENTRY("Property Description", dispid, clsid)
	// PROP_PAGE(CLSID_StockColorPage)
END_PROP_MAP()

BEGIN_CONNECTION_POINT_MAP(CRedStoneWL)
	CONNECTION_POINT_ENTRY(IID_IPropertyNotifySink)
END_CONNECTION_POINT_MAP()

BEGIN_MSG_MAP(CRedStoneWL)
	CHAIN_MSG_MAP(CComControl<CRedStoneWL>)
	DEFAULT_REFLECTION_HANDLER()
END_MSG_MAP()
// Handler prototypes:
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);



// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid)
	{
		static const IID* arr[] = 
		{
			&IID_IRedStoneWL,
		};
		for (int i=0; i<sizeof(arr)/sizeof(arr[0]); i++)
		{
			if (InlineIsEqualGUID(*arr[i], riid))
				return S_OK;
		}
		return S_FALSE;
	}

// IViewObjectEx
	DECLARE_VIEW_STATUS(VIEWSTATUS_SOLIDBKGND | VIEWSTATUS_OPAQUE)

// IRedStoneWL
public:
	CComBSTR m_bstrPassword;
	STDMETHOD(get_Password)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_Password)(/*[in]*/ BSTR newVal);
	int m_iPortalID;
	STDMETHOD(get_PortalID)(/*[out, retval]*/ long *pVal);
	STDMETHOD(put_PortalID)(/*[in]*/ long newVal);
	CComBSTR m_bstrUrlPath;
	STDMETHOD(get_UrlPath)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_UrlPath)(/*[in]*/ BSTR newVal);
	STDMETHOD(Run)();
	CComBSTR m_bstrId;
	CComBSTR m_bstrText;
	STDMETHOD(get_getID)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_getID)(/*[in]*/ BSTR newVal);

	HRESULT OnDraw(ATL_DRAWINFO& di)
	{
		RECT& rc = *(RECT*)di.prcBounds;
		Rectangle(di.hdcDraw, rc.left, rc.top, rc.right, rc.bottom);

		SetTextAlign(di.hdcDraw, TA_CENTER|TA_BASELINE);
		LPCTSTR pszText = _T("ATL 3.0 : RedStoneWL");
		TextOut(di.hdcDraw, 
			(rc.left + rc.right) / 2, 
			(rc.top + rc.bottom) / 2, 
			pszText, 
			lstrlen(pszText));

		return S_OK;
	}
};

enum
{
	ePI_RedStone,
	ePI_Yahoo,
	ePI_Paran,
	ePI_AnycallLand,
	ePI_PlayNet,
};
#endif //__REDSTONEWL_H_
