#pragma once

#include <string>
#include <ostream>
#include <random>
#include <iterator>
#include <cmath>

#include <boost/spirit/home/x3.hpp>

#include <nlohmann/json.hpp>

#include <SFML/Graphics.hpp>

namespace nl = nlohmann;

namespace sf
{

void from_json(const nl::json& j, Vector2f& v);
void from_json(const nl::json& j, Vector2i& v);

} // namespace sf

namespace tt
{

std::string defaultResourceFolder();

template<typename T>
std::ostream& operator<<(std::ostream& out, const sf::Rect<T> item)
{
    auto [left, top, width, height] = item;
    out << "{ x=" << left
        << " y=" << top
        << " w=" << width
        << " h=" << height
        << " }";

    return out;
}

template<typename T>
std::ostream& operator<<(std::ostream& out, const sf::Vector2<T> item)
{
    auto [x,y] = item;
    out << "{ x=" << x
        << " y=" << y
        << " }";

    return out;
}

namespace x3 = boost::spirit::x3;
const auto FloatRectParser
    = x3::rule<class FloatRectParser_, sf::FloatRect>{}
    = (x3::float_ >> ',' >> x3::float_ >> ',' >> x3::float_ >> ',' >> x3::float_)
        [([](auto& ctx)
            {
                auto& attr = x3::_attr(ctx);
                using boost::fusion::at_c;

                auto width = at_c<2>(attr) - at_c<0>(attr);
                auto height = at_c<3>(attr) - at_c<1>(attr);
                x3::_val(ctx)
                    = sf::FloatRect{ at_c<0>(attr), at_c<1>(attr), width, height };
            })
        ];

const auto VectorFloatParser
    = x3::rule<class VectorFloatParser_, sf::Vector2f>{}
    = (x3::float_ >> ',' >> x3::float_)
        [([](auto& ctx)
            {
                auto& attr = x3::_attr(ctx);
                using boost::fusion::at_c;
                x3::_val(ctx)
                    = sf::Vector2f{ at_c<0>(attr), at_c<1>(attr)};
            })
        ];

// custom version of `contains` that will return true if the testing
// point lies on the rectangles border
template<typename Rect, typename T>
bool RectContains(Rect rect, T x, T y)
{
    // Rectangles with negative dimensions are allowed, so we must handle them correctly

    // Compute the real min and max of the rectangle on both axes
    auto minX = std::min(rect.left, static_cast<T>(rect.left + rect.width));
    T maxX = std::max(rect.left, static_cast<T>(rect.left + rect.width));
    T minY = std::min(rect.top, static_cast<T>(rect.top + rect.height));
    T maxY = std::max(rect.top, static_cast<T>(rect.top + rect.height));

    return (x >= minX) && (x <= maxX) && (y >= minY) && (y <= maxY);
}

template <typename T>
bool RectContains(const sf::Rect<T>& rect, const sf::Vector2<T>& point)
{
    return RectContains(rect, point.x, point.y);
}

template<typename T>
auto select_randomly(const T& container)
{
    static std::random_device rd;
    static std::mt19937 gen(rd());

    std::uniform_int_distribution<> dis(0ul, static_cast<int>(std::size(container) - 1));
    auto start = container.begin();
    std::advance(start, dis(gen));
    return start;
}

template<typename NumT>
NumT RandomNumber(NumT min, NumT max)
{
    static std::random_device rd;
    static std::mt19937 gen(rd());

    std::uniform_int_distribution<> dis(static_cast<int>(min), static_cast<int>(max));
    return static_cast<NumT>(dis(gen));
}

template <typename T,
    typename = typename std::enable_if<(std::is_integral<T>::value )>::type>
inline bool exactly_one_bit_set(T n)
{
    return n && !(n & (n - 1));
}

inline float distance(const sf::Vector2f& v1, const sf::Vector2f& v2)
{
    auto x = v1.x - v2.x;
    auto y = v1.y - v2.y;
    return std::sqrt((x*x) + (y*y));
}
    
void openBrowser(const std::string& url_str);
std::string getOsString();

} // namespace tt