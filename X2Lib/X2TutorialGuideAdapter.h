#pragma once

// june-dungeons C3z-4t2: June's CX2TutorialGuideAdapter ( vtable 334c9c60, 0xc B ), the tutorial guide manager's +0xb8; map june2017_re/c3z4t_scope.tsv.

#ifdef SERV_IRUHADEV_JUNE_TUTORIAL_GUIDE_GFX
#include "../KTDXLIB/KTDGGFXManager.h"

class CX2TutorialGuideManager;

class CX2TutorialGuideAdapter : public IGfxTutorialGuideHost
{
public:
	CX2TutorialGuideAdapter( CX2TutorialGuideManager* pTutorialGuideManager, CKTDGGFXManager* pGfxManager );	// June 32965bd0
	virtual ~CX2TutorialGuideAdapter();																		// June 329659e0

	bool	IsBound() const;					// June +8 != 0
	void	ShowGuide( int iGFxUINo );			// June 32965a60
	void	HideGuide();						// June 329659b0

	virtual void	OnTutorialGuideAsCallback();	// June 32965a90 ( CX2TutorialGuideAdapterCallbackHandler )

private:
	CX2TutorialGuideManager*		m_pTutorialGuideManager;	// June +4, no June reader
	CKTDGGFXTutorialGuideBridge*	m_pBridge;					// June +8
};
#endif SERV_IRUHADEV_JUNE_TUTORIAL_GUIDE_GFX
