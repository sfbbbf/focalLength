#pragma once

#include <SFML/Graphics.hpp>
#include <list>
#include <vector>
#include <string>

#include "dynamic_blur.hpp"
#include "event_manager.hpp"

class CDrawable;

class BehaviorSelect
{
public:
	virtual void Select(std::list<std::unique_ptr<sf::Shape>> & refDrawable) = 0;
	virtual void UnSelect(std::list<std::unique_ptr<sf::Shape>>& refDrawable) = 0;

	virtual ~BehaviorSelect() = 0 {};
};


class Selectable : public BehaviorSelect
{
public:
	void Select(std::list<std::unique_ptr<sf::Shape>>& refDrawable) override {
		for (auto& pCurrent : refDrawable)
			pCurrent->setFillColor(sf::Color::Green);
	}

	void UnSelect(std::list<std::unique_ptr<sf::Shape>>& refDrawable) override {
		for (auto& pCurrent : refDrawable)
			pCurrent->setFillColor(sf::Color::White);
	}
};

class NonSelectable : public BehaviorSelect
{
public:
	void Select(std::list<std::unique_ptr<sf::Shape>>& refDrawable) override {};
	void UnSelect(std::list<std::unique_ptr<sf::Shape>>& refDrawable) override {};
};


class SelectableLens : public BehaviorSelect
{
public:
	void Select(std::list<std::unique_ptr<sf::Shape>>& refDrawable) override {
		for (auto& pCurrent : refDrawable)
			pCurrent->setFillColor(sf::Color::Green);
	}

	void UnSelect(std::list<std::unique_ptr<sf::Shape>>& refDrawable) override {
		for (auto& pCurrent : refDrawable)
			pCurrent->setFillColor(sf::Color::Cyan);
	}
};
enum EDrawableType
{
	eImage = 0,
	eScreen,
	eLensCollect,
	eLensDiffuse,
	eNone
};

class CDrawable {
public:
	struct stText
	{
		sf::Text text;
		double dXPositionToCompare;
	};

	const EDrawableType m_type;

public:
	CDrawable(EDrawableType const& eTypw) : m_type(eTypw) {};
	virtual ~CDrawable();
	void AddElement(sf::Shape * pElem);
	void Select();
	void UnSelect();
	void Draw(sf::RenderWindow* pWin);
	void SetBehavior(std::unique_ptr<BehaviorSelect>&& behavior);
	const std::list<stText>& GetText() const { return m_lText; }

	virtual sf::Vector2f GetPosition() { return sf::Vector2f(0, 0); }

	virtual void SetPosition(sf::Vector2f const& other) { return; }
	virtual void SetPosition(sf::Vector2f && other) { return; }


protected:
	void AddText(stText && text) { m_lText.push_back(std::move(text)); }


	std::list<std::unique_ptr<sf::Shape>> m_lpFigure;
	std::unique_ptr<BehaviorSelect> m_pBehaviorSelect;
	std::list<stText> m_lText;
};

struct stConstants
{
private:
	stConstants()
		: nWidth(1200), nHeight(600), m_strPathToFont("font/ARIAL.woff"), nBaseYPos(200)
	{
		for (int nCurrent = 100; nCurrent < 1100; nCurrent += 15)
		{
			m_vPossiblePositions.push_back(nCurrent);
		}
	};
public:
	static stConstants* Instance()
	{
		static std::unique_ptr<stConstants> pConst;
		if (!pConst.get())
			pConst.reset(new stConstants);
		
		return pConst.get();
	}
public:
	const int nWidth;
	const int nHeight;
	std::vector<int> m_vPossiblePositions;
	const int nBaseYPos;
	const std::string m_strPathToFont;
	const int nUpperRuler = 130;
};

class CRuler : public CDrawable
{
public:

	CRuler() : CDrawable(eNone)
	{
		ConsturtRuler();
		SetBehavior(std::move(std::make_unique<NonSelectable>()));
	}
	static const double fRulerWidth;
	static const double fRulerHeight;

	static const int nBaseStartX = 100;
	static const int nBaseStartY = 200;

	static const int nRiskWidth = 2;
private:
	void ConsturtRuler();
};

class CFlashlight
{
public:
	CFlashlight()
	{
		ConstructFlashlight();
	}
	void SetWidthSunray(const float x) { m_sunray.setSize(sf::Vector2f(x, m_sunray.getSize().y)); }

	void Draw(sf::RenderWindow* pWin) { pWin->draw(m_sunray);  pWin->draw(m_Base); }

	sf::RectangleShape* GetSunray() { return &m_sunray; }

private:
	sf::RectangleShape m_Base;
	sf::RectangleShape m_sunray;

	void ConstructFlashlight();
};

class CScreen : public CDrawable
{
public:
	CScreen() : CDrawable(eScreen)
	{
		ConstructScreen();
		SetBehavior(std::move(std::make_unique<Selectable>()));
	}
	~CScreen() override {};
	
	sf::Vector2f GetPosition() override { return (*m_lpFigure.begin())->getPosition(); }

	virtual void SetPosition(sf::Vector2f const& other) { (*m_lpFigure.begin())->setPosition(other); }
	virtual void SetPosition(sf::Vector2f&& other) { (*m_lpFigure.begin())->setPosition(other); }

	static const int m_nWidth = 2;
	static const int m_nHeight = 70;
private:
	void ConstructScreen();
};

class CButton
{
public:
	CButton(double dxsize, double dysize)
		: m_ConsButton(sf::Vector2f(dxsize, dysize)), m_bIsPressed(false)
	{
		m_ConsButton.setFillColor(m_default);
	};

	void SetPosition(double x, double y) { m_ConsButton.setPosition(sf::Vector2f(x, y)); }
	sf::Vector2f GetPosition() { return m_ConsButton.getPosition(); }

	void Draw(sf::RenderWindow* pWin);
	sf::Text& GetText() { return m_ButtonsText; }

	sf::RectangleShape& GetButtBase() { return m_ConsButton; }

	const sf::Color m_default = sf::Color::White;
	const sf::Color m_pressed{128, 128, 128};
	
	void Change(bool b)
	{
		m_bIsPressed = b;
		m_ConsButton.setFillColor(m_bIsPressed ? m_pressed : m_default);
	}
private:
	sf::RectangleShape m_ConsButton;
	sf::Text m_ButtonsText;
	bool m_bIsPressed;
};


class CLens : public CDrawable
{
public:
	CLens(EDrawableType const& eType)
		: 
		CDrawable(eType) 
	{ 
		SetBehavior(std::move(std::make_unique<SelectableLens>()));
	};

	static const int m_nWidth = 2;
	static const int m_nHeight = 70;

	sf::Vector2f GetPosition() override { return m_lpFigure.begin()->get()->getPosition(); }

	void SetPosition(sf::Vector2f&& other) override { Shift(other.x); }
	void SetPosition(sf::Vector2f const & other) override { Shift(other.x); }

	void Shift(int nNewX);

	bool Construct(std::list<CDrawable * > & vSelectable, CFlashlight & light);
};

class CEnterSpace
{

private:
	sf::Text m_r1;
	sf::Text m_r2;
};

class CImage : public CDrawable
{
public:
	CImage() : CDrawable(eImage) { Construct(); SetBehavior(std::move(std::make_unique<Selectable>())); };
	
	sf::Vector2f GetPosition() override { return m_lpFigure.begin()->get()->getPosition(); }
	void SetPosition(sf::Vector2f&& other) override { m_lpFigure.begin()->get()->setPosition(other); }
	void SetPosition(sf::Vector2f const & other) override { m_lpFigure.begin()->get()->setPosition(other); }

	static const int m_nWidth = 2;
	static const int m_nHeight = 70;
private:
	void Construct();
};


struct Arrow
{
	sf::RectangleShape m_base;
	sf::RectangleShape m_up;
	sf::RectangleShape m_what;

	double scale = 1;

	const int m_nbasesicc = 10;
	const int m_nbaselong = 100;

	sf::Vector2f m_pos;

	sf::Vector2f m_leftLim{ 400, 300 };
	sf::Vector2f m_rightLim{ 600, 500 };

	void ConstructRevers(sf::Vector2f const& position)
	{
		m_pos = position;

		const int nBaseSicc = m_nbasesicc * scale;
		const int nBaseLong = m_nbaselong * scale;

		if (position.y - nBaseLong / 2 <= m_leftLim.y || position.y + nBaseLong / 2 >= m_rightLim.y)
		{
			const int nBaleLong = m_rightLim.y - m_leftLim.y;

			m_base = sf::RectangleShape(sf::Vector2f(nBaseSicc, nBaleLong));
			m_base.setPosition(position.x - m_base.getSize().x / 2, m_leftLim.y);
		}
		else
		{
			m_base = sf::RectangleShape(sf::Vector2f(nBaseSicc, nBaseLong));
			m_base.setPosition(position.x - m_base.getSize().x / 2, position.y - m_base.getSize().y / 2);
		}
		if (m_base.getPosition().y + m_base.getSize().y + nBaseSicc > m_rightLim.y)
		{
			const int fYOut = m_base.getPosition().y + m_base.getSize().y;
			const int nAns = fYOut - m_rightLim.y;

			m_up = sf::RectangleShape(sf::Vector2f(nBaseLong - 20 * scale, nAns));
			m_up.setPosition(sf::Vector2f(m_base.getPosition().x - 35 * scale,
				m_base.getPosition().y + m_base.getSize().y));
		}
		else
		{
			m_up = sf::RectangleShape(sf::Vector2f(nBaseLong - 20 * scale, nBaseSicc));
			m_up.setPosition(sf::Vector2f(m_base.getPosition().x - 35 * scale,
				m_base.getPosition().y + m_base.getSize().y));
		}
		if (m_up.getPosition().x < m_leftLim.x)
		{
			int nSicc = m_up.getPosition().x + nBaseSicc - m_leftLim.x;
			if (nSicc < 0) nSicc = 0;
			m_what = sf::RectangleShape(sf::Vector2f(nSicc, nBaseLong >> 1));
			m_what.setPosition(sf::Vector2f(m_leftLim.x, m_up.getPosition().y - (nBaseLong >> 1)));
		}
		else
		{
			m_what = sf::RectangleShape(sf::Vector2f(nBaseSicc, nBaseLong >> 1));
			m_what.setPosition(sf::Vector2f(m_up.getPosition().x, m_up.getPosition().y - (nBaseLong >> 1)));
		}

		m_base.setFillColor(sf::Color::Yellow);
		m_what.setFillColor(sf::Color::Yellow);
		m_up.setFillColor(sf::Color::Yellow);

	}

	void draw(sf::RenderWindow* pWin)
	{
		pWin->draw(m_base);
		pWin->draw(m_up);
		pWin->draw(m_what);
	}
};


class CCard
{
public:
	CCard(sf::Vector2f const& pos)
		: m_pos(pos)
	{
		Construct(pos);
	}

	void Draw(sf::RenderWindow* pWin);

	const int m_nbasesicc = 10;
	const int m_nbaselong = 100;

	sf::Vector2f GetPos() { return m_pos; }
	Arrow& arrow() { return m_arrow; }

	void ConstructArrowDefault();

private:
	void Construct(sf::Vector2f const&);
public:
	const sf::Vector2f m_pos;
	sf::RectangleShape m_left;
	sf::RectangleShape m_right;
	sf::RectangleShape m_up;
	sf::RectangleShape m_bottom;

	std::vector<sf::RectangleShape> m_vlines;
	
	Arrow m_arrow;
};

struct LensParams
{
	double R1{ 0 };
	double R2{ 0 };

	double N_envir{ 1. };
	double N_mater{ 1.5 };
};

struct CInputField
{ 
	CInputField(sf::Vector2f const& pos, std::wstring const& str, double& d) 
		: 
		m_pos(pos), 
		m_str(str), 
		m_fConnectedNum(d)
	{};
	
	std::string m_temp;

	void Create()
	{
		m_temp = std::to_string(m_fConnectedNum);
		rect = sf::RectangleShape(sf::Vector2f(200, 2));
		rect.setPosition(m_pos);
		m_EnterPos = sf::Vector2f(m_pos.x, m_pos.y);
	}

	sf::RectangleShape rect;

	double & m_fConnectedNum;
	sf::Vector2f m_pos;
	sf::Vector2f m_EnterPos;
	std::wstring m_str;

};