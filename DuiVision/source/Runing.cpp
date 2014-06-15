#include "StdAfx.h"
#include "Runing.h"

CRuning::CRuning(HWND hWnd, CDuiObject* pDuiObject)
	: CControlBaseFont(hWnd, pDuiObject)
{
	m_bRunTime = false;
	m_nIndex = 0;
	m_nMaxIndex = 10;
	m_nCount = 0;
	m_nTimerCount = 5;
	SetBitmapCount(m_nMaxIndex);
}

CRuning::CRuning(HWND hWnd, CDuiObject* pDuiObject, UINT uControlID, CRect rc, 
				 UINT uAlignment/* = DT_CENTER*/, UINT uVAlignment/* = DT_VCENTER*/, BOOL bIsVisible/* = TRUE*/, BOOL bIsDisable/* = FALSE*/)
	: CControlBaseFont(hWnd, pDuiObject, uControlID, rc, TEXT(""), bIsVisible, bIsDisable)
{
	m_bRunTime = false;
	m_nIndex = 0;
	m_nMaxIndex = 10;
	m_nCount = 0;
	m_nTimerCount = 5;
	SetBitmapCount(m_nMaxIndex);
}

CRuning::~CRuning(void)
{
}

// 从XML设置图片个数属性
HRESULT CRuning::OnAttributeMaxIndex(const CStringA& strValue, BOOL bLoading)
{
	if (strValue.IsEmpty() || (m_pImage == NULL)) return E_FAIL;

	m_nMaxIndex = atoi(strValue);
	SetBitmapCount(m_nMaxIndex);

	m_sizeImage.SetSize(m_pImage->GetWidth() / m_nImagePicCount, m_pImage->GetHeight());
	UpdateControl(true);

	return bLoading?S_FALSE:S_OK;
}

// 从XML设置动画属性
HRESULT CRuning::OnAttributeRun(const CStringA& strValue, BOOL bLoading)
{
	if (strValue.IsEmpty()) return E_FAIL;

	BOOL bRun = (strValue == "true");
	SetRun(bRun);

	return bLoading?S_FALSE:S_OK;
}

BOOL CRuning::SetRun(BOOL bRun, int nIndex/* = -1*/)
{
	if(GetDisable())
	{
		return FALSE;
	}

	BOOL bOldRunTime = m_bRunTime;
	int nOldIndex = m_nIndex;
	m_bRunTime = bRun;

	if(nIndex >= 0 && nIndex < m_nMaxIndex)
	{
		m_nIndex = nIndex;
	}

	if(GetVisible() && ((bOldRunTime != m_bRunTime) || (nOldIndex != m_nIndex)))
	{
		UpdateControl();
	}

	return bOldRunTime;
}

BOOL CRuning::OnControlTimer()
{
	if(!m_bRunTime || !m_bIsVisible)
	{
		return FALSE;
	}

	if(++m_nCount >= m_nTimerCount)
	{
		m_nCount = 0;
		if(++m_nIndex >= m_nMaxIndex)
		{
			m_nIndex = 0;
		}
		
		UpdateControl();
		return true;
	}

	return false;
}

void CRuning::DrawControl(CDC &dc, CRect rcUpdate)
{
	int nWidth = m_rc.Width();
	int nHeight = m_rc.Height();

	if(!m_bUpdate)
	{
		UpdateMemDC(dc, nWidth * m_nMaxIndex, nHeight);

		Graphics graphics(m_memDC);
		

		CPoint point = GetOriginPoint(nWidth, nHeight, m_sizeImage.cx, m_sizeImage.cy, m_uAlignment, m_uVAlignment);

		for(int i = 0; i < m_nMaxIndex; i++)
		{
			m_memDC.BitBlt(i * nWidth, 0, nWidth, nHeight, &dc, m_rc.left ,m_rc.top, SRCCOPY);

			graphics.DrawImage(m_pImage, Rect(point.x , point.y,  m_sizeImage.cx, m_sizeImage.cy),
				i * m_sizeImage.cx, 0, m_sizeImage.cx, m_sizeImage.cy, UnitPixel);

			point.x += nWidth;
		}
	}

	dc.BitBlt(m_rc.left,m_rc.top, m_rc.Width(), m_rc.Height(), &m_memDC, m_nIndex * nWidth, 0, SRCCOPY);
}