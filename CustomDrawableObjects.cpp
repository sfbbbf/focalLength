#include "CustomDrawableObjects.h"

CDrawable::~CDrawable()
{

}

void CDrawable::AddElement(sf::Shape * pElem)
{
	m_lpFigure.emplace_back(pElem);
}

void CDrawable::Select()
{
	m_pBehaviorSelect->Select(m_lpFigure);
}

void CDrawable::UnSelect()
{
	m_pBehaviorSelect->UnSelect(m_lpFigure);
}

void CDrawable::Draw(sf::RenderWindow* pWin)
{
	for (auto& pCurrent : m_lpFigure) 
		pWin->draw(*pCurrent);
}

void CDrawable::SetBehavior(std::unique_ptr<BehaviorSelect>&& behavior)
{
	m_pBehaviorSelect = std::move(behavior);
}

const double CRuler::fRulerWidth = 992;
const double CRuler::fRulerHeight = 7.5;

void CRuler::ConsturtRuler()
{
	// основа 
	std::unique_ptr<sf::RectangleShape> pTemp(new sf::RectangleShape(sf::Vector2f(fRulerWidth, fRulerHeight)));
	pTemp->setFillColor(sf::Color::White);

	pTemp->setPosition(sf::Vector2f(nBaseStartX, nBaseStartY));
	AddElement(pTemp.release());

	// риски
	sf::Font font;
	if (!font.loadFromFile(stConstants::Instance()->m_strPathToFont))
		return;

	std::vector<int>& vPos = stConstants::Instance()->m_vPossiblePositions;
	for (int dCurrent = 0, nCounter = 0; dCurrent < vPos.size(); ++dCurrent, ++nCounter)
	{
		int nRiskHeight = 4;

		if (!nCounter || nCounter == 10 || nCounter == 5)
		{
			nRiskHeight += 15;
			if (nCounter)
				nCounter = 0;
		}
		else if (nCounter > 0 && nCounter < 5 || nCounter > 5 && nCounter < 10)
			nRiskHeight += 10;

		pTemp.reset(new sf::RectangleShape(sf::Vector2f(nRiskWidth, nRiskHeight)));

		pTemp->setFillColor(sf::Color::White);
		pTemp->setPosition(sf::Vector2f(vPos[dCurrent], nBaseStartY + 7.5));
		AddElement(pTemp.release());

		stText pNums;
		const double dOffset = dCurrent / 10 == 0 ? vPos[dCurrent] - 2 : dCurrent / 100 == 0 ? vPos[dCurrent] - 4 : vPos[dCurrent] - 3;
		
		pNums.text.setString(std::to_string(dCurrent));
		pNums.text.setFont(font);
		pNums.text.setPosition(sf::Vector2f(dOffset, nBaseStartY + 7.5 + 18));
		pNums.text.setFillColor(sf::Color::White);
		pNums.dXPositionToCompare = vPos[dCurrent];

		AddText(std::move(pNums));
	}
}	

void CFlashlight::ConstructFlashlight()
{
	m_Base = sf::RectangleShape(sf::Vector2f(40., 15.));
	m_Base.setPosition(sf::Vector2f(50., 160.));
	m_Base.setFillColor(sf::Color::White);

	m_sunray = sf::RectangleShape(sf::Vector2f(*(--stConstants::Instance()->m_vPossiblePositions.end()) - 50., 10.));
	m_sunray.setPosition(sf::Vector2f(50., 162.5));
	m_sunray.setFillColor(sf::Color::Yellow);
}

void CScreen::ConstructScreen()
{	
	const int nLastPointX = *(--stConstants::Instance()->m_vPossiblePositions.end());

	std::unique_ptr<sf::RectangleShape> pRect(new sf::RectangleShape(sf::Vector2f(m_nWidth, m_nHeight)));
	pRect->setPosition(nLastPointX, stConstants::Instance()->nUpperRuler);
	pRect->setFillColor(sf::Color::White);

	AddElement(pRect.release());
}

void CLens::Shift(int nNewX)
{
	if (m_type == EDrawableType::eLensCollect)
	{
		for (auto& i : m_lpFigure)
			i->setPosition(sf::Vector2f(nNewX, i->getPosition().y));

		auto itSecond = m_lpFigure.begin();
		++itSecond;

		auto itThird = itSecond;
		++itThird;

		itSecond->get()->setPosition(sf::Vector2f(nNewX + 2, itSecond->get()->getPosition().y));
		itThird->get()->setPosition(sf::Vector2f(nNewX + 3, itThird->get()->getPosition().y));
	}
	else
	{
		for (auto& i : m_lpFigure)
			i->setPosition(sf::Vector2f(nNewX, i->getPosition().y));

		auto itSecond = m_lpFigure.begin();
		++++++itSecond;

		auto itThird = itSecond;
		++itThird;

		itSecond->get()->setPosition(sf::Vector2f(nNewX + 1.5, itSecond->get()->getPosition().y));
		itThird->get()->setPosition(sf::Vector2f(nNewX + 2.5, itThird->get()->getPosition().y));
	}
}

bool CLens::Construct(std::list<CDrawable * > & vSelectable, CFlashlight & light)
{
	CDrawable* pScreen = nullptr;
	CDrawable* pImage = nullptr;

	CDrawable* pDiffuse = nullptr;
	CDrawable* pCollect = nullptr;

	for (auto i : vSelectable)
	{
		switch (i->m_type)
		{
		case EDrawableType::eImage:
			pImage = i;
			break;
		case EDrawableType::eLensCollect:
			pCollect = i;
			break;
		case EDrawableType::eScreen:
			pScreen = i;
			break;
		case EDrawableType::eLensDiffuse:
			pDiffuse = i;
			break;
		}
	}

	if ((pDiffuse && (m_type == EDrawableType::eLensDiffuse)) || (pCollect && (m_type == EDrawableType::eLensCollect)))
		return false;

	if (m_type == EDrawableType::eLensCollect)
	{
		std::unique_ptr<sf::RectangleShape> pRect(new sf::RectangleShape(sf::Vector2f(m_nWidth, m_nHeight)));
		stConstants* pConst = stConstants::Instance();

		pRect->setPosition(pConst->m_vPossiblePositions[1], pConst->nUpperRuler);
		pRect->setFillColor(sf::Color::Cyan);

		m_lpFigure.emplace_back(std::move(pRect));

		const int nHeight = 10;
		pRect.reset(new sf::RectangleShape(sf::Vector2f(m_nWidth, nHeight)));
		pRect->setPosition(sf::Vector2f(pConst->m_vPossiblePositions[1] + 2, pConst->nBaseYPos));
		pRect->setRotation(210);
		pRect->setFillColor(sf::Color::Cyan);

		m_lpFigure.emplace_back(std::move(pRect));

		pRect.reset(new sf::RectangleShape(sf::Vector2f(m_nWidth, 11)));
		pRect->setPosition(sf::Vector2f(pConst->m_vPossiblePositions[1] + 3, pConst->nBaseYPos));
		pRect->setRotation(150);
		pRect->setFillColor(sf::Color::Cyan);

		m_lpFigure.emplace_back(std::move(pRect));

		pRect.reset(new sf::RectangleShape(sf::Vector2f(m_nWidth, 9)));
		pRect->setPosition(sf::Vector2f(pConst->m_vPossiblePositions[1], 130));
		pRect->setRotation(30);
		pRect->setFillColor(sf::Color::Cyan);

		m_lpFigure.emplace_back(std::move(pRect));


		pRect.reset(new sf::RectangleShape(sf::Vector2f(m_nWidth, 10)));
		pRect->setPosition(sf::Vector2f(pConst->m_vPossiblePositions[1], 130));
		pRect->setRotation(330);
		pRect->setFillColor(sf::Color::Cyan);

		m_lpFigure.emplace_back(std::move(pRect));

		pScreen->SetPosition(sf::Vector2f(*--pConst->m_vPossiblePositions.end(), pConst->nUpperRuler));
		if (pDiffuse)
			pDiffuse->SetPosition(sf::Vector2f(pConst->m_vPossiblePositions[2], 0));
		
		pScreen->SetPosition(sf::Vector2f(*--pConst->m_vPossiblePositions.end(), pScreen->GetPosition().y));
		pImage->SetPosition(sf::Vector2f(pConst->m_vPossiblePositions[0], pImage->GetPosition().y));
		sf::RectangleShape * rect = light.GetSunray();
		rect->setSize(sf::Vector2f((*--pConst->m_vPossiblePositions.end()) - rect->getPosition().x, rect->getSize().y));
	}
	else
	{
		std::unique_ptr<sf::RectangleShape> pRect(new sf::RectangleShape(sf::Vector2f(m_nWidth, 50)));
		stConstants* pConst = stConstants::Instance();

		pRect->setPosition(pConst->m_vPossiblePositions[2], pConst->nUpperRuler + 10);
		pRect->setFillColor(sf::Color::Cyan);

		m_lpFigure.emplace_back(std::move(pRect));
		//
		const int nHeight = 10;
		pRect.reset(new sf::RectangleShape(sf::Vector2f(m_nWidth, 9)));
		pRect->setPosition(sf::Vector2f(pConst->m_vPossiblePositions[2], pConst->nBaseYPos - 10));
		pRect->setRotation(30);
		pRect->setFillColor(sf::Color::Cyan);

		m_lpFigure.emplace_back(std::move(pRect));

		pRect.reset(new sf::RectangleShape(sf::Vector2f(m_nWidth, 10)));
		pRect->setPosition(sf::Vector2f(pConst->m_vPossiblePositions[2], pConst->nBaseYPos - 10));
		pRect->setRotation(330);
		pRect->setFillColor(sf::Color::Cyan);

		m_lpFigure.emplace_back(std::move(pRect));
//
		pRect.reset(new sf::RectangleShape(sf::Vector2f(m_nWidth, 10)));
		pRect->setPosition(sf::Vector2f(pConst->m_vPossiblePositions[2] + 1.5, 130 + 10));
		pRect->setRotation(210);
		pRect->setFillColor(sf::Color::Cyan);

		m_lpFigure.emplace_back(std::move(pRect));

		pRect.reset(new sf::RectangleShape(sf::Vector2f(m_nWidth, 11)));
		pRect->setPosition(sf::Vector2f(pConst->m_vPossiblePositions[2] + 2.5, 130 + 10));
		pRect->setRotation(150);
		pRect->setFillColor(sf::Color::Cyan);

		m_lpFigure.emplace_back(std::move(pRect));

		if (pCollect)
			pCollect->SetPosition(sf::Vector2f(pConst->m_vPossiblePositions[1], 0));

		pScreen->SetPosition(sf::Vector2f(*--pConst->m_vPossiblePositions.end(), pScreen->GetPosition().y));
		pImage->SetPosition(sf::Vector2f(pConst->m_vPossiblePositions[0], pImage->GetPosition().y));
		sf::RectangleShape* rect = light.GetSunray();
		rect->setSize(sf::Vector2f((*--pConst->m_vPossiblePositions.end()) - rect->getPosition().x, rect->getSize().y));
	}
}

void CImage::Construct()
{
	const int nLastPointX = stConstants::Instance()->m_vPossiblePositions[0];
	std::unique_ptr<sf::RectangleShape> pRect(new sf::RectangleShape(sf::Vector2f(m_nWidth, m_nHeight)));
	pRect->setPosition(nLastPointX, stConstants::Instance()->nUpperRuler);
	pRect->setFillColor(sf::Color::White);

	AddElement(pRect.release());
}

void CButton::Draw(sf::RenderWindow* pWin)
{
	pWin->draw(m_ConsButton);
}

void CCard::Draw(sf::RenderWindow* pWin)
{
	pWin->draw(m_left);
	pWin->draw(m_right);
	pWin->draw(m_up);
	pWin->draw(m_bottom);

	for (auto& i : m_vlines)
		pWin->draw(i);

	pWin->draw(m_arrow.m_base);
	pWin->draw(m_arrow.m_up);
	pWin->draw(m_arrow.m_what);
}

void CCard::ConstructArrowDefault()
{
	m_arrow.m_base = sf::RectangleShape(sf::Vector2f(m_nbasesicc, m_nbaselong));
	m_arrow.m_base.setPosition(m_pos.x - 105, m_pos.y - 350);

	m_arrow.m_up = sf::RectangleShape(sf::Vector2f(m_nbaselong - 20, m_nbasesicc));
	m_arrow.m_up.setPosition(m_pos.x - 140, m_pos.y - 350);

	m_arrow.m_what = sf::RectangleShape(sf::Vector2f(m_nbasesicc, m_nbaselong >> 1));
	m_arrow.m_what.setPosition(m_pos.x - 140, m_pos.y - 350);

	m_arrow.m_base.setFillColor(sf::Color::Yellow);
	m_arrow.m_what.setFillColor(sf::Color::Yellow);
	m_arrow.m_up.setFillColor(sf::Color::Yellow);
}

void CCard::Construct(sf::Vector2f const& pos)
{
	const int nHeight = 2;
	const int nWidth = 200;

	m_left = sf::RectangleShape(sf::Vector2f(nHeight, nWidth));
	m_left.setPosition(pos.x - 200, pos.y - 400);

	m_bottom = sf::RectangleShape(sf::Vector2f(nWidth, nHeight));
	m_bottom.setPosition(pos.x - 200, pos.y - 200);

	m_right = sf::RectangleShape(sf::Vector2f(nHeight, nWidth));
	m_right.setPosition(pos.x, pos.y - 400);

	m_up = sf::RectangleShape(sf::Vector2f(nWidth, nHeight));
	m_up.setPosition(pos.x - 200, pos.y - 400);

	const int nHeight1 = 1;
	
	for (int i = pos.y - 200; i > pos.y - 400; i -= 20)
	{
		sf::RectangleShape rect{sf::Vector2f(nWidth, nHeight1)};
		rect.setPosition(sf::Vector2f(pos.x - 200, i));
		m_vlines.emplace_back(rect);
	}
	
	for (int i = pos.x - 200; i != pos.x; i += 20)
	{
		sf::RectangleShape rect{ sf::Vector2f(nHeight1, nWidth) };
		rect.setPosition(sf::Vector2f(i, pos.y - 400));
		m_vlines.emplace_back(rect);
	}
}
