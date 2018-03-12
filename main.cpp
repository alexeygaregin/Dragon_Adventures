#include <gamebase/Gamebase.h>
#include <iostream>
#include <fstream>

using namespace gamebase;
using namespace std;


class MyApp : public App
{
	struct Events
	{
		string name;
		float x;
		float y;
		float eventtime;
	};
	Events e;
	vector <Events> events;
	void load()
	{
		timerfirstdragon.start();
		timerplot.start();
		hp = 7;
		enemydragonsspawn = false;
		dragon.setPos(-field.width() / 2 + dragon.width() / 2, 0);
		key.hide();
		chest.hide();
		for (auto background : backgrounds.all())
		{
			background.hide();
		}
		connect(timer2, checktimered);//Вражеские драконы
		connect(timer3, checktimeredf);//Спам огненных шаров вражеских драконов
		connect(timer, checktimereb);//Спам огненных шаров боссов
		timer.repeat(3);
		timer2.repeat(3);
		timer3.repeat(3);
		plotload();
		cout << e.eventtime << endl;

	}
	void plotload()
	{
		ifstream file("plot.txt");
		while (!file.eof())
		{
			file >> e.eventtime >> e.name >> e.x >> e.y;
			events.push_back(e);
		}
	}

	void createshuriken()
	{
		auto Shur = shurikens.load("shuriken.json");
		auto vbox = field.viewBox();
		Shur.setPos(vbox.r + Shur.width(), events[ev].y);
	}

	void createfirstboss()
	{
		auto Boss1 = bosses.load("Firstboss.json");
		auto vbox = field.viewBox();
		Boss1.setPos(vbox.r + Boss1.width(), 0);
	}

	void checktimered()
	{
		if (enemydragonsspawn == false)
		{
			auto Drag = enemydragons.load("Enemydragon.json");
			auto vbox = field.viewBox();
			Drag.setPos(vbox.r - Drag.width(), randomInt(vbox.b + Drag.height() / 2, vbox.t - Drag.height() / 2));
			enemydragonsspawn = true;
		}
	}

	void checktimeredf()
	{
		for (auto enemydragon : enemydragons.all())
		{
			auto Blueball = bluefireballs.load("bluefireball.json");
			auto vbox = field.viewBox();
			Blueball.setPos(enemydragon.pos().x - 50, enemydragon.pos().y);
		}

	}
	void checktimereb()
	{
		for (auto boss : bosses.all())
		{

				auto Bossfireball = bossesfireballs.load("Firstbossredfireball.json");
				auto vbox = field.viewBox();
				Bossfireball.setPos(boss.pos().x - boss.width() / 2, boss.pos().y);
		}
	}

	void process(Input input)
	{
		using namespace gamebase::InputKey;

		if (input.pressed(Up))
		{
			dragon.move(0, 300 * timeDelta());
		}
		if (input.pressed(Down))
		{
			dragon.move(0, -300 * timeDelta());
		}
		if (input.justPressed(Space))
			auto redfireball = redfireballs.load("redfireball.json", dragon.pos().x + dragon.width() / 2, dragon.pos().y);
	}

	void move()
	{
		if (ev < events.size())
		{
			if (timerplot.time() > events[ev].eventtime)
			{

				if (events[ev].name == "shuriken")
				{
					createshuriken();
					ev++;
				}
				else if (events[ev].name == "firstboss")
				{
					firstbosstime = e.eventtime;
					createfirstboss();
					ev++;
				}
			}
		}

		cout << timerplot.time() << endl;

		/*if (ev == 0)
		{
			if (timerplot.time() > e.eventtime)
			{
				auto firstboss = bosses.load("Enemyboss.json",0, 0);
				firstboss.setPos(events[0].x, events[0].y);


			}
		}*/


		if (hp == 0)
			close();
		d.x = dragon.pos().x + field.width() / 2 - dragon.width() / 2;
		d.y = 0;
		field.setView(d);
		if (dragon.pos().y > field.height() / 2)
		{
			dragon.setPos(dragon.pos().x, -field.height() / 2);
		}
		if (dragon.pos().y < -field.height() / 2)
		{
			dragon.setPos(dragon.pos().x, field.height() / 2);
		}

		for (auto boss : bosses.all())
		{
			if (timerplot.time() < e.eventtime + 3)
			{
				boss.move(-boss.width() / 2 * timeDelta(), 0);
			}
		}

		for (auto redfireball : redfireballs.all())
		{
			for (auto shuriken : shurikens.all())
			{
				if (redfireball.intersects(shuriken))
				{
					shurikens.remove(shuriken);
					redfireballs.remove(redfireball);
					break;
				}
			}
		}
		for (auto redfireball : redfireballs.all())
		{
			redfireball.move(800 * timeDelta(), 0);
		}

		for (auto bossesfireball : bossesfireballs.all())
		{
			bossesfireball.move(-1600 * timeDelta(), 0);
		}


		for (auto bluefireball : bluefireballs.all())
		{
			bluefireball.move(-400 * timeDelta(), 0);
		}

		for (auto bluefireball : bluefireballs.all())
		{
			if (bluefireball.intersects(dragon))
			{
				hp--;
				hplabel << 'x' << hp;
				bluefireballs.remove(bluefireball);
			}
		}


		for (auto shuriken : shurikens.all())
		{
			shuriken.move(-400 * timeDelta(), 0);
			shuriken.rotate(6.28 / 2 * timeDelta());
			if (dragon.intersects(shuriken))
					{
						shurikens.remove(shuriken);
						hp = 0;
						hplabel << 'x' << hp;
					}
		}

		for (auto redfireball : redfireballs.all())
		{
			for (auto enemydragon : enemydragons.all())
			{
				if (redfireball.intersects(enemydragon))
				{
					enemydragons.remove(enemydragon);
					redfireballs.remove(redfireball);
					enemydragonsspawn = false;
				}
			}
		}
		/*cout << timerfirstdragon.time() << endl;
		if (timerfirstdragon.time() > 5)
		{

			if ( ba == 0)
			{
				auto boss = bosses.load("Enemyboss.json", field.viewBox().r, 0);
				bosses.data(boss) = 3;
				ba++;
			}
			for (auto enemydragon : enemydragons.all())
			{
				enemydragons.remove(enemydragon);
			}
				for (auto boss : bosses.all())
				{
					auto& v = bosses.data(boss);
					int b1 = 0;

					if (timerfirstdragon.time() <= 7)
					{
						boss.move(-75 * timeDelta(), 0);
					}
					for (auto redfireball : redfireballs.all())
					{
							if (redfireball.intersects(boss))
							{
								redfireballs.remove(redfireball);
								v--;
								if (v == 0)
								{
									bosses.remove(boss);
									break;
								}
							}
					}
				}
		}*/
    }
	Vec2 d;
	Timer timer;// Спам огненных шаров боссов
	Timer timer2;// Спам драконов
	Timer timer3;// Спам огненных шаров вражеских драконов
	Timer timerfirstdragon;
	Timer timerplot;// Таймер для сюжета
	int hp;
	int ba = 0;
	int ev = 0; //номер события
	bool enemydragonsspawn;
	float firstbosstime;
	FromDesign(GameObj, dragon);
	FromDesign(GameView, field);
	FromDesign(GameObj, chest);
	FromDesign(GameObj, key);
	FromDesign(GameObj, background1);
	FromDesign(GameObj, background2);
	FromDesign(GameObj, background3);
	FromDesign(GameObj, backgroundmenu);
	FromDesign(Label, hplabel);


	LayerFromDesign(void, dragons);
	LayerFromDesign(void, food);
	LayerFromDesign(void, shurikens);
	LayerFromDesign(void, backgrounds);
	LayerFromDesign(void, enemyfireballs);
	LayerFromDesign(void, enemydragons);
	LayerFromDesign(void, redfireballs);
	LayerFromDesign(void, greenfireballs);
	LayerFromDesign(void, bluefireballs);
	LayerFromDesign(void, silvercoins);
	LayerFromDesign(void, goldcoins);
	LayerFromDesign(int, bosses);
	LayerFromDesign(void, bossesfireballs);
};
 
int main(int argc, char** argv)
{
    MyApp app;
    app.setConfig("Dragon_AdventuresConfig.json");
    app.setDesign("Design.json");
    if (!app.init(&argc, argv))
        return 1;
    app.run();
    return 0;
}
