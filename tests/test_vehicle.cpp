#include <boost/test/unit_test.hpp>
#include <boost/test/data/test_case.hpp>
#include <boost/filesystem.hpp>

#include <SFML/Graphics.hpp>

#include "../src/ResourceManager.h"
#include "../src/Background.h"
#include "../src/Vehicle.h"

#include "Test.h"

namespace tools = boost::test_tools;
namespace data = boost::unit_test::data;

BOOST_AUTO_TEST_SUITE(tt)
BOOST_AUTO_TEST_SUITE(vehicle)

BOOST_AUTO_TEST_CASE(vehicleTurnTest)
{
    sf::Texture texture;
    texture.create(100, 100);

    tt::NullWindow window;

    tt::ResourceManager mgr{ boost::filesystem::path{} };
    auto bg = std::make_shared<tt::Background>("test", mgr, window, sf::Vector2f{ 5,5 });
    bg->setTexture(texture, true);
    bg->setScale(1.0f, 1.0f);

    sf::Texture vtext;
    vtext.create(10, 10);

    tt::VehicleInfo vinfo;
    vinfo.size = sf::Vector2u{ 10,10 };
    vinfo.states.emplace();
    vinfo.states->emplace("up", GameObjectState{ "up", sf::Vector2i{0,0}, 1 });
    vinfo.states->emplace("down", GameObjectState{ "down", sf::Vector2i{0,0}, 1 });
    vinfo.states->emplace("left", GameObjectState{ "left", sf::Vector2i{0,0}, 1 });
    vinfo.states->emplace("right", GameObjectState{ "right", sf::Vector2i{0,0}, 1 });

    tt::Vehicle vehicle{ vinfo, vtext, bg };
    vehicle.setSpeed(4.0f);

    tt::Path path;
    path.setRepeating(false);
    path.points().emplace_back(sf::Vector2f{ -1,0 });
    path.points().emplace_back(sf::Vector2f{ 2,0 });
    path.points().emplace_back(sf::Vector2f{ 2,10 });
    vehicle.setPath(path);

    BOOST_TEST(vehicle.getGlobalBounds().left == -5.0f);
    BOOST_TEST(vehicle.direction() == tt::Direction::RIGHT);
    BOOST_TEST(vehicle.currentTile() == (tt::Tile{ -1.0f, 0.0f }));

    vehicle.move();
    BOOST_TEST(vehicle.getGlobalBounds().left == -1.0f);
    BOOST_TEST(vehicle.getGlobalBounds().top == 0.0f);
    BOOST_TEST(vehicle.direction() == tt::Direction::RIGHT);
    BOOST_TEST(vehicle.currentTile() == (tt::Tile{ -1.0f, 0.0f }));

    vehicle.move();
    BOOST_TEST(vehicle.getGlobalBounds().left == 3.0f);
    BOOST_TEST(vehicle.getGlobalBounds().top == 0.0f);
    BOOST_TEST(vehicle.direction() == tt::Direction::RIGHT);
    BOOST_TEST(vehicle.currentTile() == (tt::Tile{ 0.0f, 0.0f }));

    vehicle.move();
    BOOST_TEST(vehicle.getGlobalBounds().left == 7.0f);
    BOOST_TEST(vehicle.getGlobalBounds().top == 0.0f);
    BOOST_TEST(vehicle.direction() == tt::Direction::RIGHT);
    BOOST_TEST(vehicle.currentTile() == (tt::Tile{ 1.0f, 0.0f }));

    vehicle.move();
    BOOST_TEST(vehicle.getGlobalBounds().left == 10.0f);
    BOOST_TEST(vehicle.getGlobalBounds().top == 0.0f);
    BOOST_TEST(vehicle.direction() == tt::Direction::DOWN);
    BOOST_TEST(vehicle.currentTile() == (tt::Tile{ 2.0f, 0.0f }));

    vehicle.move();
    BOOST_TEST(vehicle.getGlobalBounds().left == 10.0f);
    BOOST_TEST(vehicle.getGlobalBounds().top == 4.0f);
    BOOST_TEST(vehicle.direction() == tt::Direction::DOWN);
    BOOST_TEST(vehicle.currentTile() == (tt::Tile{ 2.0f, 0.0f }));

    vehicle.move();
    BOOST_TEST(vehicle.getGlobalBounds().left == 10.0f);
    BOOST_TEST(vehicle.getGlobalBounds().top == 8.0f);
    BOOST_TEST(vehicle.direction() == tt::Direction::DOWN);
    BOOST_TEST(vehicle.currentTile() == (tt::Tile{ 2.0f, 1.0f }));
}

BOOST_AUTO_TEST_CASE(fastVehicleTest)
{
    sf::Texture texture;
    texture.create(100, 100);

    tt::NullWindow window;

    tt::ResourceManager mgr{ boost::filesystem::path{} };
    auto bg = std::make_shared<tt::Background>("test", mgr, window, sf::Vector2f{ 5,5 });
    bg->setTexture(texture, true);
    bg->setScale(1.0f, 1.0f);

    sf::Texture vtext;
    vtext.create(10, 10);

    tt::VehicleInfo vinfo;
    vinfo.size = sf::Vector2u{ 10,10 };
    vinfo.states.emplace();
    vinfo.states->emplace("up", GameObjectState{ "up", sf::Vector2i{0,0}, 1 });
    vinfo.states->emplace("down", GameObjectState{ "down", sf::Vector2i{0,0}, 1 });
    vinfo.states->emplace("left", GameObjectState{ "left", sf::Vector2i{0,0}, 1 });
    vinfo.states->emplace("right", GameObjectState{ "right", sf::Vector2i{0,0}, 1 });

    tt::Vehicle vehicle{ vinfo, vtext, bg };
    vehicle.setSpeed(7.0f);

    tt::Path path;
    path.setRepeating(false);
    path.points().emplace_back(sf::Vector2f{ -1,0 });
    path.points().emplace_back(sf::Vector2f{ 2,0 });
    path.points().emplace_back(sf::Vector2f{ 2,10 });
    vehicle.setPath(path);

    BOOST_TEST(vehicle.getGlobalBounds().left == -5.0f);
    BOOST_TEST(vehicle.direction() == tt::Direction::RIGHT);
    BOOST_TEST(vehicle.currentTile() == (tt::Tile{ -1.0f, 0.0f }));

    vehicle.move();
    BOOST_TEST(vehicle.getGlobalBounds().left == 2.0f);
    BOOST_TEST(vehicle.getGlobalBounds().top == 0.0f);
    BOOST_TEST(vehicle.direction() == tt::Direction::RIGHT);
    BOOST_TEST(vehicle.currentTile() == (tt::Tile{ 0.0f, 0.0f }));

    vehicle.move();
    BOOST_TEST(vehicle.getGlobalBounds().left == 9.0f);
    BOOST_TEST(vehicle.getGlobalBounds().top == 0.0f);
    BOOST_TEST(vehicle.direction() == tt::Direction::RIGHT);
    BOOST_TEST(vehicle.currentTile() == (tt::Tile{ 1.0f, 0.0f }));

    vehicle.move();
    BOOST_TEST(vehicle.getGlobalBounds().left == 10.0f);
    BOOST_TEST(vehicle.getGlobalBounds().top == 0.0f);
    BOOST_TEST(vehicle.direction() == tt::Direction::DOWN);
    BOOST_TEST(vehicle.currentTile() == (tt::Tile{ 2.0f, 0.0f }));

    vehicle.move();
    BOOST_TEST(vehicle.getGlobalBounds().left == 10.0f);
    BOOST_TEST(vehicle.getGlobalBounds().top == 7.0f);
    BOOST_TEST(vehicle.direction() == tt::Direction::DOWN);
    BOOST_TEST(vehicle.currentTile() == (tt::Tile{ 2.0f, 1.0f }));
}

BOOST_AUTO_TEST_SUITE_END() // background
BOOST_AUTO_TEST_SUITE_END() // tt