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

    VectorResponse<TwitchAutoModSettings> TwitchOauthAPI::update_automod_settings(std::string_view broadcaster_id, TwitchAutoModSettings& settings, const bool& is_overall) {
        std::string options {"?moderator_id=" + this->m_moderator_id + "&broadcaster_id=" + std::string(broadcaster_id)};
        std::string url {TWITCH_API_BASE + "moderation/automod/settings" + options};
        Response<std::string> response = call_api(url, this->m_app_access_token, this->m_client_id, "PUT", settings.to_request(is_overall));
        if (response.data == "") {
            return {{}, "", response.code, "Bad request"};
        }
        return this->process_response<TwitchAutoModSettings>(response);
    }

    VectorResponse<TwitchBlockedTerm> TwitchOauthAPI::add_blocked_term(std::string_view broadcaster_id,
                                                                       std::string_view text) {
        std::string options {"?moderator_id=" + this->m_moderator_id + "&broadcaster_id=" + std::string(broadcaster_id)};
        std::string url {TWITCH_API_BASE + "moderation/blocked_terms" + options};
        Response<std::string> response = call_api(url, this->m_app_access_token, this->m_client_id, "POST", std::string("{\"text\":\"") + std::string(text) + "\"}");
        if (response.data == "") {
            return {{}, "", response.code, "Bad request"};
        }
        return this->process_response<TwitchBlockedTerm>(response);
    }

    Response<std::string> TwitchOauthAPI::remove_blocked_term(std::string_view broadcaster_id,
                                                              std::string_view term_id) {
        std::string options {"?moderator_id=" + this->m_moderator_id + "&broadcaster_id=" + std::string(broadcaster_id) + "&id=" + std::string(term_id)};
        std::string url {TWITCH_API_BASE + "moderation/blocked_terms" + options};
        Response<std::string> response = call_api(url, this->m_app_access_token, this->m_client_id, "DELETE");
        return response;
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

    Response<std::string> TwitchOauthAPI::get_stream_key(std::string_view broadcaster_id) {
        std::string options {"?broadcaster_id=" + std::string(broadcaster_id)};
        std::string url {TWITCH_API_BASE + "streams/key" + options};
        Response<std::string> response = call_api(url, this->m_app_access_token, this->m_client_id);
        response.data = get_object_param("\"stream_key\"", response.data);
        return response;
    }

    VectorResponse<TwitchBasicUser> TwitchOauthAPI::get_moderators(std::string_view broadcaster_id,
                                                                   std::vector<std::string> user_ids,
                                                                   std::optional<size_t> first,
                                                                   std::optional<std::string> after) {
        std::string options {"?broadcaster_id=" + std::string(broadcaster_id)};
        for (std::string user_id : user_ids) {
            options += "&user_id=" + user_id;
        }
        if (first) {
            options += "&first=" + std::to_string(first.value());
        }
        if (after) {
            options += "&after=" + after.value();
        }
        std::string url {TWITCH_API_BASE + "moderation/moderators" + options};
        Response<std::string> response = call_api(url, this->m_app_access_token, this->m_client_id);
        if (response.data == "") {
            return {{}, "", response.code, "Bad request"};
        }
        return this->process_response<TwitchBasicUser>(response);
    }

    VectorResponse<TwitchBannedUser> TwitchOauthAPI::get_banned_users(std::string_view broadcaster_id,
                                                                      std::vector<std::string> user_ids,
                                                                      std::optional<size_t> first) {
        std::string options {"?broadcaster_id=" + std::string(broadcaster_id)};
        for (std::string user_id : user_ids) {
            options += "&user_id=" + user_id;
        }
        if (first) {
            options += "&first=" + std::to_string(first.value());
        }
        std::string url {TWITCH_API_BASE + "moderation/banned" + options};
        Response<std::string> response = call_api(url, this->m_app_access_token, this->m_client_id);
        if (response.data == "") {
            return {{}, "", response.code, "Bad request"};
        }
        return this->process_response<TwitchBannedUser>(response);
    }

    VectorResponse<TwitchBannedUser> TwitchOauthAPI::get_banned_users(std::string_view broadcaster_id,
                                                                      const bool& is_after,
                                                                      std::optional<size_t> first,
                                                                      std::optional<std::string> cursor) {
        std::string options {"?broadcaster_id=" + std::string(broadcaster_id)};
        if (first) {
            options += "&first=" + std::to_string(first.value());
        }
        if (cursor) {
            options += (is_after ? "after=" : "before=") + cursor.value();
        }
        std::string url {TWITCH_API_BASE + "moderation/banned" + options};
        Response<std::string> response = call_api(url, this->m_app_access_token, this->m_client_id);
        if (response.data == "") {
            return {{}, "", response.code, "Bad request"};
        }
        return this->process_response<TwitchBannedUser>(response);
    }

    VectorResponse<TwitchStream> TwitchOauthAPI::get_followed_streams(std::string_view user_id,
                                                                      std::optional<size_t> first,
                                                                      std::optional<std::string> after) {
        std::string options {"?user_id=" + std::string(user_id)};
        if (first) {
            options += "&first=" + std::to_string(first.value());
        }
        if (after) {
            options += "&after=" + after.value();
        }
        std::string url {TWITCH_API_BASE + "streams/followed" + options};
        Response<std::string> response = call_api(url, this->m_app_access_token, this->m_client_id);
        if (response.data == "") {
            return {{}, "", response.code, "Bad request"};
        }
        return this->process_response<TwitchStream>(response);
    }

    VectorResponse<TwitchBannedEvent> TwitchOauthAPI::get_banned_events(std::string_view broadcaster_id,
                                                                        std::vector<std::string> user_ids,
                                                                        std::optional<size_t> first) {
        std::string options {"?broadcaster_id=" + std::string(broadcaster_id)};
        for (std::string user_id : user_ids) {
            options += "&user_id=" + user_id;
        }
        if (first) {
            options += "&first=" + std::to_string(first.value());
        }
        std::string url {TWITCH_API_BASE + "moderation/banned/events" + options};
        Response<std::string> response = call_api(url, this->m_app_access_token, this->m_client_id);
        if (response.data == "") {
            return {{}, "", response.code, "Bad request"};
        }
        return this->process_response<TwitchBannedEvent>(response);
    }

    VectorResponse<TwitchBannedEvent> TwitchOauthAPI::get_banned_events(std::string_view broadcaster_id,
                                                                      std::optional<size_t> first,
                                                                      std::optional<std::string> cursor) {
        std::string options {"?broadcaster_id=" + std::string(broadcaster_id)};
        if (first) {
            options += "&first=" + std::to_string(first.value());
        }
        if (cursor) {
            options += "after=" + cursor.value();
        }
        std::string url {TWITCH_API_BASE + "moderation/banned/events" + options};
        Response<std::string> response = call_api(url, this->m_app_access_token, this->m_client_id);
        if (response.data == "") {
            return {{}, "", response.code, "Bad request"};
        }
        return this->process_response<TwitchBannedEvent>(response);
    }

    VectorResponse<TwitchModeratorEvent> TwitchOauthAPI::get_moderator_events(std::string_view broadcaster_id,
                                                                              std::vector<std::string> user_ids,
                                                                              std::optional<size_t> first) {
        std::string options {"?broadcaster_id=" + std::string(broadcaster_id)};
        for (std::string user_id : user_ids) {
            options += "&user_id=" + user_id;
        }
        if (first) {
            options += "&first=" + std::to_string(first.value());
        }
        std::string url {TWITCH_API_BASE + "moderation/moderators/events" + options};
        Response<std::string> response = call_api(url, this->m_app_access_token, this->m_client_id);
        if (response.data == "") {
            return {{}, "", response.code, "Bad request"};
        }
        return this->process_response<TwitchModeratorEvent>(response);
    }

    VectorResponse<TwitchModeratorEvent> TwitchOauthAPI::get_moderator_events(std::string_view broadcaster_id,
                                                                              std::optional<size_t> first,
                                                                              std::optional<std::string> cursor) {
        std::string options {"?broadcaster_id=" + std::string(broadcaster_id)};
        if (first) {
            options += "&first=" + std::to_string(first.value());
        }
        if (cursor) {
            options += "after=" + cursor.value();
        }
        std::string url {TWITCH_API_BASE + "moderation/moderators/events" + options};
        Response<std::string> response = call_api(url, this->m_app_access_token, this->m_client_id);
        if (response.data == "") {
            return {{}, "", response.code, "Bad request"};
        }
        return this->process_response<TwitchModeratorEvent>(response);
    }

    VectorResponse<TwitchPoll> TwitchOauthAPI::get_polls(std::string_view broadcaster_id,
                                                         std::vector<std::string> ids,
                                                         std::optional<size_t> first,
                                                         std::optional<std::string> after) {
        std::string options {"?broadcaster_id=" + std::string(broadcaster_id)};
        for (std::string id : ids) {
            options += "&id=" + id;
        }
        if (first) {
            options += "&first=" + std::to_string(first.value());
        }
        if (after) {
            options += "after=" + after.value();
        }
        std::string url {TWITCH_API_BASE + "polls" + options};
        Response<std::string> response = call_api(url, this->m_app_access_token, this->m_client_id);
        if (response.data == "") {
            return {{}, "", response.code, "Bad request"};
        }
        return this->process_response<TwitchPoll>(response);
    }

    VectorResponse<TwitchPrediction> TwitchOauthAPI::get_predictions(std::string_view broadcaster_id,
                                                                     std::vector<std::string> ids,
                                                                     std::optional<size_t> first,
                                                                     std::optional<std::string> after) {
        std::string options {"?broadcaster_id=" + std::string(broadcaster_id)};
        for (std::string id : ids) {
            options += "&id=" + id;
        }
        if (first) {
            options += "&first=" + std::to_string(first.value());
        }
        if (after) {
            options += "after=" + after.value();
        }
        std::string url {TWITCH_API_BASE + "predictions" + options};
        Response<std::string> response = call_api(url, this->m_app_access_token, this->m_client_id);
        if (response.data == "") {
            return {{}, "", response.code, "Bad request"};
        }
        return this->process_response<TwitchPrediction>(response);
    }

    VectorResponse<TwitchCreatorsGoal> TwitchOauthAPI::get_creators_goals(std::string_view broadcaster_id) {
        std::string options {"?broadcaster_id=" + std::string(broadcaster_id)};
        std::string url {TWITCH_API_BASE + "goals" + options};
        Response<std::string> response = call_api(url, this->m_app_access_token, this->m_client_id);
        if (response.data == "") {
            return {{}, "", response.code, "Bad request"};
        }
        return this->process_response<TwitchCreatorsGoal>(response);
    }

    VectorResponse<TwitchChannelEditor> TwitchOauthAPI::get_channel_editors(std::string_view broadcaster_id) {
        std::string options {"?broadcaster_id=" + std::string(broadcaster_id)};
        std::string url {TWITCH_API_BASE + "channels/editors" + options};
        Response<std::string> response = call_api(url, this->m_app_access_token, this->m_client_id);
        if (response.data == "") {
            return {{}, "", response.code, "Bad request"};
        }
        return this->process_response<TwitchChannelEditor>(response);
    }

    VectorResponse<TwitchUserExtension> TwitchOauthAPI::get_user_extensions() {
        std::string url {TWITCH_API_BASE + "users/extensions/list"};
        Response<std::string> response = call_api(url, this->m_app_access_token, this->m_client_id);
        if (response.data == "") {
            return {{}, "", response.code, "Bad request"};
        }
        return this->process_response<TwitchUserExtension>(response);
    }

    VectorResponse<TwitchBitsLeaderboard> TwitchOauthAPI::get_bits_leaderboard(size_t count,
                                                                               std::optional<std::string> period,
                                                                               std::optional<std::string> started_at,
                                                                               std::optional<std::string> user_id) {
        std::string options {"?count=" + std::to_string(count)};
        if (period) {
            options += "&period=" + period.value();
        }
        if (started_at) {
            options += "started_at=" + started_at.value();
        }
        if (user_id) {
            options += "user_id=" + user_id.value();
        }
        std::string url {TWITCH_API_BASE + "bits/leaderboard" + options};
        Response<std::string> response = call_api(url, this->m_app_access_token, this->m_client_id);
        if (response.data == "") {
            return {{}, "", response.code, "Bad request"};
        }
        VectorResponseLeftovers<TwitchUserBits> users_response = this->process_response_leftovers<TwitchUserBits>(response);
        return {{TwitchBitsLeaderboard(users_response.leftovers, users_response.data)}, users_response.cursor, users_response.code, users_response.message};
    }
}