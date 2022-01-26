# TwitchPP
C++ library for Twitch API. Covers end-points mentioned in [Twitch reference](https://dev.twitch.tv/docs/api/reference).

## Installation

1. Install [libcurl](https://curl.se/libcurl/).
2. Compile `TwitchPP` with `C++17` or higher.
3. Include header file to your project (`twitchppapi.hpp`, `twitchppoauthapi.hpp` or `twitchppextendedapi.hpp`, depends which group of methods you need).

### Note
These headers linked between each other in the following way: `twitchppextendedapi.hpp` includes `twitchppapi.hpp`, and `twitchppoauthapi.hpp` includes `twitchppextendedapi.hpp`. So if you want to be able to use all methods, you need `twitchppoauthapi.hpp`. Classes described in section below.

## Usage

`TwitchPP` logically separated into 3 main classes:
- `TwitchAPI` - contains general purpose end-points, that don't require specific access tokens with any kind of permissions. Simple app access token is enough.
- `TwitchOauthAPI` - contains end-points that require OAuth token with permissions given by specific channel.
- `TwitchExtendedAPI` - contains end-points for games and extensions developers and requires extension or game ownership.

List of end-points can be found on Twitch reference page (how they separated you can check in [this gsheet](https://docs.google.com/spreadsheets/d/1NL5m4BvhNq0gbtPObv0nWg4NgndbzwHJVhJ2BOn7Td0/edit?usp=sharing). Example of methods naming: `Get Top Games` => `get_top_games`.

`TwitchAPI` example:
```cpp
#include "twitchppapi.h"

int main() {
    // Create TwitchAPI client
    TwitchPP::TwitchAPI client {<app_access_token>, <client_id>};
    // Calling get_top_games endpoint
    TwitchPP::VectorResponse<TwitchPP::TwitchGame> games_response = client.get_top_games();
    // Displaing games in console as JSON objects
    for (TwitchPP::TwitchGame game : games_response.data) {
        std::cout << game.to_json() << std::endl;
    }
    ...
}
```
### Note
List of parameters in methods usually the same as in Twitch reference, with the same names, but for some of them you might see parameters with types `string_view` and `bool` instead of expected `string_view` and `string_view`. For these methods only one of `string_view` parameters is actually required, so instead the second one, we have `bool` that determines which parameter should be used in request (usually has default value). As an example you can check `get_games` implementation in `twitchapi` files.

## Contributing
Pull requests are welcome. For major changes, please open an issue first to discuss what you would like to change.

Also if you have complains about code style, optimization, etc, feel free to open issues and leave useful comments like what can be improved and how, because I'm new in C++, and such comments will help me to improve myself. Thanks in advance!

## License
[MIT](https://github.com/ExitiUmbra/TwitchPP/blob/main/LICENSE)
