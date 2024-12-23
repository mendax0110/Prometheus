#include "Configuration.h"

Configuration& Configuration::getInstance()
{
    static Configuration instance;
    return instance;
}

void Configuration::setLanguageStandard(const std::string& standard)
{
    languageStandard = standard;
}

std::string Configuration::getLanguageStandard() const
{
    return languageStandard;
}
