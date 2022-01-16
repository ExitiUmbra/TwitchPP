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
        if (!response.data.size()) {
            return {{}, "", response.code, "Bad request"};
        }
        return this->process_response<TwitchModeratorChatSettings>(response);
    }

    VectorResponse<TwitchModeratorChatSettings> TwitchOauthAPI::update_chat_settings(TwitchModeratorChatSettings& settings) {
        std::string url {TWITCH_API_BASE + "chat/settings?broadcaster_id="
            + std::string(settings.get_broadcaster_id().size() ? settings.get_broadcaster_id() : this->m_moderator_id)
            + "&moderator_id=" + this->m_moderator_id};
        Response<std::string> response = call_api(url, this->m_app_access_token, this->m_client_id, HTTP_PATCH, settings.to_request());
        if (!response.data.size()) {
            return {{}, "", response.code, "Bad request"};
        }
        return this->process_response<TwitchModeratorChatSettings>(response);
    }

    VectorResponse<TwitchModeratorChatSettings> TwitchOauthAPI::update_chat_settings(std::string_view broadcaster_id, std::string_view settings) {
        std::string url {TWITCH_API_BASE + "chat/settings?broadcaster_id=" + std::string(broadcaster_id) + "&moderator_id=" + this->m_moderator_id};
        Response<std::string> response = call_api(url, this->m_app_access_token, this->m_client_id, HTTP_PATCH, settings.data());
        if (!response.data.size()) {
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
        if (!response.data.size()) {
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
        if (!response.data.size()) {
            return {{}, "", response.code, "Bad request"};
        }
        return this->process_response<TwitchBlockedTerm>(response);
    }

    VectorResponse<TwitchAutoModSettings> TwitchOauthAPI::get_automod_settings(std::string_view broadcaster_id) {
        std::string options {"?moderator_id=" + this->m_moderator_id + "&broadcaster_id=" + std::string(broadcaster_id)};
        std::string url {TWITCH_API_BASE + "moderation/automod/settings" + options};
        Response<std::string> response = call_api(url, this->m_app_access_token, this->m_client_id);
        if (!response.data.size()) {
            return {{}, "", response.code, "Bad request"};
        }
        return this->process_response<TwitchAutoModSettings>(response);
    }

    VectorResponse<TwitchAutoModSettings> TwitchOauthAPI::update_automod_settings(std::string_view broadcaster_id,
                                                                                  TwitchAutoModSettings& settings,
                                                                                  const bool& is_overall) {
        std::string options {"?moderator_id=" + this->m_moderator_id + "&broadcaster_id=" + std::string(broadcaster_id)};
        std::string url {TWITCH_API_BASE + "moderation/automod/settings" + options};
        Response<std::string> response = call_api(url, this->m_app_access_token, this->m_client_id, HTTP_PUT, settings.to_request(is_overall));
        if (!response.data.size()) {
            return {{}, "", response.code, "Bad request"};
        }
        return this->process_response<TwitchAutoModSettings>(response);
    }

    VectorResponse<TwitchBlockedTerm> TwitchOauthAPI::add_blocked_term(std::string_view broadcaster_id,
                                                                       std::string_view text) {
        std::string options {"?moderator_id=" + this->m_moderator_id + "&broadcaster_id=" + std::string(broadcaster_id)};
        std::string url {TWITCH_API_BASE + "moderation/blocked_terms" + options};
        Response<std::string> response = call_api(url, this->m_app_access_token, this->m_client_id, HTTP_POST, std::string("{\"text\":\"") + std::string(text) + "\"}");
        if (!response.data.size()) {
            return {{}, "", response.code, "Bad request"};
        }
        return this->process_response<TwitchBlockedTerm>(response);
    }

    Response<std::string> TwitchOauthAPI::remove_blocked_term(std::string_view broadcaster_id,
                                                              std::string_view term_id) {
        std::string options {"?moderator_id=" + this->m_moderator_id + "&broadcaster_id=" + std::string(broadcaster_id) + "&id=" + std::string(term_id)};
        std::string url {TWITCH_API_BASE + "moderation/blocked_terms" + options};
        Response<std::string> response = call_api(url, this->m_app_access_token, this->m_client_id, HTTP_DELETE);
        return response;
    }

    Response<std::string> TwitchOauthAPI::unblock_user(std::string_view target_user_id) {
        std::string options {"?target_user_id=" + std::string(target_user_id)};
        std::string url {TWITCH_API_BASE + "users/blocks" + options};
        Response<std::string> response = call_api(url, this->m_app_access_token, this->m_client_id, HTTP_DELETE);
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
        Response<std::string> response = call_api(url, this->m_app_access_token, this->m_client_id, HTTP_PUT);
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
        if (!response.data.size()) {
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
        if (!response.data.size()) {
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
            options += (is_after ? "&after=" : "&before=") + cursor.value();
        }
        std::string url {TWITCH_API_BASE + "moderation/banned" + options};
        Response<std::string> response = call_api(url, this->m_app_access_token, this->m_client_id);
        if (!response.data.size()) {
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
        if (!response.data.size()) {
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
        if (!response.data.size()) {
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
            options += "&after=" + cursor.value();
        }
        std::string url {TWITCH_API_BASE + "moderation/banned/events" + options};
        Response<std::string> response = call_api(url, this->m_app_access_token, this->m_client_id);
        if (!response.data.size()) {
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
        if (!response.data.size()) {
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
            options += "&after=" + cursor.value();
        }
        std::string url {TWITCH_API_BASE + "moderation/moderators/events" + options};
        Response<std::string> response = call_api(url, this->m_app_access_token, this->m_client_id);
        if (!response.data.size()) {
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
            options += "&after=" + after.value();
        }
        std::string url {TWITCH_API_BASE + "polls" + options};
        Response<std::string> response = call_api(url, this->m_app_access_token, this->m_client_id);
        if (!response.data.size()) {
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
            options += "&after=" + after.value();
        }
        std::string url {TWITCH_API_BASE + "predictions" + options};
        Response<std::string> response = call_api(url, this->m_app_access_token, this->m_client_id);
        if (!response.data.size()) {
            return {{}, "", response.code, "Bad request"};
        }
        return this->process_response<TwitchPrediction>(response);
    }

    VectorResponse<TwitchCreatorsGoal> TwitchOauthAPI::get_creators_goals(std::string_view broadcaster_id) {
        std::string options {"?broadcaster_id=" + std::string(broadcaster_id)};
        std::string url {TWITCH_API_BASE + "goals" + options};
        Response<std::string> response = call_api(url, this->m_app_access_token, this->m_client_id);
        if (!response.data.size()) {
            return {{}, "", response.code, "Bad request"};
        }
        return this->process_response<TwitchCreatorsGoal>(response);
    }

    VectorResponse<TwitchChannelEditor> TwitchOauthAPI::get_channel_editors(std::string_view broadcaster_id) {
        std::string options {"?broadcaster_id=" + std::string(broadcaster_id)};
        std::string url {TWITCH_API_BASE + "channels/editors" + options};
        Response<std::string> response = call_api(url, this->m_app_access_token, this->m_client_id);
        if (!response.data.size()) {
            return {{}, "", response.code, "Bad request"};
        }
        return this->process_response<TwitchChannelEditor>(response);
    }

    VectorResponse<TwitchUserExtension> TwitchOauthAPI::get_user_extensions() {
        std::string url {TWITCH_API_BASE + "users/extensions/list"};
        Response<std::string> response = call_api(url, this->m_app_access_token, this->m_client_id);
        if (!response.data.size()) {
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
        if (!response.data.size()) {
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
        Response<std::string> response = call_api(url, this->m_app_access_token, this->m_client_id, HTTP_POST, request);
        if (!response.data.size()) {
            return {{}, "", response.code, "Bad request"};
        }
        return this->process_response<TwitchBanResponse>(response);
    }

    Response<std::string> TwitchOauthAPI::unban_user(std::string_view broadcaster_id,
                                                           std::string_view user_id) {
        std::string options {"?broadcaster_id=" + std::string(broadcaster_id) + "&moderator_id=" + this->m_moderator_id + "&user_id=" + std::string(user_id)};
        std::string url {TWITCH_API_BASE + "moderation/bans" + options};
        Response<std::string> response = call_api(url, this->m_app_access_token, this->m_client_id, HTTP_DELETE);
        return response;
    }

    VectorResponse<TwitchCommercialResponse> TwitchOauthAPI::start_commercial(std::string_view broadcaster_id,
                                                                              size_t length) {
        std::string request {"{\"broadcaster_id\":\"" + std::string(broadcaster_id) + "\",\"length\":" + std::to_string(length) + "}"};
        std::string url {TWITCH_API_BASE + "channels/commercial"};
        Response<std::string> response = call_api(url, this->m_app_access_token, this->m_client_id, HTTP_POST, request);
        if (!response.data.size()) {
            return {{}, "", response.code, "Bad request"};
        }
        return this->process_response<TwitchCommercialResponse>(response);
    }

    VectorResponse<TwitchAnalyticsResponse> TwitchOauthAPI::get_game_analytics(std::optional<AnalyticsRequest> request) {
        std::string options {""};
        if (request) {
            if (request.value().id) {
                options += "&game_id=" + request.value().id.value();
            }
            if (request.value().first) {
                options += "&first=" + std::to_string(request.value().first.value());
            }
            if (request.value().started_at) {
                options += "&started_at=" + request.value().started_at.value();
            }
            if (request.value().ended_at) {
                options += "&ended_at=" + request.value().ended_at.value();
            }
            if (request.value().type) {
                options += "&type=" + request.value().type.value();
            }
            if (request.value().after) {
                options += "&after=" + request.value().after.value();
            }
        }
        std::string url {TWITCH_API_BASE + "analytics/games" + (options.size() ? ("?" + options.substr(1, options.size())) : options)};
        Response<std::string> response = call_api(url, this->m_app_access_token, this->m_client_id);
        if (!response.data.size()) {
            return {{}, "", response.code, "Bad request"};
        }
        return this->process_response<TwitchAnalyticsResponse>(response);
    }

    VectorResponse<TwitchAnalyticsResponse> TwitchOauthAPI::get_extension_analytics(std::optional<AnalyticsRequest> request) {
        std::string options {""};
        if (request) {
            // TODO: deal with only optional params in the same way everywhere, search for "is_after="
            if (request.value().id) {
                options += "&extension_id=" + request.value().id.value();
            }
            if (request.value().first) {
                options += "&first=" + std::to_string(request.value().first.value());
            }
            if (request.value().started_at) {
                options += "&started_at=" + request.value().started_at.value();
            }
            if (request.value().ended_at) {
                options += "&ended_at=" + request.value().ended_at.value();
            }
            if (request.value().type) {
                options += "&type=" + request.value().type.value();
            }
            if (request.value().after) {
                options += "&after=" + request.value().after.value();
            }
        }
        std::string url {TWITCH_API_BASE + "analytics/extensions" + (options.size() ? ("?" + options.substr(1, options.size())) : options)};
        Response<std::string> response = call_api(url, this->m_app_access_token, this->m_client_id);
        if (!response.data.size()) {
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
        Response<std::string> response = call_api(url, this->m_app_access_token, this->m_client_id, HTTP_PATCH, request);
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
        Response<std::string> response = call_api(url, this->m_app_access_token, this->m_client_id, HTTP_POST, request);
        if (!response.data.size()) {
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
        Response<std::string> response = call_api(url, this->m_app_access_token, this->m_client_id, HTTP_PATCH, request);
        if (!response.data.size()) {
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
        Response<std::string> response = call_api(url, this->m_app_access_token, this->m_client_id, HTTP_POST, request);
        if (!response.data.size()) {
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
        Response<std::string> response = call_api(url, this->m_app_access_token, this->m_client_id, HTTP_PATCH, request);
        if (!response.data.size()) {
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
        Response<std::string> response = call_api(url, this->m_app_access_token, this->m_client_id, HTTP_PATCH);
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
        Response<std::string> response = call_api(url, this->m_app_access_token, this->m_client_id, HTTP_POST, request);
        if (!response.data.size()) {
            return {{}, "", response.code, "Bad request"};
        }
        return this->process_single_response<TwitchChannelStreamSchedule>(response);
    }

    Response<std::string> TwitchOauthAPI::delete_channel_stream_schedule_segment(std::string_view broadcaster_id,
                                                                                 std::string_view id) {
        std::string options {"?broadcaster_id=" + std::string(broadcaster_id) + "&id=" + std::string(id)};
        std::string url {TWITCH_API_BASE + "schedule/segment" + options};
        Response<std::string> response = call_api(url, this->m_app_access_token, this->m_client_id, HTTP_DELETE);
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
        Response<std::string> response = call_api(url, this->m_app_access_token, this->m_client_id, HTTP_PATCH, request);
        if (!response.data.size()) {
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
        Response<std::string> response = call_api(url, this->m_app_access_token, this->m_client_id, HTTP_PUT);
        if (!response.data.size()) {
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
        Response<std::string> response = call_api(url, this->m_app_access_token, this->m_client_id, HTTP_DELETE);
        if (!response.data.size()) {
            return {{}, "", response.code, "Bad request"};
        }

        auto [data_string, _] = get_first_value(response.data.substr(1, response.data.size() - 1));
        std::string message = !data_string.size() ? get_object_param("\"message\"", response.data) : "";

        return {json_to_vector(data_string), "", response.code, message};
    }

    VectorResponse<TwitchCreatedClip> TwitchOauthAPI::create_clip(std::string_view broadcaster_id,
                                                                  std::optional<bool> has_delay) {
        std::string options {"?broadcaster_id=" + std::string(broadcaster_id)};
        if (has_delay) {
            options += "&has_delay=" + std::string(has_delay ? "true" : "false");
        }
        std::string url {TWITCH_API_BASE + "clips" + options};
        Response<std::string> response = call_api(url, this->m_app_access_token, this->m_client_id, HTTP_POST);
        if (!response.data.size()) {
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
        if (!response.data.size()) {
            return {{}, "", response.code, "Bad request"};
        }
        VectorResponseLeftovers<TwitchBroadcasterSubscription> subs_response = this->process_response_leftovers<TwitchBroadcasterSubscription>(response);
        size_t total = std::stoul(get_object_param("\"total\"", subs_response.leftovers, "0"));
        size_t points = std::stoul(get_object_param("\"points\"", subs_response.leftovers, "0"));
        return {{TwitchBroadcasterSubscriptions(total, points, subs_response.data)}, subs_response.cursor, subs_response.code, subs_response.message};
    }

    VectorResponse<TwitchCustomReward> TwitchOauthAPI::get_custom_reward(std::string_view broadcaster_id,
                                                                         std::vector<std::string> ids,
                                                                         std::optional<bool> only_manageable_rewards) {
        std::string options {"?broadcaster_id=" + std::string(broadcaster_id)};
        for (std::string reward_id : ids) {
            options += "&id=" + reward_id;
        }
        if (only_manageable_rewards) {
            options += "&only_manageable_rewards=" + std::string(only_manageable_rewards.value() ? "true" : "false");
        }
        std::string url {TWITCH_API_BASE + "channel_points/custom_rewards" + options};
        Response<std::string> response = call_api(url, this->m_app_access_token, this->m_client_id);
        if (!response.data.size()) {
            return {{}, "", response.code, "Bad request"};
        }
        return this->process_response<TwitchCustomReward>(response);
    }

    VectorResponse<TwitchCustomReward> TwitchOauthAPI::create_custom_rewards(std::string_view broadcaster_id,
                                                                             CreateRewardRequest& request) {
        std::string options {"?broadcaster_id=" + std::string(broadcaster_id)};
        std::string new_reward {"{\"title\":\"" + request.title + "\",\"cost\":" + std::to_string(request.cost)};
        if (request.prompt) {
            new_reward += ",\"prompt\":\"" + request.prompt.value() + "\"";
        }
        if (request.background_color) {
            new_reward += ",\"background_color\":\"" + request.background_color.value() + "\"";
        }
        if (request.is_enabled) {
            new_reward += ",\"is_enabled\":" + std::string(request.is_enabled.value() ? "true" : "false");
        }
        if (request.is_user_input_required) {
            new_reward += ",\"is_user_input_required\":" + std::string(request.is_user_input_required.value() ? "true" : "false");
        }
        if (request.should_redemptions_skip_request_queue) {
            new_reward += ",\"should_redemptions_skip_request_queue\":" + std::string(request.should_redemptions_skip_request_queue.value() ? "true" : "false");
        }
        if (request.is_max_per_stream_enabled) {
            new_reward += ",\"is_max_per_stream_enabled\":" + std::string(request.is_max_per_stream_enabled.value() ? "true" : "false");
        }
        if (request.max_per_stream) {
            new_reward += ",\"max_per_stream\":" + std::to_string(request.max_per_stream.value());
        }
        if (request.is_max_per_user_per_stream_enabled) {
            new_reward += ",\"is_max_per_user_per_stream_enabled\":" + std::string(request.is_max_per_user_per_stream_enabled.value() ? "true" : "false");
        }
        if (request.max_per_user_per_stream) {
            new_reward += ",\"max_per_user_per_stream\":" + std::to_string(request.max_per_user_per_stream.value());
        }
        if (request.is_global_cooldown_enabled) {
            new_reward += ",\"is_global_cooldown_enabled\":" + std::string(request.is_global_cooldown_enabled.value() ? "true" : "false");
        }
        if (request.global_cooldown_seconds) {
            new_reward += ",\"global_cooldown_seconds\":" + std::to_string(request.global_cooldown_seconds.value());
        }
        new_reward += "}";
        std::string url {TWITCH_API_BASE + "channel_points/custom_rewards" + options};
        Response<std::string> response = call_api(url, this->m_app_access_token, this->m_client_id, HTTP_POST, new_reward);
        if (!response.data.size()) {
            return {{}, "", response.code, "Bad request"};
        }
        return this->process_response<TwitchCustomReward>(response);
    }

    VectorResponse<TwitchCustomReward> TwitchOauthAPI::update_custom_reward(std::string_view broadcaster_id,
                                                                            std::string_view reward_id,
                                                                            UpdateRewardRequest& request) {
        std::string options {"?broadcaster_id=" + std::string(broadcaster_id) + "&id=" + std::string(reward_id)};
        std::string new_reward {""};
        if (request.title) {
            new_reward += ",\"title\":\"" + request.title.value() + "\"";
        }
        if (request.cost) {
            new_reward += ",\"cost\":" + std::to_string(request.cost.value());
        }
        if (request.prompt) {
            new_reward += ",\"prompt\":\"" + request.prompt.value() + "\"";
        }
        if (request.background_color) {
            new_reward += ",\"background_color\":\"" + request.background_color.value() + "\"";
        }
        if (request.is_enabled) {
            new_reward += ",\"is_enabled\":" + std::string(request.is_enabled.value() ? "true" : "false");
        }
        if (request.is_user_input_required) {
            new_reward += ",\"is_user_input_required\":" + std::string(request.is_user_input_required.value() ? "true" : "false");
        }
        if (request.is_paused) {
            new_reward += ",\"is_paused\":" + std::string(request.is_paused.value() ? "true" : "false");
        }
        if (request.should_redemptions_skip_request_queue) {
            new_reward += ",\"should_redemptions_skip_request_queue\":" + std::string(request.should_redemptions_skip_request_queue.value() ? "true" : "false");
        }
        if (request.is_max_per_stream_enabled) {
            new_reward += ",\"is_max_per_stream_enabled\":" + std::string(request.is_max_per_stream_enabled.value() ? "true" : "false");
        }
        if (request.max_per_stream) {
            new_reward += ",\"max_per_stream\":" + std::to_string(request.max_per_stream.value());
        }
        if (request.is_max_per_user_per_stream_enabled) {
            new_reward += ",\"is_max_per_user_per_stream_enabled\":" + std::string(request.is_max_per_user_per_stream_enabled.value() ? "true" : "false");
        }
        if (request.max_per_user_per_stream) {
            new_reward += ",\"max_per_user_per_stream\":" + std::to_string(request.max_per_user_per_stream.value());
        }
        if (request.is_global_cooldown_enabled) {
            new_reward += ",\"is_global_cooldown_enabled\":" + std::string(request.is_global_cooldown_enabled.value() ? "true" : "false");
        }
        if (request.global_cooldown_seconds) {
            new_reward += ",\"global_cooldown_seconds\":" + std::to_string(request.global_cooldown_seconds.value());
        }
        std::string url {TWITCH_API_BASE + "channel_points/custom_rewards" + options};
        Response<std::string> response = call_api(url, this->m_app_access_token, this->m_client_id, HTTP_PATCH, (new_reward.size() ? ("{" + new_reward.substr(1, new_reward.size()) + "}") : "{}"));
        if (!response.data.size()) {
            return {{}, "", response.code, "Bad request"};
        }
        return this->process_response<TwitchCustomReward>(response);
    }

    Response<std::string> TwitchOauthAPI::delete_custom_reward(std::string_view broadcaster_id,
                                                                                 std::string_view id) {
        std::string options {"?broadcaster_id=" + std::string(broadcaster_id) + "&id=" + std::string(id)};
        std::string url {TWITCH_API_BASE + "channel_points/custom_rewards" + options};
        Response<std::string> response = call_api(url, this->m_app_access_token, this->m_client_id, HTTP_DELETE);
        return response;
    }

    VectorResponse<TwitchCustomRewardRedemption> TwitchOauthAPI::get_custom_reward_redemption(std::string_view broadcaster_id,
                                                                                              std::string_view reward_id,
                                                                                              std::vector<std::string> ids,
                                                                                              std::optional<std::string> status,
                                                                                              std::optional<std::string> sort,
                                                                                              std::optional<size_t> first,
                                                                                              std::optional<std::string> after) {
        std::string options {"?broadcaster_id=" + std::string(broadcaster_id) + "&reward_id=" + std::string(reward_id)};
        for (std::string redemption_id : ids) {
            options += "&id=" + redemption_id;
        }
        if (status) {
            options += "&status=" + status.value();
        }
        if (sort) {
            options += "&sort=" + sort.value();
        }
        if (first) {
            options += "&first=" + std::to_string(first.value());
        }
        if (after) {
            options += "&after=" + after.value();
        }
        std::string url {TWITCH_API_BASE + "channel_points/custom_rewards/redemptions" + options};
        Response<std::string> response = call_api(url, this->m_app_access_token, this->m_client_id);
        if (!response.data.size()) {
            return {{}, "", response.code, "Bad request"};
        }
        return this->process_response<TwitchCustomRewardRedemption>(response);
    }

    VectorResponse<TwitchCustomRewardRedemption> TwitchOauthAPI::update_redemption_status(std::string_view broadcaster_id,
                                                                                          std::string_view reward_id,
                                                                                          std::string_view redemption_id,
                                                                                          std::string_view status) {
        std::string options {"?broadcaster_id=" + std::string(broadcaster_id)
            + "&reward_id=" + std::string(reward_id)
            + "&id=" + std::string(redemption_id)};
        std::string request_body {"{\"status\":\"" + std::string(status) + "\"}"};
        std::string url {TWITCH_API_BASE + "channel_points/custom_rewards/redemptions" + options};
        Response<std::string> response = call_api(url, this->m_app_access_token, this->m_client_id, HTTP_PATCH, request_body);
        if (!response.data.size()) {
            return {{}, "", response.code, "Bad request"};
        }
        return this->process_response<TwitchCustomRewardRedemption>(response);
    }

    VectorResponse<TwitchUserSubscription> TwitchOauthAPI::check_user_subscription(std::string_view broadcaster_id,
                                                                                   std::string_view user_id) {
        std::string options {"?broadcaster_id=" + std::string(broadcaster_id) + "&user_id=" + std::string(user_id)};
        std::string url {TWITCH_API_BASE + "subscriptions/user" + options};
        Response<std::string> response = call_api(url, this->m_app_access_token, this->m_client_id);
        if (!response.data.size()) {
            return {{}, "", response.code, "Bad request"};
        }
        return this->process_response<TwitchUserSubscription>(response);
    }

    VectorResponse<TwitchStreamMarker> TwitchOauthAPI::create_stream_marker(std::string_view user_id,
                                                                            std::optional<std::string_view> description) {
        std::string request_body {"{\"user_id\":\"" + std::string(user_id)};
        if (description) {
            request_body += "\",\"description\":\"" + std::string(description.value());
        }
        request_body += "\"}";
        std::string url {TWITCH_API_BASE + "streams/markers"};
        Response<std::string> response = call_api(url, this->m_app_access_token, this->m_client_id, HTTP_POST, request_body);
        if (!response.data.size()) {
            return {{}, "", response.code, "Bad request"};
        }
        return this->process_response<TwitchStreamMarker>(response);
    }

    VectorResponse<TwitchVideosWithMarkers> TwitchOauthAPI::get_stream_markers(std::string_view user_id,
                                                                               std::string_view video_id,
                                                                               std::optional<size_t> first,
                                                                               std::optional<std::string> cursor,
                                                                               const bool& is_before) {
        std::string options {"?user_id=" + std::string(user_id) + "&video_id" + std::string(video_id)};
        if (first) {
            options += "&first=" + std::to_string(first.value());
        }
        if (cursor) {
            options += (is_before ? "&before=" : "&after=") + cursor.value();
        }
        std::string url {TWITCH_API_BASE + "streams/markers" + options};
        Response<std::string> response = call_api(url, this->m_app_access_token, this->m_client_id);
        if (!response.data.size()) {
            return {{}, "", response.code, "Bad request"};
        }
        return this->process_response<TwitchVideosWithMarkers>(response);
    }

    Response<std::string> TwitchOauthAPI::replace_stream_tags(std::string_view broadcaster_id,
                                                              std::vector<std::string> tag_ids) {
        std::string options {"?broadcaster_id=" + std::string(broadcaster_id)};
        std::string request_body {"{\"tag_ids\":" + vector_to_json(tag_ids) + "}"};
        std::string url {TWITCH_API_BASE + "streams/tags" + options};
        Response<std::string> response = call_api(url, this->m_app_access_token, this->m_client_id, HTTP_PUT, request_body);
        return response;
    }

    VectorResponse<TwitchAutoModMessageStatus> TwitchOauthAPI::check_automod_status(std::string_view broadcaster_id,
                                                                                    std::vector<TwitchAutoModMessage> messages) {
        std::string options {"?broadcaster_id=" + std::string(broadcaster_id)};
        std::string request_body {"{\"data\":["};
        for (size_t i {0}; i < messages.size(); ++i) {
            request_body += (i ? "," : "") + messages.at(i).to_json();
        }
        request_body += "]}";
        std::string url {TWITCH_API_BASE + "moderation/enforcements/status" + options};
        Response<std::string> response = call_api(url, this->m_app_access_token, this->m_client_id, HTTP_POST, request_body);
        if (!response.data.size()) {
            return {{}, "", response.code, "Bad request"};
        }
        return this->process_response<TwitchAutoModMessageStatus>(response);
    }

    Response<std::string> TwitchOauthAPI::manage_held_automod_messages(TwitchAutoModMessageManaging message) {
        std::string url {TWITCH_API_BASE + "moderation/automod/message"};
        Response<std::string> response = call_api(url, this->m_app_access_token, this->m_client_id, HTTP_POST, message.to_json());
        return response;
    }

    VectorResponse<TwitchHypeTrainEvent> TwitchOauthAPI::get_hype_train_events(std::string_view broadcaster_id,
                                                                               std::optional<std::string_view> hype_train_id,
                                                                               std::optional<size_t> first,
                                                                               std::optional<std::string> cursor) {
        std::string options {"?broadcaster_id=" + std::string(broadcaster_id)};
        if (hype_train_id) {
            options += "&id=" + std::string(hype_train_id.value());
        }
        if (first) {
            options += "&first=" + std::to_string(first.value());
        }
        if (cursor) {
            options += "&cursor=" + cursor.value();
        }
        std::string url {TWITCH_API_BASE + "hypetrain/events" + options};
        Response<std::string> response = call_api(url, this->m_app_access_token, this->m_client_id);
        if (!response.data.size()) {
            return {{}, "", response.code, "Bad request"};
        }
        return this->process_response<TwitchHypeTrainEvent>(response);
    }

    Response<TwitchUserActiveExtensions> TwitchOauthAPI::get_user_active_extensions(std::string_view user_id) {
        std::string options {"?user_id=" + std::string(user_id)};
        std::string url {TWITCH_API_BASE + "users/extensions" + options};
        Response<std::string> response = call_api(url, this->m_app_access_token, this->m_client_id);

        std::string panel = get_object_param("\"panel\"", response.data);
        std::vector<TwitchUserActiveExtension> panels = this->string_to_vector_objects<TwitchUserActiveExtension>(panel);
        std::string overlay = get_object_param("\"overlay\"", response.data);
        std::vector<TwitchUserActiveExtension> overlays = this->string_to_vector_objects<TwitchUserActiveExtension>(overlay);
        std::string component = get_object_param("\"component\"", response.data);
        std::vector<TwitchUserActiveExtension> components = this->string_to_vector_objects<TwitchUserActiveExtension>(component);

        return {{panels, overlays, components}, response.cursor, response.code, response.message};
    }

    Response<TwitchUserActiveExtensions> TwitchOauthAPI::update_user_extensions(std::optional<TwitchUserActiveExtensions> extensions) {
        std::string request_body {"{\"data\":"};
        if (extensions) {
            request_body += extensions.value().to_json();
        } else {
            request_body += "{}";
        }
        request_body += "}";
        std::string url {TWITCH_API_BASE + "users/extensions"};
        Response<std::string> response = call_api(url, this->m_app_access_token, this->m_client_id, HTTP_PUT, request_body);

        std::string message = get_object_param("\"message\"", response.data);
        std::string panel = get_object_param("\"panel\"", response.data);
        std::vector<TwitchUserActiveExtension> panels = this->string_to_vector_objects<TwitchUserActiveExtension>(panel);
        std::string overlay = get_object_param("\"overlay\"", response.data);
        std::vector<TwitchUserActiveExtension> overlays = this->string_to_vector_objects<TwitchUserActiveExtension>(overlay);
        std::string component = get_object_param("\"component\"", response.data);
        std::vector<TwitchUserActiveExtension> components = this->string_to_vector_objects<TwitchUserActiveExtension>(component);

        return {{panels, overlays, components}, response.cursor, response.code, message};
    }
}