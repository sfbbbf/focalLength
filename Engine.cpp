#include "Engine.hpp"


CProrgamManager::CProrgamManager()
	:
	m_pWindow(std::make_unique<sf::RenderWindow>(sf::VideoMode(stConstants::Instance()->nWidth, stConstants::Instance()->nHeight), L"нир", sf::Style::Close))
{

}

bool CProrgamManager::Start()
{
	sf::RenderWindow* pWin = m_pWindow.get();
	ConstructAllElements();

	while (pWin->isOpen()) {

		const bool event = Events();
		if (!event)
			return false;

		pWin->clear();
		AllDraw();
		pWin->display();
	}

	return true;
}

void CProrgamManager::ConstructAllElements()
{
	m_pFlashlight.reset(new CFlashlight);
	m_pRuler.reset(new CRuler());
	m_pScreen.reset(new CScreen());

	m_pImage.reset(new CImage);

	m_lSelectable.push_back(m_pScreen.get());
	m_lSelectable.push_back(m_pImage.get());

	m_AddLens.reset(new CButton(150, 30));
	m_AddLens->SetPosition(400, 30);
	sf::Text& text = m_AddLens->GetText();

	sf::Font font;
	if (!font.loadFromFile(stConstants::Instance()->m_strPathToFont))
		return;

	text.setFont(font);
	text.setString(L"Добавить линзу");
	text.setCharacterSize(20);
	text.setFillColor(sf::Color::Black);
	text.setPosition(417, 35);

	NextSelected();

	m_pSource.reset(new CCard(sf::Vector2f(300, 700)));
	m_pSource->ConstructArrowDefault();

	m_pBefore.reset(new CCard(sf::Vector2f(600, 700)));

}

void CProrgamManager::AllDraw()
{
	sf::Font font;
	if (!font.loadFromFile(stConstants::Instance()->m_strPathToFont))
		return;

	sf::RenderWindow* pWin = m_pWindow.get();

	m_pFlashlight->Draw(pWin);
	m_pRuler->Draw(pWin);

	for (auto& j : m_pRuler->GetText())
	{
		sf::Text text(j.text.getString(), font, 10);
		text.setPosition(j.text.getPosition());
		m_pWindow->draw(text);

	}

	m_pScreen->Draw(pWin);
	m_pImage->Draw(pWin);

	if (m_pCollected.get())
		m_pCollected->Draw(pWin);
	if (m_pDiffuse.get())
		m_pDiffuse->Draw(pWin);

	m_AddLens->Draw(pWin);

	sf::Text text = m_AddLens->GetText();

	sf::Text newtext{ text.getString(), font, 15 };
	newtext.setPosition(text.getPosition());
	newtext.setFillColor(sf::Color::Black);

	m_pWindow->draw(newtext);

	if (m_pDiffuseButton)
	{
		m_pDiffuseButton->Draw(pWin);

		sf::Text text = m_pDiffuseButton->GetText();

		sf::Text newtext{ text.getString(), font, 15 };
		newtext.setPosition(text.getPosition());
		newtext.setFillColor(sf::Color::Black);

		m_pWindow->draw(newtext);
	}
	if (m_pCollectButton)
	{
		m_pCollectButton->Draw(pWin);

		sf::Text text = m_pCollectButton->GetText();

		sf::Text newtext{ text.getString(), font, 15 };
		newtext.setPosition(text.getPosition());
		newtext.setFillColor(sf::Color::Black);

		m_pWindow->draw(newtext);
	}

	m_pSource->Draw(pWin);

	m_pBefore->Draw(pWin);

	if (m_arrow)
		if (m_nBLurLevel != -1)
			BlurAndDraw();
		else
			m_arrow->draw(pWin);

	// отрисовка ввода
	if (*m_pSelected == m_pCollected.get())
	{
		sf::Text newText(m_inputR1->m_str, font, 20);

		newText.setPosition(m_inputR1->m_pos.x - 27, m_inputR1->m_pos.y - 20);
		newText.setFillColor(sf::Color::White);
		pWin->draw(m_inputR1->rect);
		pWin->draw(newText);
		//
		m_R1val = sf::Text(m_inputR1->m_temp, font, 17);
		m_R1val.setPosition(m_inputR1->m_pos.x + 5, m_inputR1->m_pos.y - 20);
		pWin->draw(m_R1val);

		newText.setString(m_inputR2->m_str);
		newText.setPosition(m_inputR2->m_pos.x - 27, m_inputR2->m_pos.y - 20);
		newText.setFillColor(sf::Color::White);
		pWin->draw(m_inputR2->rect);
		pWin->draw(newText);
		//
		m_R2val = sf::Text(m_inputR2->m_temp, font, 17);
		m_R2val.setPosition(m_inputR2->m_pos.x + 5, m_inputR2->m_pos.y - 20);
		pWin->draw(m_R2val);

		newText.setString(m_inputNevir->m_str);
		newText.setPosition(m_inputNevir->m_pos.x - 50, m_inputNevir->m_pos.y - 20);
		newText.setFillColor(sf::Color::White);
		pWin->draw(m_inputNevir->rect);
		pWin->draw(newText);
		//
		m_NenvirVal = sf::Text(m_inputNevir->m_temp, font, 17);
		m_NenvirVal.setPosition(m_inputNevir->m_pos.x + 5, m_inputNevir->m_pos.y - 20);
		pWin->draw(m_NenvirVal);

		newText.setString(m_inputNmater->m_str);
		newText.setPosition(m_inputNmater->m_pos.x - 50, m_inputNmater->m_pos.y - 20);
		newText.setFillColor(sf::Color::White);
		pWin->draw(m_inputNmater->rect);
		pWin->draw(newText);
		//
		m_NmaterVal = sf::Text(m_inputNmater->m_temp, font, 17);
		m_NmaterVal.setPosition(m_inputNmater->m_pos.x + 5, m_inputNmater->m_pos.y - 20);
		pWin->draw(m_NmaterVal);

	}
	else if (*m_pSelected == m_pDiffuse.get())
	{
		sf::Text newText(m_inputR1->m_str, font, 20);

		newText.setPosition(m_inputR1->m_pos.x - 27, m_inputR1->m_pos.y - 20);
		newText.setFillColor(sf::Color::White);
		pWin->draw(m_inputR1->rect);
		pWin->draw(newText);
		//
		m_R1val = sf::Text(m_inputR1->m_temp, font, 17);
		m_R1val.setPosition(m_inputR1->m_pos.x + 5, m_inputR1->m_pos.y - 20);
		pWin->draw(m_R1val);

		newText.setString(m_inputR2->m_str);
		newText.setPosition(m_inputR2->m_pos.x - 27, m_inputR2->m_pos.y - 20);
		newText.setFillColor(sf::Color::White);
		pWin->draw(m_inputR2->rect);
		pWin->draw(newText);
		//
		m_R2val = sf::Text(m_inputR2->m_temp, font, 17);
		m_R2val.setPosition(m_inputR2->m_pos.x + 5, m_inputR2->m_pos.y - 20);
		pWin->draw(m_R2val);

		newText.setString(m_inputNevir->m_str);
		newText.setPosition(m_inputNevir->m_pos.x - 50, m_inputNevir->m_pos.y - 20);
		newText.setFillColor(sf::Color::White);
		pWin->draw(m_inputNevir->rect);
		pWin->draw(newText);
		//
		m_NenvirVal = sf::Text(m_inputNevir->m_temp, font, 17);
		m_NenvirVal.setPosition(m_inputNevir->m_pos.x + 5, m_inputNevir->m_pos.y - 20);
		pWin->draw(m_NenvirVal);

		newText.setString(m_inputNmater->m_str);
		newText.setPosition(m_inputNmater->m_pos.x - 50, m_inputNmater->m_pos.y - 20);
		newText.setFillColor(sf::Color::White);
		pWin->draw(m_inputNmater->rect);
		pWin->draw(newText);
		//
		m_NmaterVal = sf::Text(m_inputNmater->m_temp, font, 17);
		m_NmaterVal.setPosition(m_inputNmater->m_pos.x + 5, m_inputNmater->m_pos.y - 20);
		pWin->draw(m_NmaterVal);

	}
}

bool CProrgamManager::Events()
{
	sf::RenderWindow* pWin = m_pWindow.get();

	sf::Event event;
	while (pWin->pollEvent(event))
	{
		switch (event.type)
		{
		case sf::Event::Closed:
			pWin->close();
			return false;
		case sf::Event::KeyPressed:
			switch (event.key.code)
			{
			case sf::Keyboard::Left:
				ShilftLeft();
				break;
			case sf::Keyboard::Right:
				ShiftRight();
				break;
			case sf::Keyboard::Up:
				NextSelected();
				break;
			case sf::Keyboard::Down:
				PrevSelected();
				break;
			case sf::Keyboard::Num1:
				Input(m_inputR1.get());
				break;
			case sf::Keyboard::Num2:
				Input(m_inputR2.get());
				break;
			case sf::Keyboard::Num3:
				Input(m_inputNmater.get());
				break;
			case sf::Keyboard::Num4:
				Input(m_inputNmater.get());
				break;
			}
		case sf::Event::MouseButtonPressed:
			if (event.key.code == sf::Mouse::Left)
				Pressed();
			if (event.key.code == sf::Mouse::Right)
				std::cout << sf::Mouse::getPosition(*m_pWindow).x << ' ' << sf::Mouse::getPosition(*m_pWindow).y << std::endl;
			break;
		case sf::Event::MouseButtonReleased:
			if (event.key.code == sf::Mouse::Left)
				Realeased();
			break;
		}
	}

	return true;
}

void CProrgamManager::NextSelected()
{
	if (!m_pSelected._Ptr || m_pSelected == m_lSelectable.end())
		m_pSelected = m_lSelectable.begin();
	else
	{
		(*m_pSelected)->UnSelect();
		++m_pSelected;
		if (m_pSelected == m_lSelectable.end())
			m_pSelected = m_lSelectable.begin();
	}
	if (*m_pSelected == m_pDiffuse.get())
		CreateInput(m_diffuse);

	if (*m_pSelected == m_pCollected.get())
		CreateInput(m_collect);

	(*m_pSelected)->Select();
}

void CProrgamManager::PrevSelected()
{
	(*m_pSelected)->UnSelect();

	if (m_pSelected == m_lSelectable.begin())
		m_pSelected = --m_lSelectable.end();
	else
		--m_pSelected;

	(*m_pSelected)->Select();

	if (*m_pSelected == m_pDiffuse.get())
		CreateInput(m_diffuse);

	if (*m_pSelected == m_pCollected.get())
		CreateInput(m_collect);
}

void CProrgamManager::Pressed()
{
	sf::Vector2i const pressedPos = sf::Mouse::getPosition(*m_pWindow);

	// add
	if (pressedPos.x >= 400 && pressedPos.x <= 550 && pressedPos.y >= 30 && pressedPos.y <= 60)
		m_AddLens->Change(true);
	// colle
	if (pressedPos.x >= 560 && pressedPos.x <= 560 + 115 && pressedPos.y >= 30 && pressedPos.y <= 60)
		m_pCollectButton->Change(true);
	if (pressedPos.x >= 685 && pressedPos.x <= 685 + 115 && pressedPos.y >= 30 && pressedPos.y <= 60)
		m_pDiffuseButton->Change(true);
}

void CProrgamManager::Realeased()
{
	m_AddLens->Change(false);
	if (m_pDiffuseButton)
		m_pDiffuseButton->Change(false);
	if (m_pCollectButton)
		m_pCollectButton->Change(false);

	sf::Vector2i const pressedPos = sf::Mouse::getPosition(*m_pWindow);

	if (pressedPos.x >= 400 && pressedPos.x <= 550 && pressedPos.y >= 30 && pressedPos.y <= 60)
	{
		sf::Text& textButton = m_AddLens->GetText();
		if (textButton.getString() == L"Добавить линзу")
		{

			m_pCollectButton.reset(new CButton(115, 30));
			m_pCollectButton->SetPosition(560, 30);

			sf::Text& text = m_pCollectButton->GetText();

			sf::Font font;
			if (!font.loadFromFile(stConstants::Instance()->m_strPathToFont))
				return;

			text.setFont(font);
			text.setString(L"Собирающая");
			text.setCharacterSize(20);
			text.setFillColor(sf::Color::Black);
			text.setPosition(570, 35);

			m_pDiffuseButton.reset(new CButton(130, 30));
			m_pDiffuseButton->SetPosition(685, 30);

			sf::Text& text1 = m_pDiffuseButton->GetText();

			text1.setFont(font);
			text1.setString(L"Рассеивающая");
			text1.setCharacterSize(20);
			text1.setFillColor(sf::Color::Black);
			text1.setPosition(695, 35);

			textButton.setString(L"Удалить линзы");
			textButton.setPosition(sf::Vector2f(textButton.getPosition().x + 5, textButton.getPosition().y));
		}
		else
		{
			CLens* pDel = m_pDiffuse.release();
			std::list<CDrawable* >::iterator it;
			if (pDel)
			{
				it = std::find(m_lSelectable.begin(), m_lSelectable.end(), static_cast<CDrawable*>(pDel));
				m_lSelectable.erase(it);
			}

			pDel = m_pCollected.release();
			if (pDel)
			{
				it = std::find(m_lSelectable.begin(), m_lSelectable.end(), static_cast<CDrawable*>(pDel));
				m_lSelectable.erase(it);
			}
			m_pSelected = m_lSelectable.begin();
		}
	}
	if (pressedPos.x >= 560 && pressedPos.x <= 560 + 115 && pressedPos.y >= 30 && pressedPos.y <= 60)
	{
		if (m_pCollected)
			return;

		m_pCollected.reset(new CLens(EDrawableType::eLensCollect));
		m_pCollected->Construct(m_lSelectable, *m_pFlashlight);
		m_lSelectable.push_back(m_pCollected.get());


		CreateInput(m_collect);
	}
	if (pressedPos.x >= 685 && pressedPos.x <= 685 + 115 && pressedPos.y >= 30 && pressedPos.y <= 60)
	{
		if (m_pDiffuse)
			return;

		m_pDiffuse.reset(new CLens(EDrawableType::eLensDiffuse));
		m_pDiffuse->Construct(m_lSelectable, *m_pFlashlight);
		m_lSelectable.push_back(m_pDiffuse.get());
		
		CreateInput(m_diffuse);
	}
}

void CProrgamManager::ShilftLeft()
{
	const int nCurrentX = (*m_pSelected)->GetPosition().x;
	const int nCurrentY = (*m_pSelected)->GetPosition().y;
	auto& vPossible = stConstants::Instance()->m_vPossiblePositions;
	auto itFind = std::find(vPossible.begin(), vPossible.end(), nCurrentX);

	if (itFind == vPossible.end() || itFind == vPossible.begin())
		return;

	if (*m_pSelected == m_pScreen.get())
	{
		const int nCurrentDiffusX = m_pDiffuse.get() ? m_pDiffuse->GetPosition().x : -1;
		const int nCurrentCollectedX = m_pCollected.get() ? m_pCollected->GetPosition().x : -1;
		const int nCurrentImageX = m_pImage.get()->GetPosition().x;

		const int nFindX = *(itFind - 1);
		if (nFindX <= nCurrentImageX || nFindX <= nCurrentCollectedX || nFindX <= nCurrentDiffusX)
			return;

		(*m_pSelected)->SetPosition(sf::Vector2f(nFindX, nCurrentY));

		auto pRect = m_pFlashlight->GetSunray();
		pRect->setSize(sf::Vector2f(nFindX - pRect->getPosition().x, pRect->getSize().y));
	}
	else if (*m_pSelected == m_pDiffuse.get())
	{
		const int nCurrentImageX = m_pImage->GetPosition().x;
		const int nCurrentScreen = m_pScreen->GetPosition().x;
		const int nCurrentCollected = m_pCollected.get() ? m_pCollected->GetPosition().x : -1;

		const int nFidX = *(itFind - 1);

		if (nFidX <= nCurrentImageX || nFidX >= nCurrentScreen || nFidX <= nCurrentCollected)
			return;

		m_pDiffuse->Shift(nFidX);
	}
	else if (*m_pSelected == m_pCollected.get())
	{
		const int nCurrentImageX = m_pImage->GetPosition().x;
		const int nCurrentDiffusX = m_pDiffuse.get() ? m_pDiffuse->GetPosition().x : -1;

		const int nFindX = *(itFind - 1);
		if (nFindX <= nCurrentImageX)
			return;

		m_pCollected->Shift(nFindX);
	}
	else if (*m_pSelected == m_pImage.get())
	{
		const int nItFind = *(itFind - 1);

		if (itFind == vPossible.begin())
			return;

		(*m_pSelected)->SetPosition(sf::Vector2f(nItFind, nCurrentY));
	}

	calc();
}

void CProrgamManager::ShiftRight()
{
	const int nCurrentX = (*m_pSelected)->GetPosition().x;
	const int nCurrentY = (*m_pSelected)->GetPosition().y;
	auto& vPossible = stConstants::Instance()->m_vPossiblePositions;
	auto itFind = std::find(vPossible.begin(), vPossible.end(), nCurrentX);

	if (itFind == vPossible.end() || itFind == --vPossible.end())
		return;

	if (*m_pSelected == m_pScreen.get())
	{
		(*m_pSelected)->SetPosition(sf::Vector2f(*++itFind, nCurrentY));
		auto pRect = m_pFlashlight->GetSunray();
		pRect->setSize(sf::Vector2f(*itFind - pRect->getPosition().x, pRect->getSize().y));
	}
	else if (*m_pSelected == m_pDiffuse.get())
	{
		const int nCurrentCollectedX = m_pCollected.get() ? m_pCollected->GetPosition().x : 10000;
		const int nCurrentScreenX = m_pScreen->GetPosition().x;

		const int nItFindX = *(itFind + 1);
		if (nItFindX >= nCurrentScreenX)
			return;

		m_pDiffuse->Shift(nItFindX);
	}
	else if (*m_pSelected == m_pCollected.get())
	{
		const int nCurrentScreenX = m_pScreen->GetPosition().x;
		const int nCurrentDiffuseX = m_pDiffuse ? m_pDiffuse->GetPosition().x : 10000;

		const int nItFindX = *(itFind + 1);
		if (nItFindX >= nCurrentScreenX || nItFindX >= nCurrentDiffuseX)
			return;

		m_pCollected->Shift(nItFindX);
	}
	else if (*m_pSelected == m_pImage.get())
	{
		const int nCurrentDiffusX = m_pDiffuse.get() ? m_pDiffuse->GetPosition().x : 10000;
		const int nCurrentCollectedX = m_pCollected.get() ? m_pCollected->GetPosition().x : 10000;
		const int nCurrentScreenX = m_pScreen->GetPosition().x;

		const int nItFind = *(itFind + 1);
		if (nItFind >= nCurrentScreenX || nItFind >= nCurrentDiffusX || nItFind >= nCurrentCollectedX)
			return;

		(*m_pSelected)->SetPosition(sf::Vector2f(nItFind, nCurrentY));
	}

	calc();
}

void CProrgamManager::calc()
{
	m_arrow.reset(nullptr);
	m_nBLurLevel = -1;

	const bool bOnlyCollect = m_pCollected && !m_pDiffuse;
	const bool bOnlyDiff = !m_pCollected && m_pDiffuse;
	const bool bBoth = m_pCollected && m_pDiffuse;

	auto getPos = [&](int n) -> int
	{
		auto v = stConstants::Instance()->m_vPossiblePositions;
		auto it = std::find(v.begin(), v.end(), n);
		return (it != v.end() ? it - v.begin() : -1);
	};

	if (bOnlyCollect)
	{
		const int nCurrentImage = getPos(m_pImage->GetPosition().x);
		const int nCurrentCollect = getPos(m_pCollected->GetPosition().x);
		const int nCurrentScreen = getPos(m_pScreen->GetPosition().x);

		if (nCurrentCollect == -1 || nCurrentImage == -1 || nCurrentScreen == -1)
			return;

		const int d = nCurrentCollect - nCurrentImage;
		
		if (!m_collect.N_envir || !m_collect.R1 || !m_collect.R2)
			return;

		const double _F = (m_collect.N_mater / m_collect.N_envir - 1) * (1/m_collect.R1 + 1 / m_collect.R2);

		const int F = nCurrentCollect - 1./_F;
		const int F1 = nCurrentCollect + _F;

		if (nCurrentImage >= F)
			return;

		const int f = std::round(std::pow((1./_F - 1./d), -1));
		const double G = std::abs(f) * 1. / std::abs(d);
		const double H = 1 * G;
	
		const int nHowFar = std::abs((f + nCurrentCollect) - nCurrentScreen);

		if (nHowFar != 0 && nHowFar <= 3)
			m_nBLurLevel = 0;
		else if (nHowFar > 3 && nHowFar <= 6)
			m_nBLurLevel = 1;
		else if (nHowFar > 6 && nHowFar <= 9)
			m_nBLurLevel = 2;
		else if (nHowFar > 9 && nHowFar <= 12)
			m_nBLurLevel = 3;
		else if (nHowFar > 12 && nHowFar <= 15)
			m_nBLurLevel = 4;
		else if (nHowFar > 15)
			m_nBLurLevel = 5;
		// четкое изображение 

		m_arrow.reset(new Arrow);
		m_arrow->scale = G;

		const int nx = 500;
		const int ny = 400;
		
		const sf::Vector2f center{500, 400};
		m_arrow->ConstructRevers(sf::Vector2f(center.x, center.y));
	}
	else if (bOnlyDiff)
	{
		// изображение мнимое - на экране получить нельзя
	}
	else if (bBoth)
	{
		const int nObjectX = getPos(m_pImage->GetPosition().x);
		const int nCollectX = getPos(m_pCollected->GetPosition().x);

		if (nObjectX == -1 || nCollectX == -1)
			return;

		const int d = nCollectX - nObjectX;

		if (!m_collect.N_envir || !m_collect.R1 || !m_collect.R2)
			return;

		const double F = (m_collect.N_mater / m_collect.N_envir - 1) * (1 / m_collect.R1 + 1 / m_collect.R2);
		const int f = std::round(std::pow((1. / F - 1. / d), -1));

		const int nDiffuseX = getPos(m_pDiffuse->GetPosition().x);
		if (f + nCollectX >= nDiffuseX)
			return;
		
		const int f1 = std::abs(nCollectX + f - nDiffuseX);
		const double F1 = -(m_diffuse.N_mater / m_diffuse.N_envir - 1) * (1 / m_diffuse.R1 + 1 / m_diffuse.R2);

		const double d1 = std::pow(1./F1 + 1./f1, -1);
		const double G = std::abs(f1) / std::abs(d1);
		const int nScreenX = getPos(m_pScreen->GetPosition().x);

		const int nHowFar = std::abs((std::abs(d1) + nDiffuseX) - nScreenX);

		if (nHowFar != 0 && nHowFar <= 3)
			m_nBLurLevel = 0;
		else if (nHowFar > 3 && nHowFar <= 6)
			m_nBLurLevel = 1;
		else if (nHowFar > 6 && nHowFar <= 9)
			m_nBLurLevel = 2;
		else if (nHowFar > 9 && nHowFar <= 12)
			m_nBLurLevel = 3;
		else if (nHowFar > 12 && nHowFar <= 15)
			m_nBLurLevel = 4;
		else if (nHowFar > 15)
			m_nBLurLevel = 5;

		m_arrow.reset(new Arrow);
		m_arrow->scale = G;

		const int nx = 500;
		const int ny = 400;

		const sf::Vector2f center{ 500, 400 };
		m_arrow->ConstructRevers(sf::Vector2f(center.x, center.y));
	}
}

void CProrgamManager::CreateInput(LensParams& params)
{
	sf::Vector2f const pos{ 775, 340 };
	m_inputR1.reset(new CInputField(pos, L"R1", params.R1));
	m_inputR1->Create();

	m_inputR2.reset(new CInputField(sf::Vector2f(pos.x, pos.y + 50), L"R2", params.R2));
	m_inputR2->Create();

	m_inputNevir.reset(new CInputField(sf::Vector2f(pos.x, pos.y + 100), L"Nокр", params.N_envir));
	m_inputNevir->Create();
	
	m_inputNmater.reset(new CInputField(sf::Vector2f(pos.x, pos.y + 150), L"Nмат", params.N_mater));
	m_inputNmater->Create();
}

void CProrgamManager::BlurAndDraw()
{
	sf::RenderWindow* pWin = m_pWindow.get();
	sf::Vector2u const size{ pWin->getSize() };

	Blur blur(size, m_nBLurLevel);
	sf::RenderTexture render_target;
	render_target.create(size.x, size.y);
	render_target.clear();
	render_target.draw(m_arrow->m_up);
	render_target.display();

	pWin->draw(blur.apply(render_target.getTexture()), sf::BlendAdd);

	render_target.clear();
	render_target.draw(m_arrow->m_what);
	render_target.display();

	pWin->draw(blur.apply(render_target.getTexture()), sf::BlendAdd);

	render_target.clear();
	render_target.draw(m_arrow->m_base);
	render_target.display();

	pWin->draw(blur.apply(render_target.getTexture()), sf::BlendAdd);
}

void CProrgamManager::Input(CInputField* pField)
{
	sf::RenderWindow* pWin = m_pWindow.get();
	pField->rect.setFillColor(sf::Color::Green);

	bool bEntering = true;
	std::string& temp = pField->m_temp;
	while (bEntering)
	{
		sf::Event event;
		while (m_pWindow->pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				pWin->close();

			if (event.type == sf::Event::KeyReleased)
			{
				if (event.key.code == sf::Keyboard::Enter)
				{
					bEntering = false;
					if (pField->m_temp.empty())
						pField->m_temp.push_back('0');

					float f = std::stof(pField->m_temp);
					pField->m_fConnectedNum = f;
				}
				else if (event.key.code == sf::Keyboard::Backspace)
				{
					if (temp.size())
						temp.erase(temp.size() - 1);
				}
			}
			if (event.type == sf::Event::TextEntered)
			{
				char ch;
				if (event.text.unicode < 128)
				{
					ch = static_cast<char>(event.text.unicode);
					if (ch == '.' || ch >= '0' && ch <= '9')
					{
						const int n = temp.find('.');
						if (n != -1 && ch == '.' || temp.size() == 10)
							continue;

						temp.push_back(ch);
					}
				}
			}
		}
		
		pWin->clear();
		AllDraw();
		pWin->display();
	
	}

	pField->rect.setFillColor(sf::Color::White);

}

