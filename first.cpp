#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>

bool isWhite(sf::Image& image, float px, float py){
	return image.getPixel(px, py) == sf::Color::White;
}

int main(){

	int destiny = 0;
	bool movement = false;
  
	//VARIABLES CONSTANTS i 'GLOBALS'
	int speed = 180;
	float time_to_next_sprite = 0.11;
	enum direction { down, left, right, up , none};
	const int mx[5] = {0,	-1,    1,	 0,   0};
	const int my[5] = {1,	 0,	   0,	-1,   0};

	int k = 0;
	direction directions [10] = { down, down, down, right, right, down, down, down, right, right };
	//VECTOR2
	sf::Vector2u size(1100,700);
	sf::Vector2f spriteSize(32,48);
	sf::Vector2f viewPosition(0,0);
	sf::Vector2i spriteSource(0,down);
	sf::Vector2f originalSpriteSize(0,0);
	sf::Vector2u newsize(size.x, size.y);
	sf::Vector2f playerPosition(spriteSize.x,spriteSize.y);

	//OBJECTES DE SFML
	sf::View view;
	sf::Image image;
	sf::Event event;
	sf::Clock deltaClock;
	sf::Sprite player, background;
	sf::Texture pTexture, tbackground;

	//LOAD THE IMAGES AND TEXTURES
	if(!image.loadFromFile("pics/ground.png")) std::cout << "ground Image Not Loaded " << std::endl;
	if(!tbackground.loadFromImage(image)) std::cout << "background texture Not Loaded " << std::endl;
	if(!pTexture.loadFromFile("pics/sprites.png")) std::cout << "personatge Not Loaded " << std::endl;
	spriteSize.x = originalSpriteSize.x = pTexture.getSize().x/4;
	spriteSize.y = originalSpriteSize.y = pTexture.getSize().y/4;

	//VARIABLES	
	direction d = none;
	double 	escalat = 1;
	bool mousePressed = false;
	bool keypressed = false, fly = false;
	float distx = spriteSize.x/3, disty = spriteSize.y/4;
	float deltatime = 0, movx = 0.0, movy = 0.0, scont = 0.0;

	//Set player's properties
	player.setTexture(pTexture);
	player.setPosition(playerPosition);
	//Set Background's properties
	background.setTexture(tbackground);

	//CREATE THE WINDOW
	sf::RenderWindow window(sf::VideoMode(size.x,size.y), L"Genís Bayona i Jaume", sf::Style::Resize|sf::Style::Close);

	//GAME LOOP
	while(window.isOpen()){

		//Loop for handling events
		while(window.pollEvent(event)){
			switch (event.type){
				//Close event
				case sf::Event::Closed:
					window.close();
					break;
				//KeyPressed event
				case  sf::Event::KeyPressed:
					//Close key
					if (event.key.code == sf::Keyboard::Q) {
						window.close();
					}
					//Increasing Speed
					if (event.key.code == sf::Keyboard::S) {
						if(speed < 280){
							speed += 20;
							time_to_next_sprite -= 0.01;
						}
					}
					//Decreasing Speed
					if (event.key.code == sf::Keyboard::D) {
						if(speed > 80){
							speed -= 20;
							time_to_next_sprite += 0.01;
						}
					}
					//Change to the secondary skin
					if(event.key.code == sf::Keyboard::C) {
						fly = false;
						if(!pTexture.loadFromFile("pics/capa.png")) std::cout << "capa Not Loaded " << std::endl;
							spriteSize.x = pTexture.getSize().x/4;
							spriteSize.y = pTexture.getSize().y/4;
							distx = spriteSize.x/3, disty = spriteSize.y/4;
					}
					//Change to original skin
					if(event.key.code == sf::Keyboard::P) {
						fly = false;
						if(!pTexture.loadFromFile("pics/sprites.png")) std::cout << "personatge Not Loaded " << std::endl;
							spriteSize.x = pTexture.getSize().x/4;
							spriteSize.y = pTexture.getSize().y/4;
							distx = spriteSize.x/3, disty = spriteSize.y/4;
					}
					//Change to real skin
					if(event.key.code == sf::Keyboard::T) {
					}
					//Change to bird skin and fly over the texts
					if(event.key.code == sf::Keyboard::F) {
						if(fly) fly = false;
						else fly = true;
						if(!pTexture.loadFromFile("pics/real.png")) std::cout << "real Not Loaded " << std::endl;
							spriteSize.x = pTexture.getSize().x/4;
							spriteSize.y = pTexture.getSize().y/4;
							distx = spriteSize.x/3, disty = spriteSize.y/4; 
					}
				break;
				//Resize event
				case sf::Event::Resized:
					newsize.x = event.size.width;
					newsize.y = event.size.height;
					distx = distx * newsize.x/size.x;
					disty = disty * newsize.x/size.x;
					speed = speed * newsize.x/size.x;
					escalat = escalat * newsize.x/size.x;
					playerPosition.x = playerPosition.x*newsize.x/size.x;
					playerPosition.y = playerPosition.y*newsize.x/size.x;
					//Try and catch for resizeing the window
					try {window.setSize(newsize);}
					catch(sf::Event e){std::cout << "No puc redimensionar" << std::endl;}
					size = newsize;
					//Buida la pollevent per si hi havia events amb la finestra anterior
					while(window.pollEvent(event)){}
					break;
				//Default
				default:
					//Do nothing
					break;
			}
		}	

		//Deltatime 
		deltatime = deltaClock.restart().asSeconds();
		scont += deltatime;

		//Updating if a key is pressed in a direction 'd'
	        if(!movement) d = none;
		//if(!movement) { d = directions[k];    movement = true; destiny = (spriteSource.x + 3) %8; ++k;}
		if(k > 10) k = 0;
		if(sf::Mouse::isButtonPressed(sf::Mouse::Left)){
			float mouse_x, mouse_y;
			mouse_x = sf::Mouse::getPosition(window).x; 
			mouse_y = sf::Mouse::getPosition(window).y;
			if(mouse_x > window.getSize().x*2/3) d = right;
			else if(mouse_x < window.getSize().x/3) d = left;
			else if(mouse_y < window.getSize().y/2) d = up;
			else if(mouse_y > window.getSize().y/2) d = down;
		}
		
		if(!movement && sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) 	{d = up;    movement = true; destiny = (spriteSource.x + 3) %8;}
		if(!movement && sf::Keyboard::isKeyPressed(sf::Keyboard::Down))	{d = down;  movement = true; destiny = (spriteSource.x + 3) %8;}
		if(!movement && sf::Keyboard::isKeyPressed(sf::Keyboard::Right)){d = right; movement = true; destiny = (spriteSource.x + 3) %8;}
		if(!movement && sf::Keyboard::isKeyPressed(sf::Keyboard::Left))	{d = left;  movement = true; destiny = (spriteSource.x + 3) %8;}

		//If some key is pressed
		if(d != none){
			//Set value to movement variables and update spritesource
			if(spriteSource.y == d){
				movx += mx[d]*speed*deltatime;
				movy += my[d]*speed*deltatime;
			}
			else spriteSource.y = d;
			if (scont >= time_to_next_sprite){
				scont = 0;
				if(spriteSource.x == destiny){
				  movement = false;
				}
				++spriteSource.x;
			}
		}

		//Calcula el desti del player
		int destix = playerPosition.x+movx;
		int destiy = playerPosition.y+movy;
		
		//Calcula l'equivalent del punt de la pantalla on esta el player en la imatge de background
		float px, py;
		px = image.getSize().x*destix/tbackground.getSize().x/background.getScale().x;
		py = image.getSize().y*destiy/tbackground.getSize().y/background.getScale().y;

		//Comproba que estigui sobre una zona de la imatge de color blanc
		if(fly || isWhite(image, px, py) && isWhite(image, px, image.getSize().y*(destiy-disty)/tbackground.getSize().y/background.getScale().y) &&
			isWhite(image, image.getSize().x*(destix+distx)/tbackground.getSize().x/background.getScale().x, image.getSize().y*(destiy-disty)/tbackground.getSize().y/background.getScale().y) &&
			isWhite(image, image.getSize().x*(destix-distx)/tbackground.getSize().x/background.getScale().x, image.getSize().y*(destiy-disty)/tbackground.getSize().y/background.getScale().y) &&
			isWhite(image, image.getSize().x*(destix+distx)/tbackground.getSize().x/background.getScale().x, py) &&
			isWhite(image, image.getSize().x*(destix-distx)/tbackground.getSize().x/background.getScale().x, py) 
			){
				playerPosition.x += movx;
				playerPosition.y += movy;
		}
		//Setting movement variables to 0
		movx = movy = 0;

		//Checking the sprite sources and position to be ok
		if(spriteSource.x >= 8) spriteSource.x = 0;
		float aux = background.getTexture()->getSize().y * background.getScale().y;
		if(playerPosition.x < 0) playerPosition.x = 0;
		else if (playerPosition.y < 0) playerPosition.y = 0;
		else if (playerPosition.x > window.getSize().x) playerPosition.x = window.getSize().x-1;
		else if (playerPosition.y > aux) playerPosition.y = aux -1;

		//Clear the window
		window.clear();

		//Set player properties
		player.setOrigin(spriteSize.x/2, spriteSize.y);
		player.setPosition(playerPosition.x, playerPosition.y);
		player.setTextureRect(sf::IntRect((spriteSource.x %4)*spriteSize.x, 
			(spriteSource.y%4)*spriteSize.y, spriteSize.x, spriteSize.y));
		player.setScale(window.getSize().x/600.0, window.getSize().x/600.0);
		player.scale(originalSpriteSize.x/spriteSize.x, originalSpriteSize.y/spriteSize.y);

		//Set background scale
		escalat = (double)(view.getSize().x) / (double)(tbackground.getSize().x);
		background.setScale(escalat, escalat);

		//Set view values
		view.reset(sf::FloatRect(playerPosition.x,playerPosition.y, 
								 window.getSize().x, window.getSize().y));
		if(playerPosition.y+10 > view.getSize().y/2) 
			viewPosition.y = playerPosition.y+10;
		else viewPosition.y = view.getSize().y/2;
		viewPosition.x = view.getSize().x/2;
		view.setCenter(viewPosition);
		view.setViewport(sf::FloatRect(0,0,1.0f,1.0f));

		//Set window view, draw and display
		window.setView(view);
		window.draw(background);
		window.draw(player);
		window.display();
	}
}
