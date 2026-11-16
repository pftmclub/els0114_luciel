#include "StdAfx.h"
#include ".\x2subequip.h"

CX2SubEquip::CX2SubEquip( D3DXMATRIX* pFrameMatrix, const wstring& modelName ) :
CKTDGObject( NULL ),
m_pFrameMatrix( pFrameMatrix )
{
	ASSERT( NULL != pFrameMatrix );
	m_pNormalXMesh = g_pKTDXApp->GetDeviceManager()->OpenXMesh( modelName );

	m_fScale = 1.f;
	m_vRotate = D3DXVECTOR3(0.f, 0.f, 0.f);
	m_vOffset = D3DXVECTOR3(0.f, 0.f, 0.f);

#ifdef ATTACH_SKINMESH
	m_pSkinAnim = NULL;
	m_pSkinModel = NULL;
	m_bSkinMesh = false;
#endif
#ifdef SERV_IRUHADEV_RANOX_ATTACH_SKIN_MESH
	m_bUVAnimGlow = false;
	m_fUVAnimTexOffset = 0.f;
#endif SERV_IRUHADEV_RANOX_ATTACH_SKIN_MESH
}

#ifdef ATTACH_SKINMESH
#ifdef SERV_IRUHADEV_JUNE_EQIP_LOAD_ATTACHED
CX2SubEquip::CX2SubEquip( D3DXMATRIX* pFrameMatrix, const wstring& modelName, bool bSkin, CKTDXDeviceXET* pModelTexChangeXET ) :
#else SERV_IRUHADEV_JUNE_EQIP_LOAD_ATTACHED
CX2SubEquip::CX2SubEquip( D3DXMATRIX* pFrameMatrix, const wstring& modelName, bool bSkin ) :
#endif SERV_IRUHADEV_JUNE_EQIP_LOAD_ATTACHED
CKTDGObject( NULL ),
m_pFrameMatrix( pFrameMatrix )
{
	ASSERT( NULL != pFrameMatrix );

	m_bSkinMesh = bSkin;

	m_pSkinAnim = NULL;
	m_pSkinModel = NULL;
	m_pNormalXMesh = NULL;	
	
	
	if( m_bSkinMesh == false || 
		'y' == modelName.back() || 
		'Y' == modelName.back() )
	{
		m_bSkinMesh = false;
		m_pNormalXMesh = g_pKTDXApp->GetDeviceManager()->OpenXMesh( modelName );	
	}
	else
	{
		m_pSkinModel = g_pKTDXApp->GetDeviceManager()->OpenXSkinMesh( modelName );
		if( m_pSkinModel != NULL )
		{
			m_pSkinAnim = CKTDGXSkinAnim::CreateSkinAnim();
			m_pSkinAnim->SetAnimXSkinMesh( m_pSkinModel );
#ifdef SERV_IRUHADEV_JUNE_EQIP_LOAD_ATTACHED
			m_pSkinAnim->AddModelXSkinMesh( m_pSkinModel, NULL, NULL, pModelTexChangeXET );	// june-engine R14f: June 32f09170 @32f092cd passes the equip's tex-change XET
#else SERV_IRUHADEV_JUNE_EQIP_LOAD_ATTACHED
			m_pSkinAnim->AddModelXSkinMesh( m_pSkinModel );
#endif SERV_IRUHADEV_JUNE_EQIP_LOAD_ATTACHED
			m_pSkinAnim->ChangeAnim( L"Normal", false );
			m_pSkinAnim->Play( CKTDGXSkinAnim::XAP_LOOP );
			m_pSkinAnim->UseDXMatrix( true );
		}		
	}

	m_fScale = 1.f;
	m_vRotate = D3DXVECTOR3(0.f, 0.f, 0.f);
	m_vOffset = D3DXVECTOR3(0.f, 0.f, 0.f);

#ifdef SERV_IRUHADEV_RANOX_ATTACH_SKIN_MESH
	m_bUVAnimGlow = false;
	m_fUVAnimTexOffset = 0.f;
#endif SERV_IRUHADEV_RANOX_ATTACH_SKIN_MESH
}
#endif

CX2SubEquip::~CX2SubEquip(void)
{
	SetManagerObject( NULL );

	SAFE_CLOSE( m_pNormalXMesh );
#ifdef ATTACH_SKINMESH
	SAFE_CLOSE( m_pSkinModel );
	SAFE_DELETE_KTDGOBJECT( m_pSkinAnim );
#endif
}

/*virtual*/ RENDER_HINT CX2SubEquip::OnFrameRender_Prepare()
{
#ifdef ATTACH_SKINMESH
	if( m_bSkinMesh == false )
	{
		if( NULL == m_pNormalXMesh )
			return RENDER_HINT_NORENDER;

		return RENDER_HINT_XMESH( m_NormalRenderParam.renderType, m_NormalRenderParam.bFog && m_NormalRenderParam.bFogShow );
	}
	else
	{
		if( m_pSkinAnim != NULL )
		{				
			return m_pSkinAnim->OnFrameRender_Prepare();
		}
		else
		{
			return RENDER_HINT_NORENDER;
		}
	}	
#else
	if( NULL == m_pNormalXMesh )
		return RENDER_HINT_NORENDER;

	return RENDER_HINT_XMESH( m_NormalRenderParam.renderType, m_NormalRenderParam.bFog && m_NormalRenderParam.bFogShow );
#endif
}

/*virtual*/ void CX2SubEquip::OnFrameRender_Draw()
{
#ifdef ATTACH_SKINMESH
	if( m_bSkinMesh == false )
	{
		if( NULL != m_pNormalXMesh )
		{	
			D3DXMATRIX localMove, matTrans, matRot, matScale, matWorld;
			
			D3DXMatrixIdentity( &matWorld );

			D3DXMatrixScaling(&matScale, m_fScale, m_fScale, m_fScale );
			D3DXMatrixRotationYawPitchRoll( &matRot, m_vRotate.y, m_vRotate.x, m_vRotate.z );

			D3DXMatrixTranslation( &matTrans, m_vOffset.x, m_vOffset.y, m_vOffset.z );

			D3DXMatrixMultiply( &localMove, &matScale, &matRot );
			D3DXMatrixMultiply( &localMove, &localMove, &matTrans );
			
			D3DXMatrixMultiply( &matWorld, &localMove, m_pFrameMatrix );

			g_pKTDXApp->GetDGManager()->GetXRenderer()->OnFrameRender( m_NormalRenderParam, matWorld, *m_pNormalXMesh, NULL, NULL, NULL );
		}
	}
	else
	{
		if( m_pSkinAnim != NULL)
		{			
			m_pSkinAnim->OnFrameRender_Draw();
		}
	}
#else
	if( NULL != m_pNormalXMesh )
	{
		g_pKTDXApp->GetDGManager()->GetXRenderer()->OnFrameRender( m_NormalRenderParam, *m_pFrameMatrix, *m_pNormalXMesh, NULL, NULL, NULL );
	}
#endif
}

#ifdef ATTACH_SKINMESH
HRESULT CX2SubEquip::OnFrameMove( double fTime, float fElapsedTime )
{
	if( m_bSkinMesh == true && m_pSkinAnim != NULL )
	{
		m_pSkinAnim->SetDXMatrix( *m_pFrameMatrix );
		m_pSkinAnim->OnFrameMove( fTime, fElapsedTime );

#ifdef SERV_IRUHADEV_RANOX_ATTACH_SKIN_MESH
		// Ranox FIRE_DEVIL_*/SECRET_* fighter+berserker glow overlay (ATTACH_SKIN_MESH0,
		// SKIN_MESH_RENDER_TYPE.SMRT_UV_ANIM). Scroll speed matches X2Eqip.cpp's own
		// #else fallback for the enchanted-weapon glow (X2Eqip.cpp:414,
		// ADD_UPGRADE_WEAPON_PARTICLE not defined here) rather than inventing a new one.
		// CX2SubEquip has only one render pass (unlike CX2Eqip's separate
		// m_EnchantRenderParam), so this replaces rather than overlays the normal
		// cel-shaded look -- disclosed in RANOX_EFFECT_ENGINE_GAPS.md, not tuned further.
		if( m_bUVAnimGlow == true )
		{
			m_fUVAnimTexOffset += fElapsedTime * 1.0f;

			CKTDGXRenderer::RenderParam* pRenderParam = m_pSkinAnim->GetRenderParam();
			if( pRenderParam != NULL )
			{
				pRenderParam->texOffsetStage2.y	= m_fUVAnimTexOffset;
				pRenderParam->renderType			= CKTDGXRenderer::RT_LIGHT_FLOW_LOCAL_Z;
			}
		}
#endif SERV_IRUHADEV_RANOX_ATTACH_SKIN_MESH
	}

	return S_OK;
}

void CX2SubEquip::SetUnitXSkinAnim(CKTDGXSkinAnimPtr pXSkinAnimPtr)
{
	if( m_bSkinMesh == true )
	{
		if( m_pSkinAnim != NULL )
		{
			m_pSkinAnim->SetUnitXSkinAnim( pXSkinAnimPtr );
		}
	}
}

void CX2SubEquip::SetJiggle(bool bJiggle, unsigned char fTension)
{
	if( m_bSkinMesh == true )
	{
		if( m_pSkinAnim != NULL )
		{
			m_pSkinAnim->SetJiggleMesh( bJiggle, fTension );
		}
	}
}

#ifdef SERV_IRUHADEV_JUNE_CUSTOM_RENDER_STATE_EQUIP
// june-engine R5b: June 32f08a90
void CX2SubEquip::SetDefaultRenderParam( const CKTDGXRenderer::RenderParam* pRenderParam )
{
	if( NULL != pRenderParam )
		m_DefaultRenderParam = *pRenderParam;
}

// june-engine R5b: June 32f08a50, the skin anim's param, else the plain mesh's
void CX2SubEquip::RestoreDefaultRenderParam()
{
	CKTDGXRenderer::RenderParam* pRenderParam = GetRenderParam();
	if( NULL != pRenderParam )
		*pRenderParam = m_DefaultRenderParam;
	else
		m_NormalRenderParam = m_DefaultRenderParam;
}
#endif SERV_IRUHADEV_JUNE_CUSTOM_RENDER_STATE_EQUIP
void CX2SubEquip::SetRenderParam( CKTDGXRenderer::RenderParam& pRenderParam )
{
	if( m_bSkinMesh == true && m_pSkinAnim != NULL )
	{
		CKTDGXRenderer::RenderParam *renderParam = m_pSkinAnim->GetRenderParam();
		*renderParam = pRenderParam;
	}
	
}
#endif


