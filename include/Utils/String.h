#ifndef STIMWALKER_UTILS_STRING_H
#define STIMWALKER_UTILS_STRING_H

#include "stimwalkerConfig.h"
#include <iostream>

namespace STIMWALKER_NAMESPACE::utils
{
    ///
    /// \brief Wrapper around the std::string class with augmented functionality
    ///
    class STIMWALKER_API String : public std::string
    {
    public:
        ///
        /// \brief Construct string
        ///
        String();

        ///
        /// \brief Construct string
        /// \param text The string in char format
        ///
        String(
            const char *text);

        ///
        /// \brief Construct string
        /// \param text The string in string format
        ///
        String(
            const String &text);

        ///
        /// \brief Construct string
        /// \param text The string in vector char format
        ///
        String(
            const std::basic_string<char> &text);

        ///
        /// \brief Construct string
        /// \param value The value in int format
        ///
        String(
            int value);

        ///
        /// \brief Allow to use operator=
        /// \param other The other string
        ///
        String &operator=(
            const String &other);

        ///
        /// \brief Append an unsigned int to the string
        /// \param val The unsinged int to add
        ///
        String operator+(
            size_t val);

        ///
        /// \brief Append an int to the string
        /// \param val The int to add
        ///
        String operator+(
            int val);

        ///
        /// \brief Append a double to the string
        /// \param val The double to add
        ///
        String operator+(
            double val);

        ///
        /// \brief Append a char to the string
        /// \param text The char to add
        ///
        String operator+(
            const char *text);

        ///
        /// \brief Get a specific character in the string
        /// \param idx The index of the character
        ///
        String operator()(
            size_t idx) const;

        ///
        /// \brief Extract a portion of the string
        /// \param startIdx The index of the first character
        /// \param lastIdx The index of the last character
        ///
        String operator()(
            size_t startIdx,
            size_t lastIdx) const;

        ///
        /// \brief Destroy class properly
        ///
        virtual ~String();

        ///
        /// \brief Convert a string to a lower case string
        /// \param str The string to convert
        /// \return The new string
        ///
        static String tolower(const String &str);

        ///
        /// \brief Return a lower case string
        /// \return The lower case string
        ///
        String tolower() const;

        ///
        /// \brief Convert a string to a upper case string
        /// \param str The string to convert
        /// \return The new string
        ///
        static String toupper(const String &str);

        ///
        /// \brief Return an upper case string
        /// \return The upper case string
        ///
        String toupper() const;

        ///
        /// \brief Overload of the to_string C++11 function to allow for X-digits precision
        /// \param val The double to convert
        /// \return The double converter to string
        ///
        static String to_string(
            double val);

        ///
        /// \brief Overload of the to_string C++11 function to allow for X-digits precision
        /// \param val The float to convert
        /// \return The float converter to string
        ///
        static String to_string(
            float val);

        ///
        /// \brief The trailing tags of a string
        /// \param origin The string to remove the trailing tags from
        /// \param trailTag The tag to remove
        /// \return The string without the trailing tag
        ///
        /// Example : The string "2.82823000" will return "2.82823" for the tag "0"
        ///
        static String removeTrailing(
            const String &origin,
            const String &trailTag);
    };

}

///
/// \brief To use operator<<
/// \param os The ostream
/// \param a The string to operate on
///
std::ostream &operator<<(std::ostream &os, const STIMWALKER_NAMESPACE::utils::String &a);

#endif // STIMWALKER_UTILS_STRING_H
