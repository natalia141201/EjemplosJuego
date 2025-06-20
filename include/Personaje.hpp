#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

class Personaje
{
public:
    Personaje(sf::Vector2f position)
    {
        if (!texture.loadFromFile("assets/images/conejo.png"))
        {
            std::cerr << "No se pudo cargar la imagen del conejo\n";
        }
        sprite.setTexture(texture);
        sprite.setPosition(position);
        vida = 5;
    }
    int getVida() const  
    {
        return vida;
    }
    void restarVida(int cantidad = 1) {
        vida -= cantidad;
    }
    void move(float offsetX, float offsetY)
    {
        sprite.move(offsetX, offsetY);
    }
    void draw(sf::RenderWindow &window)
    {
        window.draw(sprite);
    }
    void update(){
        if (clock.getElapsedTime().asSeconds() >= frameTime)
        {
            currentFrame = (currentFrame + 1) % numFrames;
            sprite.setTextureRect(sf::IntRect((currentFrame * 24)+50, 269, 75,  60));
            clock.restart();
        }
    }
    sf::FloatRect getBounds() const {
        return sprite.getGlobalBounds();
    }
private:
    sf::Sprite sprite;
    sf::Texture texture;
    sf::Clock clock;
    float frameTime = 0.1f;
    int currentFrame = 0;
    int numFrames = 4;
    int frameWidth = 32;
    int frameHeight = 32;
    int vida;
};
