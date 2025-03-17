#ifndef ENV_HPP_
#define ENV_HPP_

#include <cstdlib>
#include <string_view>

template<typename EnvType>
EnvType resolve_env(const std::string_view env_name)
{
    const char* env_cstr = std::getenv(env_name.data());

    if (!env_cstr)
    {
        std::cerr << "Env-variable with name " << env_name << " is missing. Add one to the .env\n" << std::flush;
        std::exit(-1);
    }

    std::stringstream ss(env_cstr);
    EnvType result;
    ss >> result;

    if (ss.fail())
    {
        std::cerr
            << "Env-variable with name " << env_name
            << " does not match type " << typeid(EnvType).name()
            << '\n' << std::flush;
        std::exit(-1);
    }

    return result;
}

#endif
