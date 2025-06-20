#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

class calcetin{
public:
    calcetin(sf::Vector2f position)
    {
        if (!textureCargada) {
            if (!texture.loadFromFile("assets/images/Calcetin.png")) {
                std::cerr << "No se pudo cargar la imagen del calcetin\n";
            }
            textureCargada = true;
        }
        sprite.setTexture(texture);
        sprite.setPosition(position);
        sprite.setScale(0.1f, 0.1f);
    }
    void move(float offsetX, float offsetY)
    {
        sprite.move(offsetX, offsetY);
    }
    void draw(sf::RenderWindow &window)
    {
        window.draw(sprite);
    }
    sf::Vector2f getPosition() const {
        return sprite.getPosition();
    }
    void setPosition(const sf::Vector2f& pos) {
        sprite.setPosition(pos);
    }
    sf::FloatRect getBounds() const {
        return sprite.getGlobalBounds();
    }
private:
    sf::Sprite sprite;
    static sf::Texture texture;
    static bool textureCargada;
};