#pragma once

#include <PlatformCore/Window/Window.hpp>
#include <Windows.h> 

#include <Ole2.h>
#include <oleidl.h>

namespace Fade { namespace PlatformCore {

class FADE_PLATFORMCORE_API CWindowsWindow : public CWindow, IDropTarget
{
public:
	CWindowsWindow();

	~CWindowsWindow() override;

	virtual bool Create(SWindowSettings& a_Settings, CWindow* a_Parent = nullptr) override;

	virtual bool Destroy() override;

	virtual void Show() override;

	virtual void Hide() override;

	virtual void BringToFront() override;

	virtual void Focus() override;

	virtual void Reshape(i32 a_X, i32 a_Y, i32 a_Width, i32 a_Height) override;

	virtual void Move(i32 a_X, i32 a_Y) override;

	virtual void Minimize() override;

	virtual void Maximize() override;

	virtual void* GetWindowHandle() const override { return m_WindowHandle; }

	virtual HRESULT __stdcall QueryInterface(REFIID iid, void ** ppvObject) override;
	virtual ULONG __stdcall AddRef(void) override;
	virtual ULONG __stdcall Release(void) override;

	virtual HRESULT DragEnter(IDataObject *pDataObj, DWORD grfKeyState, POINTL pt, DWORD *pdwEffect) override;
	virtual HRESULT DragLeave() override;
	virtual HRESULT DragOver(DWORD  grfKeyState, POINTL pt, DWORD *pdwEffect) override;
	virtual HRESULT Drop(IDataObject *pDataObj, DWORD grfKeyState, POINTL pt, DWORD *pdwEffect) override;

private:
	HWND m_WindowHandle;
	
	static const TCHAR sm_AppWindowClass[];
};

} }