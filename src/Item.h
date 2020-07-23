#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
#include <nlohmann/json.hpp>

#include "AnimatedSprite.h"

namespace nl = nlohmann;

namespace tt
{

    class Item : public AnimatedSprite
    {

        public:

            Item(       std::string id,
                        sf::Texture texture, 
                        const sf::Vector2i& size,
                        nl::json json);

            std::string getID();
            std::string getName();
            std::string getDescription();

        private:

            std::string _id;
            nl::json    _json;

    };


} // namespace tt
