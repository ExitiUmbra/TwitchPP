#include "twitchpptypes.hpp"

namespace TwitchPP {
    invalid_oauth::invalid_oauth(const std::string& message)
        : m_message {message} {
    }

    std::string_view invalid_oauth::what() const { return this->m_message; }

    missing_permission::missing_permission(const std::string& permission)
        : m_permission {permission} {
    }

    std::string_view missing_permission::what() const { return "OAuth token has no permission: " + this->m_permission; }

    TwitchEmote::TwitchEmote(const std::string& json, const std::string& emote_template) {
        this->m_id = get_object_param("\"id\"", json);
        this->m_name = get_object_param("\"name\"", json);
        this->m_url_1x = get_object_param("\"url_1x\"", json);
        this->m_url_2x = get_object_param("\"url_2x\"", json);
        this->m_url_4x = get_object_param("\"url_4x\"", json);
        this->m_format = json_to_vector(get_object_param("\"format\"", json));
        this->m_scale = json_to_vector(get_object_param("\"scale\"", json));
        this->m_theme_mode = json_to_vector(get_object_param("\"theme_mode\"", json));
        this->m_template = emote_template;
    }

    TwitchEmote::TwitchEmote(const std::string& id,
                             const std::string& name,
                             const std::string& url_1x,
                             const std::string& url_2x,
                             const std::string& url_4x,
                             const std::vector<std::string>& format,
                             const std::vector<std::string>& scale,
                             const std::vector<std::string>& theme_mode,
                             const std::string& emote_template)
                             : m_id{id},
                               m_name{name},
                               m_url_1x{url_1x},
                               m_url_2x{url_2x},
                               m_url_4x{url_4x},
                               m_format{format},
                               m_scale{scale},
                               m_theme_mode{theme_mode},
                               m_template{emote_template} {
    }

    std::string TwitchEmote::to_json() const {
        std::string json = "{\"id\":\"" + this->m_id
            + "\",\"name\":\"" + this->m_name
            + "\",\"images\":{"
            + "\"url_1x\":\"" + this->m_url_1x
            + "\",\"url_2x\":\"" + this->m_url_2x
            + "\",\"url_4x\":\"" + this->m_url_4x
            + "\"},"
            + "\"format\":" + vector_to_json(this->m_format)
            + ",\"scale\":" + vector_to_json(this->m_scale)
            + ",\"theme_mode\":" + vector_to_json(this->m_theme_mode)
            + ",\"template\":\"" + this->m_template + "\"}";
        return json;
    }

    TwitchChannelEmote::TwitchChannelEmote(const std::string& id,
                                           const std::string& name,
                                           const std::string& url_1x,
                                           const std::string& url_2x,
                                           const std::string& url_4x,
                                           const std::vector<std::string>& format,
                                           const std::vector<std::string>& scale,
                                           const std::vector<std::string>& theme_mode,
                                           const std::string& emote_template,
                                           const std::string& emote_type,
                                           const std::string& emote_set_id,
                                           const std::string& tier,
                                           std::optional<std::string_view> owner_id)
                                           : TwitchEmote{id, name, url_1x, url_2x, url_4x, format, scale, theme_mode, emote_template},
                                             m_emote_type{emote_type},
                                             m_emote_set_id{emote_set_id},
                                             m_tier{tier},
                                             m_owner_id{owner_id.value_or("")} {
    }

    TwitchChannelEmote::TwitchChannelEmote(const std::string& json,
                                           const std::string& emote_template,
                                           std::optional<std::string_view> owner_id)
                                           : TwitchEmote{json, emote_template} {
        this->m_emote_type = get_object_param("\"emote_type\"", json);
        this->m_emote_set_id = get_object_param("\"emote_set_id\"", json);
        this->m_tier = get_object_param("\"tier\"", json);
        this->m_owner_id = owner_id.value_or(get_object_param("\"owner_id\"", json));
    }

    std::string TwitchChannelEmote::to_json() const {
        std::string json = "{\"id\":\"" + this->m_id
            + "\",\"name\":\"" + this->m_name
            + "\",\"emote_type\":\"" + this->m_emote_type
            + "\",\"emote_set_id\":\"" + this->m_emote_set_id
            + "\",\"tier\":\"" + this->m_tier
            + "\",\"owner_id\":\"" + this->m_owner_id
            + "\",\"images\":{"
            + "\"url_1x\":\"" + this->m_url_1x
            + "\",\"url_2x\":\"" + this->m_url_2x
            + "\",\"url_4x\":\"" + this->m_url_4x
            + "\"},"
            + "\"format\":" + vector_to_json(this->m_format)
            + ",\"scale\":" + vector_to_json(this->m_scale)
            + ",\"theme_mode\":" + vector_to_json(this->m_theme_mode)
            + ",\"template\":\"" + this->m_template + "\"}";
        return json;
    }

    TwitchChannelEditor::TwitchChannelEditor(const std::string& json) {
        this->m_user_id = get_object_param("\"user_id\"", json);
        this->m_user_name = get_object_param("\"user_name\"", json);
        this->m_created_at = get_object_param("\"created_at\"", json);
    }

    TwitchChannelEditor::TwitchChannelEditor(const std::string& user_id,
                                             const std::string& user_name,
                                             const std::string& created_at)
                                             : m_user_id{user_id},
                                               m_user_name{user_name},
                                               m_created_at{created_at} {
    }

    std::string TwitchChannelEditor::to_json() const {
        std::string json = "{\"user_id\":\"" + this->m_user_id
            + "\",\"user_name\":\"" + this->m_user_name
            + "\",\"created_at\":\"" + this->m_created_at
            + "\"}";
        return json;
    }

    TwitchUserExtension::TwitchUserExtension(const std::string& json) {
        this->m_id = get_object_param("\"id\"", json);
        this->m_version = get_object_param("\"version\"", json);
        this->m_name = get_object_param("\"name\"", json);
        this->m_can_activate = get_object_param("\"can_activate\"", json) == "true";
        this->m_type = json_to_vector(get_object_param("\"type\"", json));
    }

    TwitchUserExtension::TwitchUserExtension(const std::string& id,
                                             const std::string& version,
                                             const std::string& name,
                                             const bool& can_activate,
                                             const std::vector<std::string>& type)
                                             : m_id{id},
                                               m_version{version},
                                               m_name{name},
                                               m_can_activate{can_activate},
                                               m_type{type} {
    }

    std::string TwitchUserExtension::to_json() const {
        std::string json = "{\"id\":\"" + this->m_id
            + "\",\"version\":\"" + this->m_version
            + "\",\"name\":\"" + this->m_name
            + "\",\"can_activate\":" + (this->m_can_activate ? "true" : "false")
            + ",\"type\":" + vector_to_json(this->m_type) + "}";
        return json;
    }

    TwitchBasicUser::TwitchBasicUser(const std::string& json) {
        this->m_user_id = get_object_param("\"user_id\"", json);
        this->m_user_name = get_object_param("\"user_name\"", json);
        this->m_user_login = get_object_param("\"user_login\"", json);
        if (!this->m_user_name.size()) {
            this->m_user_name = get_object_param("\"display_name\"", json);
        }
    }

    TwitchBasicUser::TwitchBasicUser(const std::string& user_id,
                                     const std::string& user_name,
                                     const std::string& user_login)
                                     : m_user_id{user_id},
                                       m_user_name{user_name},
                                       m_user_login{user_login} {
    }

    std::string TwitchBasicUser::to_json() const {
        std::string json = "{\"user_id\":\"" + this->m_user_id
            + "\",\"user_name\":\"" + this->m_user_name
            + "\",\"user_login\":\"" + this->m_user_login
            + "\"}";
        return json;
    }

    TwitchAnalyticsResponse::TwitchAnalyticsResponse(const std::string& json) {
        this->m_id = get_object_param("\"game_id\"", json);
        if (!this->m_id.size()) {
            this->m_id = get_object_param("\"extension_id\"", json);
            this->m_id_var = "extension_id";
        } else {
            this->m_id_var = "game_id";
        }
        this->m_started_at = get_object_param("\"started_at\"", json);
        this->m_ended_at = get_object_param("\"ended_at\"", json);
        this->m_type = get_object_param("\"type\"", json);
        this->m_url = get_object_param("\"url\"", json);
    }

    TwitchAnalyticsResponse::TwitchAnalyticsResponse(const std::string& id,
                                                     const std::string& id_var,
                                                     const std::string& started_at,
                                                     const std::string& ended_at,
                                                     const std::string& type,
                                                     const std::string& url)
                                                     : m_id{id},
                                                       m_id_var{id_var},
                                                       m_started_at{started_at},
                                                       m_ended_at{ended_at},
                                                       m_type{type},
                                                       m_url{url} {
    }

    std::string TwitchAnalyticsResponse::to_json() const {
        std::string json = "{\"" + this->m_id_var + "\":\"" + this->m_id
            + "\",\"date_range\":{\"started_at\":\"" + this->m_started_at
            + "\",\"ended_at\":\"" + this->m_ended_at
            + "\"},\"type\":\"" + this->m_type
            + "\",\"url\":\"" + this->m_url
            + "\"}";
        return json;
    }

    TwitchCommercialResponse::TwitchCommercialResponse(const std::string& json) {
        this->m_message = get_object_param("\"message\"", json);
        this->m_length = std::stoul(get_object_param("\"length\"", json, "0"));
        this->m_retry_after = std::stoul(get_object_param("\"retry_after\"", json, "0"));
    }

    TwitchCommercialResponse::TwitchCommercialResponse(const std::string& message,
                                                       const size_t& length,
                                                       const size_t& retry_after)
                                                       : m_message{message},
                                                         m_length{length},
                                                         m_retry_after{retry_after} {
    }

    std::string TwitchCommercialResponse::to_json() const {
        std::string json = "{\"message\":\"" + this->m_message
            + "\",\"length\":" + std::to_string(this->m_length)
            + ",\"retry_after\":" + std::to_string(this->m_retry_after)
            + "}";
        return json;
    }

    TwitchBanResponse::TwitchBanResponse(const std::string& json) {
        this->m_broadcaster_id = get_object_param("\"broadcaster_id\"", json);
        this->m_moderator_id = get_object_param("\"moderator_id\"", json);
        this->m_user_id = get_object_param("\"user_id\"", json);
        this->m_end_time = get_object_param("\"end_time\"", json);
    }

    TwitchBanResponse::TwitchBanResponse(const std::string& broadcaster_id,
                                         const std::string& moderator_id,
                                         const std::string& user_id,
                                         const std::string& end_time)
                                         : m_broadcaster_id{broadcaster_id},
                                           m_moderator_id{moderator_id},
                                           m_user_id{user_id},
                                           m_end_time{end_time} {
    }

    std::string TwitchBanResponse::to_json() const {
        std::string json = "{\"broadcaster_id\":\"" + this->m_broadcaster_id
            + "\",\"moderator_id\":\"" + this->m_moderator_id
            + "\",\"user_id\":\"" + this->m_user_id
            + "\",\"end_time\":\"" + this->m_end_time
            + "\"}";
        return json;
    }

    TwitchBannedUser::TwitchBannedUser(const std::string& json) : TwitchBasicUser{json} {
        this->m_moderator_id = get_object_param("\"moderator_id\"", json);
        this->m_moderator_login = get_object_param("\"moderator_login\"", json);
        this->m_moderator_name = get_object_param("\"moderator_name\"", json);
        this->m_expires_at = get_object_param("\"expires_at\"", json);
        this->m_reason = get_object_param("\"reason\"", json);
    }

    TwitchBannedUser::TwitchBannedUser(const std::string& user_id,
                                       const std::string& user_name,
                                       const std::string& user_login,
                                       const std::string& moderator_id,
                                       const std::string& moderator_login,
                                       const std::string& moderator_name,
                                       const std::string& expires_at,
                                       const std::string& reason)
                                       : TwitchBasicUser{user_id, user_name, user_login},
                                         m_moderator_id{moderator_id},
                                         m_moderator_login{moderator_login},
                                         m_moderator_name{moderator_name},
                                         m_expires_at{expires_at},
                                         m_reason{reason} {
    }

    std::string TwitchBannedUser::to_json() const {
        std::string json = "{\"user_id\":\"" + this->m_user_id
            + "\",\"user_name\":\"" + this->m_user_name
            + "\",\"user_login\":\"" + this->m_user_login
            + "\",\"moderator_id\":\"" + this->m_moderator_id
            + "\",\"moderator_login\":\"" + this->m_moderator_login
            + "\",\"moderator_name\":\"" + this->m_moderator_name
            + "\",\"expires_at\":\"" + this->m_expires_at
            + "\",\"reason\":\"" + this->m_reason
            + "\"}";
        return json;
    }

    TwitchUserBits::TwitchUserBits(const std::string& json) : TwitchBasicUser{json} {
        this->m_rank = std::stoul(get_object_param("\"rank\"", json, "0"));
        this->m_score = std::stoul(get_object_param("\"score\"", json, "0"));
    }

    TwitchUserBits::TwitchUserBits(const std::string& user_id,
                                   const std::string& user_name,
                                   const std::string& user_login,
                                   const size_t& rank,
                                   const size_t& score)
                                   : TwitchBasicUser{user_id, user_name, user_login},
                                     m_rank{rank},
                                     m_score{score} {
    }

    std::string TwitchUserBits::to_json() const {
        std::string json = "{\"user_id\":\"" + this->m_user_id
            + "\",\"user_name\":\"" + this->m_user_name
            + "\",\"user_login\":\"" + this->m_user_login
            + "\",\"rank\":" + std::to_string(this->m_rank)
            + ",\"score\":" + std::to_string(this->m_score)
            + "}";
        return json;
    }

    TwitchBitsLeaderboard::TwitchBitsLeaderboard(const std::string& json, const std::vector<TwitchUserBits>& users) {
        this->m_started_at = get_object_param("\"started_at\"", json);
        this->m_ended_at = get_object_param("\"ended_at\"", json);
        this->m_total = std::stoul(get_object_param("\"total\"", json, "0"));
        this->m_users = users;
    }

    TwitchBitsLeaderboard::TwitchBitsLeaderboard(const std::string& started_at,
                                                 const std::string& ended_at,
                                                 const size_t& total,
                                                 const std::vector<TwitchUserBits>& users)
                                                 : m_started_at{started_at},
                                                   m_ended_at{ended_at},
                                                   m_total{total},
                                                   m_users{users} {
    }

    std::string TwitchBitsLeaderboard::to_json() const {
        std::string json = "{\"started_at\":\"" + this->m_started_at
            + "\",\"ended_at\":\"" + this->m_ended_at
            + "\",\"total\":" + std::to_string(this->m_total)
            + ",\"users\":[";
        for (size_t i {0}; i < this->m_users.size(); ++i) {
            json += this->m_users.at(i).to_json();
            if (i + 1 < this->m_users.size()) {
                json += ',';
            }
        }
        return json + "]}";
    }

    TwitchModeratorEventData::TwitchModeratorEventData(const std::string& json) : TwitchBasicUser{json} {
        this->m_broadcaster_id = get_object_param("\"broadcaster_id\"", json);
        this->m_broadcaster_login = get_object_param("\"broadcaster_login\"", json);
        this->m_broadcaster_name = get_object_param("\"broadcaster_name\"", json);
    }

    TwitchModeratorEventData::TwitchModeratorEventData(const std::string& user_id,
                                                       const std::string& user_name,
                                                       const std::string& user_login,
                                                       const std::string& broadcaster_id,
                                                       const std::string& broadcaster_login,
                                                       const std::string& broadcaster_name)
                                                       : TwitchBasicUser{user_id, user_name, user_login},
                                                         m_broadcaster_id{broadcaster_id},
                                                         m_broadcaster_login{broadcaster_login},
                                                         m_broadcaster_name{broadcaster_name} {
    }

    std::string TwitchModeratorEventData::to_json() const {
        std::string json = "{\"user_id\":\"" + this->m_user_id
            + "\",\"user_name\":\"" + this->m_user_name
            + "\",\"user_login\":\"" + this->m_user_login
            + "\",\"broadcaster_id\":\"" + this->m_broadcaster_id
            + "\",\"broadcaster_login\":\"" + this->m_broadcaster_login
            + "\",\"broadcaster_name\":\"" + this->m_broadcaster_name
            + "\"}";
        return json;
    }

    TwitchBannedUserEx::TwitchBannedUserEx(const std::string& json) : TwitchBannedUser{json} {
        this->m_broadcaster_id = get_object_param("\"broadcaster_id\"", json);
        this->m_broadcaster_login = get_object_param("\"broadcaster_login\"", json);
        this->m_broadcaster_name = get_object_param("\"broadcaster_name\"", json);
    }

    TwitchBannedUserEx::TwitchBannedUserEx(const std::string& user_id,
                                           const std::string& user_name,
                                           const std::string& user_login,
                                           const std::string& moderator_id,
                                           const std::string& moderator_login,
                                           const std::string& moderator_name,
                                           const std::string& expires_at,
                                           const std::string& reason,
                                           const std::string& broadcaster_id,
                                           const std::string& broadcaster_login,
                                           const std::string& broadcaster_name)
                                           : TwitchBannedUser{user_id,
                                                              user_name,
                                                              user_login,
                                                              moderator_id,
                                                              moderator_login,
                                                              moderator_name,
                                                              expires_at,
                                                              reason},
                                             m_broadcaster_id{broadcaster_id},
                                             m_broadcaster_login{broadcaster_login},
                                             m_broadcaster_name{broadcaster_name} {
    }

    std::string TwitchBannedUserEx::to_json() const {
        std::string json = "{\"user_id\":\"" + this->m_user_id
            + "\",\"user_name\":\"" + this->m_user_name
            + "\",\"user_login\":\"" + this->m_user_login
            + "\",\"moderator_id\":\"" + this->m_moderator_id
            + "\",\"moderator_login\":\"" + this->m_moderator_login
            + "\",\"moderator_name\":\"" + this->m_moderator_name
            + "\",\"expires_at\":\"" + this->m_expires_at
            + "\",\"reason\":\"" + this->m_reason
            + "\",\"broadcaster_id\":\"" + this->m_broadcaster_id
            + "\",\"broadcaster_login\":\"" + this->m_broadcaster_login
            + "\",\"broadcaster_name\":\"" + this->m_broadcaster_name
            + "\"}";
        return json;
    }

    TwitchBannedEvent::TwitchBannedEvent(const std::string& json) {
        this->m_id = get_object_param("\"id\"", json);
        this->m_event_type = get_object_param("\"event_type\"", json);
        this->m_event_timestamp = get_object_param("\"event_timestamp\"", json);
        this->m_version = get_object_param("\"version\"", json);
        this->m_event_data = std::make_shared<TwitchBannedUserEx>(get_object_param("\"event_data\"", json));
    }

    TwitchBannedEvent::TwitchBannedEvent(const std::string& id,
                                         const std::string& event_type,
                                         const std::string& event_timestamp,
                                         const std::string& version,
                                         TwitchBannedUserEx& event_data)
                                         : m_id{id},
                                           m_event_type{event_type},
                                           m_event_timestamp{event_timestamp},
                                           m_version{version},
                                           m_event_data{&event_data} {
    }

    std::string TwitchBannedEvent::to_json() const {
        std::string json = "{\"id\":\"" + this->m_id
            + "\",\"event_type\":\"" + this->m_event_type
            + "\",\"event_timestamp\":\"" + this->m_event_timestamp
            + "\",\"version\":\"" + this->m_version
            + "\",\"event_data\":" + this->m_event_data->to_json()
            + "}";
        return json;
    }

    TwitchModeratorEvent::TwitchModeratorEvent(const std::string& json) {
        this->m_id = get_object_param("\"id\"", json);
        this->m_event_type = get_object_param("\"event_type\"", json);
        this->m_event_timestamp = get_object_param("\"event_timestamp\"", json);
        this->m_version = get_object_param("\"version\"", json);
        this->m_event_data = std::make_shared<TwitchModeratorEventData>(get_object_param("\"event_data\"", json));
    }

    TwitchModeratorEvent::TwitchModeratorEvent(const std::string& id,
                                               const std::string& event_type,
                                               const std::string& event_timestamp,
                                               const std::string& version,
                                               TwitchModeratorEventData& event_data)
                                               : m_id{id},
                                                 m_event_type{event_type},
                                                 m_event_timestamp{event_timestamp},
                                                 m_version{version},
                                                 m_event_data{&event_data} {
    }

    std::string TwitchModeratorEvent::to_json() const {
        std::string json = "{\"id\":\"" + this->m_id
            + "\",\"event_type\":\"" + this->m_event_type
            + "\",\"event_timestamp\":\"" + this->m_event_timestamp
            + "\",\"version\":\"" + this->m_version
            + "\",\"event_data\":" + this->m_event_data->to_json()
            + "}";
        return json;
    }

    DatetimePeriod::DatetimePeriod(const std::string& json) {
        this->m_start_time = get_object_param("\"start_time\"", json);
        this->m_end_time = get_object_param("\"end_time\"", json);
    }

    DatetimePeriod::DatetimePeriod(const std::string& start_time,
                                   const std::string& end_time)
                                   : m_start_time{start_time},
                                     m_end_time{end_time} {
    }

    std::string DatetimePeriod::to_json() const {
        std::string json = "{\"start_time\":\"" + this->m_start_time
            + "\",\"end_time\":\"" + this->m_end_time
            + "\"}";
        return json;
    }

    TwitchCategory::TwitchCategory(const std::string& json) {
        this->m_id = get_object_param("\"id\"", json);
        this->m_name = get_object_param("\"name\"", json);
    }

    TwitchCategory::TwitchCategory(const std::string& id,
                                   const std::string& name)
                                   : m_id{id},
                                     m_name{name} {
    }

    std::string TwitchCategory::get_id() const {
        return this->m_id;
    }

    std::string TwitchCategory::get_name() const {
        return this->m_name;
    }

    std::string TwitchCategory::to_json() const {
        return "{\"id\":\"" + this->m_id + "\",\"name\":\"" + this->m_name + "\"}";
    }

    TwitchGame::TwitchGame(const std::string& json) : TwitchCategory{json} {
        this->m_box_art_url = get_object_param("\"box_art_url\"", json);
    }

    TwitchGame::TwitchGame(const std::string& id,
                           const std::string& name,
                           const std::string& box_art_url)
                           : TwitchCategory{id, name},
                             m_box_art_url{box_art_url} {
    }

    std::string TwitchGame::get_box_art_url(const size_t& width, const size_t& height) {
        std::string sized_url {this->m_box_art_url};
        size_t width_start = sized_url.find("{width}");
        sized_url.replace(width_start, 7, std::to_string(width));
        size_t height_start = sized_url.find("{height}", width_start);
        sized_url.replace(height_start, 8, std::to_string(height));
        return sized_url;
    }

    std::string TwitchGame::to_json() const {
        std::string category_json = TwitchCategory::to_json();
        return category_json.substr(0, category_json.size() - 1) + ",\"box_art_url\":\"" + this->m_box_art_url + "\"}";
    }

    TwitchUser::TwitchUser(const std::string& json) {
        this->m_id = get_object_param("\"id\"", json);
        this->m_login = get_object_param("\"login\"", json);
        this->m_display_name = get_object_param("\"display_name\"", json);
        this->m_type = get_object_param("\"type\"", json);
        this->m_broadcaster_type = get_object_param("\"broadcaster_type\"", json);
        this->m_description = get_object_param("\"description\"", json);
        this->m_profile_image_url = get_object_param("\"profile_image_url\"", json);
        this->m_offline_image_url = get_object_param("\"offline_image_url\"", json);
        this->m_created_at = get_object_param("\"created_at\"", json);
        this->m_view_count = std::stoul(get_object_param("\"view_count\"", json, "0"));
        this->m_email = get_object_param("\"email\"", json);
    }

    TwitchUser::TwitchUser(const std::string& id,
                           const std::string& login,
                           const std::string& display_name,
                           const std::string& type,
                           const std::string& broadcaster_type,
                           const std::string& description,
                           const std::string& profile_image_url,
                           const std::string& offline_image_url,
                           const std::string& created_at,
                           const size_t& view_count,
                           const std::string& email)
                           : m_id{id},
                             m_login{login},
                             m_display_name{display_name},
                             m_type{type},
                             m_broadcaster_type{broadcaster_type},
                             m_description{description},
                             m_profile_image_url{profile_image_url},
                             m_offline_image_url{offline_image_url},
                             m_created_at{created_at},
                             m_view_count{view_count},
                             m_email{email} {
    }

    std::string TwitchUser::get_id() const {
        return this->m_id;
    }

    std::string TwitchUser::to_json() const {
        std::string json = "{\"id\":\"" + this->m_id
            + "\",\"login\":\"" + this->m_login
            + "\",\"display_name\":\"" + this->m_display_name
            + "\",\"type\":\"" + this->m_type
            + "\",\"broadcaster_type\":\"" + this->m_broadcaster_type
            + "\",\"description\":\"" + this->m_description
            + "\",\"profile_image_url\":\"" + this->m_profile_image_url
            + "\",\"offline_image_url\":\"" + this->m_offline_image_url
            + "\",\"created_at\":\"" + this->m_created_at
            + "\",\"view_count\":" + std::to_string(this->m_view_count);
        if (this->m_email != "") {
            json += ",\"email\":\"" + this->m_email + "\"";
        }
        json += "}";
        return json;
    }

    TwitchChannelInformation::TwitchChannelInformation(const std::string& json) {
        this->m_broadcaster_id = get_object_param("\"broadcaster_id\"", json);
        this->m_broadcaster_login = get_object_param("\"broadcaster_login\"", json);
        this->m_broadcaster_name = get_object_param("\"broadcaster_name\"", json);
        this->m_game_name = get_object_param("\"game_name\"", json);
        this->m_game_id = get_object_param("\"game_id\"", json);
        this->m_broadcaster_language = get_object_param("\"broadcaster_language\"", json);
        this->m_title = get_object_param("\"title\"", json);
        this->m_delay = std::stoul(get_object_param("\"delay\"", json, "0"));
    }

    TwitchChannelInformation::TwitchChannelInformation(const std::string& broadcaster_id,
                                                       const std::string& broadcaster_login,
                                                       const std::string& broadcaster_name,
                                                       const std::string& game_name,
                                                       const std::string& game_id,
                                                       const std::string& broadcaster_language,
                                                       const std::string& title,
                                                       const size_t& delay)
                                                       : m_broadcaster_id{broadcaster_id},
                                                         m_broadcaster_login{broadcaster_login},
                                                         m_broadcaster_name{broadcaster_name},
                                                         m_game_name{game_name},
                                                         m_game_id{game_id},
                                                         m_broadcaster_language{broadcaster_language},
                                                         m_title{title},
                                                         m_delay{delay} {
    }

    std::string TwitchChannelInformation::to_json() const {
        std::string json = "{\"broadcaster_id\":\"" + this->m_broadcaster_id
            + "\",\"broadcaster_login\":\"" + this->m_broadcaster_login
            + "\",\"broadcaster_name\":\"" + this->m_broadcaster_name
            + "\",\"broadcaster_language\":\"" + this->m_broadcaster_language
            + "\",\"game_id\":\"" + this->m_game_id
            + "\",\"game_name\":\"" + this->m_game_name
            + "\",\"title\":\"" + this->m_title
            + "\",\"delay\":" + std::to_string(this->m_delay)
            + "}";
        return json;
    }

    TwitchChatSettings::TwitchChatSettings(const std::string& json) {
        std::string smwt {get_object_param("\"slow_mode_wait_time\"", json, "null")};
        std::string fmd {get_object_param("\"follower_mode_duration\"", json, "null")};
        this->m_broadcaster_id = get_object_param("\"broadcaster_id\"", json);
        this->m_slow_mode = get_object_param("\"slow_mode\"", json) == "true";
        this->m_slow_mode_wait_time = smwt == "null" ? -1 : std::stoi(smwt);
        this->m_follower_mode = get_object_param("\"follower_mode\"", json) == "true";
        this->m_follower_mode_duration = fmd == "null" ? -1 : std::stoi(fmd);
        this->m_subscriber_mode = get_object_param("\"subscriber_mode\"", json) == "true";
        this->m_emote_mode = get_object_param("\"emote_mode\"", json) == "true";
        this->m_unique_chat_mode = get_object_param("\"unique_chat_mode\"", json) == "true";
    }

    TwitchChatSettings::TwitchChatSettings(const std::string& broadcaster_id,
                                           const bool& slow_mode,
                                           const int& slow_mode_wait_time,
                                           const bool& follower_mode,
                                           const int& follower_mode_duration,
                                           const bool& subscriber_mode,
                                           const bool& emote_mode,
                                           const bool& unique_chat_mode)
                                           : m_broadcaster_id{broadcaster_id},
                                             m_slow_mode{slow_mode},
                                             m_slow_mode_wait_time{slow_mode_wait_time},
                                             m_follower_mode{follower_mode},
                                             m_follower_mode_duration{follower_mode_duration},
                                             m_subscriber_mode{subscriber_mode},
                                             m_emote_mode{emote_mode},
                                             m_unique_chat_mode{unique_chat_mode} {
    }

    std::string TwitchChatSettings::to_json() const {
        return "{\"broadcaster_id\":\"" + this->m_broadcaster_id
            + "\",\"slow_mode\":" + (this->m_slow_mode ? "true" : "false")
            + ",\"slow_mode_wait_time\":" + (this->m_slow_mode_wait_time > -1 ? std::to_string(this->m_slow_mode_wait_time) : "null")
            + ",\"follower_mode\":" + (this->m_follower_mode ? "true" : "false")
            + ",\"follower_mode_duration\":" + (this->m_follower_mode_duration > -1 ? std::to_string(this->m_follower_mode_duration) : "null")
            + ",\"subscriber_mode\":" + (this->m_subscriber_mode ? "true" : "false")
            + ",\"emote_mode\":" + (this->m_emote_mode ? "true" : "false")
            + ",\"unique_chat_mode\":" + (this->m_unique_chat_mode ? "true" : "false")
            + "}";
    }

    TwitchModeratorChatSettings::TwitchModeratorChatSettings(const std::string& json)
                                                             : TwitchChatSettings{json} {
        std::string nmcdd {get_object_param("\"non_moderator_chat_delay_duration\"", json, "null")};
        this->m_moderator_id = get_object_param("\"moderator_id\"", json);
        this->m_non_moderator_chat_delay = get_object_param("\"non_moderator_chat_delay\"", json) == "true";
        this->m_non_moderator_chat_delay_duration = nmcdd == "null" ? -1 : std::stoi(nmcdd);
    }

    TwitchModeratorChatSettings::TwitchModeratorChatSettings(const std::string& broadcaster_id,
                                                             const bool& slow_mode,
                                                             const int& slow_mode_wait_time,
                                                             const bool& follower_mode,
                                                             const int& follower_mode_duration,
                                                             const bool& subscriber_mode,
                                                             const bool& emote_mode,
                                                             const bool& unique_chat_mode,
                                                             const std::string& moderator_id,
                                                             const bool& non_moderator_chat_delay,
                                                             const int& non_moderator_chat_delay_duration)
                                                             : TwitchChatSettings{broadcaster_id,
                                                                                  slow_mode,
                                                                                  slow_mode_wait_time,
                                                                                  follower_mode,
                                                                                  follower_mode_duration,
                                                                                  subscriber_mode,
                                                                                  emote_mode,
                                                                                  unique_chat_mode},
                                                               m_moderator_id{moderator_id},
                                                               m_non_moderator_chat_delay{non_moderator_chat_delay},
                                                               m_non_moderator_chat_delay_duration{non_moderator_chat_delay_duration} {
    }

    std::string TwitchModeratorChatSettings::to_json() const {
        std::string json = TwitchChatSettings::to_json();
        return json.substr(0, json.size() - 1)
            + ",\"moderator_id\":\"" + this->m_broadcaster_id
            + "\",\"non_moderator_chat_delay\":" + (this->m_follower_mode ? "true" : "false")
            + ",\"non_moderator_chat_delay_duration\":" + (this->m_follower_mode_duration > -1 ? std::to_string(this->m_follower_mode_duration) : "null")
            + "}";
    }

    std::string TwitchModeratorChatSettings::get_broadcaster_id() const {
        return this->m_broadcaster_id;
    }

    std::string TwitchModeratorChatSettings::to_request() const {
        std::string json = std::string("{\"slow_mode\":") + (this->m_slow_mode ? "true" : "false")
            + ",\"slow_mode_wait_time\":" + (this->m_slow_mode_wait_time > -1 ? std::to_string(this->m_slow_mode_wait_time) : "null")
            + ",\"follower_mode\":" + (this->m_follower_mode ? "true" : "false")
            + ",\"follower_mode_duration\":" + (this->m_follower_mode_duration > -1 ? std::to_string(this->m_follower_mode_duration) : "null")
            + ",\"subscriber_mode\":" + (this->m_subscriber_mode ? "true" : "false")
            + ",\"emote_mode\":" + (this->m_emote_mode ? "true" : "false")
            + ",\"unique_chat_mode\":" + (this->m_unique_chat_mode ? "true" : "false")
            + ",\"non_moderator_chat_delay\":" + (this->m_follower_mode ? "true" : "false")
            + ",\"non_moderator_chat_delay_duration\":" + (this->m_follower_mode_duration > -1 ? std::to_string(this->m_follower_mode_duration) : "null");
        return json + "}";
    }

    TwitchChannel::TwitchChannel(const std::string& json) {
        this->m_id = get_object_param("\"id\"", json);
        this->m_broadcaster_language = get_object_param("\"broadcaster_language\"", json);
        this->m_broadcaster_login = get_object_param("\"broadcaster_login\"", json);
        this->m_display_name = get_object_param("\"display_name\"", json);
        this->m_game_id = get_object_param("\"game_id\"", json);
        this->m_game_name = get_object_param("\"game_name\"", json);
        this->m_is_live = get_object_param("\"is_live\"", json) == "true";
        this->m_tags_ids = json_to_vector(get_object_param("\"tags_ids\"", json));
        this->m_thumbnail_url = get_object_param("\"thumbnail_url\"", json);
        this->m_title = get_object_param("\"title\"", json);
        this->m_started_at = get_object_param("\"started_at\"", json);
    }

    TwitchChannel::TwitchChannel(const std::string& id,
                                 const std::string& broadcaster_language,
                                 const std::string& broadcaster_login,
                                 const std::string& display_name,
                                 const std::string& game_id,
                                 const std::string& game_name,
                                 const bool& is_live,
                                 const std::vector<std::string>& tags_ids,
                                 const std::string& thumbnail_url,
                                 const std::string& title,
                                 const std::string& started_at)
                                 : m_id{id},
                                   m_broadcaster_language{broadcaster_language},
                                   m_broadcaster_login{broadcaster_login},
                                   m_display_name{display_name},
                                   m_game_id{game_id},
                                   m_game_name{game_name},
                                   m_is_live{is_live},
                                   m_tags_ids{tags_ids},
                                   m_thumbnail_url{thumbnail_url},
                                   m_title{title},
                                   m_started_at{started_at} {
    }

    std::string TwitchChannel::to_json() const {
        std::string json = "{\"id\":\"" + this->m_id
            + "\",\"broadcaster_language\":\"" + this->m_broadcaster_language
            + "\",\"broadcaster_login\":\"" + this->m_broadcaster_login
            + "\",\"display_name\":\"" + this->m_display_name
            + "\",\"game_id\":\"" + this->m_game_id
            + "\",\"game_name\":\"" + this->m_game_name
            + "\",\"emote_mode\":" + (this->m_is_live ? "true" : "false")
            + ",\"tags_ids\":" + vector_to_json(this->m_tags_ids)
            + ",\"thumbnail_url\":" + this->m_thumbnail_url
            + "\",\"title\":\"" + this->m_title
            + "\",\"started_at\":\"" + this->m_started_at
            + "\"}";
        return json;
    }

    TwitchChannelTeam::TwitchChannelTeam(const std::string& json) {
        this->m_id = get_object_param("\"id\"", json);
        this->m_broadcaster_id = get_object_param("\"broadcaster_id\"", json);
        this->m_broadcaster_name = get_object_param("\"broadcaster_name\"", json);
        this->m_broadcaster_login = get_object_param("\"broadcaster_login\"", json);
        this->m_background_image_url = get_object_param("\"background_image_url\"", json);
        this->m_banner = get_object_param("\"banner\"", json);
        this->m_created_at = get_object_param("\"created_at\"", json);
        this->m_updated_at = get_object_param("\"updated_at\"", json);
        this->m_info = get_object_param("\"info\"", json);
        this->m_thumbnail_url = get_object_param("\"thumbnail_url\"", json);
        this->m_team_name = get_object_param("\"team_name\"", json);
        this->m_team_display_name = get_object_param("\"team_display_name\"", json);
    }

    TwitchChannelTeam::TwitchChannelTeam(const std::string& id,
                                         const std::string& broadcaster_id,
                                         const std::string& broadcaster_name,
                                         const std::string& broadcaster_login,
                                         const std::string& background_image_url,
                                         const std::string& banner,
                                         const std::string& created_at,
                                         const std::string& updated_at,
                                         const std::string& info,
                                         const std::string& thumbnail_url,
                                         const std::string& team_name,
                                         const std::string& team_display_name)
                                         : m_id{id},
                                           m_broadcaster_id{broadcaster_id},
                                           m_broadcaster_name{broadcaster_name},
                                           m_broadcaster_login{broadcaster_login},
                                           m_background_image_url{background_image_url},
                                           m_banner{banner},
                                           m_created_at{created_at},
                                           m_updated_at{updated_at},
                                           m_info{info},
                                           m_thumbnail_url{thumbnail_url},
                                           m_team_name{team_name},
                                           m_team_display_name{team_display_name} {
    }

    std::string TwitchChannelTeam::to_json() const {
        std::string json = "{\"id\":\"" + this->m_id
            + "\",\"broadcaster_id\":\"" + this->m_broadcaster_id
            + "\",\"broadcaster_name\":\"" + this->m_broadcaster_name
            + "\",\"broadcaster_login\":\"" + this->m_broadcaster_login
            + "\",\"background_image_url\":" + (this->m_background_image_url == "null" ? this->m_background_image_url : std::string("\"" + this->m_background_image_url + "\""))
            + ",\"banner\":\"" + (this->m_banner == "null" ? this->m_banner : std::string("\"" + this->m_banner + "\""))
            + ",\"created_at\":\"" + this->m_created_at
            + "\",\"updated_at\":\"" + this->m_updated_at
            + "\",\"info\":\"" + this->m_info
            + "\",\"thumbnail_url\":\"" + this->m_thumbnail_url
            + "\",\"team_name\":\"" + this->m_team_name
            + "\",\"team_display_name\":\"" + this->m_team_display_name
            + "\"}";
        return json;
    }

    TwitchStream::TwitchStream(const std::string& json) {
        this->m_id = get_object_param("\"id\"", json);
        this->m_user_id = get_object_param("\"user_id\"", json);
        this->m_user_name = get_object_param("\"user_name\"", json);
        this->m_user_login = get_object_param("\"user_login\"", json);
        this->m_game_id = get_object_param("\"game_id\"", json);
        this->m_game_name = get_object_param("\"game_name\"", json);
        this->m_type = get_object_param("\"type\"", json);
        this->m_title = get_object_param("\"title\"", json);
        this->m_started_at = get_object_param("\"started_at\"", json);
        this->m_language = get_object_param("\"language\"", json);
        this->m_thumbnail_url = get_object_param("\"thumbnail_url\"", json);
        this->m_tag_ids = json_to_vector(get_object_param("\"tag_ids\"", json));
        this->m_viewer_count = std::stoul(get_object_param("\"viewer_count\"", json, "0"));
        this->m_is_mature = get_object_param("\"is_mature\"", json) == "true";
    }

    TwitchStream::TwitchStream(const std::string& id,
                               const std::string& user_id,
                               const std::string& user_name,
                               const std::string& user_login,
                               const std::string& game_id,
                               const std::string& game_name,
                               const std::string& type,
                               const std::string& title,
                               const std::string& started_at,
                               const std::string& language,
                               const std::string& thumbnail_url,
                               const std::vector<std::string>& tag_ids,
                               const size_t& viewer_count,
                               const bool& is_mature)
                               : m_id{id},
                                 m_user_id{user_id},
                                 m_user_name{user_name},
                                 m_user_login{user_login},
                                 m_game_id{game_id},
                                 m_game_name{game_name},
                                 m_type{type},
                                 m_title{title},
                                 m_started_at{started_at},
                                 m_language{language},
                                 m_thumbnail_url{thumbnail_url},
                                 m_tag_ids{tag_ids},
                                 m_viewer_count{viewer_count},
                                 m_is_mature{is_mature} {
    }

    std::string TwitchStream::to_json() const {
        std::string json = "{\"id\":\"" + this->m_id
            + "\",\"user_id\":\"" + this->m_user_id
            + "\",\"user_name\":\"" + this->m_user_name
            + "\",\"user_login\":\"" + this->m_user_login
            + "\",\"game_id\":\"" + this->m_game_id
            + "\",\"game_name\":\"" + this->m_game_name
            + "\",\"type\":\"" + this->m_type
            + "\",\"title\":\"" + this->m_title
            + "\",\"started_at\":\"" + this->m_started_at
            + "\",\"language\":\"" + this->m_language
            + "\",\"thumbnail_url\":\"" + this->m_thumbnail_url
            + "\",\"tags_ids\":" + vector_to_json(this->m_tag_ids)
            + ",\"viewer_count\":" + std::to_string(this->m_viewer_count)
            + ",\"is_mature\":" + (this->m_is_mature ? "true" : "false")
            + "}";
        return json;
    }

    TwitchUsersFollows::TwitchUsersFollows(const std::string& json) {
        this->m_from_id = get_object_param("\"from_id\"", json);
        this->m_from_login = get_object_param("\"from_login\"", json);
        this->m_from_name = get_object_param("\"from_name\"", json);
        this->m_to_id = get_object_param("\"to_id\"", json);
        this->m_to_name = get_object_param("\"to_name\"", json);
        this->m_followed_at = get_object_param("\"followed_at\"", json);
    }

    TwitchUsersFollows::TwitchUsersFollows(const std::string& from_id,
                                           const std::string& from_login,
                                           const std::string& from_name,
                                           const std::string& to_id,
                                           const std::string& to_name,
                                           const std::string& followed_at)
                                           : m_from_id{from_id},
                                             m_from_login{from_login},
                                             m_from_name{from_name},
                                             m_to_id{to_id},
                                             m_to_name{to_name},
                                             m_followed_at{followed_at} {
    }

    std::string TwitchUsersFollows::to_json() const {
        std::string json = "{\"from_id\":\"" + this->m_from_id
            + "\",\"from_login\":\"" + this->m_from_login
            + "\",\"from_name\":\"" + this->m_from_name
            + "\",\"to_id\":\"" + this->m_to_id
            + "\",\"to_name\":\"" + this->m_to_name
            + "\",\"followed_at\":\"" + this->m_followed_at
            + "\"}";
        return json;
    }

    TwitchClips::TwitchClips(const std::string& json) {
        this->m_id = get_object_param("\"id\"", json);
        this->m_url = get_object_param("\"url\"", json);
        this->m_embed_url = get_object_param("\"embed_url\"", json);
        this->m_broadcaster_id = get_object_param("\"broadcaster_id\"", json);
        this->m_broadcaster_name = get_object_param("\"broadcaster_name\"", json);
        this->m_creator_id = get_object_param("\"creator_id\"", json);
        this->m_creator_name = get_object_param("\"creator_name\"", json);
        this->m_video_id = get_object_param("\"video_id\"", json);
        this->m_game_id = get_object_param("\"game_id\"", json);
        this->m_language = get_object_param("\"language\"", json);
        this->m_title = get_object_param("\"title\"", json);
        this->m_view_count = std::stoul(get_object_param("\"view_count\"", json, "0"));
        this->m_created_at = get_object_param("\"created_at\"", json);
        this->m_thumbnail_url = get_object_param("\"thumbnail_url\"", json);
        this->m_duration = std::stoul(get_object_param("\"duration\"", json, "0"));
    }

    TwitchClips::TwitchClips(const std::string& id,
                             const std::string& url,
                             const std::string& embed_url,
                             const std::string& broadcaster_id,
                             const std::string& broadcaster_name,
                             const std::string& creator_id,
                             const std::string& creator_name,
                             const std::string& video_id,
                             const std::string& game_id,
                             const std::string& language,
                             const std::string& title,
                             const size_t& view_count,
                             const std::string& created_at,
                             const std::string& thumbnail_url,
                             const size_t& duration)
                             : m_id{id},
                               m_url{url},
                               m_embed_url{embed_url},
                               m_broadcaster_id{broadcaster_id},
                               m_broadcaster_name{broadcaster_name},
                               m_creator_id{creator_id},
                               m_creator_name{creator_name},
                               m_video_id{video_id},
                               m_game_id{game_id},
                               m_language{language},
                               m_title{title},
                               m_view_count{view_count},
                               m_created_at{created_at},
                               m_thumbnail_url{thumbnail_url},
                               m_duration{duration} {
    }

    std::string TwitchClips::to_json() const {
        std::string json = "{\"id\":\"" + this->m_id
            + "\",\"url\":\"" + this->m_url
            + "\",\"embed_url\":\"" + this->m_embed_url
            + "\",\"broadcaster_id\":\"" + this->m_broadcaster_id
            + "\",\"broadcaster_name\":\"" + this->m_broadcaster_name
            + "\",\"creator_id\":\"" + this->m_creator_id
            + "\",\"creator_name\":\"" + this->m_creator_name
            + "\",\"video_id\":\"" + this->m_video_id
            + "\",\"game_id\":\"" + this->m_game_id
            + "\",\"language\":\"" + this->m_language
            + "\",\"title\":\"" + this->m_title
            + "\",\"view_count\":" + std::to_string(this->m_view_count)
            + ",\"created_at\":\"" + this->m_created_at
            + "\",\"thumbnail_url\":\"" + this->m_thumbnail_url
            + "\",\"duration\":" + std::to_string(this->m_duration)
            + "}";
        return json;
    }

    TwitchVideos::TwitchVideos(const std::string& json) {
        this->m_id = get_object_param("\"id\"", json);
        this->m_stream_id = get_object_param("\"stream_id\"", json);
        this->m_user_id = get_object_param("\"user_id\"", json);
        this->m_user_login = get_object_param("\"user_login\"", json);
        this->m_user_name = get_object_param("\"user_name\"", json);
        this->m_title = get_object_param("\"title\"", json);
        this->m_description = get_object_param("\"description\"", json);
        this->m_created_at = get_object_param("\"created_at\"", json);
        this->m_published_at = get_object_param("\"published_at\"", json);
        this->m_url = get_object_param("\"url\"", json);
        this->m_thumbnail_url = get_object_param("\"thumbnail_url\"", json);
        this->m_viewable = get_object_param("\"viewable\"", json);
        this->m_view_count = std::stoul(get_object_param("\"view_count\"", json, "0"));
        this->m_language = get_object_param("\"language\"", json);
        this->m_type = get_object_param("\"type\"", json);
        this->m_duration = get_object_param("\"duration\"", json);
        this->m_muted_segments = json_to_segment_vector(get_object_param("\"muted_segments\"", json));
    }

    TwitchVideos::TwitchVideos(const std::string& id,
                               const std::string& stream_id,
                               const std::string& user_id,
                               const std::string& user_login,
                               const std::string& user_name,
                               const std::string& title,
                               const std::string& description,
                               const std::string& created_at,
                               const std::string& published_at,
                               const std::string& url,
                               const std::string& thumbnail_url,
                               const std::string& viewable,
                               const size_t& view_count,
                               const std::string& language,
                               const std::string& type,
                               const std::string& duration,
                               std::vector<VideoSegment> muted_segments)
                               : m_id{id},
                                 m_stream_id{stream_id},
                                 m_user_id{user_id},
                                 m_user_login{user_login},
                                 m_user_name{user_name},
                                 m_title{title},
                                 m_description{description},
                                 m_created_at{created_at},
                                 m_published_at{published_at},
                                 m_url{url},
                                 m_thumbnail_url{thumbnail_url},
                                 m_viewable{viewable},
                                 m_view_count{view_count},
                                 m_language{language},
                                 m_type{type},
                                 m_duration{duration},
                                 m_muted_segments{muted_segments} {
    }

    std::string TwitchVideos::to_json() const {
        std::string json = "{\"id\":\"" + this->m_id
            + "\",\"stream_id\":\"" + this->m_stream_id
            + "\",\"user_id\":\"" + this->m_user_id
            + "\",\"user_login\":\"" + this->m_user_login
            + "\",\"user_name\":\"" + this->m_user_name
            + "\",\"title\":\"" + this->m_title
            + "\",\"description\":\"" + this->m_description
            + "\",\"created_at\":\"" + this->m_created_at
            + "\",\"published_at\":\"" + this->m_published_at
            + "\",\"url\":\"" + this->m_url
            + "\",\"thumbnail_url\":\"" + this->m_thumbnail_url
            + "\",\"viewable\":\"" + this->m_viewable
            + "\",\"view_count\":" + std::to_string(this->m_view_count)
            + ",\"language\":\"" + this->m_language
            + "\",\"type\":\"" + this->m_type
            + "\",\"duration\":\"" + this->m_duration
            + "\",\"muted_segments\":[";
        for (size_t i {0}; i < this->m_muted_segments.size(); ++i) {
            json += "{\"duration\":" + std::to_string(this->m_muted_segments.at(i).duration)
                + ",\"offset\":" + std::to_string(this->m_muted_segments.at(i).offset) + "}";
            if (i + 1 < this->m_muted_segments.size()) {
                json += ',';
            }
        }
        return json + "]}";
    }

    TwitchScheduleSegment::TwitchScheduleSegment(const std::string& json) {
        this->m_id = get_object_param("\"id\"", json);
        this->m_start_time = get_object_param("\"start_time\"", json);
        this->m_end_time = get_object_param("\"end_time\"", json);
        this->m_title = get_object_param("\"title\"", json);
        this->m_canceled_until = get_object_param("\"canceled_until\"", json);
        std::string category_str = get_object_param("\"category\"", json, "null");
        if (category_str == "null") {
            this->m_category = nullptr;
        } else {
            this->m_category = std::make_shared<TwitchCategory>(category_str);
        }
        this->m_is_recurring = get_object_param("\"is_recurring\"", json) == "true";
    }

    TwitchScheduleSegment::TwitchScheduleSegment(const std::string& id,
                                                 const std::string& start_time,
                                                 const std::string& end_time,
                                                 const std::string& title,
                                                 const std::string& canceled_until,
                                                 TwitchCategory& category,
                                                 const bool& is_recurring)
                                                 : m_id{id},
                                                   m_start_time{start_time},
                                                   m_end_time{end_time},
                                                   m_title{title},
                                                   m_canceled_until{canceled_until},
                                                   m_category{&category},
                                                   m_is_recurring{is_recurring} {
    }

    std::string TwitchScheduleSegment::to_json() const {
        std::string json = "{\"id\":\"" + this->m_id
            + "\",\"start_time\":\"" + this->m_start_time
            + "\",\"end_time\":\"" + this->m_end_time
            + "\",\"title\":\"" + this->m_title
            + "\",\"canceled_until\":" + (this->m_canceled_until == "null" ? this->m_canceled_until : std::string("\"" + this->m_canceled_until + "\""))
            + ",\"category\":" + (this->m_category == nullptr ? "null" : this->m_category->to_json())
            + ",\"is_recurring\":" + (this->m_is_recurring ? "true" : "false")
            + "}";
        return json;
    }

    TwitchChannelStreamSchedule::TwitchChannelStreamSchedule(const std::string& json) {
        std::string string_with_segments = get_object_param("\"segments\"", json);
        std::vector<std::string> str_segments = json_to_vector(string_with_segments);
        for (std::string segment : str_segments) {
            this->m_segments.push_back(TwitchScheduleSegment(segment));
        }
        this->m_broadcaster_id = get_object_param("\"broadcaster_id\"", json);
        this->m_broadcaster_name = get_object_param("\"broadcaster_name\"", json);
        this->m_broadcaster_login = get_object_param("\"broadcaster_login\"", json);
        std::string vacation = get_object_param("\"vacation\"", json);
        if (vacation != "null") {
            this->m_vacation = std::make_shared<DatetimePeriod>(vacation);
        }
    }

    TwitchChannelStreamSchedule::TwitchChannelStreamSchedule(const std::vector<TwitchScheduleSegment>& segments,
                                                             const std::string& broadcaster_id,
                                                             const std::string& broadcaster_name,
                                                             const std::string& broadcaster_login,
                                                             DatetimePeriod* vacation)
                                                             : m_segments{segments},
                                                               m_broadcaster_id{broadcaster_id},
                                                               m_broadcaster_name{broadcaster_name},
                                                               m_broadcaster_login{broadcaster_login},
                                                               m_vacation{vacation} {}

    std::string TwitchChannelStreamSchedule::to_json() const {
        std::string json {"{\"segments\":["};
        for (size_t i {0}; i < this->m_segments.size(); ++i) {
            json += this->m_segments.at(i).to_json();
            if (i + 1 < this->m_segments.size()) {
                json += ',';
            }
        }
        json += "],\"broadcaster_id\":\"" + this->m_broadcaster_id
            + "\",\"broadcaster_name\":\"" + this->m_broadcaster_name
            + "\",\"broadcaster_login\":\"" + this->m_broadcaster_login
            + "\",\"vacation\":" + (this->m_vacation == nullptr ? "null" : this->m_vacation->to_json())
            + "}";
        return json;
    }

    TwitchTeam::TwitchTeam(const std::string& json) {
        this->m_id = get_object_param("\"id\"", json);
        this->m_team_name = get_object_param("\"team_name\"", json);
        this->m_team_display_name = get_object_param("\"team_display_name\"", json);
        this->m_info = get_object_param("\"info\"", json);
        this->m_thumbnail_url = get_object_param("\"thumbnail_url\"", json);
        this->m_banner = get_object_param("\"banner\"", json);
        this->m_background_image_url = get_object_param("\"background_image_url\"", json);
        this->m_created_at = get_object_param("\"created_at\"", json);
        this->m_updated_at = get_object_param("\"updated_at\"", json);
        std::string string_with_users = get_object_param("\"users\"", json);
        std::vector<std::string> str_users = json_to_vector(string_with_users);
        for (std::string user : str_users) {
            this->m_users.push_back(TwitchBasicUser(user));
        }
    }

    TwitchTeam::TwitchTeam(const std::string& id,
                           const std::string& team_name,
                           const std::string& team_display_name,
                           const std::string& info,
                           const std::string& thumbnail_url,
                           const std::string& banner,
                           const std::string& background_image_url,
                           const std::string& created_at,
                           const std::string& updated_at,
                           const std::vector<TwitchBasicUser>& users)
                           : m_id{id},
                             m_team_name{team_name},
                             m_team_display_name{team_display_name},
                             m_info{info},
                             m_thumbnail_url{thumbnail_url},
                             m_banner{banner},
                             m_background_image_url{background_image_url},
                             m_created_at{created_at},
                             m_updated_at{updated_at},
                             m_users{users} {
    }

    std::string TwitchTeam::to_json() const {
        std::string json = "{\"id\":\"" + this->m_id
            + "\",\"team_name\":\"" + this->m_team_name
            + "\",\"team_display_name\":\"" + this->m_team_display_name
            + "\",\"info\":\"" + this->m_info
            + "\",\"thumbnail_url\":\"" + this->m_thumbnail_url
            + "\",\"banner\":\"" + this->m_banner
            + "\",\"background_image_url\":\"" + this->m_background_image_url
            + "\",\"created_at\":\"" + this->m_created_at
            + "\",\"updated_at\":\"" + this->m_updated_at
            + "\",\"users\":[";
        for (size_t i {0}; i < this->m_users.size(); ++i) {
            json += this->m_users.at(i).to_json();
            if (i + 1 < this->m_users.size()) {
                json += ',';
            }
        }
        return json + "]}";
    }

    TwitchBadgeVersion::TwitchBadgeVersion(const std::string& json) {
        this->m_id = get_object_param("\"id\"", json);
        this->m_image_url_1x = get_object_param("\"image_url_1x\"", json);
        this->m_image_url_2x = get_object_param("\"image_url_2x\"", json);
        this->m_image_url_4x = get_object_param("\"image_url_4x\"", json);
    }

    TwitchBadgeVersion::TwitchBadgeVersion(const std::string& id,
                                           const std::string& image_url_1x,
                                           const std::string& image_url_2x,
                                           const std::string& image_url_4x)
                                           : m_id{id},
                                             m_image_url_1x{image_url_1x},
                                             m_image_url_2x{image_url_2x},
                                             m_image_url_4x{image_url_4x} {
    }

    std::string TwitchBadgeVersion::to_json() const {
        std::string json = "{\"id\":\"" + this->m_id
            + "\",\"image_url_1x\":\"" + this->m_image_url_1x
            + "\",\"image_url_2x\":\"" + this->m_image_url_2x
            + "\",\"image_url_4x\":\"" + this->m_image_url_4x
            + "\"}";
        return json;
    }

    TwitchBadgeSet::TwitchBadgeSet(const std::string& json) {
        this->m_set_id = get_object_param("\"set_id\"", json);
        std::string string_with_versions = get_object_param("\"versions\"", json);
        std::vector<std::string> str_versions = json_to_vector(string_with_versions);
        for (std::string version : str_versions) {
            this->m_versions.push_back(TwitchBadgeVersion(version));
        }
    }

    TwitchBadgeSet::TwitchBadgeSet(const std::string& set_id,
                                   const std::vector<TwitchBadgeVersion>& versions)
                                   : m_set_id{set_id},
                                     m_versions{versions} {
    }

    std::string TwitchBadgeSet::to_json() const {
        std::string json = "{\"set_id\":\"" + this->m_set_id
            + "\",\"versions\":[";
        for (size_t i {0}; i < this->m_versions.size(); ++i) {
            json += this->m_versions.at(i).to_json();
            if (i + 1 < this->m_versions.size()) {
                json += ',';
            }
        }
        json += "]}";
        return json;
    }

    TwitchImageUrls::TwitchImageUrls(const std::string& json) {
        this->m_1 = get_object_param("\"1\"", json);
        this->m_15 = get_object_param("\"1.5\"", json);
        this->m_2 = get_object_param("\"2\"", json);
        this->m_3 = get_object_param("\"3\"", json);
        this->m_4 = get_object_param("\"4\"", json);
    }
    // TODO: check if can be changed to StringMap
    TwitchImageUrls::TwitchImageUrls(const std::string& url_1,
                                     const std::string& url_15,
                                     const std::string& url_2,
                                     const std::string& url_3,
                                     const std::string& url_4)
                                     : m_1{url_1},
                                       m_15{url_15},
                                       m_2{url_2},
                                       m_3{url_3},
                                       m_4{url_4} {
    }

    std::string TwitchImageUrls::to_json() const {
        std::string json = (this->m_1 != "" ? "{\"1\":\"" + this->m_1
            + "\",\"1.5\":\"" + this->m_15
            + "\",\"2\":\"" + this->m_2
            + "\",\"3\":\"" + this->m_3
            + "\",\"4\":\"" + this->m_4
            + "\"}" : "");
        return json;
    }

    TwitchImageTypes::TwitchImageTypes(const std::string& json) {
        std::string animated_str = get_object_param("\"animated\"", json);
        std::string static_str = get_object_param("\"static\"", json);
        this->m_animated = (animated_str.size() ? std::make_shared<TwitchImageUrls>(animated_str) : nullptr);
        this->m_static = (static_str.size() ? std::make_shared<TwitchImageUrls>(static_str) : nullptr);
    }

    TwitchImageTypes::TwitchImageTypes(TwitchImageUrls& animated,
                                       TwitchImageUrls& img_static)
                                       : m_animated{&animated},
                                         m_static{&img_static} {
    }

    std::string TwitchImageTypes::to_json() const {
        std::string json {"{"};
        if (this->m_animated) {
            json += "\"animated\":" + this->m_animated->to_json();
        }
        if (this->m_static) {
            if (this->m_animated) {
                json += ",";
            }
            json += "\"static\":" + this->m_static->to_json();
        }
        return json + "}";
    }

    TwitchImageThemes::TwitchImageThemes(const std::string& json) {
        std::string dark_str = get_object_param("\"dark\"", json);
        std::string light_str = get_object_param("\"light\"", json);
        this->m_dark = (dark_str.size() ? std::make_shared<TwitchImageTypes>(dark_str) : nullptr);
        this->m_light = (light_str.size() ? std::make_shared<TwitchImageTypes>(light_str) : nullptr);
    }

    TwitchImageThemes::TwitchImageThemes(TwitchImageTypes& dark,
                                         TwitchImageTypes& light)
                                         : m_dark{&dark},
                                           m_light{&light} {
    }

    std::string TwitchImageThemes::to_json() const {
        std::string json {"{"};
        if (this->m_dark) {
            json += "\"dark\":" + this->m_dark->to_json();
        }
        if (this->m_light) {
            if (this->m_dark) {
                json += ",";
            }
            json += "\"light\":" + this->m_light->to_json();
        }
        return json + "}";
    }

    TwitchCheermoteTier::TwitchCheermoteTier(const std::string& json) {
        this->m_id = get_object_param("\"id\"", json);
        this->m_min_bits = std::stoul(get_object_param("\"min_bits\"", json, "0"));
        this->m_color = get_object_param("\"color\"", json);
        this->m_can_cheer = get_object_param("\"can_cheer\"", json) == "true";
        this->m_show_in_bits_card = get_object_param("\"show_in_bits_card\"", json) == "true";
        this->m_images = TwitchImageThemes(get_object_param("\"images\"", json));
    }

    TwitchCheermoteTier::TwitchCheermoteTier(const std::string& id,
                                             const size_t& min_bits,
                                             const std::string& color,
                                             const bool& can_cheer,
                                             const bool& show_in_bits_card,
                                             TwitchImageThemes& images)
                                             : m_id{id},
                                               m_min_bits{min_bits},
                                               m_color{color},
                                               m_can_cheer{can_cheer},
                                               m_show_in_bits_card{show_in_bits_card},
                                               m_images{images} {
    }

    std::string TwitchCheermoteTier::to_json() const {
        std::string json = "{\"id\":\"" + this->m_id
            + "\",\"min_bits\":" + std::to_string(this->m_min_bits)
            + ",\"color\":\"" + this->m_color
            + "\",\"can_cheer\":" + (this->m_can_cheer ? "true" : "false")
            + ",\"show_in_bits_card\":" + (this->m_show_in_bits_card ? "true" : "false")
            + ",\"images\":" + this->m_images.to_json()
            + "}";
        return json;
    }

    TwitchCheermote::TwitchCheermote(const std::string& json) {
        this->m_prefix = get_object_param("\"prefix\"", json);
        this->m_type = get_object_param("\"type\"", json);
        this->m_order = std::stoul(get_object_param("\"order\"", json, "0"));
        this->m_last_updated = get_object_param("\"last_updated\"", json);
        this->m_is_charitable = get_object_param("\"is_charitable\"", json) == "true";
        std::string string_with_tiers = get_object_param("\"tiers\"", json);
        std::vector<std::string> str_tiers = json_to_vector(string_with_tiers);
        for (std::string tier : str_tiers) {
            this->m_tiers.push_back(TwitchCheermoteTier(tier));
        }
    }

    TwitchCheermote::TwitchCheermote(const std::string& prefix,
                                     const std::string& type,
                                     const size_t& order,
                                     const std::string& last_updated,
                                     const bool& is_charitable,
                                     const std::vector<TwitchCheermoteTier>& tiers)
                                     : m_prefix{prefix},
                                       m_type{type},
                                       m_order{order},
                                       m_last_updated{last_updated},
                                       m_is_charitable{is_charitable},
                                       m_tiers{tiers} {
    }

    std::string TwitchCheermote::to_json() const {
        std::string json = "{\"prefix\":\"" + this->m_prefix
            + "\",\"type\":\"" + this->m_type
            + "\",\"order\":" + std::to_string(this->m_order)
            + ",\"last_updated\":\"" + this->m_last_updated
            + "\",\"is_charitable\":" + (this->m_is_charitable ? "true" : "false")
            + ",\"tiers\":[";
        for (size_t i {0}; i < this->m_tiers.size(); ++i) {
            json += this->m_tiers.at(i).to_json();
            if (i + 1 < this->m_tiers.size()) {
                json += ',';
            }
        }
        return json + "]}";
    }

    TwitchBlockedTerm::TwitchBlockedTerm(const std::string& json) {
        this->m_broadcaster_id = get_object_param("\"broadcaster_id\"", json);
        this->m_moderator_id = get_object_param("\"moderator_id\"", json);
        this->m_id = get_object_param("\"id\"", json);
        this->m_text = get_object_param("\"text\"", json);
        this->m_created_at = get_object_param("\"created_at\"", json);
        this->m_updated_at = get_object_param("\"updated_at\"", json);
        this->m_expires_at = get_object_param("\"expires_at\"", json);
    }

    TwitchBlockedTerm::TwitchBlockedTerm(const std::string& broadcaster_id,
                                         const std::string& moderator_id,
                                         const std::string& id,
                                         const std::string& text,
                                         const std::string& created_at,
                                         const std::string& updated_at,
                                         const std::string& expires_at)
                                         : m_broadcaster_id{broadcaster_id},
                                           m_moderator_id{moderator_id},
                                           m_id{id},
                                           m_text{text},
                                           m_created_at{created_at},
                                           m_updated_at{updated_at},
                                           m_expires_at{expires_at} {
    }

    std::string TwitchBlockedTerm::to_json() const {
        std::string json = "{\"broadcaster_id\":\"" + this->m_broadcaster_id
            + "\",\"moderator_id\":\"" + this->m_moderator_id
            + "\",\"id\":\"" + this->m_id
            + "\",\"text\":\"" + this->m_text
            + "\",\"created_at\":\"" + this->m_created_at
            + "\",\"updated_at\":\"" + this->m_updated_at
            + "\",\"expires_at\":" + (this->m_expires_at == "null" ? this->m_expires_at : std::string("\"" + this->m_expires_at + "\""))
            + "}";
        return json;
    }

    TwitchAutoModSettings::TwitchAutoModSettings(const std::string& json) {
        this->m_broadcaster_id = get_object_param("\"broadcaster_id\"", json);
        this->m_moderator_id = get_object_param("\"moderator_id\"", json);
        std::string overall {get_object_param("\"overall_level\"", json, "null")};
        this->m_overall_level = overall == "null" ? -1 : std::stoi(overall);
        this->m_disability = std::stoi(get_object_param("\"disability\"", json, "0"));
        this->m_aggression = std::stoi(get_object_param("\"aggression\"", json, "0"));
        this->m_sexuality_sex_or_gender = std::stoi(get_object_param("\"sexuality_sex_or_gender\"", json, "0"));
        this->m_misogyny = std::stoi(get_object_param("\"misogyny\"", json, "0"));
        this->m_bullying = std::stoi(get_object_param("\"bullying\"", json, "0"));
        this->m_swearing = std::stoi(get_object_param("\"swearing\"", json, "0"));
        this->m_race_ethnicity_or_religion = std::stoi(get_object_param("\"race_ethnicity_or_religion\"", json, "0"));
        this->m_sex_based_terms = std::stoi(get_object_param("\"sex_based_terms\"", json, "0"));
    }

    TwitchAutoModSettings::TwitchAutoModSettings(const std::string& broadcaster_id,
                                                 const std::string& moderator_id,
                                                 const int& overall_level,
                                                 const int& disability,
                                                 const int& aggression,
                                                 const int& sexuality_sex_or_gender,
                                                 const int& misogyny,
                                                 const int& bullying,
                                                 const int& swearing,
                                                 const int& race_ethnicity_or_religion,
                                                 const int& sex_based_terms)
                                                 : m_broadcaster_id{broadcaster_id},
                                                   m_moderator_id{moderator_id},
                                                   m_overall_level{overall_level},
                                                   m_disability{disability},
                                                   m_aggression{aggression},
                                                   m_sexuality_sex_or_gender{sexuality_sex_or_gender},
                                                   m_misogyny{misogyny},
                                                   m_bullying{bullying},
                                                   m_swearing{swearing},
                                                   m_race_ethnicity_or_religion{race_ethnicity_or_religion},
                                                   m_sex_based_terms{sex_based_terms} {
    }

    std::string TwitchAutoModSettings::to_json() const {
        std::string json = "{\"broadcaster_id\":\"" + this->m_broadcaster_id
            + "\",\"moderator_id\":\"" + this->m_moderator_id
            + "\",\"overall_level\":" + (this->m_overall_level > -1 ? std::to_string(this->m_overall_level) : "null")
            + ",\"disability\":" + std::to_string(this->m_disability)
            + ",\"aggression\":" + std::to_string(this->m_aggression)
            + ",\"sexuality_sex_or_gender\":" + std::to_string(this->m_sexuality_sex_or_gender)
            + ",\"misogyny\":" + std::to_string(this->m_misogyny)
            + ",\"bullying\":" + std::to_string(this->m_bullying)
            + ",\"swearing\":" + std::to_string(this->m_swearing)
            + ",\"race_ethnicity_or_religion\":" + std::to_string(this->m_race_ethnicity_or_religion)
            + ",\"sex_based_terms\":" + std::to_string(this->m_sex_based_terms)
            + "}";
        return json;
    }

    std::string TwitchAutoModSettings::to_request(const bool& is_overall) const {
        std::string json = (is_overall ? ("{\"overall_level\":" + (this->m_overall_level > -1 ? std::to_string(this->m_overall_level) : "null") + "}")
            : ("{\"disability\":" + std::to_string(this->m_disability)
            + ",\"aggression\":" + std::to_string(this->m_aggression)
            + ",\"sexuality_sex_or_gender\":" + std::to_string(this->m_sexuality_sex_or_gender)
            + ",\"misogyny\":" + std::to_string(this->m_misogyny)
            + ",\"bullying\":" + std::to_string(this->m_bullying)
            + ",\"swearing\":" + std::to_string(this->m_swearing)
            + ",\"race_ethnicity_or_religion\":" + std::to_string(this->m_race_ethnicity_or_religion)
            + ",\"sex_based_terms\":" + std::to_string(this->m_sex_based_terms)
            + "}"));
        return json;
    }

    TwitchPollChoice::TwitchPollChoice(const std::string& json) {
        this->m_id = get_object_param("\"id\"", json);
        this->m_title = get_object_param("\"title\"", json);
        this->m_votes = std::stoul(get_object_param("\"votes\"", json, "0"));
        this->m_channel_points_votes = std::stoul(get_object_param("\"channel_points_votes\"", json, "0"));
        this->m_bits_votes = std::stoul(get_object_param("\"bits_votes\"", json, "0"));
    }

    TwitchPollChoice::TwitchPollChoice(const std::string& id,
                                       const std::string& title,
                                       const size_t& votes,
                                       const size_t& channel_points_votes,
                                       const size_t& bits_votes)
                                       : m_id{id},
                                         m_title{title},
                                         m_votes{votes},
                                         m_channel_points_votes{channel_points_votes},
                                         m_bits_votes{bits_votes} {
    }

    std::string TwitchPollChoice::to_json() const {
        std::string json = "{\"id\":\"" + this->m_id
            + "\",\"title\":\"" + this->m_title
            + "\",\"votes\":" + std::to_string(this->m_votes)
            + ",\"channel_points_votes\":" + std::to_string(this->m_channel_points_votes)
            + ",\"bits_votes\":" + std::to_string(this->m_bits_votes)
            + "}";
        return json;
    }

    TwitchPoll::TwitchPoll(const std::string& json) {
        this->m_id = get_object_param("\"id\"", json);
        this->m_broadcaster_id = get_object_param("\"broadcaster_id\"", json);
        this->m_broadcaster_name = get_object_param("\"broadcaster_name\"", json);
        this->m_broadcaster_login = get_object_param("\"broadcaster_login\"", json);
        this->m_title = get_object_param("\"title\"", json);
        this->m_status = get_object_param("\"status\"", json);
        this->m_started_at = get_object_param("\"started_at\"", json);
        this->m_bits_voting_enabled = get_object_param("\"bits_voting_enabled\"", json) == "true";
        this->m_channel_points_voting_enabled = get_object_param("\"channel_points_voting_enabled\"", json) == "true";
        this->m_bits_per_vote = std::stoul(get_object_param("\"bits_per_vote\"", json, "0"));
        this->m_channel_points_per_vote = std::stoul(get_object_param("\"channel_points_per_vote\"", json, "0"));
        this->m_duration = std::stoul(get_object_param("\"duration\"", json, "0"));
        std::string string_with_choices = get_object_param("\"choices\"", json);
        std::vector<std::string> str_choices = json_to_vector(string_with_choices);
        for (std::string choice : str_choices) {
            this->m_choices.push_back(TwitchPollChoice(choice));
        }
    }

    TwitchPoll::TwitchPoll(const std::string& id,
                           const std::string& broadcaster_id,
                           const std::string& broadcaster_name,
                           const std::string& broadcaster_login,
                           const std::string& title,
                           const std::string& status,
                           const std::string& started_at,
                           const bool& bits_voting_enabled,
                           const bool& channel_points_voting_enabled,
                           const size_t& bits_per_vote,
                           const size_t& channel_points_per_vote,
                           const size_t& duration,
                           const std::vector<TwitchPollChoice>& choices)
                           : m_id{id},
                             m_broadcaster_id{broadcaster_id},
                             m_broadcaster_name{broadcaster_name},
                             m_broadcaster_login{broadcaster_login},
                             m_title{title},
                             m_status{status},
                             m_started_at{started_at},
                             m_bits_voting_enabled{bits_voting_enabled},
                             m_channel_points_voting_enabled{channel_points_voting_enabled},
                             m_bits_per_vote{bits_per_vote},
                             m_channel_points_per_vote{channel_points_per_vote},
                             m_duration{duration},
                             m_choices{choices} {
    }

    std::string TwitchPoll::get_id() const {
        return this->m_id;
    }

    std::string TwitchPoll::to_json() const {
        std::string json = "{\"id\":\"" + this->m_id
            + "\",\"broadcaster_id\":\"" + this->m_broadcaster_id
            + "\",\"broadcaster_name\":\"" + this->m_broadcaster_name
            + "\",\"broadcaster_login\":\"" + this->m_broadcaster_login
            + "\",\"title\":\"" + this->m_title
            + "\",\"status\":\"" + this->m_status
            + "\",\"started_at\":\"" + this->m_started_at
            + "\",\"bits_voting_enabled\":" + (this->m_bits_voting_enabled ? "true" : "false")
            + ",\"channel_points_voting_enabled\":" + (this->m_channel_points_voting_enabled ? "true" : "false")
            + ",\"bits_per_vote\":" + std::to_string(this->m_bits_per_vote)
            + ",\"channel_points_per_vote\":" + std::to_string(this->m_channel_points_per_vote)
            + ",\"duration\":" + std::to_string(this->m_duration)
            + ",\"choices\":[";
        for (size_t i {0}; i < this->m_choices.size(); ++i) {
            json += this->m_choices.at(i).to_json();
            if (i + 1 < this->m_choices.size()) {
                json += ',';
            }
        }
        return json + "]}";
    }

    TwitchPredictor::TwitchPredictor(const std::string& json) : TwitchBasicUser{json} {
        this->m_channel_points_used = std::stoul(get_object_param("\"channel_points_used\"", json, "0"));
        this->m_channel_points_won = std::stoul(get_object_param("\"channel_points_won\"", json, "0"));
    }

    TwitchPredictor::TwitchPredictor(const std::string& user_id,
                                     const std::string& user_name,
                                     const std::string& user_login,
                                     const size_t& channel_points_used,
                                     const size_t& channel_points_won)
                                     : TwitchBasicUser{user_id, user_name, user_login},
                                       m_channel_points_used{channel_points_used},
                                       m_channel_points_won{channel_points_won} {
    }

    std::string TwitchPredictor::to_json() const {
        std::string json = "{\"user_id\":\"" + this->m_user_id
            + "\",\"user_name\":\"" + this->m_user_name
            + "\",\"user_login\":\"" + this->m_user_login
            + "\",\"channel_points_used\":" + std::to_string(this->m_channel_points_used)
            + ",\"channel_points_won\":" + std::to_string(this->m_channel_points_won)
            + "}";
        return json;
    }

    TwitchPredictionOutcome::TwitchPredictionOutcome(const std::string& json) {
        this->m_id = get_object_param("\"id\"", json);
        this->m_title = get_object_param("\"title\"", json);
        this->m_color = get_object_param("\"color\"", json);
        this->m_users = std::stoul(get_object_param("\"users\"", json, "0"));
        this->m_channel_points = std::stoul(get_object_param("\"channel_points\"", json, "0"));
        std::string string_with_predictors = get_object_param("\"top_predictors\"", json);
        if (string_with_predictors != "null" && string_with_predictors != "") {
            std::vector<std::string> str_predictors = json_to_vector(string_with_predictors);
            for (std::string predictor : str_predictors) {
                this->m_top_predictors.push_back(TwitchPredictor(predictor));
            }
        }
    }

    TwitchPredictionOutcome::TwitchPredictionOutcome(const std::string& id,
                                                     const std::string& title,
                                                     const std::string& color,
                                                     const size_t& users,
                                                     const size_t& channel_points,
                                                     const std::vector<TwitchPredictor>& top_predictors)
                                                     : m_id{id},
                                                       m_title{title},
                                                       m_color{color},
                                                       m_users{users},
                                                       m_channel_points{channel_points},
                                                       m_top_predictors{top_predictors} {
    }

    std::string TwitchPredictionOutcome::to_json() const {
        std::string json = "{\"id\":\"" + this->m_id
            + "\",\"title\":\"" + this->m_title
            + "\",\"color\":\"" + this->m_color
            + ",\"users\":" + std::to_string(this->m_users)
            + ",\"channel_points\":" + std::to_string(this->m_channel_points)
            + ",\"top_predictors\":";
        if (this->m_top_predictors.size()) {
            json += "[";
            for (size_t i {0}; i < this->m_top_predictors.size(); ++i) {
                json += this->m_top_predictors.at(i).to_json();
                if (i + 1 < this->m_top_predictors.size()) {
                    json += ',';
                }
            }
            json += "]";
        } else {
            json += "null";
        }
        return json + "}";
    }

    TwitchPrediction::TwitchPrediction(const std::string& json) {
        this->m_id = get_object_param("\"id\"", json);
        this->m_broadcaster_id = get_object_param("\"broadcaster_id\"", json);
        this->m_broadcaster_name = get_object_param("\"broadcaster_name\"", json);
        this->m_broadcaster_login = get_object_param("\"broadcaster_login\"", json);
        this->m_title = get_object_param("\"title\"", json);
        this->m_status = get_object_param("\"status\"", json);
        this->m_winning_outcome_id = get_object_param("\"winning_outcome_id\"", json);
        this->m_created_at = get_object_param("\"created_at\"", json);
        this->m_ended_at = get_object_param("\"ended_at\"", json);
        this->m_locked_at = get_object_param("\"locked_at\"", json);
        this->m_prediction_window = std::stoul(get_object_param("\"prediction_window\"", json, "0"));
        std::string string_with_outcomes = get_object_param("\"outcomes\"", json);
        std::vector<std::string> str_outcomes = json_to_vector(string_with_outcomes);
        for (std::string outcomes : str_outcomes) {
            this->m_outcomes.push_back(TwitchPredictionOutcome(outcomes));
        }
    }

    TwitchPrediction::TwitchPrediction(const std::string& id,
                                       const std::string& broadcaster_id,
                                       const std::string& broadcaster_name,
                                       const std::string& broadcaster_login,
                                       const std::string& title,
                                       const std::string& status,
                                       const std::string& winning_outcome_id,
                                       const std::string& created_at,
                                       const std::string& ended_at,
                                       const std::string& locked_at,
                                       const size_t& prediction_window,
                                       const std::vector<TwitchPredictionOutcome>& outcomes)
                                       : m_id{id},
                                         m_broadcaster_id{broadcaster_id},
                                         m_broadcaster_name{broadcaster_name},
                                         m_broadcaster_login{broadcaster_login},
                                         m_title{title},
                                         m_status{status},
                                         m_winning_outcome_id{winning_outcome_id},
                                         m_created_at{created_at},
                                         m_ended_at{ended_at},
                                         m_locked_at{locked_at},
                                         m_prediction_window{prediction_window},
                                         m_outcomes{outcomes} {
    }

    std::string TwitchPrediction::get_id() const {
        return this->m_id;
    }

    std::string TwitchPrediction::to_json() const {
        std::string json {"{\"id\":\"" + this->m_id
            + "\",\"broadcaster_id\":\"" + this->m_broadcaster_id
            + "\",\"broadcaster_name\":\"" + this->m_broadcaster_name
            + "\",\"broadcaster_login\":\"" + this->m_broadcaster_login
            + "\",\"title\":\"" + this->m_title
            + "\",\"status\":\"" + this->m_status
            + "\",\"winning_outcome_id\":" + (this->m_winning_outcome_id == "null" ? this->m_winning_outcome_id : std::string("\"" + this->m_winning_outcome_id + "\""))
            + ",\"created_at\":\"" + this->m_created_at
            + "\",\"ended_at\":" + (this->m_ended_at == "null" ? this->m_ended_at : std::string("\"" + this->m_ended_at + "\""))
            + ",\"locked_at\":" + (this->m_locked_at == "null" ? this->m_locked_at : std::string("\"" + this->m_locked_at + "\""))
            + ",\"prediction_window\":" + std::to_string(this->m_prediction_window)
            + ",\"outcomes\":["};
        for (size_t i {0}; i < this->m_outcomes.size(); ++i) {
            json += this->m_outcomes.at(i).to_json();
            if (i + 1 < this->m_outcomes.size()) {
                json += ',';
            }
        }
        return json + "]}";
    }

    TwitchCreatorsGoal::TwitchCreatorsGoal(const std::string& json) {
        this->m_id = get_object_param("\"id\"", json);
        this->m_broadcaster_id = get_object_param("\"broadcaster_id\"", json);
        this->m_broadcaster_name = get_object_param("\"broadcaster_name\"", json);
        this->m_broadcaster_login = get_object_param("\"broadcaster_login\"", json);
        this->m_type = get_object_param("\"type\"", json);
        this->m_description = get_object_param("\"description\"", json);
        this->m_created_at = get_object_param("\"created_at\"", json);
        this->m_current_amount = std::stoul(get_object_param("\"current_amount\"", json, "0"));
        this->m_target_amount = std::stoul(get_object_param("\"target_amount\"", json, "0"));
    }

    TwitchCreatorsGoal::TwitchCreatorsGoal(const std::string& id,
                                           const std::string& broadcaster_id,
                                           const std::string& broadcaster_name,
                                           const std::string& broadcaster_login,
                                           const std::string& type,
                                           const std::string& description,
                                           const std::string& created_at,
                                           const size_t& current_amount,
                                           const size_t& target_amount)
                                           : m_id{id},
                                             m_broadcaster_id{broadcaster_id},
                                             m_broadcaster_name{broadcaster_name},
                                             m_broadcaster_login{broadcaster_login},
                                             m_type{type},
                                             m_description{description},
                                             m_created_at{created_at},
                                             m_current_amount{current_amount},
                                             m_target_amount{target_amount} {
    }

    std::string TwitchCreatorsGoal::to_json() const {
        std::string json = "{\"id\":\"" + this->m_id
            + "\",\"broadcaster_id\":\"" + this->m_broadcaster_id
            + "\",\"broadcaster_name\":\"" + this->m_broadcaster_name
            + "\",\"broadcaster_login\":\"" + this->m_broadcaster_login
            + "\",\"type\":\"" + this->m_type
            + "\",\"description\":\"" + this->m_description
            + "\",\"created_at\":\"" + this->m_created_at
            + "\",\"current_amount\":" + std::to_string(this->m_current_amount)
            + ",\"target_amount\":" + std::to_string(this->m_target_amount)
            + "}";
        return json;
    }

    TwitchCreatedClip::TwitchCreatedClip(const std::string& json,
                                         const size_t& limit,
                                         const size_t& remaining)
                                         : m_limit{limit},
                                           m_remaining{remaining} {
        this->m_id = get_object_param("\"id\"", json);
        this->m_edit_url = get_object_param("\"edit_url\"", json);
    }

    TwitchCreatedClip::TwitchCreatedClip(const std::string& id,
                                         const std::string& edit_url,
                                         const size_t& limit,
                                         const size_t& remaining)
                                         : m_id{id},
                                           m_edit_url{edit_url},
                                           m_limit{limit},
                                           m_remaining{remaining} {
    }

    std::string TwitchCreatedClip::to_json() const {
        std::string json = "{\"id\":\"" + this->m_id
            + "\",\"edit_url\":\"" + this->m_edit_url
            + "\",\"limit\":" + std::to_string(this->m_limit)
            + ",\"remaining\":" + std::to_string(this->m_remaining)
            + "}";
        return json;
    }

    TwitchBroadcasterSubscription::TwitchBroadcasterSubscription(const std::string& json) {
        this->m_broadcaster_id = get_object_param("\"broadcaster_id\"", json);
        this->m_broadcaster_name = get_object_param("\"broadcaster_name\"", json);
        this->m_broadcaster_login = get_object_param("\"broadcaster_login\"", json);
        this->m_gifter_id = get_object_param("\"gifter_id\"", json);
        this->m_gifter_login = get_object_param("\"gifter_login\"", json);
        this->m_gifter_name = get_object_param("\"gifter_name\"", json);
        this->m_tier = get_object_param("\"tier\"", json);
        this->m_plan_name = get_object_param("\"plan_name\"", json);
        this->m_user_id = get_object_param("\"user_id\"", json);
        this->m_user_name = get_object_param("\"user_name\"", json);
        this->m_user_login = get_object_param("\"user_login\"", json);
        this->m_is_gift = get_object_param("\"is_gift\"", json) == "true";
    }

    TwitchBroadcasterSubscription::TwitchBroadcasterSubscription(const std::string& broadcaster_id,
                                                                 const std::string& broadcaster_login,
                                                                 const std::string& broadcaster_name,
                                                                 const std::string& gifter_id,
                                                                 const std::string& gifter_login,
                                                                 const std::string& gifter_name,
                                                                 const std::string& tier,
                                                                 const std::string& plan_name,
                                                                 const std::string& user_id,
                                                                 const std::string& user_name,
                                                                 const std::string& user_login,
                                                                 const bool& is_gift)
                                                                 : m_broadcaster_id{broadcaster_id},
                                                                   m_broadcaster_login{broadcaster_login},
                                                                   m_broadcaster_name{broadcaster_name},
                                                                   m_gifter_id{gifter_id},
                                                                   m_gifter_login{gifter_login},
                                                                   m_gifter_name{gifter_name},
                                                                   m_tier{tier},
                                                                   m_plan_name{plan_name},
                                                                   m_user_id{user_id},
                                                                   m_user_name{user_name},
                                                                   m_user_login{user_login},
                                                                   m_is_gift{is_gift} {
    }

    std::string TwitchBroadcasterSubscription::to_json() const {
        std::string json = "{\"broadcaster_id\":\"" + this->m_broadcaster_id
            + "\",\"broadcaster_name\":\"" + this->m_broadcaster_name
            + "\",\"broadcaster_login\":\"" + this->m_broadcaster_login
            + "\",\"gifter_id\":\"" + this->m_gifter_id
            + "\",\"gifter_login\":\"" + this->m_gifter_login
            + "\",\"gifter_name\":\"" + this->m_gifter_name
            + "\",\"tier\":\"" + this->m_tier
            + "\",\"plan_name\":\"" + this->m_plan_name
            + "\",\"user_id\":\"" + this->m_user_id
            + "\",\"user_name\":\"" + this->m_user_name
            + "\",\"user_login\":\"" + this->m_user_login
            + "\",\"is_gift\":" + std::string(this->m_is_gift ? "true" : "false")
            + "}";
        return json;
    }

    TwitchBroadcasterSubscriptions::TwitchBroadcasterSubscriptions(const size_t& total,
                                                                   const size_t& points,
                                                                   const std::vector<TwitchBroadcasterSubscription>& broadcaster_subscriptions)
                                                                   : m_total{total},
                                                                     m_points{points},
                                                                     m_broadcaster_subscriptions{broadcaster_subscriptions} {
    }

    std::string TwitchBroadcasterSubscriptions::to_json() const {
        std::string json = "{\"total\":" + std::to_string(this->m_total)
            + ",\"points\":" + std::to_string(this->m_points)
            + ",\"broadcaster_subscriptions\":[";
        for (size_t i {0}; i < this->m_broadcaster_subscriptions.size(); ++i) {
            if (i) {
                json += ",";
            }
            json += this->m_broadcaster_subscriptions.at(i).to_json();
        }
        json += "]}";
        return json;
    }

    TwitchCustomReward::TwitchCustomReward(const std::string& json) {
        this->m_id = get_object_param("\"id\"", json);
        this->m_broadcaster_id = get_object_param("\"broadcaster_id\"", json);
        this->m_broadcaster_name = get_object_param("\"broadcaster_name\"", json);
        this->m_broadcaster_login = get_object_param("\"broadcaster_login\"", json);
        this->m_title = get_object_param("\"title\"", json);
        this->m_prompt = get_object_param("\"prompt\"", json);
        this->m_background_color = get_object_param("\"background_color\"", json);
        this->m_cooldown_expires_at = get_object_param("\"cooldown_expires_at\"", json);
        this->m_cost = std::stoul(get_object_param("\"cost\"", json, "0"));
        this->m_is_enabled = get_object_param("\"is_enabled\"", json) == "true";
        this->m_is_user_input_required = get_object_param("\"is_user_input_required\"", json) == "true";
        this->m_is_paused = get_object_param("\"is_paused\"", json) == "true";
        this->m_is_in_stock = get_object_param("\"is_in_stock\"", json) == "true";
        this->m_should_redemptions_skip_request_queue = get_object_param("\"should_redemptions_skip_request_queue\"", json) == "true";
        std::string redemptions = get_object_param("\"m_redemptions_redeemed_current_stream\"", json, "-1");
        this->m_redemptions_redeemed_current_stream = redemptions == "null" ? -1 : std::stoi(redemptions);
        std::string max_per_stream_setting = get_object_param("\"max_per_stream_setting\"", json);
        this->m_max_per_stream_setting_enabled = get_object_param("\"is_enabled\"", max_per_stream_setting) == "true";
        this->m_max_per_stream = std::stoul(get_object_param("\"max_per_stream\"", max_per_stream_setting, "0"));
        std::string max_per_user_per_stream_setting = get_object_param("\"max_per_user_per_stream_setting\"", json);
        this->m_max_per_user_per_stream_setting_enabled = get_object_param("\"is_enabled\"", max_per_user_per_stream_setting) == "true";
        this->m_max_per_user_per_stream = std::stoul(get_object_param("\"max_per_user_per_stream\"", max_per_user_per_stream_setting, "0"));
        std::string global_cooldown_setting = get_object_param("\"global_cooldown_setting\"", json);
        this->m_global_cooldown_setting_enabled = get_object_param("\"is_enabled\"", global_cooldown_setting) == "true";
        this->m_global_cooldown_seconds = std::stoul(get_object_param("\"global_cooldown_seconds\"", global_cooldown_setting, "0"));
        std::string image = get_object_param("\"image\"", json);
        if (image != "null") {
            this->m_url_1x = get_object_param("\"url_1x\"", image);
            this->m_url_2x = get_object_param("\"url_2x\"", image);
            this->m_url_4x = get_object_param("\"url_4x\"", image);
        }
        std::string default_image = get_object_param("\"default_image\"", json);
        this->m_default_url_1x = get_object_param("\"url_1x\"", default_image);
        this->m_default_url_2x = get_object_param("\"url_2x\"", default_image);
        this->m_default_url_4x = get_object_param("\"url_4x\"", default_image);
    }

    TwitchCustomReward::TwitchCustomReward(const std::string& id,
                                           const std::string& broadcaster_id,
                                           const std::string& broadcaster_name,
                                           const std::string& broadcaster_login,
                                           const std::string& title,
                                           const std::string& prompt,
                                           const std::string& background_color,
                                           const std::string& cooldown_expires_at,
                                           const size_t& cost,
                                           const bool& is_enabled,
                                           const bool& is_user_input_required,
                                           const bool& is_paused,
                                           const bool& is_in_stock,
                                           const bool& should_redemptions_skip_request_queue,
                                           const int& redemptions_redeemed_current_stream,
                                           const bool& max_per_stream_setting_enabled,
                                           const size_t& max_per_stream,
                                           const bool& max_per_user_per_stream_setting_enabled,
                                           const size_t& max_per_user_per_stream,
                                           const bool& global_cooldown_setting_enabled,
                                           const size_t& global_cooldown_seconds,
                                           const std::string& url_1x,
                                           const std::string& url_2x,
                                           const std::string& url_4x,
                                           const std::string& default_url_1x,
                                           const std::string& default_url_2x,
                                           const std::string& default_url_4x)
                                           : m_id{id},
                                             m_broadcaster_id{broadcaster_id},
                                             m_broadcaster_name{broadcaster_name},
                                             m_broadcaster_login{broadcaster_login},
                                             m_title{title},
                                             m_prompt{prompt},
                                             m_background_color{background_color},
                                             m_cooldown_expires_at{cooldown_expires_at},
                                             m_cost{cost},
                                             m_is_enabled{is_enabled},
                                             m_is_user_input_required{is_user_input_required},
                                             m_is_paused{is_paused},
                                             m_is_in_stock{is_in_stock},
                                             m_should_redemptions_skip_request_queue{should_redemptions_skip_request_queue},
                                             m_redemptions_redeemed_current_stream{redemptions_redeemed_current_stream},
                                             m_max_per_stream_setting_enabled{max_per_stream_setting_enabled},
                                             m_max_per_stream{max_per_stream},
                                             m_max_per_user_per_stream_setting_enabled{max_per_user_per_stream_setting_enabled},
                                             m_max_per_user_per_stream{max_per_user_per_stream},
                                             m_global_cooldown_setting_enabled{global_cooldown_setting_enabled},
                                             m_global_cooldown_seconds{global_cooldown_seconds},
                                             m_url_1x{url_1x},
                                             m_url_2x{url_2x},
                                             m_url_4x{url_4x},
                                             m_default_url_1x{default_url_1x},
                                             m_default_url_2x{default_url_2x},
                                             m_default_url_4x{default_url_4x} {
    }

    std::string TwitchCustomReward::to_json() const {
        std::string json = "{\"id\":\"" + this->m_id
            + "\",\"broadcaster_id\":\"" + this->m_broadcaster_id
            + "\",\"broadcaster_name\":\"" + this->m_broadcaster_name
            + "\",\"broadcaster_login\":\"" + this->m_broadcaster_login
            + "\",\"title\":\"" + this->m_title
            + "\",\"prompt\":\"" + this->m_prompt
            + "\",\"background_color\":\"" + this->m_background_color
            + "\",\"cooldown_expires_at\":" + (this->m_cooldown_expires_at == "null" ? this->m_cooldown_expires_at : ("\"" + this->m_cooldown_expires_at + "\""))
            + ",\"cost\":" + std::to_string(this->m_cost)
            + ",\"is_enabled\":" + std::string(this->m_is_enabled ? "true" : "false")
            + ",\"is_user_input_required\":" + std::string(this->m_is_user_input_required ? "true" : "false")
            + ",\"is_paused\":" + std::string(this->m_is_paused ? "true" : "false")
            + ",\"is_in_stock\":" + std::string(this->m_is_in_stock ? "true" : "false")
            + ",\"should_redemptions_skip_request_queue\":" + std::string(this->m_should_redemptions_skip_request_queue ? "true" : "false")
            + ",\"redemptions_redeemed_current_stream\":" + (this->m_redemptions_redeemed_current_stream == -1 ? std::string("null") : std::to_string(this->m_redemptions_redeemed_current_stream))
            + ",\"max_per_stream_setting\":{\"is_enabled\":" + std::string(this->m_max_per_stream_setting_enabled ? "true" : "false")
            + ",\"max_per_stream\":" + std::to_string(this->m_max_per_stream)
            + "},\"max_per_user_per_stream_setting\":{\"is_enabled\":" + std::string(this->m_max_per_user_per_stream_setting_enabled ? "true" : "false")
            + ",\"max_per_user_per_stream\":" + std::to_string(this->m_max_per_user_per_stream)
            + "},\"global_cooldown_setting\":{\"is_enabled\":" + std::string(this->m_global_cooldown_setting_enabled ? "true" : "false")
            + ",\"global_cooldown_seconds\":" + std::to_string(this->m_global_cooldown_seconds)
            + "},\"default_image\":{\"url_1x\":\"" + this->m_default_url_1x
            + "\",\"url_2x\":\"" + this->m_default_url_2x
            + "\",\"url_4x\":\"" + this->m_default_url_4x
            + "\"},\"image\":";
        if (!this->m_url_1x.size()) {
            json += "null";
        } else {
            json += "{\"url_1x\":\"" + this->m_url_1x
                + "\",\"url_2x\":\"" + this->m_url_2x
                + "\",\"url_4x\":\"" + this->m_url_4x + "\"}";
        }
        return json + "}";
    }

    TwitchCustomRewardRedemption::TwitchCustomRewardRedemption(const std::string& json) {
        this->m_id = get_object_param("\"id\"", json);
        this->m_broadcaster_id = get_object_param("\"broadcaster_id\"", json);
        this->m_broadcaster_name = get_object_param("\"broadcaster_name\"", json);
        this->m_broadcaster_login = get_object_param("\"broadcaster_login\"", json);
        this->m_user_id = get_object_param("\"user_id\"", json);
        this->m_user_name = get_object_param("\"user_name\"", json);
        this->m_user_login = get_object_param("\"user_login\"", json);
        this->m_user_input = get_object_param("\"user_input\"", json);
        this->m_status = get_object_param("\"status\"", json);
        this->m_redeemed_at = get_object_param("\"redeemed_at\"", json);
        std::string reward = get_object_param("\"reward\"", json);
        this->m_reward_id = get_object_param("\"id\"", reward);
        this->m_reward_title = get_object_param("\"title\"", reward);
        this->m_reward_prompt = get_object_param("\"prompt\"", reward);
        this->m_reward_cost = std::stoul(get_object_param("\"cost\"", reward, "0"));
    }

    TwitchCustomRewardRedemption::TwitchCustomRewardRedemption(const std::string& id,
                                                               const std::string& broadcaster_id,
                                                               const std::string& broadcaster_name,
                                                               const std::string& broadcaster_login,
                                                               const std::string& user_id,
                                                               const std::string& user_name,
                                                               const std::string& user_login,
                                                               const std::string& user_input,
                                                               const std::string& status,
                                                               const std::string& redeemed_at,
                                                               const std::string& reward_id,
                                                               const std::string& reward_title,
                                                               const std::string& reward_prompt,
                                                               const size_t& reward_cost)
                                                               : m_id{id},
                                                                 m_broadcaster_id{broadcaster_id},
                                                                 m_broadcaster_name{broadcaster_name},
                                                                 m_broadcaster_login{broadcaster_login},
                                                                 m_user_id{user_id},
                                                                 m_user_name{user_name},
                                                                 m_user_login{user_login},
                                                                 m_user_input{user_input},
                                                                 m_status{status},
                                                                 m_redeemed_at{redeemed_at},
                                                                 m_reward_id{reward_id},
                                                                 m_reward_title{reward_title},
                                                                 m_reward_prompt{reward_prompt},
                                                                 m_reward_cost{reward_cost} {
    }

    std::string TwitchCustomRewardRedemption::to_json() const {
        std::string json = "{\"id\":\"" + this->m_id
            + "\",\"broadcaster_id\":\"" + this->m_broadcaster_id
            + "\",\"broadcaster_name\":\"" + this->m_broadcaster_name
            + "\",\"broadcaster_login\":\"" + this->m_broadcaster_login
            + "\",\"user_id\":\"" + this->m_user_id
            + "\",\"user_name\":\"" + this->m_user_name
            + "\",\"user_login\":\"" + this->m_user_login
            + "\",\"user_input\":\"" + this->m_user_input
            + "\",\"status\":\"" + this->m_status
            + "\",\"redeemed_at\":\"" + this->m_redeemed_at
            + "\",\"reward\":{\"id\":\"" + this->m_reward_id
            + "\",\"title\":\"" + this->m_reward_title
            + "\",\"prompt\":\"" + this->m_reward_prompt
            + "\",\"cost\":" + std::to_string(this->m_reward_cost)
            + "}}";
        return json;
    }

    TwitchUserSubscription::TwitchUserSubscription(const std::string& json) {
        this->m_broadcaster_id = get_object_param("\"broadcaster_id\"", json);
        this->m_broadcaster_name = get_object_param("\"broadcaster_name\"", json);
        this->m_broadcaster_login = get_object_param("\"broadcaster_login\"", json);
        this->m_gifter_login = get_object_param("\"gifter_login\"", json);
        this->m_gifter_name = get_object_param("\"gifter_name\"", json);
        this->m_tier = get_object_param("\"tier\"", json);
        this->m_is_gift = get_object_param("\"is_gift\"", json) == "true";
    }

    TwitchUserSubscription::TwitchUserSubscription(const std::string& broadcaster_id,
                                                   const std::string& broadcaster_login,
                                                   const std::string& broadcaster_name,
                                                   const std::string& gifter_login,
                                                   const std::string& gifter_name,
                                                   const std::string& tier,
                                                   const bool& is_gift)
                                                   : m_broadcaster_id{broadcaster_id},
                                                     m_broadcaster_login{broadcaster_login},
                                                     m_broadcaster_name{broadcaster_name},
                                                     m_gifter_login{gifter_login},
                                                     m_gifter_name{gifter_name},
                                                     m_tier{tier},
                                                     m_is_gift{is_gift} {
    }

    std::string TwitchUserSubscription::to_json() const {
        std::string json = "{\"broadcaster_id\":\"" + this->m_broadcaster_id
            + "\",\"broadcaster_name\":\"" + this->m_broadcaster_name
            + "\",\"broadcaster_login\":\"" + this->m_broadcaster_login
            + "\",\"gifter_login\":\"" + this->m_gifter_login
            + "\",\"gifter_name\":\"" + this->m_gifter_name
            + "\",\"tier\":\"" + this->m_tier
            + "\",\"is_gift\":" + std::string(this->m_is_gift ? "true" : "false")
            + "}";
        return json;
    }

    TwitchStreamMarker::TwitchStreamMarker(const std::string& json) {
        this->m_id = get_object_param("\"id\"", json);
        this->m_description = get_object_param("\"description\"", json);
        this->m_created_at = get_object_param("\"created_at\"", json);
        this->m_url = get_object_param("\"URL\"", json);
        this->m_position_seconds = std::stoul(get_object_param("\"position_seconds\"", json, "0"));
    }

    TwitchStreamMarker::TwitchStreamMarker(const std::string& id,
                                           const std::string& description,
                                           const std::string& created_at,
                                           const std::string& url,
                                           const size_t& position_seconds)
                                           : m_id{id},
                                             m_description{description},
                                             m_created_at{created_at},
                                             m_url{url},
                                             m_position_seconds{position_seconds} {
    }

    std::string TwitchStreamMarker::to_json() const {
        std::string json = "{\"id\":\"" + this->m_id
            + "\",\"description\":\"" + this->m_description
            + "\",\"created_at\":\"" + this->m_created_at
            + (this->m_url.size() ? "\",\"URL\":\"" + this->m_url : this->m_url)
            + "\",\"position_seconds\":" + std::to_string(this->m_position_seconds)
            + "}";
        return json;
    }

    TwitchVideoWithMarkers::TwitchVideoWithMarkers(const std::string& json) {
        this->m_video_id = get_object_param("\"video_id\"", json);
        std::string string_with_markers = get_object_param("\"markers\"", json);
        std::vector<std::string> str_markers = json_to_vector(string_with_markers);
        for (std::string marker : str_markers) {
            this->m_markers.push_back(TwitchStreamMarker(marker));
        }
    }

    TwitchVideoWithMarkers::TwitchVideoWithMarkers(const std::string& video_id,
                                                   const std::vector<TwitchStreamMarker>& markers)
                                                   : m_video_id{video_id},
                                                     m_markers{markers} {
    }

    std::string TwitchVideoWithMarkers::to_json() const {
        std::string json = "{\"video_id\":\"" + this->m_video_id
            + "\",\"markers\":[";
        for (size_t i {0}; i < this->m_markers.size(); ++i) {
            json += this->m_markers.at(i).to_json();
            if (i + 1 < this->m_markers.size()) {
                json += ',';
            }
        }
        return json + "]}";
    }

    TwitchVideosWithMarkers::TwitchVideosWithMarkers(const std::string& json) {
        this->m_user_id = get_object_param("\"user_id\"", json);
        this->m_user_name = get_object_param("\"user_name\"", json);
        this->m_user_login = get_object_param("\"user_login\"", json);
        std::string string_with_videos = get_object_param("\"videos\"", json);
        std::vector<std::string> str_videos = json_to_vector(string_with_videos);
        for (std::string video : str_videos) {
            this->m_videos.push_back(TwitchVideoWithMarkers(video));
        }
    }

    TwitchVideosWithMarkers::TwitchVideosWithMarkers(const std::string& user_id,
                                                     const std::string& user_name,
                                                     const std::string& user_login,
                                                     const std::vector<TwitchVideoWithMarkers>& videos)
                                                     : m_user_id{user_id},
                                                       m_user_name{user_name},
                                                       m_user_login{user_login},
                                                       m_videos{videos} {
    }

    std::string TwitchVideosWithMarkers::to_json() const {
        std::string json = "{\"user_id\":\"" + this->m_user_id
            + "\",\"user_name\":\"" + this->m_user_name
            + "\",\"user_login\":\"" + this->m_user_login
            + "\",\"videos\":[";
        for (size_t i {0}; i < this->m_videos.size(); ++i) {
            json += this->m_videos.at(i).to_json();
            if (i + 1 < this->m_videos.size()) {
                json += ',';
            }
        }
        return json + "]}";
    }

    TwitchStreamTag::TwitchStreamTag(const std::string& json) {
        this->m_tag_id = get_object_param("\"tag_id\"", json);
        this->m_is_auto = get_object_param("\"is_auto\"", json) == "true";
        std::string localization_names = get_object_param("\"localization_names\"", json);
        this->m_localization_names = string_to_string_map(localization_names);
        std::string localization_descriptions = get_object_param("\"localization_descriptions\"", json);
        this->m_localization_descriptions = string_to_string_map(localization_descriptions);
    }

    TwitchStreamTag::TwitchStreamTag(const std::string& tag_id,
                                     const bool& is_auto,
                                     StringMap localization_names,
                                     StringMap localization_descriptions)
                                     : m_tag_id{tag_id},
                                       m_is_auto{is_auto},
                                       m_localization_names{localization_names},
                                       m_localization_descriptions{localization_descriptions} {
    }

    std::string TwitchStreamTag::to_json() const {
        std::string json = "{\"tag_id\":\"" + m_tag_id
            + "\",\"is_auto\":" + std::string(m_is_auto ? "true" : "false")
            + ",\"localization_names\":{" + string_map_to_string(m_localization_names)
            + "},\"localization_descriptions\":{" + string_map_to_string(m_localization_descriptions);
        return json + "}}";
    }

    TwitchAutoModMessage::TwitchAutoModMessage(const std::string& json) {
        this->m_msg_id = get_object_param("\"msg_id\"", json);
        this->m_msg_text = get_object_param("\"msg_text\"", json);
        this->m_user_id = get_object_param("\"user_id\"", json);
    }

    TwitchAutoModMessage::TwitchAutoModMessage(const std::string& msg_id,
                                               const std::string& msg_text,
                                               const std::string& user_id)
                                               : m_msg_id{msg_id},
                                                 m_msg_text{msg_text},
                                                 m_user_id{user_id} {
    }

    std::string TwitchAutoModMessage::to_json() const {
        const std::string json {"{\"msg_id\":\"" + this->m_msg_id
            + "\",\"msg_text\":\"" + this->m_msg_text
            + "\",\"user_id\":\"" + this->m_user_id
            + "\"}"};
        return json;
    }

    TwitchAutoModMessageStatus::TwitchAutoModMessageStatus(const std::string& json) {
        this->m_msg_id = get_object_param("\"msg_id\"", json);
        this->m_is_permitted = get_object_param("\"is_permitted\"", json) == "true";
    }

    TwitchAutoModMessageStatus::TwitchAutoModMessageStatus(const std::string& msg_id,
                                                           const bool& is_permitted)
                                                           : m_msg_id{msg_id},
                                                             m_is_permitted{is_permitted} {
    }

    std::string TwitchAutoModMessageStatus::to_json() const {
        std::string json = "{\"msg_id\":\"" + this->m_msg_id
            + "\",\"is_permitted\":" + std::string(this->m_is_permitted ? "true" : "false")
            + "}";
        return json;
    }

    TwitchAutoModMessageManaging::TwitchAutoModMessageManaging(const std::string& json) {
        this->m_msg_id = get_object_param("\"msg_id\"", json);
        this->m_user_id = get_object_param("\"user_id\"", json);
        this->m_action = get_object_param("\"action\"", json);
    }

    TwitchAutoModMessageManaging::TwitchAutoModMessageManaging(const std::string& msg_id,
                                                               const std::string& user_id,
                                                               const std::string& action)
                                                               : m_msg_id{msg_id},
                                                                 m_user_id{user_id},
                                                                 m_action{action} {
    }

    std::string TwitchAutoModMessageManaging::to_json() const {
        std::string json = "{\"msg_id\":\"" + this->m_msg_id
            + "\",\"user_id\":\"" + this->m_user_id
            + "\",\"action\":\"" + this->m_action
            + "\"}";
        return json;
    }

    TwitchHypeTrainContribution::TwitchHypeTrainContribution(const std::string& json) {
        this->m_user = get_object_param("\"user\"", json);
        this->m_type = get_object_param("\"type\"", json);
        this->m_total = std::stoul(get_object_param("\"total\"", json, "0"));
    }

    TwitchHypeTrainContribution::TwitchHypeTrainContribution(const std::string& user,
                                                             const std::string& type,
                                                             const size_t& total)
                                                             : m_user{user},
                                                               m_type{type},
                                                               m_total{total} {
    }

    std::string TwitchHypeTrainContribution::to_json() const {
        std::string json = "{\"user\":\"" + this->m_user
            + "\",\"type\":\"" + this->m_type
            + "\",\"total\":" + std::to_string(this->m_total)
            + "}";
        return json;
    }

    TwitchHypeTrainData::TwitchHypeTrainData(const std::string& json) {
        this->m_id = get_object_param("\"id\"", json);
        this->m_broadcaster_id = get_object_param("\"broadcaster_id\"", json);
        this->m_cooldown_end_time = get_object_param("\"cooldown_end_time\"", json);
        this->m_started_at = get_object_param("\"started_at\"", json);
        this->m_expires_at = get_object_param("\"expires_at\"", json);
        this->m_level = std::stoul(get_object_param("\"level\"", json, "0"));
        this->m_total = std::stoul(get_object_param("\"total\"", json, "0"));
        this->m_goal = std::stoul(get_object_param("\"goal\"", json, "0"));
        std::string last_contribution = get_object_param("\"last_contribution\"", json, "null");
        if (last_contribution == "null") {
            this->m_last_contribution = nullptr;
        } else {
            this->m_last_contribution = std::make_shared<TwitchHypeTrainContribution>(last_contribution);
        }
        std::string top_contributions = get_object_param("\"top_contributions\"", json);
        std::vector<std::string> str_contributions = json_to_vector(top_contributions);
        for (std::string contribution : str_contributions) {
            this->m_top_contributions.push_back(TwitchHypeTrainContribution(contribution));
        }
    }

    TwitchHypeTrainData::TwitchHypeTrainData(const std::string& id,
                                             const std::string& broadcaster_id,
                                             const std::string& cooldown_end_time,
                                             const std::string& started_at,
                                             const std::string& expires_at,
                                             const size_t& level,
                                             const size_t& total,
                                             const size_t& goal,
                                             std::shared_ptr<TwitchHypeTrainContribution> last_contribution,
                                             const std::vector<TwitchHypeTrainContribution>& top_contributions)
                                             : m_id{id},
                                               m_broadcaster_id{broadcaster_id},
                                               m_cooldown_end_time{cooldown_end_time},
                                               m_started_at{started_at},
                                               m_expires_at{expires_at},
                                               m_level{level},
                                               m_total{total},
                                               m_goal{goal},
                                               m_last_contribution{last_contribution},
                                               m_top_contributions{top_contributions} {
    }

    std::string TwitchHypeTrainData::to_json() const {
        std::string json = "{\"id\":\"" + this->m_id
            + "\",\"broadcaster_id\":\"" + this->m_broadcaster_id
            + "\",\"cooldown_end_time\":\"" + this->m_cooldown_end_time
            + "\",\"started_at\":\"" + this->m_started_at
            + "\",\"expires_at\":\"" + this->m_expires_at
            + "\",\"level\":" + std::to_string(this->m_level)
            + ",\"total\":" + std::to_string(this->m_total)
            + ",\"goal\":" + std::to_string(this->m_goal)
            + ",\"last_contribution\":" + (this->m_last_contribution == nullptr ? "null" : this->m_last_contribution->to_json())
            + ",\"top_contributions\":[";
        for (size_t i {0}; i < this->m_top_contributions.size(); ++i) {
            json += this->m_top_contributions.at(i).to_json();
            if (i + 1 < this->m_top_contributions.size()) {
                json += ',';
            }
        }
        return json + "]}";
    }

    TwitchHypeTrainEvent::TwitchHypeTrainEvent(const std::string& json) {
        this->m_id = get_object_param("\"id\"", json);
        this->m_event_type = get_object_param("\"event_type\"", json);
        this->m_event_timestamp = get_object_param("\"event_timestamp\"", json);
        this->m_version = get_object_param("\"version\"", json);
        this->m_event_data = std::make_shared<TwitchHypeTrainData>(get_object_param("\"event_data\"", json));
    }

    TwitchHypeTrainEvent::TwitchHypeTrainEvent(const std::string& id,
                                               const std::string& event_type,
                                               const std::string& event_timestamp,
                                               const std::string& version,
                                               TwitchHypeTrainData& event_data)
                                               : m_id{id},
                                                 m_event_type{event_type},
                                                 m_event_timestamp{event_timestamp},
                                                 m_version{version},
                                                 m_event_data{&event_data} {
    }

    std::string TwitchHypeTrainEvent::to_json() const {
        std::string json = "{\"id\":\"" + this->m_id
            + "\",\"event_type\":\"" + this->m_event_type
            + "\",\"event_timestamp\":\"" + this->m_event_timestamp
            + "\",\"version\":\"" + this->m_version
            + "\",\"event_data\":" + this->m_event_data->to_json()
            + "}";
        return json;
    }

    TwitchMusicArtist::TwitchMusicArtist(const std::string& json) {
        this->m_id = get_object_param("\"id\"", json);
        this->m_name = get_object_param("\"name\"", json);
        this->m_creator_channel_id = get_object_param("\"creator_channel_id\"", json);
    }

    TwitchMusicArtist::TwitchMusicArtist(const std::string& id,
                                         const std::string& name,
                                         const std::string& creator_channel_id)
                                         : m_id{id},
                                           m_name{name},
                                           m_creator_channel_id{creator_channel_id} {
    }

    std::string TwitchMusicArtist::to_json() const {
        std::string json = "{\"id\":\"" + this->m_id
            + "\",\"name\":\"" + this->m_name
            + "\",\"creator_channel_id\":" + (this->m_creator_channel_id == "null" ? this->m_creator_channel_id : std::string("\"" + this->m_creator_channel_id + "\""))
            + "}";
        return json;
    }

    TwitchMusicAlbum::TwitchMusicAlbum(const std::string& json) {
        this->m_id = get_object_param("\"id\"", json);
        this->m_name = get_object_param("\"name\"", json);
        this->m_image_url = get_object_param("\"image_url\"", json);
    }

    TwitchMusicAlbum::TwitchMusicAlbum(const std::string& id,
                                       const std::string& name,
                                       const std::string& image_url)
                                       : m_id{id},
                                         m_name{name},
                                         m_image_url{image_url} {
    }

    std::string TwitchMusicAlbum::to_json() const {
        std::string json = "{\"id\":\"" + this->m_id
            + "\",\"name\":\"" + this->m_name
            + "\",\"image_url\":\"" + this->m_image_url
            + "\"}";
        return json;
    }

    TwitchMusicSource::TwitchMusicSource(const std::string& json) {
        this->m_id = get_object_param("\"id\"", json);
        this->m_title = get_object_param("\"title\"", json);
        this->m_content_type = get_object_param("\"content_type\"", json);
        this->m_soundtrack_url = get_object_param("\"SoundtrackURL\"", json);
        this->m_spotify_url = get_object_param("\"spotify_url\"", json);
        this->m_image_url = get_object_param("\"image_url\"", json);
    }

    TwitchMusicSource::TwitchMusicSource(const std::string& id,
                                         const std::string& title,
                                         const std::string& content_type,
                                         const std::string& soundtrack_url,
                                         const std::string& spotify_url,
                                         const std::string& image_url)
                                         : m_id{id},
                                           m_title{title},
                                           m_content_type{content_type},
                                           m_soundtrack_url{soundtrack_url},
                                           m_spotify_url{spotify_url},
                                           m_image_url{image_url} {
    }

    std::string TwitchMusicSource::to_json() const {
        std::string json = "{\"id\":\"" + this->m_id
            + "\",\"title\":\"" + this->m_title
            + "\",\"content_type\":\"" + this->m_content_type
            + "\",\"SoundtrackURL\":\"" + this->m_soundtrack_url
            + "\",\"spotify_url\":\"" + this->m_spotify_url
            + "\",\"image_url\":\"" + this->m_image_url
            + "\"}";
        return json;
    }

    TwitchTrack::TwitchTrack(const std::string& json) {
        this->m_id = get_object_param("\"id\"", json);
        this->m_title = get_object_param("\"title\"", json);
        this->m_duration = std::stoul(get_object_param("\"duration\"", json, "0"));
        std::string album = get_object_param("\"album\"", json, "null");
        if (album == "null") {
            this->m_album = nullptr;
        } else {
            this->m_album = std::make_shared<TwitchMusicAlbum>(album);
        }
        std::string artists = get_object_param("\"artists\"", json);
        std::vector<std::string> str_artists = json_to_vector(artists);
        for (std::string artist : str_artists) {
            this->m_artists.push_back(TwitchMusicArtist(artist));
        }
    }

    TwitchTrack::TwitchTrack(const std::string& id,
                             const std::string& title,
                             const size_t& duration,
                             std::shared_ptr<TwitchMusicAlbum> album,
                             const std::vector<TwitchMusicArtist>& artists)
                             : m_id{id},
                               m_title{title},
                               m_duration{duration},
                               m_album{album},
                               m_artists{artists} {
    }

    std::string TwitchTrack::to_json() const {
        std::string json = "{\"id\":\"" + this->m_id
            + "\",\"title\":\"" + this->m_title
            + "\",\"duration\":" + std::to_string(this->m_duration)
            + ",\"album\":" + (this->m_album == nullptr ? "null" : this->m_album->to_json())
            + ",\"artists\":[";
        for (size_t i {0}; i < this->m_artists.size(); ++i) {
            json += this->m_artists.at(i).to_json();
            if (i + 1 < this->m_artists.size()) {
                json += ',';
            }
        }
        return json + "]}";
    }

    TwitchCurrentTrack::TwitchCurrentTrack(const std::string& json) {
        std::string track = get_object_param("\"track\"", json, "null");
        if (track == "null") {
            this->m_track = nullptr;
        } else {
            this->m_track = std::make_shared<TwitchTrack>(track);
        }
        std::string source = get_object_param("\"source\"", json, "null");
        if (source == "null") {
            this->m_source = nullptr;
        } else {
            this->m_source = std::make_shared<TwitchMusicSource>(source);
        }
    }

    TwitchCurrentTrack::TwitchCurrentTrack(std::shared_ptr<TwitchTrack> track,
                                           std::shared_ptr<TwitchMusicSource> source)
                                           : m_track{track},
                                             m_source{source} {
    }

    std::string TwitchCurrentTrack::to_json() const {
        std::string json = "{\"track\":" + (this->m_track == nullptr ? "null" : this->m_track->to_json())
            + ",\"source\":" + (this->m_source == nullptr ? "null" : this->m_source->to_json());
        return json + "}";
    }

    TwitchBasicPlaylist::TwitchBasicPlaylist(const std::string& json) {
        this->m_id = get_object_param("\"id\"", json);
        this->m_title = get_object_param("\"title\"", json);
        this->m_description = get_object_param("\"description\"", json);
        this->m_image_url = get_object_param("\"image_url\"", json);
    }

    TwitchBasicPlaylist::TwitchBasicPlaylist(const std::string& id,
                                             const std::string& title,
                                             const std::string& description,
                                             const std::string& image_url)
                                             : m_id{id},
                                               m_title{title},
                                               m_description{description},
                                               m_image_url{image_url} {
    }

    std::string TwitchBasicPlaylist::to_json() const {
        std::string json = "{\"id\":\"" + this->m_id
            + "\",\"title\":\"" + this->m_title
            + "\",\"description\":\"" + this->m_description
            + "\",\"image_url\":\"" + this->m_image_url
            + "\"}";
        return json;
    }

    TwitchPlaylist::TwitchPlaylist(const std::string& json) : TwitchBasicPlaylist{json} {
        std::string tracks = get_object_param("\"tracks\"", json);
        std::vector<std::string> str_tracks = json_to_vector(tracks);
        for (std::string track : str_tracks) {
            this->m_tracks.push_back(TwitchTrack(track));
        }
    }

    TwitchPlaylist::TwitchPlaylist(const std::string& id,
                                   const std::string& title,
                                   const std::string& description,
                                   const std::string& image_url,
                                   const std::vector<TwitchTrack>& tracks)
                                   : TwitchBasicPlaylist{id, title, description, image_url},
                                     m_tracks{tracks} {
    }

    std::string TwitchPlaylist::to_json() const {
        std::string basic_json {TwitchBasicPlaylist::to_json()};
        std::string json = basic_json.substr(0, basic_json.size() - 1) + ",\"catalog_tracks\":[";
        for (size_t i {0}; i < this->m_tracks.size(); ++i) {
            json += this->m_tracks.at(i).to_json();
            if (i + 1 < this->m_tracks.size()) {
                json += ',';
            }
        }
        return json + "]}";
    }

    TwitchTransport::TwitchTransport(const std::string& json) {
        this->m_method = get_object_param("\"method\"", json);
        this->m_callback = get_object_param("\"callback\"", json);
        this->m_secret = get_object_param("\"secret\"", json);
    }

    TwitchTransport::TwitchTransport(const std::string& method,
                                     const std::string& callback,
                                     const std::string& secret)
                                     : m_method{method},
                                       m_callback{callback},
                                       m_secret{secret} {
    }

    std::string TwitchTransport::to_json() const {
        std::string json = "{\"method\":\"" + this->m_method
            + "\",\"callback\":\"" + this->m_callback
            + "\"}";
        return json;
    }

    TwitchCondition::TwitchCondition(const std::string& json) {
        this->m_broadcaster_user_id = get_object_param("\"broadcaster_user_id\"", json);
        this->m_from_broadcaster_user_id = get_object_param("\"from_broadcaster_user_id\"", json);
        this->m_to_broadcaster_user_id = get_object_param("\"to_broadcaster_user_id\"", json);
        this->m_reward_id = get_object_param("\"reward_id\"", json);
        this->m_organization_id = get_object_param("\"organization_id\"", json);
        this->m_category_id = get_object_param("\"category_id\"", json);
        this->m_campaign_id = get_object_param("\"campaign_id\"", json);
        this->m_extension_client_id = get_object_param("\"extension_client_id\"", json);
        this->m_client_id = get_object_param("\"client_id\"", json);
        this->m_user_id = get_object_param("\"user_id\"", json);
    }

    TwitchCondition::TwitchCondition(const std::string& id,
                                     std::string_view id_type) {
        if (id_type == CONDITION_BROADCASTER) {
            this->m_broadcaster_user_id = id;
        } else if (id_type == CONDITION_USER) {
            this->m_user_id = id;
        } else if (id_type == CONDITION_CLIENT) {
            this->m_client_id = id;
        } else if (id_type == CONDITION_EXTENSION_CLIENT) {
            this->m_extension_client_id = id;
        }
    }

    TwitchCondition::TwitchCondition(const std::string& broadcaster_user_id,
                                     const std::string& seconcary_parameter,
                                     const bool& is_reward) {
        if (is_reward) {
            this->m_broadcaster_user_id = broadcaster_user_id;
            this->m_reward_id = seconcary_parameter;
        } else {
            this->m_from_broadcaster_user_id = broadcaster_user_id;
            this->m_to_broadcaster_user_id = seconcary_parameter;
        }
    }

    TwitchCondition::TwitchCondition(const std::string& organization_id,
                                     const std::string& category_id,
                                     const std::string& campaign_id)
                                     : m_organization_id{organization_id},
                                       m_category_id{category_id},
                                       m_campaign_id{campaign_id} {
    }

    std::string TwitchCondition::to_json() const {
        std::string json = "{";
        if (this->m_broadcaster_user_id.size()) {
            json += "\"broadcaster_user_id\":\"" + this->m_broadcaster_user_id + "\"";
        }
        if (this->m_reward_id.size()) {
            json += ",\"reward_id\":\"" + this->m_reward_id + "\"";
        }
        if (this->m_from_broadcaster_user_id.size()) {
            json += "\"from_broadcaster_user_id\":\""
                + this->m_from_broadcaster_user_id
                + (this->m_to_broadcaster_user_id.size() ? "\"," : "\"");
        }
        if (this->m_to_broadcaster_user_id.size()) {
            json += "\"to_broadcaster_user_id\":\""
                + this->m_to_broadcaster_user_id + "\"";
        }
        if (this->m_organization_id.size()) {
            json += "\"organization_id\":\"" + this->m_organization_id + "\"";
        }
        if (this->m_category_id.size()) {
            json += ",\"category_id\":\"" + this->m_category_id + "\"";
        }
        if (this->m_campaign_id.size()) {
            json += ",\"campaign_id\":\"" + this->m_campaign_id + "\"";
        }
        if (this->m_extension_client_id.size()) {
            json += "\"extension_client_id\":\"" + this->m_extension_client_id + "\"";
        }
        if (this->m_client_id.size()) {
            json += "\"client_id\":\"" + this->m_client_id + "\"";
        }
        if (this->m_user_id.size()) {
            json += "\"user_id\":\"" + this->m_user_id + "\"";
        }
        return json += "}";
    }

    TwitchEventSubSubscription::TwitchEventSubSubscription(const std::string& json) {
        this->m_id = get_object_param("\"id\"", json);
        this->m_status = get_object_param("\"status\"", json);
        this->m_type = get_object_param("\"type\"", json);
        this->m_version = get_object_param("\"version\"", json);
        this->m_created_at = get_object_param("\"created_at\"", json);
        this->m_cost = std::stoul(get_object_param("\"cost\"", json, "0"));
        std::string transport = get_object_param("\"transport\"", json, "null");
        if (transport == "null") {
            this->m_transport = nullptr;
        } else {
            this->m_transport = std::make_shared<TwitchTransport>(transport);
        }
        std::string condition = get_object_param("\"condition\"", json, "null");
        if (condition == "null") {
            this->m_condition = nullptr;
        } else {
            this->m_condition = std::make_shared<TwitchCondition>(condition);
        }
    }

    TwitchEventSubSubscription::TwitchEventSubSubscription(const std::string& id,
                                                           const std::string& status,
                                                           const std::string& type,
                                                           const std::string& version,
                                                           const std::string& created_at,
                                                           const size_t& cost,
                                                           std::shared_ptr<TwitchTransport> transport,
                                                           std::shared_ptr<TwitchCondition> condition)
                                                           : m_id{id},
                                                             m_status{status},
                                                             m_type{type},
                                                             m_version{version},
                                                             m_created_at{created_at},
                                                             m_cost{cost},
                                                             m_transport{transport},
                                                             m_condition{condition} {
    }

    std::string TwitchEventSubSubscription::to_json() const {
        std::string json = "{\"id\":\"" + this->m_id
            + "\",\"status\":\"" + this->m_status
            + "\",\"type\":\"" + this->m_type
            + "\",\"version\":\"" + this->m_version
            + "\",\"created_at\":\"" + this->m_created_at
            + "\",\"cost\":" + std::to_string(this->m_cost)
            + ",\"transport\":" + (this->m_transport == nullptr ? "null" : this->m_transport->to_json())
            + ",\"condition\":" + (this->m_condition == nullptr ? "null" : this->m_condition->to_json());
        return json + "}";
    }

    TwitchEventSubSubscriptions::TwitchEventSubSubscriptions(const std::string& json,
                                                             const std::vector<TwitchEventSubSubscription>& subscriptions)
                                                             : m_subscriptions{subscriptions} {
        this->m_total = std::stoul(get_object_param("\"total\"", json, "0"));
        this->m_total_cost = std::stoul(get_object_param("\"total_cost\"", json, "0"));
        this->m_max_total_cost = std::stoul(get_object_param("\"max_total_cost\"", json, "0"));
    }

    TwitchEventSubSubscriptions::TwitchEventSubSubscriptions(const size_t& total,
                                                             const size_t& total_cost,
                                                             const size_t& max_total_cost,
                                                             const std::vector<TwitchEventSubSubscription>& subscriptions)
                                                             : m_total{total},
                                                               m_total_cost{total_cost},
                                                               m_max_total_cost{max_total_cost},
                                                               m_subscriptions{subscriptions} {
    }

    std::string TwitchEventSubSubscriptions::to_json() const {
        std::string json = "{\"total\":" + std::to_string(this->m_total)
            + ",\"total_cost\":" + std::to_string(this->m_total_cost)
            + ",\"max_total_cost\":" + std::to_string(this->m_max_total_cost)
            + ",\"subscriptions\":[";
        for (size_t i {0}; i < this->m_subscriptions.size(); ++i) {
            json += this->m_subscriptions.at(i).to_json();
            if (i + 1 < this->m_subscriptions.size()) {
                json += ',';
            }
        }
        return json + "]}";
    }

    TwitchUserActiveExtension::TwitchUserActiveExtension(const std::string& json) {
        this->m_active = get_object_param("\"active\"", json) == "true";
        if (this->m_active) {
            this->m_id = get_object_param("\"id\"", json);
            this->m_name = get_object_param("\"name\"", json);
            this->m_version = get_object_param("\"version\"", json);
            this->m_x_pos = std::stoi(get_object_param("\"x_pos\"", json, "-1"));
            this->m_y_pos = std::stoi(get_object_param("\"y_pos\"", json, "-1"));
        }
    }

    TwitchUserActiveExtension::TwitchUserActiveExtension(const bool& active,
                                                         const std::string& id,
                                                         const std::string& name,
                                                         const std::string& version,
                                                         const int& x_pos,
                                                         const int& y_pos)
                                                         : m_active{active},
                                                           m_id{id},
                                                           m_name{name},
                                                           m_version{version},
                                                           m_x_pos{x_pos},
                                                           m_y_pos{y_pos} {
    }

    std::string TwitchUserActiveExtension::to_json() const {
        std::string json {"{\"active\":" + std::string(this->m_active ? "true" : "false")};
        if (this->m_active) {
            json += ",\"id\":\"" + this->m_id
                + "\",\"name\":\"" + this->m_name
                + "\",\"version\":\"" + this->m_version + "\"";
            if (this->m_x_pos > -1 && this->m_y_pos > -1) {
                json += ",\"x\":" + std::to_string(this->m_x_pos)
                    + ",\"y\":" + std::to_string(this->m_y_pos);
            }
        }
        return json + "}";
    }

    TwitchUserActiveExtensions::TwitchUserActiveExtensions(const std::vector<TwitchUserActiveExtension>& panel,
                                                           const std::vector<TwitchUserActiveExtension>& overlay,
                                                           const std::vector<TwitchUserActiveExtension>& component)
                                                           : m_panel{panel},
                                                             m_overlay{overlay},
                                                             m_component{component} {
    }

    std::string TwitchUserActiveExtensions::to_json() const {
        std::string json {"{\"panel\":{"};
        for (size_t i {0}; i < this->m_panel.size(); ++i) {
            json += "\"" + std::to_string(i + 1) + "\":" + this->m_panel.at(i).to_json();
            if (i + 1 < this->m_panel.size()) {
                json += ',';
            }
        }
        json += "},\"overlay\":{";
        for (size_t i {0}; i < this->m_overlay.size(); ++i) {
            json += "\"" + std::to_string(i + 1) + "\":" + this->m_overlay.at(i).to_json();
            if (i + 1 < this->m_overlay.size()) {
                json += ',';
            }
        }
        json += "},\"component\":{";
        for (size_t i {0}; i < this->m_component.size(); ++i) {
            json += "\"" + std::to_string(i + 1) + "\":" + this->m_component.at(i).to_json();
            if (i + 1 < this->m_component.size()) {
                json += ',';
            }
        }
        return json + "}}";
    }

    TwitchCodeStatus::TwitchCodeStatus(const std::string& json) {
        this->m_code = get_object_param("\"code\"", json);
        this->m_status = get_object_param("\"status\"", json);
    }

    TwitchCodeStatus::TwitchCodeStatus(const std::string& code,
                                       const std::string& status)
                                       : m_code{code},
                                         m_status{status} {
    }

    std::string TwitchCodeStatus::to_json() const {
        std::string json = "{\"code\":\"" + this->m_code
            + "\",\"status\":\"" + this->m_status
            + "\"}";
        return json;
    }

    TwitchDropsEntitlement::TwitchDropsEntitlement(const std::string& json) {
        this->m_id = get_object_param("\"id\"", json);
        this->m_benefit_id = get_object_param("\"benefit_id\"", json);
        this->m_timestamp = get_object_param("\"timestamp\"", json);
        this->m_user_id = get_object_param("\"user_id\"", json);
        this->m_game_id = get_object_param("\"game_id\"", json);
        this->m_fulfillment_status = get_object_param("\"fulfillment_status\"", json);
        this->m_updated_at = get_object_param("\"updated_at\"", json);
    }

    TwitchDropsEntitlement::TwitchDropsEntitlement(const std::string& id,
                                                   const std::string& benefit_id,
                                                   const std::string& timestamp,
                                                   const std::string& user_id,
                                                   const std::string& game_id,
                                                   const std::string& fulfillment_status,
                                                   const std::string& updated_at)
                                                   : m_id{id},
                                                     m_benefit_id{benefit_id},
                                                     m_timestamp{timestamp},
                                                     m_user_id{user_id},
                                                     m_game_id{game_id},
                                                     m_fulfillment_status{fulfillment_status},
                                                     m_updated_at{updated_at} {
    }

    std::string TwitchDropsEntitlement::to_json() const {
        std::string json = "{\"id\":\"" + this->m_id
            + "\",\"benefit_id\":\"" + this->m_benefit_id
            + "\",\"timestamp\":\"" + this->m_timestamp
            + "\",\"user_id\":\"" + this->m_user_id
            + "\",\"game_id\":\"" + this->m_game_id
            + "\",\"fulfillment_status\":\"" + this->m_fulfillment_status
            + "\",\"updated_at\":\"" + this->m_updated_at
            + "\"}";
        return json;
    }

    TwitchDropsEntitlementStatus::TwitchDropsEntitlementStatus(const std::string& json) {
        this->m_status = get_object_param("\"status\"", json);
        this->m_ids = json_to_vector(get_object_param("\"ids\"", json));
    }

    TwitchDropsEntitlementStatus::TwitchDropsEntitlementStatus(const std::string& status,
                                                               const std::vector<std::string>& ids)
                                                               : m_status{status},
                                                                 m_ids{ids} {
    }

    std::string TwitchDropsEntitlementStatus::to_json() const {
        std::string json = "{\"status\":\"" + this->m_status
            + ",\"ids\":" + vector_to_json(this->m_ids) + "}";
        return json;
    }

    TwitchExtensionConfigurationSegment::TwitchExtensionConfigurationSegment(const std::string& json) {
        this->m_segment = get_object_param("\"segment\"", json);
        this->m_content = get_object_param("\"content\"", json);
        this->m_version = get_object_param("\"version\"", json);
        this->m_broadcaster_id = get_object_param("\"broadcaster_id\"", json);
    }

    TwitchExtensionConfigurationSegment::TwitchExtensionConfigurationSegment(const std::string& segment,
                                                                             const std::string& content,
                                                                             const std::string& version,
                                                                             std::optional<std::string> broadcaster_id)
                                                                             : m_segment{segment},
                                                                               m_content{content},
                                                                               m_version{version},
                                                                               m_broadcaster_id{broadcaster_id.value_or("")} {
    }

    std::string TwitchExtensionConfigurationSegment::to_json() const {
        std::string json = "{\"segment\":\"" + this->m_segment
            + "\",\"content\":\"" + this->m_content
            + "\",\"version\":\"" + this->m_version;
        if (this->m_broadcaster_id.size()) {
            json += "\",\"broadcaster_id\":\"" + this->m_broadcaster_id;
        }
        return json + "\"}";
    }

    TwitchExtensionSecret::TwitchExtensionSecret(const std::string& json) {
        this->m_content = get_object_param("\"content\"", json);
        this->m_active_at = get_object_param("\"active_at\"", json);
        this->m_expires_at = get_object_param("\"expires_at\"", json);
    }

    TwitchExtensionSecret::TwitchExtensionSecret(const std::string& content,
                                                 const std::string& active_at,
                                                 const std::string& expires_at)
                                                 : m_content{content},
                                                   m_active_at{active_at},
                                                   m_expires_at{expires_at} {
    }

    std::string TwitchExtensionSecret::to_json() const {
        std::string json = "{\"content\":\"" + this->m_content
            + "\",\"active_at\":\"" + this->m_active_at
            + "\",\"expires_at\":\"" + this->m_expires_at
            + "\"}";
        return json;
    }

    TwitchExtensionSecrets::TwitchExtensionSecrets(const std::string& json) {
        this->m_format_version = std::stoul(get_object_param("\"format_version\"", json, "0"));
        std::vector<std::string> secrets = json_to_vector(get_object_param("\"ids\"", json));
        for (std::string secret : secrets) {
            this->m_secrets.push_back(TwitchExtensionSecret(secret));
        }
    }

    TwitchExtensionSecrets::TwitchExtensionSecrets(const size_t& format_version,
                                                   const std::vector<TwitchExtensionSecret>& secrets)
                                                   : m_format_version{format_version},
                                                     m_secrets{secrets} {
    }

    std::string TwitchExtensionSecrets::to_json() const {
        std::string json = "{\"format_version\":\"" + std::to_string(this->m_format_version)
            + ",\"secrets\":[";
        for (size_t i {0}; i < this->m_secrets.size(); ++i) {
            json += this->m_secrets.at(i).to_json();
            if (i + 1 < this->m_secrets.size()) {
                json += ',';
            }
        }
        return json + "]}";
    }

    TwitchExtensionLiveChannel::TwitchExtensionLiveChannel(const std::string& json) {
        this->m_broadcaster_id = get_object_param("\"broadcaster_id\"", json);
        this->m_broadcaster_name = get_object_param("\"broadcaster_name\"", json);
        this->m_game_name = get_object_param("\"game_name\"", json);
        this->m_game_id = get_object_param("\"game_id\"", json);
        this->m_title = get_object_param("\"title\"", json);
    }

    TwitchExtensionLiveChannel::TwitchExtensionLiveChannel(const std::string& broadcaster_id,
                                                           const std::string& broadcaster_name,
                                                           const std::string& game_name,
                                                           const std::string& game_id,
                                                           const std::string& title)
                                                           : m_broadcaster_id{broadcaster_id},
                                                             m_broadcaster_name{broadcaster_name},
                                                             m_game_name{game_name},
                                                             m_game_id{game_id},
                                                             m_title{title} {
    }

    std::string TwitchExtensionLiveChannel::to_json() const {
        std::string json = "{\"broadcaster_id\":\"" + this->m_broadcaster_id
            + "\",\"broadcaster_name\":\"" + this->m_broadcaster_name
            + "\",\"game_name\":\"" + this->m_game_name
            + "\",\"game_id\":\"" + this->m_game_id
            + "\",\"title\":\"" + this->m_title
            + "\"}";
        return json;
    }

    TwitchExtensionBitsProduct::TwitchExtensionBitsProduct(const std::string& json) {
        this->m_sku = get_object_param("\"sku\"", json);
        this->m_display_name = get_object_param("\"display_name\"", json);
        if (!this->m_display_name.size()) {
            this->m_display_name = get_object_param("\"displayName\"", json) == "true";
        }
        this->m_expiration = get_object_param("\"expiration\"", json);
        this->m_domain = get_object_param("\"domain\"", json);
        this->m_cost_type = get_object_param("\"type\"", json);

        this->m_cost_amount = std::stoul(get_object_param("\"amount\"", json, "0"));
        this->m_in_development = get_object_param("\"in_development\"", json) == "true";
        if (!this->m_in_development) {
            this->m_in_development = get_object_param("\"inDevelopment\"", json) == "true";
        }
        this->m_is_broadcast = get_object_param("\"is_broadcast\"", json) == "true";
        if (!this->m_is_broadcast) {
            this->m_is_broadcast = get_object_param("\"broadcast\"", json) == "true";
        }
    }

    TwitchExtensionBitsProduct::TwitchExtensionBitsProduct(const std::string& sku,
                                                           const std::string& display_name,
                                                           const std::string& expiration,
                                                           const std::string& cost_type,
                                                           const size_t& cost_amount,
                                                           const bool& in_development,
                                                           const bool& is_broadcast,
                                                           std::optional<std::string> domain)
                                                           : m_sku{sku},
                                                             m_display_name{display_name},
                                                             m_expiration{expiration},
                                                             m_cost_type{cost_type},
                                                             m_cost_amount{cost_amount},
                                                             m_in_development{in_development},
                                                             m_is_broadcast{is_broadcast},
                                                             m_domain{domain.value_or("")} {
    }

    std::string TwitchExtensionBitsProduct::to_json() const {
        std::string json = "{\"sku\":\"" + this->m_sku
            + "\",\"display_name\":\"" + this->m_display_name
            + "\",\"expiration\":\"" + this->m_expiration
            + "\",\"cost\":{\"type\":\"" + this->m_cost_type
            + ",\"amount\":" + std::to_string(this->m_cost_amount)
            + "},\"in_development\":" + (this->m_in_development ? "true" : "false")
            + ",\"is_broadcast\":" + (this->m_is_broadcast ? "true" : "false");
        if (this->m_domain.size()) {
            json += ",\"domain\":\"" + this->m_domain + "\"";
        }
        return json + "}";
    }

    TwitchExtensionTransaction::TwitchExtensionTransaction(const std::string& json) {
        this->m_id = get_object_param("\"id\"", json);
        this->m_timestamp = get_object_param("\"timestamp\"", json);
        this->m_broadcaster_id = get_object_param("\"broadcaster_id\"", json);
        this->m_broadcaster_login = get_object_param("\"broadcaster_login\"", json);
        this->m_broadcaster_name = get_object_param("\"broadcaster_name\"", json);
        this->m_user_id = get_object_param("\"user_id\"", json);
        this->m_user_login = get_object_param("\"user_login\"", json);
        this->m_user_name = get_object_param("\"user_name\"", json);
        this->m_product_type = get_object_param("\"product_type\"", json);
        std::string product_data = get_object_param("\"product_data\"", json, "null");
        if (product_data == "null") {
            this->m_product_data = nullptr;
        } else {
            this->m_product_data = std::make_shared<TwitchExtensionBitsProduct>(product_data);
        }
    }

    TwitchExtensionTransaction::TwitchExtensionTransaction(const std::string& id,
                                                           const std::string& timestamp,
                                                           const std::string& broadcaster_id,
                                                           const std::string& broadcaster_login,
                                                           const std::string& broadcaster_name,
                                                           const std::string& user_id,
                                                           const std::string& user_login,
                                                           const std::string& user_name,
                                                           const std::string& product_type,
                                                           std::shared_ptr<TwitchExtensionBitsProduct> product_data)
                                                           : m_id{id},
                                                             m_timestamp{timestamp},
                                                             m_broadcaster_id{broadcaster_id},
                                                             m_broadcaster_login{broadcaster_login},
                                                             m_broadcaster_name{broadcaster_name},
                                                             m_user_id{user_id},
                                                             m_user_login{user_login},
                                                             m_user_name{user_name},
                                                             m_product_type{product_type},
                                                             m_product_data{product_data} {
    }

    std::string TwitchExtensionTransaction::to_json() const {
        std::string json = "{\"id\":\"" + this->m_id
            + "\",\"timestamp\":\"" + this->m_timestamp
            + "\",\"broadcaster_id\":\"" + this->m_broadcaster_id
            + "\",\"broadcaster_login\":\"" + this->m_broadcaster_login
            + "\",\"broadcaster_name\":\"" + this->m_broadcaster_name
            + "\",\"user_id\":\"" + this->m_user_id
            + "\",\"user_login\":\"" + this->m_user_login
            + "\",\"user_name\":\"" + this->m_user_name
            + "\",\"product_type\":\"" + this->m_product_type
            + "\",\"product_data\":" + (this->m_product_data == nullptr ? "null" : this->m_product_data->to_json());
        return json + "}";
    }

    TwitchExtensionComponent::TwitchExtensionComponent(const std::string& json) {
        this->m_viewer_url = get_object_param("\"viewer_url\"", json);
        this->m_height = std::stoul(get_object_param("\"height\"", json, "0"));
        this->m_aspect_width = std::stoul(get_object_param("\"aspect_width\"", json, "0"));
        this->m_aspect_height = std::stoul(get_object_param("\"aspect_height\"", json, "0"));
        this->m_aspect_ratio_x = std::stoul(get_object_param("\"aspect_ratio_x\"", json, "0"));
        this->m_aspect_ratio_y = std::stoul(get_object_param("\"aspect_ratio_y\"", json, "0"));
        this->m_scale_pixels = std::stoul(get_object_param("\"scale_pixels\"", json, "0"));
        this->m_target_height = std::stoul(get_object_param("\"target_height\"", json, "0"));
        this->m_size = std::stoul(get_object_param("\"size\"", json, "0"));
        this->m_zoom_pixels = std::stoul(get_object_param("\"zoom_pixels\"", json, "0"));
        this->m_autoscale = get_object_param("\"autoscale\"", json) == "true";
        this->m_zoom = get_object_param("\"zoom\"", json) == "true";
        this->m_can_link_external_content = get_object_param("\"can_link_external_content\"", json) == "true";
    }
    // TODO: check that bool is const everywhere
    TwitchExtensionComponent::TwitchExtensionComponent(const std::string& viewer_url,
                                                       const size_t& height,
                                                       const size_t& aspect_width,
                                                       const size_t& aspect_height,
                                                       const size_t& aspect_ratio_x,
                                                       const size_t& aspect_ratio_y,
                                                       const size_t& scale_pixels,
                                                       const size_t& target_height,
                                                       const size_t& size,
                                                       const size_t& zoom_pixels,
                                                       const bool& autoscale,
                                                       const bool& zoom,
                                                       const bool& can_link_external_content)
                                                       : m_viewer_url{viewer_url},
                                                         m_height{height},
                                                         m_aspect_width{aspect_width},
                                                         m_aspect_height{aspect_height},
                                                         m_aspect_ratio_x{aspect_ratio_x},
                                                         m_aspect_ratio_y{aspect_ratio_y},
                                                         m_scale_pixels{scale_pixels},
                                                         m_target_height{target_height},
                                                         m_size{size},
                                                         m_zoom_pixels{zoom_pixels},
                                                         m_autoscale{autoscale},
                                                         m_zoom{zoom},
                                                         m_can_link_external_content{can_link_external_content} {
    }

    std::string TwitchExtensionComponent::to_json(std::optional<std::string_view> type) const {
        std::string json {"{\"viewer_url\":\"" + this->m_viewer_url + "\""};
        if (type.value_or("") == "video_overlay" || type.value_or("") == "panel") {
            json += ",\"can_link_external_content\":" + std::string(this->m_can_link_external_content ? "true" : "false");
        }
        if (type.value_or("") == "panel") {
            json += ",\"height\":" + this->m_height;
        }
        if (type.value_or("") == "component") {
            json += ",\"aspect_width\":" + std::to_string(this->m_aspect_width)
                + ",\"aspect_height\":" + std::to_string(this->m_aspect_height)
                + ",\"aspect_ratio_x\":" + std::to_string(this->m_aspect_ratio_x)
                + ",\"aspect_ratio_y\":" + std::to_string(this->m_aspect_ratio_y)
                + ",\"scale_pixels\":" + std::to_string(this->m_scale_pixels)
                + ",\"target_height\":" + std::to_string(this->m_target_height)
                + ",\"size\":" + std::to_string(this->m_size)
                + ",\"zoom_pixels\":" + std::to_string(this->m_zoom_pixels)
                + ",\"autoscale\":" + std::string(this->m_autoscale ? "true" : "false")
                + ",\"zoom\":" + std::string(this->m_zoom ? "true" : "false");
        }
        return json + "}";
    }

    TwitchExtensionViews::TwitchExtensionViews(const std::string& json) {
        std::string mobile = get_object_param("\"mobile\"", json, "null");
        if (mobile == "null") {
            this->m_mobile = nullptr;
        } else {
            this->m_mobile = std::make_shared<TwitchExtensionComponent>(mobile);
        }
        std::string panel = get_object_param("\"panel\"", json, "null");
        if (panel == "null") {
            this->m_panel = nullptr;
        } else {
            this->m_panel = std::make_shared<TwitchExtensionComponent>(panel);
        }
        std::string video_overlay = get_object_param("\"video_overlay\"", json, "null");
        if (video_overlay == "null") {
            this->m_video_overlay = nullptr;
        } else {
            this->m_video_overlay = std::make_shared<TwitchExtensionComponent>(video_overlay);
        }
        std::string component = get_object_param("\"component\"", json, "null");
        if (component == "null") {
            this->m_component = nullptr;
        } else {
            this->m_component = std::make_shared<TwitchExtensionComponent>(component);
        }
    }

    TwitchExtensionViews::TwitchExtensionViews(std::shared_ptr<TwitchExtensionComponent> mobile,
                                               std::shared_ptr<TwitchExtensionComponent> panel,
                                               std::shared_ptr<TwitchExtensionComponent> video_overlay,
                                               std::shared_ptr<TwitchExtensionComponent> component)
                                               : m_mobile{mobile},
                                                 m_panel{panel},
                                                 m_video_overlay{video_overlay},
                                                 m_component{component} {
    }

    std::string TwitchExtensionViews::to_json() const {
        std::string json = "{\"mobile\":" + (this->m_mobile == nullptr ? "null" : this->m_mobile->to_json())
            + ",\"panel\":" + (this->m_panel == nullptr ? "null" : this->m_panel->to_json("panel"))
            + ",\"video_overlay\":" + (this->m_video_overlay == nullptr ? "null" : this->m_video_overlay->to_json("video_overlay"))
            + ",\"component\":" + (this->m_component == nullptr ? "null" : this->m_component->to_json("component"));
        return json + "}";
    }

    TwitchExtension::TwitchExtension(const std::string& json) {
        this->m_id = get_object_param("\"id\"", json);
        this->m_name = get_object_param("\"name\"", json);
        this->m_author_name = get_object_param("\"author_name\"", json);
        this->m_configuration_location = get_object_param("\"configuration_location\"", json);
        this->m_description = get_object_param("\"description\"", json);
        this->m_eula_tos_url = get_object_param("\"eula_tos_url\"", json);
        this->m_icon_url = get_object_param("\"icon_url\"", json);
        this->m_privacy_policy_url = get_object_param("\"privacy_policy_url\"", json);
        this->m_state = get_object_param("\"state\"", json);
        this->m_subscriptions_support_level = get_object_param("\"subscriptions_support_level\"", json);
        this->m_summary = get_object_param("\"summary\"", json);
        this->m_support_email = get_object_param("\"support_email\"", json);
        this->m_version = get_object_param("\"version\"", json);
        this->m_viewer_summary = get_object_param("\"viewer_summary\"", json);
        this->m_bits_enabled = get_object_param("\"bits_enabled\"", json) == "true";
        this->m_can_install = get_object_param("\"can_install\"", json) == "true";
        this->m_has_chat_support = get_object_param("\"has_chat_support\"", json) == "true";
        this->m_request_identity_link = get_object_param("\"request_identity_link\"", json) == "true";
        this->m_allowlisted_config_urls = json_to_vector(get_object_param("\"allowlisted_config_urls\"", json));
        this->m_allowlisted_panel_urls = json_to_vector(get_object_param("\"allowlisted_panel_urls\"", json));
        this->m_screenshot_urls = json_to_vector(get_object_param("\"screenshot_urls\"", json));
        std::string icon_urls = get_object_param("\"icon_urls\"", json);
        this->m_icon_urls = string_to_string_map(icon_urls);
        std::string views = get_object_param("\"views\"", json, "null");
        if (views == "null") {
            this->m_views = nullptr;
        } else {
            this->m_views = std::make_shared<TwitchExtensionViews>(views);
        }
    }

    TwitchExtension::TwitchExtension(const std::string& id,
                                     const std::string& name,
                                     const std::string& author_name,
                                     const std::string& configuration_location,
                                     const std::string& description,
                                     const std::string& eula_tos_url,
                                     const std::string& icon_url,
                                     const std::string& privacy_policy_url,
                                     const std::string& state,
                                     const std::string& subscriptions_support_level,
                                     const std::string& summary,
                                     const std::string& support_email,
                                     const std::string& version,
                                     const std::string& viewer_summary,
                                     const bool& bits_enabled,
                                     const bool& can_install,
                                     const bool& has_chat_support,
                                     const bool& request_identity_link,
                                     const std::vector<std::string>& allowlisted_config_urls,
                                     const std::vector<std::string>& allowlisted_panel_urls,
                                     const std::vector<std::string>& screenshot_urls,
                                     StringMap icon_urls,
                                     std::shared_ptr<TwitchExtensionViews> views)
                                     : m_id{id},
                                       m_name{name},
                                       m_author_name{author_name},
                                       m_configuration_location{configuration_location},
                                       m_description{description},
                                       m_eula_tos_url{eula_tos_url},
                                       m_icon_url{icon_url},
                                       m_privacy_policy_url{privacy_policy_url},
                                       m_state{state},
                                       m_subscriptions_support_level{subscriptions_support_level},
                                       m_summary{summary},
                                       m_support_email{support_email},
                                       m_version{version},
                                       m_viewer_summary{viewer_summary},
                                       m_bits_enabled{bits_enabled},
                                       m_can_install{can_install},
                                       m_has_chat_support{has_chat_support},
                                       m_request_identity_link{request_identity_link},
                                       m_allowlisted_config_urls{allowlisted_config_urls},
                                       m_allowlisted_panel_urls{allowlisted_panel_urls},
                                       m_screenshot_urls{screenshot_urls},
                                       m_icon_urls{icon_urls},
                                       m_views{views} {
    }

    std::string TwitchExtension::to_json() const {
        std::string json = "{\"id\":\"" + this->m_id
            + "\",\"name\":\"" + this->m_name
            + "\",\"author_name\":\"" + this->m_author_name
            + "\",\"configuration_location\":\"" + this->m_configuration_location
            + "\",\"description\":\"" + this->m_description
            + "\",\"eula_tos_url\":\"" + this->m_eula_tos_url
            + "\",\"icon_url\":\"" + this->m_icon_url
            + "\",\"privacy_policy_url\":\"" + this->m_privacy_policy_url
            + "\",\"state\":\"" + this->m_state
            + "\",\"subscriptions_support_level\":\"" + this->m_subscriptions_support_level
            + "\",\"summary\":\"" + this->m_summary
            + "\",\"support_email\":\"" + this->m_support_email
            + "\",\"version\":\"" + this->m_version
            + "\",\"viewer_summary\":\"" + this->m_viewer_summary
            + "\",\"bits_enabled\":" + std::string(this->m_bits_enabled ? "true" : "false")
            + ",\"can_install\":" + std::string(this->m_can_install ? "true" : "false");
            + ",\"has_chat_support\":" + std::string(this->m_has_chat_support ? "true" : "false")
            + ",\"request_identity_link\":" + std::string(this->m_request_identity_link ? "true" : "false");
            + ",\"allowlisted_config_urls\":" + vector_to_json(this->m_allowlisted_config_urls)
            + ",\"allowlisted_panel_urls\":" + vector_to_json(this->m_allowlisted_panel_urls)
            + ",\"screenshot_urls\":" + vector_to_json(this->m_screenshot_urls)
            + ",\"icon_urls\":{" + string_map_to_string(this->m_icon_urls)
            + "},\"views\":" + (this->m_views == nullptr ? "null" : this->m_views->to_json());
        return json + "}";
    }
}
