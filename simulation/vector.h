#ifndef CPPWINUITESTS_SIMULATION_VECTOR_H
#define CPPWINUITESTS_SIMULATION_VECTOR_H

namespace APowers {

/// @brief Structure representing a vector in 2D space.
struct Vector
{
    /// @brief The x value of the vector.
    double x{};

    /// @brief The y value of the vector.
    double y{};

    /// @brief Default initializes a new Vector.
    Vector() noexcept = default;

    /// @brief Initializes a new Vector using the given values.
    /// @param pX The x value of the vector.
    /// @param pY The y value of the vector.
    Vector(double pX, double pY) noexcept;

    /// @brief Add another vector to this one.
    ///
    /// @param other The other vector to add.
    /// @return This vector.
    inline Vector &operator+=(Vector const &other) noexcept
    {
        x += other.x;
        y += other.y;
        return *this;
    }

    /// @brief Subtracts another vector from this one.
    ///
    /// @param other The other vector to subtract.
    /// @return This vector.
    inline Vector &operator-=(Vector const &other) noexcept
    {
        x -= other.x;
        y -= other.y;
        return *this;
    }
};

/// @brief Adds two vectors, creating a new one.
///
/// @param a The first vector of the addition.
/// @param b The second vector of the addition.
/// @return The sum of the given vectors.
inline Vector operator+(Vector const &a, Vector const &b) noexcept { return Vector(a.x + b.x, a.y + b.y); }

/// @brief Subtract b from a and returns the result.
///
/// @param a The vector to subtract from.
/// @param b The vector to subtract.
/// @return The resulting vector.
inline Vector operator-(Vector const &a, Vector const &b) noexcept { return Vector(a.x - b.x, a.y - b.y); }

} // namespace APowers

#endif // !CPPWINUITESTS_SIMULATION_VECTOR_H
