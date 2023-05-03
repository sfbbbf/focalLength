#pragma once

#include "CustomDrawableObjects.h"
#include <iostream>

class CProrgamManager : private sf::NonCopyable {
public:
	CProrgamManager();
	bool Start();
private:
	void ConstructAllElements();
	void AllDraw();
	bool Events();
	void NextSelected();
	void PrevSelected();

	void Pressed();

	void Realeased();

	void ShilftLeft();
	void ShiftRight();

	void calc();
	void CreateInput(LensParams& params);
	void BlurAndDraw();

	void Input(CInputField* pField);
private:
	std::unique_ptr<sf::RenderWindow> m_pWindow;
		
	std::unique_ptr<CRuler> m_pRuler;

	std::list<CDrawable* > m_lSelectable;
	std::list<CDrawable* >::iterator m_pSelected;

	std::unique_ptr<CLens> m_pDiffuse;
	std::unique_ptr<CLens> m_pCollected;

	std::unique_ptr<CFlashlight> m_pFlashlight;

	std::unique_ptr<CScreen> m_pScreen;
	std::unique_ptr<CImage> m_pImage;

	std::unique_ptr<CButton> m_AddLens;
	std::unique_ptr<CButton> m_pDiffuseButton;
	std::unique_ptr<CButton> m_pCollectButton;

	std::unique_ptr<CCard> m_pSource;
	std::unique_ptr<CCard> m_pBefore;

	std::unique_ptr<Arrow> m_arrow;

	LensParams m_diffuse;
	LensParams m_collect;


	std::unique_ptr<CInputField> m_inputR1;
	sf::Text m_R1val;
	std::unique_ptr<CInputField> m_inputR2;
	sf::Text m_R2val;
	std::unique_ptr<CInputField> m_inputNmater;
	sf::Text m_NmaterVal;
	std::unique_ptr<CInputField> m_inputNevir;
	sf::Text m_NenvirVal;

	int m_nBLurLevel;
	int m_nSelected{ -1 };
};
