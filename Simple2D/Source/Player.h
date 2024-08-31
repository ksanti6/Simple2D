#pragma once


class Player
{
private:
	explicit Player() {}
	Player(const Player&) = delete;
	void operator= (const Player&) = delete;

public:
	void Init(void);
	void Update(void);
	void Shutdown(void);

	static Player& GetInstance();

private:
	int m_score = 0;
	int m_lives = 3;

};