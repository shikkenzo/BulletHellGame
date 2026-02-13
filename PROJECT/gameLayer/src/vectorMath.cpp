#include "vectorMath.h"

float vectorToAngle(sf::Vector2f direction)
{
	return (std::atan2(direction.y, direction.x) * 180.f / 3.14159265f) + 90;
}

sf::Vector2f angleToVector(float angleDegrees)
{
	float l_radians = (angleDegrees - 90.f) * 3.14159265f / 180.f;
	return sf::Vector2f(std::cos(l_radians), std::sin(l_radians));
}
float getVectorMagnitude(sf::Vector2f vector)
{
	return std::sqrt(vector.x * vector.x + vector.y * vector.y);
}
sf::Vector2f normalizeVector(sf::Vector2f vector)
{
	return vector / getVectorMagnitude(vector);
}