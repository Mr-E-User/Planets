#include <SFML/Graphics.hpp>
#include <iostream>
#include <math.h>

const int size = 127;
const int fps = 30;
const int G = 50000;
const int scaleFactorV = 2;

float standardR = 20.f, standardM = 100.f;
float rChange = 2, mChange = 25;
bool disp = true;

bool vecOn = false;

bool stationary = true;

float width = 1800, height = 1000;

int loc = -1;

sf::Vector2f cm;

struct body
{
	sf::Vector2f p, v, a;
	float r, m;
	bool s;

	body() { p.x = 0; p.y = 0; v.x = 0; v.y = 0; a.x = 0; a.y = 0; r = 0; m = 0; s = false; }

	body(float x_, float y_, float vx_, float vy_, float ax_, float ay_, float r_, float m_, bool s_) { p.x = x_; p.y = y_; v.x = vx_; v.y = vy_; a.x = ax_; a.y = ay_; r = r_; m = m_; s = s_; }

	//Set Values
	void setPos(float changex, float changey) { p.x = changex; p.y = changey; }

	void setVel(float changevx, float changevy) { v.x = changevx; v.y = changevy; }

	void setAcc(float changeax, float changeay) { a.x = changeax; a.y = changeay; }

	void setR(float change) { r = change; }

	void setM(float change) { m = change; }

	//Get Values
	sf::Vector2f getPos() { return p; }
	
	sf::Vector2f getVel() { return v; }

	sf::Vector2f getAcc() { return a; }

	float getR() { return r; }

	float getM() { return m; }

	//Update
	void tick()
	{
		if (!s)
		{
			p.x += v.x / fps;
			p.y += v.y / fps;
			v.x += a.x / fps;
			v.y += a.y / fps;
		}
	}
};

/*sf::Vector2f centerPos(int howmany, body arg[], float totalmass) 
{
	if (howmany < 0 || totalmass < 0.1)
	{
		return sf::Vector2f(0, 0);
	}
	else
	{
		float xComponent = arg[howmany].getPos().x * arg[howmany].getM() / totalmass;
		float yComponent = arg[howmany].getPos().y * arg[howmany].getM() / totalmass;

		return sf::Vector2f(xComponent + centerPos(howmany - 1, arg, totalmass).x, yComponent + centerPos(howmany - 1, arg, totalmass).y);
	}
}*/

sf::Vector2f distance(sf::Vector2f a, sf::Vector2f b)
{
	return b - a;
}

sf::Vector2f findForce(int location, body arg[])
{
	sf::Vector2f forces(0, 0);
	for (int i = 0; i <= loc; i++)
	{
		if (i != location)
		{
			sf::Vector2f dist(distance(arg[location].getPos(), arg[i].getPos()));
			if (sqrt(dist.x * dist.x + dist.y * dist.y) > 1)
			{
				float fMagnitude = G * (arg[i].getM() * arg[location].getM()) / (dist.x * dist.x + dist.y * dist.y);
				float posMagnitude = sqrt(dist.x * dist.x + dist.y * dist.y);
				forces += sf::Vector2f(dist.x / posMagnitude * fMagnitude, dist.y / posMagnitude * fMagnitude);
			}
		}
	}
	return forces;
}

void collisionStuff(body arg[])
{
	for (int i = 0; i <= loc; i++)
	{
		for (int j = 0; j < i; j++)
		{
			sf::Vector2f dist(distance(arg[i].getPos(), arg[j].getPos()));
			float magnitude = sqrt(dist.x * dist.x + dist.y * dist.y);
			if (magnitude < arg[i].getR() + arg[j].getR())
			{
				if (arg[i].getM() > arg[j].getM())
				{
					arg[j] = body();
					arg[i].setAcc(0, 0);
					arg[i].setVel(0, 0);
				}
				else if (arg[i].getM() < arg[j].getM())
				{
					arg[i] = body();
					arg[j].setAcc(0, 0);
					arg[j].setVel(0, 0);
				}
				else
					arg[i] = arg[j] = body();
			}
		}
	}
}

int main()
{
	sf::RenderWindow w(sf::VideoMode(width, height), "spacelol");
	w.setFramerateLimit(fps);

	sf::VertexArray line(sf::Lines, 2);

	sf::Vector2f tmp1(0, 0), tmp2(0, 0);

	body bodies[size];

	cm.x = 0; 
	cm.y = 0;

	while (w.isOpen())
	{
		sf::Event event;
		while (w.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				w.close();

			if (event.type == sf::Event::MouseButtonPressed)
			{
				tmp1.x = sf::Mouse::getPosition(w).x;
				tmp1.y = sf::Mouse::getPosition(w).y;
			}

			if (event.type == sf::Event::MouseButtonReleased)
			{
				loc += 1;
				if (!stationary)
				{
					bodies[loc] = body(tmp1.x, tmp1.y, distance(tmp1, tmp2).x / scaleFactorV, distance(tmp1, tmp2).y / scaleFactorV, 0, 0, standardR, standardM, false);
				}
				else
				{
					bodies[loc] = body(tmp1.x, tmp1.y, 0, 0, 0, 0, standardR, standardM, true);
				}
			}

			if (event.type == sf::Event::KeyPressed)
			{
				if (event.key.code == sf::Keyboard::Space)
					vecOn = !vecOn;
				else if (event.key.code == sf::Keyboard::T)
				{
					stationary = !stationary;
					std::cout << "stationary creation: " << stationary << std::endl;
				}
				else if (event.key.code == sf::Keyboard::R)
				{
					for (int i = 0; i <= loc; i++)
						bodies[i] = body();
					loc = -1;
				}
			}
		}

		if (!sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			tmp1.x = sf::Mouse::getPosition(w).x;
			tmp1.y = sf::Mouse::getPosition(w).y;
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) { standardR += rChange; disp = true; }
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) { standardR -= rChange; disp = true; }
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) { standardR += rChange * 4; disp = true; }
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) { standardR -= rChange * 4; disp = true; }
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) { standardM += mChange; disp = true; }
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) { standardM -= mChange; disp = true; }
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) { standardM += mChange * 4; disp = true; }
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) { standardM -= mChange * 4; disp = true; }

		if (disp)
		{
			disp = !disp;
			std::cout << "R: " << standardR << std::endl;
			std::cout << "M: " << standardM << std::endl;
		}

		tmp2.x = sf::Mouse::getPosition(w).x;
		tmp2.y = sf::Mouse::getPosition(w).y;


		line[0].position = sf::Vector2f(tmp1);
		line[1].position = sf::Vector2f(tmp2);

		w.clear();
		
		if (loc != -1)
		{
			//draw and find total mass necessary
			float totalmass = 0;
			for (int i = 0; i <= loc; i++)
			{
				sf::CircleShape c;
				int r = (int)(bodies[i].getR()) % 255;
				int g = (int)(bodies[i].getM()) % 255;
				int b = (int)(bodies[i].getR() + bodies[i].getM()) % 255;
				c.setFillColor(sf::Color(r, g, b));
				c.setPosition(bodies[i].getPos() - sf::Vector2f(bodies[i].getR(), bodies[i].getR()));
				c.setRadius(bodies[i].getR());
				w.draw(c);

				totalmass += bodies[i].getM();
			}
			
			for (int i = 0; i <= loc; i++)
			{
				sf::Vector2f force = findForce(i, bodies);

				bodies[i].setAcc(force.x / bodies[i].getM(), force.y / bodies[i].getM());

				if (vecOn)
				{
					sf::VertexArray accVec(sf::Lines, 2);
					sf::VertexArray velVec(sf::Lines, 2);

					accVec[0].position = bodies[i].getPos();
					accVec[1].position = sf::Vector2f(bodies[i].getAcc() + bodies[i].getPos());
					accVec[0].color = sf::Color::Red;
					accVec[1].color = sf::Color::Red;

					velVec[0].position = bodies[i].getPos();
					velVec[1].position = sf::Vector2f(bodies[i].getVel() + bodies[i].getPos());
					velVec[0].color = sf::Color::Yellow;
					velVec[1].color = sf::Color::Yellow;

					w.draw(accVec);
					w.draw(velVec);
				}
				
				bodies[i].tick();
			
			}
		}

		collisionStuff(bodies);

		w.draw(line);

		w.display();
	}

	return 0;
}