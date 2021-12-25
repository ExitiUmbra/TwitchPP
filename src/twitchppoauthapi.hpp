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
            VectorResponse<TwitchBasicUser> get_user_block_list(std::string_view broadcaster_id,
                                                                std::optional<size_t> first = std::nullopt,
                                                                std::optional<std::string_view> after = std::nullopt);
            VectorResponse<TwitchBlockedTerm> get_blocked_terms(std::string_view broadcaster_id,
                                                                std::optional<size_t> first = std::nullopt,
                                                                std::optional<std::string_view> after = std::nullopt);
            VectorResponse<TwitchAutoModSettings> get_automod_settings(std::string_view broadcaster_id);
            Response<std::string> unblock_user(std::string_view target_user_id);
            Response<std::string> block_user(std::string_view target_user_id,
                                             std::optional<std::string_view> source_context = std::nullopt,
                                             std::optional<std::string_view> reason = std::nullopt);
            VectorResponse<TwitchBlockedTerm> add_blocked_term(std::string_view broadcaster_id,
                                                               std::string_view text);
            Response<std::string> remove_blocked_term(std::string_view broadcaster_id,
                                                      std::string_view term_id);
    };
}