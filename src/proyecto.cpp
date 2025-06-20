#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include "Personaje.hpp"
#include "Zanahorias.hpp"
#include "Calcetin.hpp"


sf::Texture calcetin::texture;
bool calcetin::textureCargada = false;

double velocidad = 2.3;

enum class GameState { Start, Playing, GameOver };

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "Cabbit SFML");
    window.setFramerateLimit(60);

    // Fondo
    sf::Texture fondoTexture;
    if (!fondoTexture.loadFromFile("assets/images/fondo.png")) 
    {
        std::cerr << "No se pudo cargar la imagen de fondo\n";
        return -1;
    }
    sf::Sprite fondoSprite(fondoTexture);
    sf::Vector2u textureSize = fondoTexture.getSize();
    sf::Vector2u windowSize = window.getSize();
    fondoSprite.setScale(
        float(windowSize.x) / textureSize.x,
        float(windowSize.y) / textureSize.y );

    sf::Font font;
    if (!font.loadFromFile("assets/fonts/Minecraft.ttf")) {
        std::cerr << "No se pudo cargar la fuente\n";   
    }
    
    srand(static_cast<unsigned>(time(0)));

    std::vector<Zanahorias> zanahoriasVec;
    std::vector<calcetin> calcetinVec;

    // Estado del juego y score
    GameState estado = GameState::Start;
    int score = 0;
    Personaje conejo(sf::Vector2f(400, 300));

    // Inicializa zanahorias y calcetines
    auto reiniciarObjetos = [&]() {
        zanahoriasVec.clear();
        calcetinVec.clear();
        for (int i = 0; i < 5; ++i) {
            float x = rand() % (window.getSize().x - 50);
            float y = rand() % (window.getSize().y - 50);
            zanahoriasVec.emplace_back(sf::Vector2f(x, y));    
        }
        for (int i = 0; i < 5; ++i) {
            float x = rand() % (window.getSize().x - 50);
            float y = rand() % (window.getSize().y - 50);
            calcetinVec.emplace_back(sf::Vector2f(x, y));    
        }
    };

    reiniciarObjetos();

    // Bucle principal
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();

        if (estado == GameState::Start) {
            // Pantalla de instrucciones
            sf::Text instrucciones;
            instrucciones.setFont(font);
            instrucciones.setString(
                "INSTRUCCIONES\n\n"
                "Mueve al conejo con las flechas\n"
                "Toca zanahorias para sumar puntos\n"
                "Evita los calcetines (pierdes vida)\n\n"
                "Presiona ESPACIO para empezar"
            );
            instrucciones.setCharacterSize(28);
            instrucciones.setFillColor(sf::Color::White);
            instrucciones.setPosition(60, 100);
            window.draw(instrucciones);

            // Espera a que el usuario presione ESPACIO para empezar
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
                score = 0;
                conejo = Personaje(sf::Vector2f(400, 300));
                reiniciarObjetos();
                estado = GameState::Playing;
                sf::sleep(sf::milliseconds(200));
            }
        }
        else if (estado == GameState::Playing) {
            window.draw(fondoSprite);
            conejo.update();
            conejo.draw(window);

            // Zanahorias
            for (auto& z : zanahoriasVec) {
                z.draw(window);
                z.move(0.0f, 2.0f);

                if (conejo.getBounds().intersects(z.getBounds())) {
                    score += 1;
                    float nuevaX = rand() % (window.getSize().x - 50);
                    z.setPosition(sf::Vector2f(nuevaX, -50));
                }
                sf::Vector2f pos = z.getPosition();
                if (pos.y > window.getSize().y) {
                    float nuevaX = rand() % (window.getSize().x - 50);
                    z.setPosition(sf::Vector2f(nuevaX, -50));
                }
            }

            // Calcetines
            for (auto& c : calcetinVec) {
                c.draw(window);
                c.move(0.0f, 2.0f);

                if (conejo.getBounds().intersects(c.getBounds())) {
                    conejo.restarVida();
                    float nuevaX = rand() % (window.getSize().x - 50);
                    c.setPosition(sf::Vector2f(nuevaX, -50));
                }
                sf::Vector2f pos = c.getPosition();
                if (pos.y > window.getSize().y) {
                    float nuevaX = rand() % (window.getSize().x - 50);
                    c.setPosition(sf::Vector2f(nuevaX, -50));
                }
            }

            // Textos de vida y score
            sf::Text vidaTexto;
            vidaTexto.setFont(font);
            vidaTexto.setString("Vida: " + std::to_string(conejo.getVida()));
            vidaTexto.setCharacterSize(30);
            vidaTexto.setFillColor(sf::Color::Cyan);
            vidaTexto.setPosition(10, 10);
            window.draw(vidaTexto);

            sf::Text scoreTexto;
            scoreTexto.setFont(font);
            scoreTexto.setString("Score: " + std::to_string(score));
            scoreTexto.setCharacterSize(30);
            scoreTexto.setFillColor(sf::Color::Yellow);
            scoreTexto.setPosition(10, 50);
            window.draw(scoreTexto);

            // Movimiento del conejo
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
                conejo.move(velocidad * -1, 0);
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
                conejo.move(velocidad, 0);
            }
            

            // Si la vida llega a 0, cambia el estado
            if (conejo.getVida() <= 0) {
                estado = GameState::GameOver;
                sf::sleep(sf::milliseconds(200));
            }
        }
        else if (estado == GameState::GameOver) {
            // Pantalla de Game Over
            sf::Text gameOver;
            gameOver.setFont(font);
            gameOver.setString("GAME OVER\n\nScore: " + std::to_string(score) + "\n\nPresiona R para reiniciar");
            gameOver.setCharacterSize(36);
            gameOver.setFillColor(sf::Color::Red);
            gameOver.setPosition(120, 180);
            window.draw(gameOver);

            // Espera a que el usuario presione R para reiniciar
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::R)) {
                estado = GameState::Start;
                sf::sleep(sf::milliseconds(200));
            }
        }

        window.display();
    }

    return 0;
}