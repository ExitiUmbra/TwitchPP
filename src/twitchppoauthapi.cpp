#include "twitchppoauthapi.hpp"

namespace TwitchPP {
    TwitchOauthAPI::TwitchOauthAPI(const std::string& app_access_token, const std::string& client_id)
            : TwitchAPI{app_access_token, client_id} {
        VectorResponse<TwitchUser> user {this->get_users()};
        if (user.code > 200) {
            throw invalid_oauth {};
        }
        this->m_moderator_id = user.data[0].get_id();
    }

    TwitchOauthAPI::TwitchOauthAPI(const std::string& app_access_token, const std::string& client_id, const std::string& moderator_id)
            : TwitchAPI{app_access_token, client_id}, m_moderator_id{moderator_id} {
    }

    VectorResponse<TwitchModeratorChatSettings> TwitchOauthAPI::get_chat_settings(std::string_view broadcaster_id, bool chat_delay) {
        std::string url {TWITCH_API_BASE + "chat/settings?broadcaster_id=" + std::string(broadcaster_id)};
        if (chat_delay) {
            url += "&moderator_id=" + this->m_moderator_id;
        }
        Response<std::string> response = call_api(url, this->m_app_access_token, this->m_client_id);
        if (response.data == "") {
            return {{}, "", response.code, "Bad request"};
        }
        return this->process_response<TwitchModeratorChatSettings>(response);
    }

    VectorResponse<TwitchModeratorChatSettings> TwitchOauthAPI::update_chat_settings(TwitchModeratorChatSettings& settings) {
        std::string url {TWITCH_API_BASE + "chat/settings?broadcaster_id="
            + std::string(settings.get_broadcaster_id() == "" ? this->m_moderator_id : settings.get_broadcaster_id())
            + "&moderator_id=" + this->m_moderator_id};
        Response<std::string> response = call_api(url, this->m_app_access_token, this->m_client_id, HTTP_PATCH, settings.to_request());
        if (response.data == "") {
            return {{}, "", response.code, "Bad request"};
        }
        return this->process_response<TwitchModeratorChatSettings>(response);
    }

    VectorResponse<TwitchModeratorChatSettings> TwitchOauthAPI::update_chat_settings(std::string_view broadcaster_id, std::string_view settings) {
        std::string url {TWITCH_API_BASE + "chat/settings?broadcaster_id=" + std::string(broadcaster_id) + "&moderator_id=" + this->m_moderator_id};
        Response<std::string> response = call_api(url, this->m_app_access_token, this->m_client_id, HTTP_PATCH, settings.data());
        if (response.data == "") {
            return {{}, "", response.code, "Bad request"};
        }
        return this->process_response<TwitchModeratorChatSettings>(response);
    }

    VectorResponse<TwitchBasicUser> TwitchOauthAPI::get_user_block_list(std::string_view broadcaster_id,
                                                                        std::optional<size_t> first,
                                                                        std::optional<std::string_view> after) {
        std::string options {"?moderator_id=" + this->m_moderator_id + "&broadcaster_id=" + std::string(broadcaster_id)};
        if (first) {
            options += "&first=" + std::to_string(first.value());
        }
        if (after) {
            options += "&after=" + std::string(after.value());
        }
        std::string url {TWITCH_API_BASE + "users/blocks" + options};
        Response<std::string> response = call_api(url, this->m_app_access_token, this->m_client_id);
        if (response.data == "") {
            return {{}, "", response.code, "Bad request"};
        }
        return this->process_response<TwitchBasicUser>(response);
    }

    VectorResponse<TwitchBlockedTerm> TwitchOauthAPI::get_blocked_terms(std::string_view broadcaster_id,
                                                                        std::optional<size_t> first,
                                                                        std::optional<std::string_view> after) {
        std::string options {"?moderator_id=" + this->m_moderator_id + "&broadcaster_id=" + std::string(broadcaster_id)};
        if (first) {
            options += "&first=" + std::to_string(first.value());
        }
        if (after) {
            options += "&after=" + std::string(after.value());
        }
        std::string url {TWITCH_API_BASE + "moderation/blocked_terms" + options};
        Response<std::string> response = call_api(url, this->m_app_access_token, this->m_client_id);
        if (response.data == "") {
            return {{}, "", response.code, "Bad request"};
        }
        return this->process_response<TwitchBlockedTerm>(response);
    }

    VectorResponse<TwitchAutoModSettings> TwitchOauthAPI::get_automod_settings(std::string_view broadcaster_id) {
        std::string options {"?moderator_id=" + this->m_moderator_id + "&broadcaster_id=" + std::string(broadcaster_id)};
        std::string url {TWITCH_API_BASE + "moderation/automod/settings" + options};
        Response<std::string> response = call_api(url, this->m_app_access_token, this->m_client_id);
        if (response.data == "") {
            return {{}, "", response.code, "Bad request"};
        }
        return this->process_response<TwitchAutoModSettings>(response);
    }

    Response<std::string> TwitchOauthAPI::unblock_user(std::string_view target_user_id) {
        std::string options {"?target_user_id=" + std::string(target_user_id)};
        std::string url {TWITCH_API_BASE + "users/blocks" + options};
        Response<std::string> response = call_api(url, this->m_app_access_token, this->m_client_id, "DELETE");
        return response;
    }

    Response<std::string> TwitchOauthAPI::block_user(std::string_view target_user_id,
                                                     std::optional<std::string_view> source_context,
                                                     std::optional<std::string_view> reason) {
        std::string options {"?target_user_id=" + std::string(target_user_id)};
        if (source_context) {
            options += "&source_context=" + std::string(source_context.value());
        }
        if (reason) {
            options += "&reason=" + std::string(reason.value());
        }
        std::string url {TWITCH_API_BASE + "users/blocks" + options};
        Response<std::string> response = call_api(url, this->m_app_access_token, this->m_client_id, "PUT");
        return response;
    }
}