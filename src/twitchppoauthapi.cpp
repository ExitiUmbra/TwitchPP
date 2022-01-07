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

    VectorResponse<TwitchBanResponse> TwitchOauthAPI::ban_user(std::string_view broadcaster_id,
                                                               std::string_view user_id,
                                                               std::string_view reason,
                                                               std::optional<size_t> duration) {
        std::string options {"?broadcaster_id=" + std::string(broadcaster_id) + "&moderator_id=" + this->m_moderator_id};
        std::string request {"{\"data\":{\"user_id\":\"" + std::string(user_id) + "\",\"reason\":\"" + std::string(reason) + "\""};
        if (duration) {
            request += ",\"duration\":" + std::to_string(duration.value());
        }
        request += "}}";
        std::string url {TWITCH_API_BASE + "moderation/bans" + options};
        Response<std::string> response = call_api(url, this->m_app_access_token, this->m_client_id, "POST", request);
        if (response.data == "") {
            return {{}, "", response.code, "Bad request"};
        }
        return this->process_response<TwitchBanResponse>(response);
    }

    Response<std::string> TwitchOauthAPI::unban_user(std::string_view broadcaster_id,
                                                           std::string_view user_id) {
        std::string options {"?broadcaster_id=" + std::string(broadcaster_id) + "&moderator_id=" + this->m_moderator_id + "&user_id=" + std::string(user_id)};
        std::string url {TWITCH_API_BASE + "moderation/bans" + options};
        Response<std::string> response = call_api(url, this->m_app_access_token, this->m_client_id, "DELETE");
        return response;
    }

    VectorResponse<TwitchCommercialResponse> TwitchOauthAPI::start_commercial(std::string_view broadcaster_id,
                                                                              size_t length) {
        std::string request {"{\"broadcaster_id\":\"" + std::string(broadcaster_id) + "\",\"length\":" + std::to_string(length) + "}"};
        std::string url {TWITCH_API_BASE + "channels/commercial"};
        Response<std::string> response = call_api(url, this->m_app_access_token, this->m_client_id, "POST", request);
        if (response.data == "") {
            return {{}, "", response.code, "Bad request"};
        }
        return this->process_response<TwitchCommercialResponse>(response);
    }

    VectorResponse<TwitchAnalyticsResponse> TwitchOauthAPI::get_game_analytics(std::optional<AnalyticsRequest> request) {
        std::string options {"?"};
        if (request) {
            if (request.value().id) {
                options += "game_id=" + request.value().id.value();
            }
            if (request.value().first) {
                options += "&first=" + std::to_string(request.value().first.value());
            }
            if (request.value().started_at) {
                options += "started_at=" + request.value().started_at.value();
            }
            if (request.value().ended_at) {
                options += "ended_at=" + request.value().ended_at.value();
            }
            if (request.value().type) {
                options += "type=" + request.value().type.value();
            }
            if (request.value().after) {
                options += "after=" + request.value().after.value();
            }
        }
        std::string url {TWITCH_API_BASE + "analytics/games" + options};
        Response<std::string> response = call_api(url, this->m_app_access_token, this->m_client_id);
        if (response.data == "") {
            return {{}, "", response.code, "Bad request"};
        }
        return this->process_response<TwitchAnalyticsResponse>(response);
    }

    VectorResponse<TwitchAnalyticsResponse> TwitchOauthAPI::get_extension_analytics(std::optional<AnalyticsRequest> request) {
        std::string options {"?"};
        if (request) {
            if (request.value().id) {
                options += "extension_id=" + request.value().id.value();
            }
            if (request.value().first) {
                options += "&first=" + std::to_string(request.value().first.value());
            }
            if (request.value().started_at) {
                options += "started_at=" + request.value().started_at.value();
            }
            if (request.value().ended_at) {
                options += "ended_at=" + request.value().ended_at.value();
            }
            if (request.value().type) {
                options += "type=" + request.value().type.value();
            }
            if (request.value().after) {
                options += "after=" + request.value().after.value();
            }
        }
        std::string url {TWITCH_API_BASE + "analytics/extensions" + options};
        Response<std::string> response = call_api(url, this->m_app_access_token, this->m_client_id);
        if (response.data == "") {
            return {{}, "", response.code, "Bad request"};
        }
        return this->process_response<TwitchAnalyticsResponse>(response);
    }

    Response<std::string> TwitchOauthAPI::modify_channel_information(std::string_view broadcaster_id, ChannelInformation& info) {
        std::string options {"?broadcaster_id=" + std::string(broadcaster_id)};
        std::string request {"{"};
        if (info.game_id) {
            request += "\"game_id\":\"" + std::string(info.game_id.value()) + "\"";
        }
        if (info.broadcaster_language) {
            if (request != "{") {
                request += ",";
            }
            request += "\"broadcaster_language\":\"" + std::string(info.broadcaster_language.value()) + "\"";
        }
        if (info.title) {
            if (request != "{") {
                request += ",";
            }
            request += "\"title\":\"" + std::string(info.title.value()) + "\"";
        }
        if (info.delay) {
            if (request != "{") {
                request += ",";
            }
            request += "\"delay\":" + std::to_string(info.delay.value());
        }
        request += "}";
        std::string url {TWITCH_API_BASE + "channels" + options};
        Response<std::string> response = call_api(url, this->m_app_access_token, this->m_client_id, "PATCH", request);
        return response;
    }

    VectorResponse<TwitchPoll> TwitchOauthAPI::create_poll(std::string_view broadcaster_id,
                                                           std::string_view title,
                                                           std::vector<std::string_view> choices,
                                                           const size_t& duration,
                                                           const size_t& bits,
                                                           const size_t& channel_points) {
        std::string request {"{"};
        request += "\"broadcaster_id\":\"" + std::string(broadcaster_id)
            + "\",\"title\":\"" + std::string(title)
            + "\",\"duration\":" + std::to_string(duration)
            + ",\"choices\":[";
        for (size_t i {0}; i < choices.size(); ++i) {
            request += "{\"title\":\"" + std::string(choices.at(i)) + "\"}";
            if ((i + 1) != choices.size()) {
                request += ",";
            }
        }
        request += "]";
        if (bits > 0) {
            request += ",\"bits_voting_enabled\":true,\"bits_per_vote\":" + std::to_string(bits);
        }
        if (channel_points > 0) {
            request += ",\"channel_points_voting_enabled\":true,\"channel_points_per_vote\":" + std::to_string(channel_points);
        }
        request += "}";
        std::string url {TWITCH_API_BASE + "polls"};
        Response<std::string> response = call_api(url, this->m_app_access_token, this->m_client_id, "POST", request);
        if (response.data == "") {
            return {{}, "", response.code, "Bad request"};
        }
        return this->process_response<TwitchPoll>(response);
    }

    VectorResponse<TwitchPoll> TwitchOauthAPI::end_poll(std::string_view broadcaster_id,
                                                        std::string_view id,
                                                        std::string_view status) {
        std::string request {"{"};
        request += "\"broadcaster_id\":\"" + std::string(broadcaster_id)
            + "\",\"id\":\"" + std::string(id)
            + "\",\"status\":\"" + std::string(status);
        request += "\"}";
        std::string url {TWITCH_API_BASE + "polls"};
        Response<std::string> response = call_api(url, this->m_app_access_token, this->m_client_id, "PATCH", request);
        if (response.data == "") {
            return {{}, "", response.code, "Bad request"};
        }
        return this->process_response<TwitchPoll>(response);
    }

    VectorResponse<TwitchPrediction> TwitchOauthAPI::create_prediction(std::string_view broadcaster_id,
                                                                       std::string_view title,
                                                                       std::vector<std::string_view> outcomes,
                                                                       const size_t& prediction_window) {
        std::string request {"{"};
        request += "\"broadcaster_id\":\"" + std::string(broadcaster_id)
            + "\",\"title\":\"" + std::string(title)
            + "\",\"prediction_window\":" + std::to_string(prediction_window)
            + ",\"outcomes\":[";
        for (size_t i {0}; i < outcomes.size(); ++i) {
            request += "{\"title\":\"" + std::string(outcomes.at(i)) + "\"}";
            if ((i + 1) != outcomes.size()) {
                request += ",";
            }
        }
        request += "]}";
        std::string url {TWITCH_API_BASE + "predictions"};
        Response<std::string> response = call_api(url, this->m_app_access_token, this->m_client_id, "POST", request);
        if (response.data == "") {
            return {{}, "", response.code, "Bad request"};
        }
        return this->process_response<TwitchPrediction>(response);
    }

    VectorResponse<TwitchPrediction> TwitchOauthAPI::end_prediction(std::string_view broadcaster_id,
                                                                    std::string_view id,
                                                                    std::string_view status) {
        std::string request {"{"};
        request += "\"broadcaster_id\":\"" + std::string(broadcaster_id)
            + "\",\"id\":\"" + std::string(id)
            + "\",\"status\":\"" + std::string(status);
        request += "\"}";
        std::string url {TWITCH_API_BASE + "predictions"};
        Response<std::string> response = call_api(url, this->m_app_access_token, this->m_client_id, "PATCH", request);
        if (response.data == "") {
            return {{}, "", response.code, "Bad request"};
        }
        return this->process_response<TwitchPrediction>(response);
    }

    Response<std::string> TwitchOauthAPI::update_channel_stream_schedule(std::string_view broadcaster_id,
                                                                         std::optional<bool> is_vacation_enabled,
                                                                         std::optional<std::string_view> vacation_start_time,
                                                                         std::optional<std::string_view> vacation_end_time,
                                                                         std::optional<std::string_view> timezone) {
        std::string options {"?broadcaster_id=" + std::string(broadcaster_id)};
        if (is_vacation_enabled) {
            options += "&is_vacation_enabled=" + std::string(is_vacation_enabled.value() ? "true" : "false");
            if (is_vacation_enabled.value()) {
                options += "&vacation_start_time=" + std::string(vacation_start_time.value())
                    + "&vacation_end_time=" + std::string(vacation_end_time.value())
                    + "&timezone=" + std::string(timezone.value());
            }
        }
        std::string url {TWITCH_API_BASE + "schedule/settings" + options};
        Response<std::string> response = call_api(url, this->m_app_access_token, this->m_client_id, "PATCH");
        return response;
    }

    VectorResponse<TwitchChannelStreamSchedule> TwitchOauthAPI::create_channel_stream_schedule_segment(std::string_view broadcaster_id,
                                                                                                       std::string_view start_time,
                                                                                                       std::string_view timezone,
                                                                                                       const bool& is_recurring,
                                                                                                       std::optional<std::string_view> duration,
                                                                                                       std::optional<std::string_view> title,
                                                                                                       std::optional<std::string_view> category_id) {
        std::string options {"?broadcaster_id=" + std::string(broadcaster_id)};
        std::string request {"{"};
        request += "\"start_time\":\"" + std::string(start_time)
            + "\",\"timezone\":\"" + std::string(timezone)
            + "\",\"is_recurring\":" + std::string(is_recurring ? "true" : "false");
        if (duration) {
            request += ",\"duration\":\"" + std::string(duration.value()) + "\"";
        }
        if (title) {
            request += ",\"title\":\"" + std::string(title.value()) + "\"";
        }
        if (category_id) {
            request += ",\"category_id\":\"" + std::string(category_id.value()) + "\"";
        }
        request += "}";
        std::string url {TWITCH_API_BASE + "schedule/segment" + options};
        Response<std::string> response = call_api(url, this->m_app_access_token, this->m_client_id, "POST", request);
        if (response.data == "") {
            return {{}, "", response.code, "Bad request"};
        }
        return this->process_single_response<TwitchChannelStreamSchedule>(response);
    }

    Response<std::string> TwitchOauthAPI::delete_channel_stream_schedule_segment(std::string_view broadcaster_id,
                                                                                 std::string_view id) {
        std::string options {"?broadcaster_id=" + std::string(broadcaster_id) + "&id=" + std::string(id)};
        std::string url {TWITCH_API_BASE + "schedule/segment" + options};
        Response<std::string> response = call_api(url, this->m_app_access_token, this->m_client_id, "DELETE");
        return response;
    }

    VectorResponse<TwitchChannelStreamSchedule> TwitchOauthAPI::update_channel_stream_schedule_segment(std::string_view broadcaster_id,
                                                                                                       std::string_view id,
                                                                                                       std::optional<std::string_view> start_time,
                                                                                                       std::optional<std::string_view> timezone,
                                                                                                       std::optional<bool> is_canceled,
                                                                                                       std::optional<std::string_view> duration,
                                                                                                       std::optional<std::string_view> title,
                                                                                                       std::optional<std::string_view> category_id) {
        std::string options {"?broadcaster_id=" + std::string(broadcaster_id) + "&id=" + std::string(id)};
        std::string request {"{"};
        if (start_time) {
            request += "\"start_time\":\"" + std::string(start_time.value()) + "\"";
        }
        if (timezone) {
            if (request != "{") {
                request += ",";
            }
            request += "\"timezone\":\"" + std::string(timezone.value()) + "\"";
        }
        if (is_canceled) {
            if (request != "{") {
                request += ",";
            }
            request += "\"is_canceled\":" + std::string(is_canceled.value() ? "true" : "false");
        }
        if (duration) {
            if (request != "{") {
                request += ",";
            }
            request += "\"duration\":\"" + std::string(duration.value()) + "\"";
        }
        if (title) {
            if (request != "{") {
                request += ",";
            }
            request += "\"title\":\"" + std::string(title.value()) + "\"";
        }
        if (category_id) {
            if (request != "{") {
                request += ",";
            }
            request += "\"category_id\":\"" + std::string(category_id.value()) + "\"";
        }
        request += "}";
        std::string url {TWITCH_API_BASE + "schedule/segment" + options};
        Response<std::string> response = call_api(url, this->m_app_access_token, this->m_client_id, "PATCH", request);
        if (response.data == "") {
            return {{}, "", response.code, "Bad request"};
        }
        return this->process_single_response<TwitchChannelStreamSchedule>(response);
    }

    VectorResponse<TwitchUser> TwitchOauthAPI::update_user(std::string_view description) {
        CURL *curl = curl_easy_init();
        if (!curl) {
            return {{}, "", 400, "Client API error"};
        }
        char *description_output = curl_easy_escape(curl, description.data(), description.size());
        std::string options {"?description=" + std::string(description_output)};
        curl_free(description_output);
        curl_easy_cleanup(curl);
        std::string url {TWITCH_API_BASE + "users" + options};
        Response<std::string> response = call_api(url, this->m_app_access_token, this->m_client_id, "PUT");
        if (response.data == "") {
            return {{}, "", response.code, "Bad request"};
        }
        return this->process_response<TwitchUser>(response);
    }

    VectorResponse<std::string> TwitchOauthAPI::delete_videos(std::vector<std::string> ids) {
        std::string options {"?"};
        for (std::string clip_id : ids) {
            options += (options == "?" ? "id=" : "&id=") + clip_id;
        }
        std::string url {TWITCH_API_BASE + "videos" + options};
        Response<std::string> response = call_api(url, this->m_app_access_token, this->m_client_id, "DELETE");
        if (response.data == "") {
            return {{}, "", response.code, "Bad request"};
        }
        return this->process_response<std::string>(response);
    }

    VectorResponse<TwitchCreatedClip> TwitchOauthAPI::create_clip(std::string_view broadcaster_id,
                                                                  std::optional<bool> has_delay) {
        std::string options {"?broadcaster_id=" + std::string(broadcaster_id)};
        if (has_delay) {
            options += "&has_delay=" + std::string(has_delay ? "true" : "false");
        }
        std::string url {TWITCH_API_BASE + "clips" + options};
        Response<std::string> response = call_api(url, this->m_app_access_token, this->m_client_id, "POST");
        if (response.data == "") {
            return {{}, "", response.code, "Bad request"};
        }
        size_t limit {};
        size_t remaining {};
        auto [data_string, _] = get_first_value(response.data.substr(1, response.data.size() - 1));
        for (std::string header : response.headers) {
            if (header.starts_with("ratelimit-limit:")) {
                limit = std::stoul(header.substr(17, header.size()));
            }
            if (header.starts_with("ratelimit-remaining:")) {
                remaining = std::stoul(header.substr(21, header.size()));
            }
        }
        VectorResponse<TwitchCreatedClip> response_with_clip {{TwitchCreatedClip(data_string, limit, remaining)}, response.cursor, response.code, response.message};
        return response_with_clip;
    }

    VectorResponse<TwitchBroadcasterSubscriptions> TwitchOauthAPI::get_broadcaster_subscriptions(std::string_view broadcaster_id,
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
        std::string url {TWITCH_API_BASE + "subscriptions" + options};
        Response<std::string> response = call_api(url, this->m_app_access_token, this->m_client_id);
        if (response.data == "") {
            return {{}, "", response.code, "Bad request"};
        }
        VectorResponseLeftovers<TwitchBroadcasterSubscription> subs_response = this->process_response_leftovers<TwitchBroadcasterSubscription>(response);
        size_t total = std::stoul(get_object_param("\"total\"", subs_response.leftovers, "0"));
        size_t points = std::stoul(get_object_param("\"points\"", subs_response.leftovers, "0"));
        return {{TwitchBroadcasterSubscriptions(total, points, subs_response.data)}, subs_response.cursor, subs_response.code, subs_response.message};
    }
}