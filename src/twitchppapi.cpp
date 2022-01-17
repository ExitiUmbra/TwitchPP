#include "twitchppapi.hpp"

namespace TwitchPP {
    TwitchAPI::TwitchAPI(const std::string& app_access_token,
                         const std::string& client_id)
                         : m_app_access_token{app_access_token},
                           m_client_id{client_id} {
    }

    template<typename T>
    VectorResponse<T> TwitchAPI::process_response(Response<std::string>& response,
                                                  std::string_view additional_field) {
        auto [data_string, leftovers] = get_first_value(response.data.substr(1, response.data.size() - 1));
        auto [cursor_string, _] = get_first_value(leftovers);
        std::string cursor = get_object_param("\"cursor\"", cursor_string);
        std::string additional_value = get_object_param(std::string("\"") + additional_field.data() + "\"", leftovers);
        std::string message = !data_string.size() ? get_object_param("\"message\"", response.data) : "";

        std::pair<std::string, std::string> cycle {"", data_string};
        std::vector<T> elements;
        do {
            cycle = get_first_value(cycle.second);
            if (cycle.first != "") {
                elements.push_back(T(cycle.first, additional_value));
            }
        } while(cycle.second != "");

        return {elements, cursor, response.code, message};
    }

    VectorResponse<TwitchGame> TwitchAPI::search_categories(std::string_view query,
                                                            std::optional<size_t> first,
                                                            std::optional<std::string_view> starting_cursor) {
        std::string options {"?query=" + replace_string(std::string(query), " ", "+")};
        if (first) {
            options += "&first=" + std::to_string(first.value());
        }
        if (starting_cursor) {
            options += "&after=" + std::string(starting_cursor.value());
        }
        std::string url {TWITCH_API_BASE + "search/categories" + options};
        Response<std::string> response = call_api(url, this->m_app_access_token, this->m_client_id);
        if (!response.data.size()) {
            return {{}, "", response.code, "Bad request"};
        }
        return this->process_response<TwitchGame>(response);
    }

    VectorResponse<TwitchGame> TwitchAPI::get_games(std::string_view game_query,
                                                    const bool& search_by_name) {
        std::string options {search_by_name ? "?name=" : "?id="};
        options += replace_string(std::string(game_query), " ", "+");

        std::string url {TWITCH_API_BASE + "games" + options};
        Response<std::string> response = call_api(url, this->m_app_access_token, this->m_client_id);
        if (!response.data.size()) {
            return {{}, "", response.code, "Bad request"};
        }
        return this->process_response<TwitchGame>(response);
    }

    VectorResponse<TwitchGame> TwitchAPI::get_games(const std::vector<std::string>& game_query,
                                                    const bool& search_by_name) {
        std::string parameter {search_by_name ? "name" : "id"};
        std::string options {"?"};
        for (std::string_view element : game_query) {
            if (options != "?") {
                options += "&";
            }
            options += parameter + "=" + replace_string(std::string(element), " ", "+");
        }

        std::string url {TWITCH_API_BASE + "games" + options};
        Response<std::string> response = call_api(url, this->m_app_access_token, this->m_client_id);
        if (!response.data.size()) {
            return {{}, "", response.code, "Bad request"};
        }
        return this->process_response<TwitchGame>(response);
    }

    VectorResponse<TwitchGame> TwitchAPI::get_top_games(std::optional<size_t> first,
                                                        std::optional<std::string> starting_cursor,
                                                        const bool& is_before) {
        std::string options {"?"};
        if (first) {
            options += std::string("first=") + std::to_string(first.value()) + (starting_cursor ? "&" : "");
        }
        if (starting_cursor) {
            options += (is_before ? "before=" : "after=") + starting_cursor.value();
        }
        std::string url {TWITCH_API_BASE + "games/top" + options};
        Response<std::string> response = call_api(url, this->m_app_access_token, this->m_client_id);
        if (!response.data.size()) {
            return {{}, "", response.code, "Bad request"};
        }
        return this->process_response<TwitchGame>(response);
    }

    VectorResponse<TwitchUser> TwitchAPI::get_users(std::optional<std::string_view> user_query,
                                                    std::optional<bool> search_by_login) {
        std::string options {"?"};
        if (user_query) {
            options += std::string(search_by_login.value() ? "login=" : "id=") + std::string(user_query.value());
        }
        std::string url {TWITCH_API_BASE + "users" + options};
        Response<std::string> response = call_api(url, this->m_app_access_token, this->m_client_id);
        if (!response.data.size()) {
            return {{}, "", response.code, "Bad request"};
        }
        return this->process_response<TwitchUser>(response);
    }

    VectorResponse<TwitchUser> TwitchAPI::get_users(const std::vector<std::string>& user_query,
                                                    const bool& search_by_login) {
        std::string parameter {search_by_login ? "login" : "id"};
        std::string options {"?"};
        for (std::string_view element : user_query) {
            if (options != "?") {
                options += "&";
            }
            options += parameter + "=" + std::string(element);
        }
        std::string url {TWITCH_API_BASE + "users" + options};
        Response<std::string> response = call_api(url, this->m_app_access_token, this->m_client_id);
        if (!response.data.size()) {
            return {{}, "", response.code, "Bad request"};
        }
        return this->process_response<TwitchUser>(response);
    }

    VectorResponse<TwitchEmote> TwitchAPI::get_global_emotes() {
        std::string url {TWITCH_API_BASE + "chat/emotes/global"};
        Response<std::string> response = call_api(url, this->m_app_access_token, this->m_client_id);
        if (!response.data.size()) {
            return {{}, "", response.code, "Bad request"};
        }
        return this->process_response<TwitchEmote>(response, "template");
    }

    VectorResponse<TwitchChannelEmote> TwitchAPI::get_channel_emotes(std::string_view broadcaster_id) {
        std::string url {TWITCH_API_BASE + "chat/emotes?broadcaster_id=" + std::string(broadcaster_id)};
        Response<std::string> response = call_api(url, this->m_app_access_token, this->m_client_id);
        if (!response.data.size()) {
            return {{}, "", response.code, "Bad request"};
        }

        auto [data_string, leftovers] = get_first_value(response.data.substr(1, response.data.size() - 1));
        auto [cursor_string, _] = get_first_value(leftovers);
        std::string cursor = get_object_param("\"cursor\"", cursor_string);
        std::string additional_value = get_object_param("\"template\"", leftovers);
        std::string message = !data_string.size() ? get_object_param("\"message\"", response.data) : "";

        std::pair<std::string, std::string> cycle {"", data_string};
        std::vector<TwitchChannelEmote> elements;
        do {
            cycle = get_first_value(cycle.second);
            if (cycle.first != "") {
                elements.push_back(TwitchChannelEmote(cycle.first, additional_value, broadcaster_id));
            }
        } while(cycle.second != "");

        return {elements, cursor, response.code, message};
    }

    VectorResponse<TwitchChannelEmote> TwitchAPI::get_emote_sets(std::string_view emote_set_id) {
        std::string options {"?emote_set_id=" + std::string(emote_set_id)};
        std::string url {TWITCH_API_BASE + "chat/emotes/set" + options};
        Response<std::string> response = call_api(url, this->m_app_access_token, this->m_client_id);
        if (!response.data.size()) {
            return {{}, "", response.code, "Bad request"};
        }
        return this->process_response<TwitchChannelEmote>(response, "template");
    }

    VectorResponse<TwitchChannelEmote> TwitchAPI::get_emote_sets(const std::vector<std::string>& emote_set_ids) {
        std::string options {"?"};
        for (std::string_view element : emote_set_ids) {
            if (options != "?") {
                options += "&";
            }
            options += "emote_set_id=" + std::string(element);
        }
        std::string url {TWITCH_API_BASE + "chat/emotes/set" + options};
        Response<std::string> response = call_api(url, this->m_app_access_token, this->m_client_id);
        if (!response.data.size()) {
            return {{}, "", response.code, "Bad request"};
        }
        return this->process_response<TwitchChannelEmote>(response, "template");
    }

    VectorResponse<TwitchChannelInformation> TwitchAPI::get_channel_information(std::string_view broadcaster_id) {
        std::string url {TWITCH_API_BASE + "channels?broadcaster_id=" + std::string(broadcaster_id)};
        Response<std::string> response = call_api(url, this->m_app_access_token, this->m_client_id);
        if (!response.data.size()) {
            return {{}, "", response.code, "Bad request"};
        }
        return this->process_response<TwitchChannelInformation>(response);
    }

    VectorResponse<TwitchChatSettings> TwitchAPI::get_chat_settings(std::string_view broadcaster_id) {
        std::string url {TWITCH_API_BASE + "chat/settings?broadcaster_id=" + std::string(broadcaster_id)};
        Response<std::string> response = call_api(url, this->m_app_access_token, this->m_client_id);
        if (!response.data.size()) {
            return {{}, "", response.code, "Bad request"};
        }
        return this->process_response<TwitchChatSettings>(response);
    }

    VectorResponse<TwitchChannel> TwitchAPI::search_channels(std::string_view query,
                                                             std::optional<bool> live_only,
                                                             std::optional<size_t> first,
                                                             std::optional<std::string> starting_cursor) {
        std::string url {TWITCH_API_BASE + "search/channels?query=" + std::string(query)};
        if (first) {
            url += std::string("&first=") + std::to_string(first.value());
        }
        if (starting_cursor) {
            url += std::string("&after=") + starting_cursor.value();
        }
        if (live_only) {
            url += std::string("&live_only=") + (live_only.value() ? "true" : "false");
        }
        Response<std::string> response = call_api(url, this->m_app_access_token, this->m_client_id);
        if (!response.data.size()) {
            return {{}, "", response.code, "Bad request"};
        }
        return this->process_response<TwitchChannel>(response);
    }

    VectorResponse<TwitchChannelTeam> TwitchAPI::get_channel_teams(std::string_view broadcaster_id) {
        std::string url {TWITCH_API_BASE + "teams/channel?broadcaster_id=" + std::string(broadcaster_id)};
        Response<std::string> response = call_api(url, this->m_app_access_token, this->m_client_id);
        if (!response.data.size()) {
            return {{}, "", response.code, "Bad request"};
        }
        return this->process_response<TwitchChannelTeam>(response);
    }

    VectorResponse<TwitchStream> TwitchAPI::get_streams(std::optional<StreamsRequest> streams_request) {
        std::string options {"?"};
        if (streams_request) {
            if (streams_request.value().first) {
                options += "first=" + std::to_string(streams_request.value().first.value()) + "&";
            }
            if (streams_request.value().cursor) {
                options += (streams_request.value().is_before ? "before=" : "after=") + streams_request.value().cursor.value() + "&";
            }
            for (std::string game_id : streams_request.value().game_ids) {
                options += "game_id=" + game_id + "&";
            }
            for (std::string language : streams_request.value().languages) {
                options += "language=" + language + "&";
            }
            for (std::string user_id : streams_request.value().user_ids) {
                options += "user_id=" + user_id + "&";
            }
            for (std::string user_login : streams_request.value().user_logins) {
                options += "user_login=" + user_login + "&";
            }
        }
        std::string url {TWITCH_API_BASE + "streams" + options};
        Response<std::string> response = call_api(url, this->m_app_access_token, this->m_client_id);
        if (!response.data.size()) {
            return {{}, "", response.code, "Bad request"};
        }
        return this->process_response<TwitchStream>(response);
    }

    VectorResponse<TwitchUsersFollows> TwitchAPI::get_users_follows(std::string_view user_id,
                                                                    const bool& is_from,
                                                                    std::optional<size_t> first,
                                                                    std::optional<std::string> after) {
        std::string options {"?"};
        options += (is_from ? "from_id=" : "to_id=") + std::string(user_id);
        if (first) {
            options += "&first=" + std::to_string(first.value());
        }
        if (after) {
            options += "&after=" + after.value();
        }
        std::string url {TWITCH_API_BASE + "users/follows" + options};
        Response<std::string> response = call_api(url, this->m_app_access_token, this->m_client_id);
        if (!response.data.size()) {
            return {{}, "", response.code, "Bad request"};
        }
        return this->process_response<TwitchUsersFollows>(response);
    }

    VectorResponse<TwitchClips> TwitchAPI::get_clips(ClipsRequest& clips_request) {
        std::string options {"?broadcaster_id="};
        options += clips_request.broadcaster_id + "&game_id=" + clips_request.game_id;
        for (std::string clip_id : clips_request.ids) {
            options += "&id=" + clip_id;
        }
        if (clips_request.first) {
            options += "&first=" + std::to_string(clips_request.first.value());
        }
        if (clips_request.cursor) {
            options += (clips_request.is_before ? "&before=" : "&after=") + clips_request.cursor.value();
        }
        if (clips_request.started_at) {
            options += "&started_at=" + clips_request.started_at.value();
        }
        if (clips_request.ended_at) {
            options += "&ended_at=" + clips_request.ended_at.value();
        }
        std::string url {TWITCH_API_BASE + "clips" + options};
        Response<std::string> response = call_api(url, this->m_app_access_token, this->m_client_id);
        if (!response.data.size()) {
            return {{}, "", response.code, "Bad request"};
        }
        return this->process_response<TwitchClips>(response);
    }

    VectorResponse<TwitchVideos> TwitchAPI::get_videos(VideosRequest& videos_request) {
        std::string options {"?"};
        for (std::string clip_id : videos_request.ids) {
            options += (options == "?" ? "id=" : "&id=") + clip_id;
        }
        if (videos_request.user_id) {
            options += (options == "?" ? "user_id=" : "&user_id=") + videos_request.user_id.value();
        }
        if (videos_request.game_id) {
            options += (options == "?" ? "game_id=" : "&game_id=") + videos_request.game_id.value();
        }
        if (videos_request.first) {
            options += "&first=" + std::to_string(videos_request.first.value());
        }
        if (videos_request.cursor) {
            options += (videos_request.is_before ? "&before=" : "&after=") + videos_request.cursor.value();
        }
        if (videos_request.language) {
            options += "&language=" + videos_request.language.value();
        }
        if (videos_request.period) {
            options += "&period=" + videos_request.period.value();
        }
        if (videos_request.sort) {
            options += "&sort=" + videos_request.sort.value();
        }
        if (videos_request.type) {
            options += "&type=" + videos_request.type.value();
        }
        std::string url {TWITCH_API_BASE + "videos" + options};
        Response<std::string> response = call_api(url, this->m_app_access_token, this->m_client_id);
        if (!response.data.size()) {
            return {{}, "", response.code, "Bad request"};
        }
        return this->process_response<TwitchVideos>(response);
    }

    VectorResponse<TwitchChannelStreamSchedule> TwitchAPI::get_channel_stream_schedule(std::string_view broadcaster_id, std::optional<OptionalScheduleRequest> options) {
        std::string url {TWITCH_API_BASE + "schedule?broadcaster_id=" + std::string(broadcaster_id)};
        if (options) {
            for (std::string segment_id : options.value().ids) {
                url += "&id=" + segment_id;
            }
            if (options.value().first) {
                url += "&first=" + std::to_string(options.value().first.value());
            }
            if (options.value().after) {
                url += "&after=" + options.value().after.value();
            }
            if (options.value().start_time) {
                url += "&start_time=" + options.value().start_time.value();
            }
            if (options.value().utc_offset) {
                url += "&utc_offset=" + options.value().utc_offset.value();
            }
        }
        Response<std::string> response = call_api(url, this->m_app_access_token, this->m_client_id);
        if (!response.data.size()) {
            return {{}, "", response.code, "Bad request"};
        }
        return this->process_single_response<TwitchChannelStreamSchedule>(response);
    }

    VectorResponse<TwitchTeam> TwitchAPI::get_teams(std::string_view team_query,
                                                    const bool& search_by_name) {
        std::string options {search_by_name ? "?name=" : "?id="};
        options += replace_string(std::string(team_query), " ", "+");

        std::string url {TWITCH_API_BASE + "teams" + options};
        Response<std::string> response = call_api(url, this->m_app_access_token, this->m_client_id);
        if (!response.data.size()) {
            return {{}, "", response.code, "Bad request"};
        }
        return this->process_response<TwitchTeam>(response);
    }

    VectorResponse<TwitchTeam> TwitchAPI::get_teams(const std::vector<std::string>& team_query,
                                                    const bool& search_by_name) {
        std::string parameter {search_by_name ? "name" : "id"};
        std::string options {"?"};
        for (std::string_view element : team_query) {
            if (options != "?") {
                options += "&";
            }
            options += parameter + "=" + replace_string(std::string(element), " ", "+");
        }

        std::string url {TWITCH_API_BASE + "teams" + options};
        Response<std::string> response = call_api(url, this->m_app_access_token, this->m_client_id);
        if (!response.data.size()) {
            return {{}, "", response.code, "Bad request"};
        }
        return this->process_response<TwitchTeam>(response);
    }

    VectorResponse<TwitchBadgeSet> TwitchAPI::get_global_chat_badges() {
        std::string url {TWITCH_API_BASE + "chat/badges/global"};
        Response<std::string> response = call_api(url, this->m_app_access_token, this->m_client_id);
        if (!response.data.size()) {
            return {{}, "", response.code, "Bad request"};
        }
        return this->process_response<TwitchBadgeSet>(response);
    }

    VectorResponse<TwitchBadgeSet> TwitchAPI::get_channel_chat_badges(std::string_view broadcaster_id) {
        std::string url {TWITCH_API_BASE + "chat/badges?broadcaster_id=" + std::string(broadcaster_id)};
        Response<std::string> response = call_api(url, this->m_app_access_token, this->m_client_id);
        if (!response.data.size()) {
            return {{}, "", response.code, "Bad request"};
        }
        return this->process_response<TwitchBadgeSet>(response);
    }

    Response<std::string> TwitchAPI::get_channel_icalendar(std::string_view broadcaster_id) {
        std::string url {TWITCH_API_BASE + "schedule/icalendar?broadcaster_id=" + std::string(broadcaster_id)};
        Response<std::string> response = call_api(url, this->m_app_access_token, this->m_client_id);
        return response;
    }

    VectorResponse<TwitchCheermote> TwitchAPI::get_cheermotes(std::optional<std::string_view> broadcaster_id) {
        std::string options {""};
        if (broadcaster_id) {
            options += "?broadcaster_id=" + std::string(broadcaster_id.value());
        }
        std::string url {TWITCH_API_BASE + "bits/cheermotes" + options};
        Response<std::string> response = call_api(url, this->m_app_access_token, this->m_client_id);
        if (!response.data.size()) {
            return {{}, "", response.code, "Bad request"};
        }
        return this->process_response<TwitchCheermote>(response);
    }

    VectorResponse<TwitchBasicPlaylist> TwitchAPI::get_soundtrack_playlists() {
        std::string url {TWITCH_API_BASE + "soundtrack/playlists"};
        Response<std::string> response = call_api(url, this->m_app_access_token, this->m_client_id);
        if (!response.data.size()) {
            return {{}, "", response.code, "Bad request"};
        }
        return this->process_response<TwitchBasicPlaylist>(response);
    }

    VectorResponse<TwitchPlaylist> TwitchAPI::get_soundtrack_playlist(std::string_view playlist_id) {
        std::string options {"?id=" + std::string(playlist_id)};
        std::string url {TWITCH_API_BASE + "soundtrack/playlist" + options};
        Response<std::string> response = call_api(url, this->m_app_access_token, this->m_client_id);
        if (!response.data.size()) {
            return {{}, "", response.code, "Bad request"};
        }
        return this->process_response<TwitchPlaylist>(response);
    }

    VectorResponse<TwitchCurrentTrack> TwitchAPI::get_soundtrack_current_track(std::string_view broadcaster_id) {
        std::string options {"?broadcaster_id=" + std::string(broadcaster_id)};
        std::string url {TWITCH_API_BASE + "soundtrack/current_track" + options};
        Response<std::string> response = call_api(url, this->m_app_access_token, this->m_client_id);
        if (!response.data.size()) {
            return {{}, "", response.code, "Bad request"};
        }
        return this->process_response<TwitchCurrentTrack>(response);
    }

    VectorResponse<TwitchStreamTag> TwitchAPI::get_all_stream_tags(const std::vector<std::string>& tag_ids,
                                                                   std::optional<size_t> first,
                                                                   std::optional<std::string> after) {
        std::string options {""};
        for (std::string tag_id : tag_ids) {
            options += "&tag_id=" + tag_id;
        }
        if (first) {
            options += "&first=" + std::to_string(first.value());
        }
        if (after) {
            options += "&after=" + after.value();
        }
        std::string url {TWITCH_API_BASE + "tags/streams" + (options.size() ? ("?" + options.substr(1, options.size())) : options)};
        Response<std::string> response = call_api(url, this->m_app_access_token, this->m_client_id);
        if (!response.data.size()) {
            return {{}, "", response.code, "Bad request"};
        }
        return this->process_response<TwitchStreamTag>(response);
    }

    VectorResponse<TwitchStreamTag> TwitchAPI::get_stream_tags(std::string_view broadcaster_id) {
        std::string options {"?broadcaster_id=" + std::string(broadcaster_id)};
        std::string url {TWITCH_API_BASE + "streams/tags" + options};
        Response<std::string> response = call_api(url, this->m_app_access_token, this->m_client_id);
        if (!response.data.size()) {
            return {{}, "", response.code, "Bad request"};
        }
        return this->process_response<TwitchStreamTag>(response);
    }
}
