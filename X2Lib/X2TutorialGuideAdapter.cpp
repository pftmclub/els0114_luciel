#include "StdAfx.h"

#ifdef SERV_IRUHADEV_JUNE_TUTORIAL_GUIDE_GFX
#include "X2TutorialGuideAdapter.h"
#include "Offline/X2OfflineLog.h"

// june-dungeons C3z-4t2: log tag JUNE_FX C3z-4t ( offline_server.log ); every function names its June address.

CX2TutorialGuideAdapter::CX2TutorialGuideAdapter( CX2TutorialGuideManager* pTutorialGuideManager, CKTDGGFXManager* pGfxManager ) :
m_pTutorialGuideManager( pTutorialGuideManager ),
m_pBridge( NULL )
{
	m_pBridge = new CKTDGGFXTutorialGuideBridge( pGfxManager, this );	// June 32965bd0: no bindings without the root movie's adapter
	if( false == m_pBridge->IsBound() )
		CX2OfflineLog::Server( L"JUNE_FX C3z-4t gfx adapter unbound: no GFx root movie, the guide movie stays off (JUNE_GFX MGR lines in ErrorLog.txt)" );
}

CX2TutorialGuideAdapter::~CX2TutorialGuideAdapter()
{
	SAFE_DELETE( m_pBridge );	// June 329659e0
}

bool CX2TutorialGuideAdapter::IsBound() const
{
	return NULL != m_pBridge && true == m_pBridge->IsBound();
}

void CX2TutorialGuideAdapter::ShowGuide( int iGFxUINo )
{
	if( NULL == m_pBridge )
		return;
	m_pBridge->ShowGuide( iGFxUINo );	// June 32965a60
#ifdef SERV_IRUHADEV_JUNE_TUTORIAL_GUIDE_DEBUG
	CX2OfflineLog::Server( L"JUNE_FX C3z-4t gfx show %d: %d observer(s) on tutorialGuide.showGuide", iGFxUINo, m_pBridge->GetShowGuideObserverCount() );
#endif SERV_IRUHADEV_JUNE_TUTORIAL_GUIDE_DEBUG
	if( true == m_pBridge->IsBound() && 0 == m_pBridge->GetShowGuideObserverCount() )
		CX2OfflineLog::Server( L"JUNE_FX C3z-4t gfx show %d not seen: tutorial_guide.gfx never bound tutorialGuide.showGuide (data194 widget)", iGFxUINo );
}

void CX2TutorialGuideAdapter::HideGuide()
{
	if( NULL == m_pBridge )
		return;
	m_pBridge->HideGuide();		// June 329659b0
#ifdef SERV_IRUHADEV_JUNE_TUTORIAL_GUIDE_DEBUG
	CX2OfflineLog::Server( L"JUNE_FX C3z-4t gfx hide" );
#endif SERV_IRUHADEV_JUNE_TUTORIAL_GUIDE_DEBUG
}

void CX2TutorialGuideAdapter::OnTutorialGuideAsCallback()
{
	// June 32965a90 answers the call and drops its function object, so nothing else runs
#ifdef SERV_IRUHADEV_JUNE_TUTORIAL_GUIDE_DEBUG
	CX2OfflineLog::Server( L"JUNE_FX C3z-4t gfx callback tutorialGudie" );
#endif SERV_IRUHADEV_JUNE_TUTORIAL_GUIDE_DEBUG
}
#endif SERV_IRUHADEV_JUNE_TUTORIAL_GUIDE_GFX
