#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <string>

/// @brief Singleton class for storing configuration settings
class Configuration
{
public:
    /**
     * @brief Get the singleton instance of the Configuration class
     * @return -> The singleton instance of the Configuration class
     */
    static Configuration& getInstance();

    /**
     * @brief Set the language standard to use
     * @param standard -> The language standard to use
     */
    void setLanguageStandard(const std::string& standard);

    /**
     * @brief Get the language standard
     * @return -> The language standard
     */
    std::string getLanguageStandard() const;

private:
    Configuration() = default;
    std::string languageStandard;

    Configuration(const Configuration&) = delete;
    Configuration& operator=(const Configuration&) = delete;
};

#endif // CONFIGURATION_H
