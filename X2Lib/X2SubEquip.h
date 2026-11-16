#pragma once

// cx2eqip에 attach되는 메시이다.
class CX2Eqip;
class CX2SubEquip : public CKTDGObject
{
private:
	CX2SubEquip( D3DXMATRIX* pFrameMatrix, const wstring& modelName );
#ifdef ATTACH_SKINMESH
#ifdef SERV_IRUHADEV_JUNE_EQIP_LOAD_ATTACHED
	CX2SubEquip( D3DXMATRIX* pFrameMatrix, const wstring& modelName, bool bSkin, CKTDXDeviceXET* pModelTexChangeXET = NULL );	// june-engine R14f: June 32f09170
#else SERV_IRUHADEV_JUNE_EQIP_LOAD_ATTACHED
	CX2SubEquip( D3DXMATRIX* pFrameMatrix, const wstring& modelName, bool bSkin );
#endif SERV_IRUHADEV_JUNE_EQIP_LOAD_ATTACHED
#endif
	virtual ~CX2SubEquip(void);

public:
	static CX2SubEquip* CreateSubEquip( D3DXMATRIX* pFrameMatrix, const wstring& modelName ) { return new CX2SubEquip( pFrameMatrix, modelName ); }
#ifdef ATTACH_SKINMESH
#ifdef SERV_IRUHADEV_JUNE_EQIP_LOAD_ATTACHED
	static CX2SubEquip* CreateSubEquip( D3DXMATRIX* pFrameMatrix, const wstring& modelName, bool bSkin, CKTDXDeviceXET* pModelTexChangeXET = NULL ) { return new CX2SubEquip( pFrameMatrix, modelName, bSkin, pModelTexChangeXET ); }
#else SERV_IRUHADEV_JUNE_EQIP_LOAD_ATTACHED
	static CX2SubEquip* CreateSubEquip( D3DXMATRIX* pFrameMatrix, const wstring& modelName, bool bSkin ) { return new CX2SubEquip( pFrameMatrix, modelName, bSkin ); }
#endif SERV_IRUHADEV_JUNE_EQIP_LOAD_ATTACHED
#endif

#ifdef ATTACH_SKINMESH
	virtual HRESULT OnFrameMove( double fTime, float fElapsedTime );
#endif

	virtual CKTDGXRenderer::RenderParam* GetRenderParam() { if( NULL != m_pSkinAnim) return m_pSkinAnim->GetRenderParam(); return NULL;}
	virtual RENDER_HINT OnFrameRender_Prepare();
	virtual void OnFrameRender_Draw();

	CKTDGXRenderer::RenderParam& GetNormalRenderParam() { return m_NormalRenderParam; }
	
#ifdef ATTACH_SKINMESH
	void SetUnitXSkinAnim(CKTDGXSkinAnimPtr pXSkinAnimPtr);
	void SetJiggle(bool bJiggle, unsigned char fTension);
	void SetRenderParam( CKTDGXRenderer::RenderParam& pRenderParam );
#ifdef SERV_IRUHADEV_JUNE_CUSTOM_RENDER_STATE_EQUIP
	void SetDefaultRenderParam( const CKTDGXRenderer::RenderParam* pRenderParam );	// june-engine R5b: June 32f08a90
	void RestoreDefaultRenderParam();	// june-engine R5b: June 32f08a50
#endif SERV_IRUHADEV_JUNE_CUSTOM_RENDER_STATE_EQUIP
	bool GetIsSkinMesh() { return m_bSkinMesh; }
#endif

#ifdef SERV_IRUHADEV_RANOX_ATTACH_SKIN_MESH
	// Ranox FIRE_DEVIL_*/SECRET_* fighter+berserker ATTACH_SKIN_MESH0 glow overlay.
	// See RANOX_EFFECT_ENGINE_GAPS.md.
	void SetUVAnimGlow( bool bVal ) { m_bUVAnimGlow = bVal; }
#endif SERV_IRUHADEV_RANOX_ATTACH_SKIN_MESH

	void SetScale(float fVal) { m_fScale = fVal; }
	void SetRotate(D3DXVECTOR3 vRot) { m_vRotate = vRot; }
	void SetOffset(D3DXVECTOR3 vOffset) { m_vOffset = vOffset; }

private:
	D3DXMATRIX* m_pFrameMatrix;	// attach된 부모 eqip의 해당 frame의 combine_matrix

	CKTDXDeviceXMesh* m_pNormalXMesh;
	CKTDGXRenderer::RenderParam m_NormalRenderParam;
#ifdef SERV_IRUHADEV_JUNE_CUSTOM_RENDER_STATE_EQUIP
	CKTDGXRenderer::RenderParam m_DefaultRenderParam;	// june-engine R5b: June +0x268 ( both ctors ), what the attach path left in the sub-equip's param
#endif SERV_IRUHADEV_JUNE_CUSTOM_RENDER_STATE_EQUIP

#ifdef ATTACH_SKINMESH
	bool					m_bSkinMesh;
	CKTDGXSkinAnim			*m_pSkinAnim;
	CKTDXDeviceXSkinMesh	*m_pSkinModel;
#endif

#ifdef SERV_IRUHADEV_RANOX_ATTACH_SKIN_MESH
	bool					m_bUVAnimGlow;
	float					m_fUVAnimTexOffset;
#endif SERV_IRUHADEV_RANOX_ATTACH_SKIN_MESH

	float		m_fScale;
	D3DXVECTOR3 m_vRotate;
	D3DXVECTOR3 m_vOffset;
};
