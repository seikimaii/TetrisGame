#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <stdlib.h>
#include <time.h>
#include <fstream>
#include <string> 
#include <sstream>
int main() {
	srand((unsigned)time(NULL));
	const int fieldWidth = 10; 
	const int fieldHeight = 20;
	std::ifstream ifs("score.txt", std::ios::in);
	if (!ifs.is_open()) {
		return 1;
	}
	sf::SoundBuffer start;
	if (!start.loadFromFile("start.wav")) {
		return EXIT_FAILURE;
	}
	sf::Sound sound(start);
	sound.setVolume(20);
	sound.play();
	sf::SoundBuffer restart;
	if (!restart.loadFromFile("restart.wav")) {
		return EXIT_FAILURE;
	}
	sf::SoundBuffer end;
	if (!end.loadFromFile("end.wav")) {
		return EXIT_FAILURE;
	}
	sf::SoundBuffer getPoint;
	if (!getPoint.loadFromFile("getpoint.wav")) {
		return EXIT_FAILURE;
	}
	std::string old_score;
	std::vector<int> score;
	while (getline(ifs, old_score)) {
		std::stringstream ss;
		int scoreToint;
		ss << old_score;
		ss >> scoreToint;
		score.push_back(scoreToint);
	}
	ifs.close();
	enum class Type {
		None,
		O,
		I,
		L,
		rev_L,
		Z,
		rev_Z,
		arrow
	};

	Type field[fieldWidth][fieldHeight] = {};
	sf::Texture score_texture;
	if (!score_texture.loadFromFile("score_table.png")) {
		return EXIT_FAILURE;
	}
	sf::Sprite score_sprite(score_texture);
	score_sprite.setPosition(55.f, 180.f);
	sf::Texture backgroundTexture;
	if (!backgroundTexture.loadFromFile("background.png")) {
		return EXIT_FAILURE;
	}
	sf::Texture redTexture;
	if (!redTexture.loadFromFile("red.png")) {
		return EXIT_FAILURE;
	}
	sf::Texture yellowTexture;
	if (!yellowTexture.loadFromFile("yellow.png")) {
		return EXIT_FAILURE;
	}
	sf::Texture lightBlueTexture;
	if (!lightBlueTexture.loadFromFile("light_blue.png")) {
		return EXIT_FAILURE;
	}
	sf::Texture orangeTexture;
	if (!orangeTexture.loadFromFile("orange.png")) {
		return EXIT_FAILURE;
	}
	sf::Texture blueTexture;
	if (!blueTexture.loadFromFile("blue.png")) {
		return EXIT_FAILURE;
	}
	sf::Texture purpleTexture;
	if (!purpleTexture.loadFromFile("purple.png")) {
		return EXIT_FAILURE;
	}
	sf::Texture greenTexture;
	if (!greenTexture.loadFromFile("green.png")) {
		return EXIT_FAILURE;
	}
	std::map<Type, std::vector<std::vector<sf::Vector2i>>> shapes = {
		{
			Type::O,
			{
				{
					sf::Vector2i(0, 0), 			// O:     OO    (0,-1) (1,-1)
					sf::Vector2i(1, 0),				//        OO    (0, 0) (1, 0)
					sf::Vector2i(0,-1),
					sf::Vector2i(1,-1),
				}
			},
		},
		{
			Type::I,
			{ 
				{ // I:     OOOO   (-1, 0) (0, 0) (1,0) (2,0)  shape_Is[0]
					sf::Vector2i(-1, 0),
					sf::Vector2i(0, 0),
					sf::Vector2i(1, 0),
					sf::Vector2i(2, 0),
				},
				{
					sf::Vector2i(0,-2), // I:     O   (0,-2)
					sf::Vector2i(0,-1), //        O   (0,-1)
					sf::Vector2i(0, 0), //        O   (0, 0)
					sf::Vector2i(0, 1), //        O   (0, 1)
				}
			}
		},
		{
			Type::L,
			{
				{
					sf::Vector2i(0,-2),// L : O    (0,-2)
					sf::Vector2i(0,-1),//     O    (0,-1)
					sf::Vector2i(0,0), //     O O  (0,0) (1,0)
					sf::Vector2i(1,0)
				},
				{
					sf::Vector2i(2,0),   // L : O O O  (0,0) (1,0) (2,0)
					sf::Vector2i(0,1),   //     O      (0,1)
					sf::Vector2i(0,0),    //
					sf::Vector2i(1,0)     //
				},
				{
					sf::Vector2i(0,2),  // L : O O   (-1,0) (0,0)
					sf::Vector2i(0,1),  //       O          (0,1)
					sf::Vector2i(0,0),   //       O          (0,2)
					sf::Vector2i(-1,0)    //
				},
				{
					sf::Vector2i(0,-1), //  L :    O  (0,-1)
					sf::Vector2i(-2,0), //     O O O  (-2,0) (-1,0) (0,0)
					sf::Vector2i(0,0),  //
					sf::Vector2i(-1,0)   //
				}
			}
		},
		{
			Type::rev_L,
			{
				{
					sf::Vector2i(0,-2),// L :   O        (0,-2)
					sf::Vector2i(0,-1),//       O        (0,-1)
					sf::Vector2i(0,0), //     O O (-1,0) (0,0)
					sf::Vector2i(-1,0)
				},
				{
					sf::Vector2i(0,-1),// L :            
					sf::Vector2i(0,0),//       O  (0,-1)
					sf::Vector2i(1,0), //      OOO(0,0) (1,0)(2,0)
					sf::Vector2i(2,0)
				},
				{
					sf::Vector2i(0,0),// L :   O O  (0,0)(1,0)
					sf::Vector2i(0,1),//       O    (0,1)
					sf::Vector2i(0,2), //      O    (0,2)
					sf::Vector2i(1,0)
				},
				{
					sf::Vector2i(-1,0),// L : OOO (-2,0)(-1,0)(0,0)    
					sf::Vector2i(0,1),//        O             (0,1)
					sf::Vector2i(0,0), //         
					sf::Vector2i(-2,0)
				}
			}
		},
		{
			Type::Z,
			{
				{ 
					sf::Vector2i(-1, -1), //Z    OO  (-1,-1)(0,-1)
					sf::Vector2i(0, -1),  //      OO         (0,0)(1,0)
					sf::Vector2i(0, 0),
					sf::Vector2i(1, 0),
				},
				{
					sf::Vector2i(1,-1), //	     O        (1,-1)
					sf::Vector2i(0,0), //       OO   (0,0)(1, 0)
					sf::Vector2i(1, 0), //      O    (0,1)
					sf::Vector2i(0, 1), //           
				}
			}
		},
		{
			Type::rev_Z,
			{
				{ 
					sf::Vector2i(0, -1),//    OO           (0,-1)(1,-1)
					sf::Vector2i(0, 0), //   OO      (-1,0)(0,0)
					sf::Vector2i(1, -1),
					sf::Vector2i(-1, 0),
				},
				{
					sf::Vector2i(-1,-1), //  O     (-1,-1)
					sf::Vector2i(-1,0), //  OO    (-1,0)(0,0)
					sf::Vector2i(0, 0), //   O          (0,1)
					sf::Vector2i(0, 1), //        
				}
			}
		},
		{	
			Type::arrow,                //        (0,-1)
			{                           //  (-1,0)(0,0)(1,0)
				{
					sf::Vector2i(0,-1),
					sf::Vector2i(-1,0),
					sf::Vector2i(0, 0),
					sf::Vector2i(1,0)
				},							
				{						
					sf::Vector2i(0,-1),		//		(0,-1)
					sf::Vector2i(1,0),		//		(0,0)(1,0)
					sf::Vector2i(0, 0),		//      (0,1)
					sf::Vector2i(0,1)
				},
				{
					sf::Vector2i(0,1),     //
					sf::Vector2i(-1,0),     //	(-1,0)(0,0)(1,0)
					sf::Vector2i(0, 0),		//		  (0,1)
					sf::Vector2i(1,0)
				},
				{
					sf::Vector2i(0,-1),   //       (0,-1)
					sf::Vector2i(-1,0),	  //  (-1,0)(0,0)
					sf::Vector2i(0, 0),   //       (0,1)
					sf::Vector2i(0,1) 
				}
			}
		},
	};
	Type currentType = Type(rand() % 7 + 1);
	int currentIndex = 0;

	const int blockWidth = yellowTexture.getSize().x;
	const int blockHeight = yellowTexture.getSize().y;

	const int windowWidth = fieldWidth * blockWidth;
	const int windowHeight = fieldHeight * blockHeight;
	int count = 0;
	float speed = 0.7f;
	sf::Font font;
	if (!font.loadFromFile("arial.ttf")) {
		return EXIT_FAILURE;
	}
	sf::Text point("Point:", font);
	sf::Text Game_end("Game Over!", font,45);
	Game_end.setPosition(
		float((fieldWidth / 2 - 3) * blockWidth),2.f* blockHeight);
	std::map<Type, sf::Sprite> sprites = {
		{Type::O, sf::Sprite(yellowTexture)},
		{Type::I, sf::Sprite(lightBlueTexture)},
		{Type::L, sf::Sprite(redTexture)},
		{Type::arrow, sf::Sprite(blueTexture)},
		{Type::rev_L, sf::Sprite(orangeTexture)},
		{Type::Z, sf::Sprite(purpleTexture)},
		{Type::rev_Z, sf::Sprite(greenTexture)}
	};
	
	backgroundTexture.setRepeated(true);
	sf::Sprite backgroundSprite(
		backgroundTexture,
		sf::IntRect(0, 0, windowWidth, windowHeight));
	
	std::vector<sf::Vector2i> currentShape;
	sf::Sprite currentSprite;

	const sf::Vector2i origin(fieldWidth / 2, 0);
	sf::Vector2i pos(origin);

	sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), L"俄羅斯方塊");
	sf::Clock clock;
	bool isOver = false;
	bool isCount = false;
	while (window.isOpen()) {
		currentShape = shapes[currentType][currentIndex];
		currentSprite = sprites[currentType];
		
		
		enum class Action {
			Hold,
			MoveDown,
			MoveLeft,
			MoveRight,
			Rotate,
		};
		Action action = Action::Hold;
		sf::Event evt;
		if (window.pollEvent(evt)) {
			if (evt.type == sf::Event::Closed) {
				window.close(); 
			}
			if (evt.type == sf::Event::KeyPressed) {
				switch (evt.key.code) {
				case sf::Keyboard::Left:
					action = Action::MoveLeft;
					break;
				case sf::Keyboard::Right:
					action = Action::MoveRight;
					break;
				case sf::Keyboard::Down:
					action = Action::MoveDown;
					break;
				case sf::Keyboard::Up:
					action = Action::Rotate;
					break;
				case sf::Keyboard::Space:
					action = Action::MoveDown;
					speed = 0.0f;
					break;
				case sf::Keyboard::R:
					isOver = false;
					isCount = false;
					speed = 0.7f;
					pos = origin;
					count = 0;
					currentType = Type(rand() % 7 + 1);
					currentIndex = 0;
					for (int y = 0; y < fieldHeight;y++) {
						for (int x = 0; x < fieldWidth; x++) {
							field[x][y] = Type::None;
						}
					}
					
					sound.setBuffer(restart);
					sound.play();
					break;
				}
			}
		}
		if (clock.getElapsedTime().asSeconds() >= speed) {
			action = Action::MoveDown;
			clock.restart();
		}

		sf::Vector2i nextPos(pos);
		int nextIndex = currentIndex;
		if (action == Action::Rotate) {
			// 0 => 1 => 0 => 1 ...
			nextIndex = (nextIndex + 1) % shapes[currentType].size();
		}
		std::vector<sf::Vector2i> nextShape = shapes[currentType][nextIndex];
		switch (action) {
		case Action::Hold:
			break;
		case Action::MoveDown:
			nextPos.y++;
			break;
		case Action::MoveLeft:
			nextPos.x--;
			break;		
		case Action::MoveRight:
			nextPos.x++;
			break;
		}
		
		int countEmpty = 0;
		for (const sf::Vector2i d : nextShape) { //檢查4個位置是否都合法
			sf::Vector2i np = nextPos + d;
			if (np.x >= 0 && np.x < fieldWidth &&
				np.y < fieldHeight &&
				(np.y < 0 || field[np.x][np.y] == Type::None)) {
				countEmpty++;
			}
		}

		if (countEmpty == 4) {  //如果位置都合法 把下一步驟的資訊交給要畫出來的參數
			pos = nextPos;
			currentIndex = nextIndex;
			currentShape = nextShape;
		}
		else { 
			if (action == Action::MoveDown && isOver == false) {//如果下一步驟位置不合法且動作為向下
				speed = 0.6f;
				for (const sf::Vector2i& d : currentShape) {//檢查目前圖形的各點
					sf::Vector2i np = pos + d;
					if (np.y >= 0) {//將目前位置y座標>0的點畫出目前圖形
						field[np.x][np.y] = currentType;
					}
				}

				for (int y = 0; y < fieldHeight; y++) {//如果下一步驟位置不合法且動作為向下
					bool isFull = true;//檢查每列是否填滿
					for (int x = 0; x < fieldWidth; x++) {
						if (field[x][y] == Type::None) {
							isFull = false;
						}
					}
					if (isFull) {//如果有某列填滿
						for (int ty = y; ty > 0; ty--) {
							for (int x = 0; x < fieldWidth; x++) {
								field[x][ty] = field[x][ty - 1];//所有列往下降一列
							}
						}
						for (int x = 0; x < fieldWidth; x++) {//最上面一列清除
							field[x][0] = Type::None;
						}
						count++;
						sound.setBuffer(getPoint);
						sound.play();
					}
				}
				if (count <= 10) {
					speed = 0.7f;
				}
				else if (count <= 20) {
					speed = 0.5f;
				}
				else if (count <= 30) {
					speed = 0.3f;
				}
				else {
					speed = 0.15f;
				}
				pos = origin;
				currentType = Type(rand() % 7 + 1);
				currentIndex = 0;
			}
		}

		window.clear();

		window.draw(backgroundSprite);

		// 繪製背景的場地
		for (int x = 0; x < fieldWidth; x++) {
			for (int y = 0; y < fieldHeight; y++) {
				if (field[x][y] == Type::None) continue;

				sf::Sprite& s = sprites[field[x][y]];
				s.setPosition(
					float(x * blockWidth),
					float(y * blockHeight));
				window.draw(s);
			}
		}
		// 繪製控制的物體
		// O:     OO    (0,-1) (1,-1)
		//        OO    (0, 0) (1, 0)
		if (!isOver) {
			for (const sf::Vector2i& d : currentShape) {
				sf::Vector2i np = pos + d;
				currentSprite.setPosition(
					float(np.x * blockWidth),
					float(np.y * blockHeight));
				window.draw(currentSprite);
			}
		}
		//繪製分數
		sf::Text point_value(std::to_string(count), font);
		point_value.setPosition(point.getLocalBounds().width + 5, 0.f);
		window.draw(point);
		window.draw(point_value);
		if (!isOver) {
			if (field[origin.x][origin.y] != Type::None) {
				isOver = true;
			}
		}
		if (isOver == true && isCount == false) {
			isCount = true;
			score.push_back(count);
			for (int i = score.size() - 1; i > 0; i--) {
				if (score[i] > score[i - 1]) {
					int temp = score[i];
					score[i] = score[i - 1];
					score[i - 1] = temp;
				}
			}
			score.pop_back();
		}
		if (isOver) {
			window.draw(Game_end);
			window.draw(score_sprite);
			//畫分數
			int samescore = 0;
			for (int i = 0; i < int(score.size()); i++) {
				if (score[i] != count) {
					sf::Text s(std::to_string(score[i]), font);
					s.setPosition(250.f,float(194+i*1.4*blockHeight));
					window.draw(s);
				}
				else if (score[i] == count && samescore == 0) {
					sf::Text s(std::to_string(score[i]), font);
					s.setFillColor(sf::Color::Red);
					s.setPosition(250.f, float(194+i *1.4* blockHeight));
					window.draw(s);
					samescore++;
				}
				else {
					sf::Text s(std::to_string(score[i]), font);
					s.setPosition(250.f, float(194+i*1.4* blockHeight));
					window.draw(s);
				}
			}
		}
		window.display();
	}
	std::ofstream ofs("score.txt", std::ios::out);
	for (int i = 0; i < int(score.size()); i++) {
		ofs << std::to_string(score[i]) << std::endl;
	}
	ofs.close();
	sound.setBuffer(end);
	sound.play();
	sf::sleep(sf::seconds(2.7f));
	return EXIT_SUCCESS;
}