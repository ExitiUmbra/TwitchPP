#include "twitchppapi.hpp"

namespace TwitchPP {
    class TwitchOauthAPI : public TwitchAPI {
        private:
            std::string m_moderator_id {""};
        public:
            TwitchOauthAPI(const std::string& app_access_token, const std::string& client_id);
            TwitchOauthAPI(const std::string& app_access_token, const std::string& client_id, const std::string& moderator_id);
            VectorResponse<TwitchModeratorChatSettings> get_chat_settings(std::string_view broadcaster_id, bool chat_delay);
            VectorResponse<TwitchModeratorChatSettings> update_chat_settings(TwitchModeratorChatSettings& settings);
            VectorResponse<TwitchModeratorChatSettings> update_chat_settings(std::string_view broadcaster_id, std::string_view settings);
    };
}