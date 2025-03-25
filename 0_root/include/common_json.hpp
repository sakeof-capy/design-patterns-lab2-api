#ifndef COMMON_JSON_HPP_
#define COMMON_JSON_HPP_

#include <string_view>

constexpr std::string_view SINGLE_CAPY_JSON = R"({
    "id": 1,
    "name": "Capy McCapface",
    "age": 4,
    "friends": [2, 3],
    "habitatId": 101
})";

constexpr std::string_view CAPY_ARRAY_JSON = R"([
  {
    "id": 1,
    "name": "Capy McCapface",
    "age": 4,
    "friends": [2, 3],
    "habitatId": 101
  },
  {
    "id": 2,
    "name": "Hydrochoerus",
    "age": 6,
    "friends": [1, 3],
    "habitatId": 102
  },
    {
    "id": 3,
    "name": "Gort",
    "age": 2,
    "friends": [1, 2],
    "habitatId": 101
  }
])";

constexpr std::string_view SINGLE_HABITAT_JSON = R"({
    "id": 0,
    "name": "Lake",
    "location": "Big Mountain"
})";

constexpr std::string_view HABITAT_ARRAY_JSON = R"([
    {
        "id": 0,
        "name": "Lake",
        "location": "Big Mountain"
    },
    {
        "id": 1,
        "name": "Hut",
        "location": "Small Mountain"
    },
])";

#endif // COMMON_JSON_HPP_
